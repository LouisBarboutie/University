/*
    DESCRIPTION:
    This AU program performs O1 and P1 optimization using "poptau" program.
    It launches the shimming in the end and requests user to start the actual experiments.
    Before executing, the user needs to copy the name of the dataset, and to place itself in one of the experiments.
    The user will be asked to input the parameters for the dataset.

    Author: Louis-Hendrik Barboutie
    e-mail: louis.barboutie@gmail.com

    Changelog
    - 22/09/2022    created
*/

AUERR = mbopt(curdat);
QUITMSG("--- AU program mbopt finished ---")

/* ============================================================================= */
/* Libraries                                                                     */
/* ============================================================================= */

#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <inc/exptUtil>

/* ============================================================================= */
/* Function declaration                                                          */
/* ============================================================================= */

// compare function for qsort
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

// str concatenation function. variables are: output to be written into, input1, input2
void assemble_path(char *path, char *prefix, char *suffix);
int CalcExpTime ();

/* ============================================================================= */
/* Main program                                                                  */
/* ============================================================================= */

int mbopt(const char* curdat){
    /* --------------------- */
    /* ----- Variables ----- */
    /* --------------------- */

	int i; // generic counter variable
    float P1; // variable for storing the value of the P90
    int n_exp; // amount of experiments
    int n_cpmg1pr; // counter for the amount of cpmg experiments
    int exp_time;
    int masr; // variable for storing the rotation frequency value
    int t_equi; // time given to the sample to equilibrate the temperature, after the temperature has been reached
    float temp; // variable for storing the temperature value
    float H2O_peak_freq; // value of the frequency corresponding to the peak of H2O
    float SR; // shift reference value 
    char peaklist_path[PATH_MAX]; // path to the list containing the data from the peakpicking
    char title_path[PATH_MAX]; // path to the title file 
    char dataset_name[PATH_MAX]; // variable for holding the name of the dataset
    char default_path[PATH_MAX]; // path to the default directory where the datasets folders are
    char dataset_path[PATH_MAX]; // variable for holding the path to the dataset
    char ans[2][32]; // list of answers
    char quest[PATH_MAX]; // string for storing questions
    char exp_name[32]; // variable for holding the name of an experiment
    char line[PATH_MAX]; // string for reading the files
    int exp_name_list[64]; // list of all the experiment numbers, modify the value in the brackets to have more possible experiments
    float F1P, F2P, MI; // limits and minimum intensity for the peak picking
    char *ptr; // variable for strtol()
    FILE *fptr; // pointer for editing files
    DIR *dataset_folder; // pointer to read directories
    struct dirent *entry; // variable used for reading directories

    /* ----------------------------------------------- */
    /* ----- User prompts and parameter settings ----- */
    /* ----------------------------------------------- */

    // get the peaklist name
    strcpy(dataset_name, "dataset_name"); // default value
    GETSTRING("enter name of dataset (right click -> rename -> copy):", dataset_name) // user input
    
    strcpy(ans[0], "y"); // default value
    GETSTRING("halt the rotation at the end? [y/n]", ans[0]) // user input
    
    // set temperature and masr 
    masr = 3501; // default values
    temp = 277;
    t_equi = 0;
    GETINT("enter masr frequency (Hz):", masr) // user input
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
    strcpy(default_path, "/opt/nmrdata/users/jesus/ux_data/nmr"); // default path goes to jesus directorz
    assemble_path(dataset_path, default_path, dataset_name); // build path to dataset
    assemble_path(peaklist_path, dataset_path, "1/pdata/1/peaklist"); // build path to peaklist
     
    // peak picking parameters
    F1P = 6; // default values
    F2P = 4;
    MI = 5;
    //GETFLOAT("enter left limit for peak picking (ppm):", F1P) // user input values
    //GETFLOAT("enter right limit for peak picking (ppm):", F2P)
    //GETFLOAT("enter minimum intensity for peak picking:", MI)
    STOREPAR("F1P", F1P) // store the parameters
    STOREPAR("F2P", F2P)
    STOREPAR("MI", MI)

    /* ------------------------------------------------------------------------------------ */
    /* ----- Read the dataset directory for counting and getting names of experiments ----- */
    /* ------------------------------------------------------------------------------------ */
    
    n_exp = 0; // initialize the counter

    dataset_folder = opendir(dataset_path); // open the dataset directory for reading

    // failsafe in case the dataset folder returns and error
    if (dataset_folder == NULL){
        Proc_err(DEF_ERR_OPT, "error opening the folder");
        return EXIT_FAILURE;
    }

    // store the experiment names in a list
    while (entry = readdir(dataset_folder)){ // read and store each entry 
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){ // do not store the names for current and parent directory
            continue;
        }
        else {
            exp_name_list[n_exp] = strtol(entry->d_name,&ptr, 10); // store the names of the experiments as integers and count the number of experiments
            n_exp++; // count the experiments
        }
    }

    // clean up
    closedir(dataset_folder);	
    
    // do a sort of exp_name_list, because the read isn't in numerical order
    qsort(exp_name_list, n_exp, sizeof(int), cmpfunc);
    
    // count how many cpmg experiments will be done
    n_cpmg1pr = 0; // initialize counter
    for ( i = 0; i < n_exp; i++){ // iterate over the list
        if ( exp_name_list[i] > 9 && exp_name_list[i] < 100){
            n_cpmg1pr++; // count cpmg experiments
        }
    }
    Proc_err(DEF_ERR_OPT, "found %i cpmg experiments", n_cpmg1pr);
    /* ---------------------------------- */
    /* ----- Parameter Optimization ----- */
    /* ---------------------------------- */

    // perform automatic wobb
    XCMD("atma")

    // go into the first experiment and do a simple acquisition sequence followed by fourier transform and automatic phase correction and display in active window
    REXPNO(1)
    ZG
    FT
    APK
    VIEWDATA_SAMEWIN

	// do peak picking	
    PPP 

    // read peaklist and find the peak frequency
    fptr = fopen(peaklist_path, "r"); // open the file in read mode
    
    // failsafe if file doesn't exist
    if (fptr == NULL){ 
       Proc_err(DEF_ERR_OPT, "error opening file %s ", peaklist_path);
       return EXIT_FAILURE;
    }

    // read the file line by line until until it reads a number, hopefully the frequency for water
    while (fgets(line, sizeof(line), fptr) != NULL){ 
        if (sscanf(line, "%f", &H2O_peak_freq) == 1){
            break;
        } 
    }

    // close the file again
    fclose(fptr);
	
    // correct the frequencz with the reference shift
	FETCHPAR("SR", &SR) // get reference shift value
	H2O_peak_freq += SR; // correct peak picking value with reference shift
	
    // set the parameter in all the experiments
    i = 0;
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
    i = 0;
    TIMES(n_exp) 
    	REXPNO(exp_name_list[i])
    	SETCURDATA
        STOREPAR("P1", P1)
    	i++;
    END

    // append P1 value to title file
    for (i = 0; i < n_exp; i++){
        // get path to title file
        sprintf(exp_name, "%i", exp_name_list[i]); // need the name as str and not int
	    assemble_path(title_path, dataset_path, exp_name); 
        assemble_path(title_path, title_path, "pdata/1/title");

        // open title file in append mode and print P1
        fptr = fopen(title_path, "a");
        
        // failsafe
        if ( fptr == NULL){ 
            Proc_err(DEF_ERR_OPT, "error opening file %s", title_path);
        }
        
        // append the value of P1
        fprintf(fptr, "P1 = %f us\n", P1);
        
        // close the file again
        fclose(fptr);
    }

    // go into the first and second experiment and do a simple acquisition sequence
    for ( i=0; i<2; i++){
        REXPNO(i+1)
        ZG
        FT
        APK
        VIEWDATA_SAMEWIN
    }

    // go to exp 100 for shimming and launch gs
    REXPNO(100)
    ZG
    FT
    APK
    VIEWDATA_SAMEWIN
    Proc_err(DEF_ERR_OPT, "You have 60s to select a peak for shimming, once tmas launches the spectral window can't be changed");
    CPR_exec("gs", WAIT_START); // execute gs command
    sleep(60); // give 60s to select the peak for the shims
    CPR_exec("topshim changemasshims", WAIT_TERM); // execute tmas command
    XCMD("halt") // stop gs 
    // overwrite the shim acquisition
    ZG
    FT
    APK
    VIEWDATA_SAMEWIN
   
    /* -------------------------------- */
    /* ----- Acquisition Sequence ----- */
    /* -------------------------------- */
    
    // user input to proceed
    strcpy(ans[1], "y");
    sprintf(quest, "start exp 10 to %i: [y/n]", 10 + n_cpmg1pr);
    GETSTRING(quest, ans[1])
    
    Proc_err(DEF_ERR_OPT, "got the value");

    // do the acquisition for all counted experiments
    exp_time = 0;
    if ( strcmp(ans[1], "y") == 0){
        Proc_err(DEF_ERR_OPT, "string compare successful");
        for (i = 10; i < 10 + n_cpmg1pr; i++){
            REXPNO(i)
            SETCURDATA
            exp_time += CalcExpTime();
        }
        Proc_err(DEF_ERR_OPT, "total exp time is %i h, %i s", exp_time/3600, exp_time%3600);
        for (i = 10; i < 10 + n_cpmg1pr; i++){
            REXPNO(i)
            SETCURDATA
            ZG
            FT
            APK           
        }
    }

    // rotation stop
    if (strcmp(ans[0], "y") == 0){
        MASH // halt the rotation
    }

    return EXIT_SUCCESS;
}

/* ============================================================================= */
/* Functions                                                                     */
/* ============================================================================= */

void assemble_path(char *path, char *prefix, char *suffix){
    strcpy(path, prefix);
    strcat(path, "/");
    strcat(path, suffix);
    return;
}