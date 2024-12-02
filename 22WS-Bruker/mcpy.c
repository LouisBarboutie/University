/*
    mcpy 

    author: Louis-Hendrik Barboutie

*/

AUERR = mcpy();
QUITMSG("--- AU program mcpy finished ---")

/* ============================================================================= */
/* Libraries                                                                     */
/* ============================================================================= */

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

/* ============================================================================= */
/* Function declaration                                                          */
/* ============================================================================= */

void assemble_path(char *path, char *prefix, char *suffix);
void file_copy(char *ref_file_path, char *new_file_path);

/* ============================================================================= */
/* Main program                                                                  */
/* ============================================================================= */

int mcpy(void){
    /* --------------------- */
    /* ----- Variables ----- */
    /* --------------------- */

    char file_names[6][6] = { "acqu", "acqus", "outd", "proc", "procs", "title"}; // list of the names of files to copy
    char new_dir_path_default[PATH_MAX];
    char new_dataset_name[PATH_MAX];
    char ref_dataset_name[PATH_MAX];
    char new_dataset_path[PATH_MAX];
    char ref_dataset_path[PATH_MAX];
    char new_dataset_exp_path[PATH_MAX];
    char ref_dataset_exp_path[PATH_MAX];
    char new_file_path[PATH_MAX];
    char ref_file_path[PATH_MAX];

    char exp_name[PATH_MAX];
    char temp_ch;
    char *ptr;
    char ans[32];

    FILE *fptr_ref; // pointer to open the reference files
    FILE *fptr_dest; // pointer to open the destination files 

    DIR *ref_dataset_folder; // pointer to the reference dataset folder
    struct dirent *entry; 

    int i, j; // counters
    int n_exp;
    int exp_list[64];

    // define default directory path
    strcpy(new_dir_path_default, "/opt/nmrdata/users/jesus/ux_data/nmr");
   
    /* ------------------------ */
    /* ----- User prompts ----- */
    /* ------------------------ */

    // get name of new dataset
    strcpy(new_dataset_name, "new_dataset_name");
    GETSTRING("enter name of new dataset:", new_dataset_name)
    strcpy(ref_dataset_name, "ref_dataset_name");
    GETSTRING("enter name of ref dataset:", ref_dataset_name)
    
    // option to add date
    strcpy(ans,"y");
    GETSTRING("add date to title file? [y/n]", ans)

    // construct paths
    assemble_path(new_dataset_path, new_dir_path_default, new_dataset_name);
    assemble_path(ref_dataset_path, new_dir_path_default, ref_dataset_name);
    
    // create new dataset with all permissions (read/write)
    if (mkdir(new_dataset_path, 0777) == -1){
        Proc_err(DEF_ERR_OPT, "failed to create new dataset");
        return EXIT_FAILURE;
    }
    
    /* ------------------------------------------------------------------------------------ */
    /* ----- Read the dataset directory for counting and getting names of experiments ----- */
    /* ------------------------------------------------------------------------------------ */

    // check the amount and names of experiments to create
    ref_dataset_folder = opendir(ref_dataset_path);

    // failsafe
    if (ref_dataset_folder == NULL){
        Proc_err(DEF_ERR_OPT, "error opening the folder");
        return EXIT_FAILURE;
    }

    // store the experiment names in a list
    n_exp = 0;
    while (entry = readdir(ref_dataset_folder)){
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;
        }
        else {
            exp_list[n_exp] = strtol(entry->d_name,&ptr, 10);
            n_exp++;
        }
    }

    closedir(ref_dataset_folder);
    
    /* -------------------------------- */
    /* ----- Copy Loop go Brrrrrr ----- */
    /* -------------------------------- */

    for (i = 0; i < n_exp; i++)
    { 
        // store path to experiment and sync with the reference
        sprintf(exp_name, "%i", exp_list[i]);
        assemble_path(new_dataset_exp_path, new_dataset_path, exp_name);
        assemble_path(ref_dataset_exp_path, ref_dataset_path, exp_name);
        
        // create experiment folder
        if (mkdir(new_dataset_exp_path, 0777) == -1){
            Proc_err(DEF_ERR_OPT, "failed to create new experiment, nbr %i, inside dataset", ref_dataset_exp_path);
            return EXIT_FAILURE;
        }

        // create and copy the reference files for the first level
        for (j = 0; j < 2; j++){
            // get the reference and new file paths
            assemble_path(new_file_path, new_dataset_exp_path, file_names[j]);
            assemble_path(ref_file_path, ref_dataset_exp_path, file_names[j]);
            
            file_copy(ref_file_path, new_file_path);
        }

        // create first subfolder
        strcat(new_dataset_exp_path, "/pdata");
        strcat(ref_dataset_exp_path, "/pdata");
        if (mkdir(new_dataset_exp_path, 0777) == -1){
            Proc_err(DEF_ERR_OPT, "failed to create new directory: %s", ref_dataset_exp_path);
            return EXIT_FAILURE;
        }

        // create second subfolder
        strcat(new_dataset_exp_path,"/1");
        strcat(ref_dataset_exp_path, "/1");
        if (mkdir(new_dataset_exp_path, 0777) == -1){
            Proc_err(DEF_ERR_OPT, "failed to create new directory: %s", ref_dataset_exp_path);
            return EXIT_FAILURE;
        }

        // copy files loop
        for (j = 2; j < 6; j++){
            // get the path for the files to copy
            assemble_path(new_file_path, new_dataset_exp_path, file_names[j]);
            assemble_path(ref_file_path, ref_dataset_exp_path, file_names[j]);
            
            file_copy(ref_file_path, new_file_path);
        }
    }
    
    return EXIT_SUCCESS;
}

/* ============================================================================= */
/* Functions                                                                     */
/* ============================================================================= */

void file_copy(char *ref_file_path, char *new_file_path){
    FILE *fptr_ref, *fptr_dest;
    char temp_ch;
    
    // open files
    fptr_ref = fopen(ref_file_path, "r");
    fptr_dest = fopen(new_file_path, "w");

    if (fptr_ref == NULL || fptr_dest == NULL){
        Proc_err(DEF_ERR_OPT, "error opening file %s or %s", ref_file_path, new_file_path);
    }

    temp_ch = fgetc(fptr_ref); // this is the temporary char which gets filled as each new char is read, and read the first char of the reference file
    while (temp_ch != EOF){ // loop until reaching End Of File
        fputc(temp_ch, fptr_dest); // write the char into the destination file 
        temp_ch = fgetc(fptr_ref); // get next char in the reference file
    }

    // close files
    fclose(fptr_ref);
    fclose(fptr_dest);
    return;
}

void assemble_path(char *path, char *prefix, char *suffix){
    strcpy(path, prefix);
    strcat(path, "/");
    strcat(path, suffix);
    return;
}