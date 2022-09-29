/*
    Description:
    This AU program performs automatic peak picking, for filtering out water signal.
    
    With the input of ppm ranges, it looks for the peak and applies the correct offset frequency for the all experiments, and launches the zg and zgpr acquisition

    How-to-use: (follow step-by-step)
    - create new dataset
    - copy the name of the new dataset to clipboard
    - execute program

    Author: Louis-Hendrik Barboutie
    e-mail: louis.barboutie@gmail.com

    Changelog
    - 22/09/2022    created
*/

AUERR = au_metabol_2(curdat);
QUITMSG("--- AU program au_metabol_2 finished ---")

#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

// compare function for qsort
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int au_metabol_2(const char* curdat){
	int i;
    float P1;
    int n_exp; // amount of experiments
    int masr;
    int t_equi;
    float temp;
    float H2O_peak_freq; // value of the frequency corresponding to the peak of H2O
    float SR; // shift reference value 
    char default_dir[PATH_MAX];
    char peaklist_path[PATH_MAX]; 
    char dataset_name[PATH_MAX];
    char default_path[PATH_MAX];
    char dataset_path[PATH_MAX];
    char ans[32];
    char line[PATH_MAX]; // string for reading the files
    int exp_name_list[64]; // modify the value in the brackets to have more possible experiments
    float F1P, F2P, MI; // limits and minimum intensity for the peak picking
    char *ptr; // variable for strtol()
    FILE *fptr; // pointer to the peaklist
    DIR *dataset_folder; 
    struct dirent *entry;

    // get the peaklist name, with user input
    strcpy(dataset_name, "dataset_name");
    GETSTRING("enter name of dataset (right click -> rename -> copy):", dataset_name)
    
    strcpy(ans, "y");
    GETSTRING("halt the rotation at the end? [y/n]", ans)
    
    // temperature and masr input
    masr = 3501;
    temp = 277;
    t_equi = 10;
    GETINT("enter masr frequency (Hz):", masr)
    GETFLOAT("enter desired temperature (K):", temp)
    GETINT("wait how long after temperature equilibration (s):", t_equi)
    STOREPAR("TE",temp) // set the temperature parameter
    STOREPAR("MASR",masr) // set the rotation speed parameter
    STOREPAR("L 31",masr) // set the rotation speed parameter
    TESET // set the target temperature
    MASI // insert sample
    MASR // set the target rotation speed
    MASG(10) //start the rotation
    TEREADY(t_equi,0.1) // start the temperature change and wait t_equi after stabilization

    // build the path to the peaklist file
    strcpy(default_path, "/opt/topspin3.6.3.b.11/data/jesus/nmr/");
    strcpy(dataset_path, default_path); // build path to dataset
    strcat(dataset_path, dataset_name);
    strcpy(peaklist_path, dataset_path); // build path to peaklist
    strcat(peaklist_path, "/1/pdata/1/peaklist"); // path should now look like this: "dataset_path/1/pdata/1/peaklist"
	
    n_exp = 0;
    dataset_folder = opendir(dataset_path);

    // failsafe
    if (dataset_folder == NULL){
        Proc_err(DEF_ERR_OPT, "error opening the folder");
        return EXIT_FAILURE;
    }

    // store the experiment names in a list
    while (entry = readdir(dataset_folder)){
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            // do not store the names for current and parent directory
            continue;
        }
        else {
            // store the names of the experiments as integers and count the number of experiments
            exp_name_list[n_exp] = strtol(entry->d_name,&ptr, 10);
            n_exp++;
        }
    }

    // clean up
    closedir(dataset_folder);	
        
    // do a sort of exp_name_list here. use qsort
    qsort(exp_name_list, n_exp, sizeof(int), cmpfunc);
    
    // perform automatic wobb
    XCMD("atma")

    // go into the first experiment and do a simple acquisition sequence
    REXPNO(1)
    ZG
    FT
    APK

    // peak picking parameters, user input
    F1P = 6; // default values
    F2P = 4;
    MI = 5;
    //GETFLOAT("enter left limit for peak picking (ppm):", F1P) // user input values
    //GETFLOAT("enter right limit for peak picking (ppm):", F2P)
    //GETFLOAT("enter minimum intensity for peak picking:", MI)
    STOREPAR("F1P", F1P) // store the parameters
    STOREPAR("F2P", F2P)
    STOREPAR("MI", MI)

	// do peak picking	
    PPP 

    // read peaklist and find the peak frequency
    fptr = fopen(peaklist_path, "r");
    if (fptr == NULL){ // failsafe if file doesn't exist
       Proc_err(DEF_ERR_OPT, "error opening file %s ", peaklist_path);
       return EXIT_FAILURE;
    }
    while (fgets(line, sizeof(line), fptr) != NULL){ // read the file line by line until
        if (sscanf(line, "%f", &H2O_peak_freq) == 1){
            break;
        } 
    }
    fclose(fptr);
		
	FETCHPAR("SR", &SR) // get reference shift value
	H2O_peak_freq += SR; // correct peak picking value with reference shift
	
    // set the parameter in all the experiments
    i =0;
    TIMES(n_exp) 
    	REXPNO(exp_name_list[i])
    	SETCURDATA
    	STOREPAR("o1", H2O_peak_freq)
        i++;
    END

    // P1 optimization sequence
    REXPNO(1)// go back to first experiment
    XAU("poptau", curdat); // XAU( au_program, arguments)

    // set the correct value of P1
    FETCHPAR("P1", &P1)
    P1 /= 2;

    // set the parameter in all the experiments
    i =0;
    TIMES(n_exp) 
    	REXPNO(exp_name_list[i])
    	SETCURDATA
        STOREPAR("P1", P1)
    	i++;
    END

    // go into the first and second experiment and do a simple acquisition sequence
    for ( i=0; i<2; i++){
        REXPNO(i+1)
        ZG
        FT
        APK
    }

    REXPNO(100)
    ZG
    FT
    APK
    CPR_exec("gs", WAIT_START);
    sleep(30); // give 30s to select the peak for the shims
    CPR_exec("topshim changemasshims", WAIT_TERM); // doesn"t work in parallel with gs

    if (strcmp(ans,"y") == 0){
        MASH // halt the rotation
    }

    return EXIT_SUCCESS;
}