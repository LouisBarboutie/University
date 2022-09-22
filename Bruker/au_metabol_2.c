/*
    Description:
    This AU program performs automatic peak picking, for filtering out water signal.
     With the input of ppm ranges, it looks for the peak and applies the correct offset frequency for the following experiments

    Author: Louis-Hendrik Barboutie
    e-mail: louis.barboutie@gmail.com

    Changelog
    - 22/09/2022    created
*/

AUERR = au_metabol_2(curdat);
QUITMSG("--- AU program au_metabol_2 finished ---")

int au_metabol_2(const char* curdat){
	int i = 0;
    float H2O_peak_freq; 
    char peaklist_path[PATH_MAX]; 
    char dataset_name[PATH_MAX];
    char line[PATH_MAX]; // string for reading the files
    int exp_name[7] = { 1, 2, 10, 11, 12, 13, 100};
    float F1P, F2P, MI; // limits and minimum intensity for the peak picking
    FILE *fptr;

    // get the peaklist path, with user input
    strcpy(peaklist_path, "dataset_path");
    GETSTRING("enter path to dataset (right click -> copy, remove blank space in the end!!):", peaklist_path)
    strcat(peaklist_path, "/1/pdata/1/peaklist"); // path should now look like this: "dataset_path/1/pdata/1/peaklist"

    // peak picking parameters, user input
    F1P = 5.1; // default values
    F2P = 4.9;
    MI = 5;
    GETFLOAT("enter left limit for peak picking (ppm):", F1P) // user input values
    GETFLOAT("enter right limit for peak picking (ppm):", F2P)
    GETFLOAT("enter minimum intensity for peak picking:", MI)
    STOREPAR("F1P", F1P) // store the parameters
    STOREPAR("F2P", F2P)
    STOREPAR("MI", MI)

	// do peak picking	
    PPP

    // read peaklist and find the peak frequency
    fptr = fopen(peaklist_path, "r");
    if (fptr == NULL){
       Proc_err(DEF_ERR_OPT, "error opening file %s ", peaklist_path);
       return EXIT_FAILURE;
    }
    while (fgets(line, sizeof(line), fptr) != NULL){
        if (sscanf(line, "%f", &H2O_peak_freq) == 1){
            break;
        } 
    }
    fclose(fptr);
 
    // set the parameter  
    TIMES(7) 
    	REXPNO(exp_name[i])
    	SETCURDATA
    	STOREPAR("o1", H2O_peak_freq)
    	i++;
    END

    return EXIT_SUCCESS;
}