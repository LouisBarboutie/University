/*
    Description:

    Author: Louis-Hendrik Barboutie
    e-mail: louis.barboutie@gmail.com
    GitHub: https://github.com/TakashiSenpai/University/tree/main/Bruker

    Changelog:
    - 05/10/2022    created
*/

AUERR = hygcpy(curdat);
QUITMSG("--- AU program hygcpy finished ---")

/* ===================== */
/* ===== Libraries ===== */
/* ===================== */

#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <inc/exptUtil>

int build_title(char *new_dataset_title, struct tm dt, char *compound, int rotor_nbr){
    
    // variables
    char path[PATH_MAX];
    char title_path[PATH_MAX];
    char exp_name_list[64][8]; // experiment name list as string
    int n_exp;
    int i;
    FILE *fptr;
    DIR *ref_dataset_folder; // pointer to the reference dataset folder
    struct dirent *entry;  // variable used for reading directories

    // build default path to dataset
    sprintf(path, "/opt/users/jesus/ux_data/nmr/%s", new_dataset_title);
		Proc_err(DEF_ERR_OPT, "built the path %s", path);
    n_exp = 0; // initialize counter

    // open the directory for reading
    ref_dataset_folder = opendir(path); 

    // failsafe
    if (ref_dataset_folder == NULL){
        Proc_err(DEF_ERR_OPT, "error opening the folder");
        return EXIT_FAILURE;
    }

    // store the experiment names in a list
    while (entry = readdir(ref_dataset_folder)){ // read and store each entry 
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){ // do not store the names for current and parent directory
            continue;
        }
        else {
            strcpy(exp_name_list[n_exp], entry->d_name); // store the names of the experiments 
            n_exp++; // update counter
        }
    }
    Proc_err(DEF_ERR_OPT, "found %i experiments", n_exp);
    
    // close the dataset
    closedir(ref_dataset_folder);

    for ( i = 0; i < n_exp; i++ ){
        sprintf(title_path, "%s/%s/pdata/1/title", path, exp_name_list[i]);
        Proc_err(DEF_ERR_OPT, "built the path %s", title_path);
        fptr = fopen(title_path, "w");
        if ( fptr == NULL ){
        	Proc_err(DEF_ERR_OPT, "error opening the file");
        	continue;
        }
        fprintf(fptr, "%s\n%02d/%02d/%04d\ncompound: %s\nspinner: %i", new_dataset_title, dt.tm_mday, dt.tm_mon + 1, dt.tm_year + 1900, compound, rotor_nbr);
        fclose(fptr);
    }

  return EXIT_SUCCESS;
}

/* =================== */
/* ===== Program ===== */
/* =================== */

int hygcpy(const char* curdat){

    /* --------------------- */
    /* ----- Variables ----- */
    /* --------------------- */
    char ref_dataset_title[PATH_MAX];
    char new_dataset_title[PATH_MAX];
    char compound[PATH_MAX];
    int rotor_nbr;
		
    time_t T = time(NULL); 
    struct tm dt = *localtime(&T);
    
    // default values for title
    sprintf(compound, "XXX");
    GETSTRING("enter compound name:", compound)
    GETINT("enter rotor number:", rotor_nbr)

    // definition of reference dataset
    sprintf(ref_dataset_title, "rotovide_ref_0.7mm");
    Proc_err(DEF_ERR_OPT, "reference dataset is: %s", ref_dataset_title);
    
    // create new dataset title 
    sprintf(new_dataset_title, "HyC_%s_0.7mm_R%i_%02d_%02d_%04d", compound, rotor_nbr, dt.tm_mday, dt.tm_mon + 1, dt.tm_year + 1900);
    
    // set the reference dataset as current dataset to operate on
    DATASET(ref_dataset_title, 1, 1, disk, user)
    SETCURDATA

    // create new dataset and copy reference dataset
    WRPA(new_dataset_title, 1, 1, disk, user)

    // set the new dataset as current dataset to operate on
    DATASET(new_dataset_title, 1, 1, disk, user)
    SETCURDATA

    // copy the parameter files and display the data
    RPAR("rov1.par","acqu")
    RPAR("rov1.par","proc")
    VIEWDATA_SAMEWIN

		Proc_err(DEF_ERR_OPT, "successfully created exp 1, it is the currently active dataset");
    // open the sr gui
    CPR_exec("sendgui sr", WAIT_TERM);

    // create new dataset and copy reference dataset
    WRPA(new_dataset_title, 26, 1, disk, user)

    // set the new dataset as current dataset to operate on
    DATASET(new_dataset_title, 26, 1, disk, user)
    SETCURDATA
    
    // copy the parameter files and display the data
    RPAR("rov26.par","acqu")
    RPAR("rov26.par","proc")
    VIEWDATA_SAMEWIN

		Proc_err(DEF_ERR_OPT, "successfully created exp 26, it is the currently active dataset");
    // open the sr gui
    CPR_exec("sendgui sr", WAIT_TERM);

    // set the reference dataset as current dataset and display the data
    DATASET(ref_dataset_title, 1, 1, disk, user)
    VIEWDATA_SAMEWIN

    build_title(new_dataset_title, dt, compound, rotor_nbr);

    return EXIT_SUCCESS;
}
