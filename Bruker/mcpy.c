/*
    mcpy 

    author: Louis-Hendrik Barboutie

*/

AUERR = mcpy();
QUITMSG("--- AU program mcpy finished ---")

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

int mcpy(void){

    time_t T = time(NULL);
    struct tm dt = *localtime(&T);
    char exp_date[PATH_MAX];
    char exp_date_title[PATH_MAX];

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

    // get the current date for printing in title file and dataset name
    getdate(&dt);
    sprintf(exp_date, "%02d/%02d/%04d", dt.tm_mday, dt.tm_mon + 1, dt.tm_year + 1900);
    sprintf(exp_date_title, "%02d%02d%04d", dt.tm_mday, dt.tm_mon + 1, dt.tm_year + 1900);

    // define default directory path
    strcpy(new_dir_path_default, "/opt/nmrdata/users/jesus/ux_data/nmr/");



    // get name of new dataset
    strcpy(new_dataset_name, "new_dataset_name");
    GETSTRING("enter name of new dataset:", new_dataset_name)
    strcpy(ref_dataset_name, "ref_dataset_name");
    GETSTRING("enter name of ref dataset:", ref_dataset_name)
    
    // option to add date
    strcpy(ans,"y");
    GETSTRING("add date to title file? [y/n]", ans)

    // construct paths
    strcpy(new_dataset_path, new_dir_path_default);
    strcat(new_dataset_path, new_dataset_name);
    strcpy(ref_dataset_path, new_dir_path_default);
    strcat(ref_dataset_path, ref_dataset_name);
    
    // create new dataset with all permissions (read/write)
    if (mkdir(new_dataset_path, 0777) == -1){
        Proc_err(DEF_ERR_OPT, "failed to create new dataset");
        return EXIT_FAILURE;
    }

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

    for (i = 0; i < n_exp; i++)
    { 
        // store path to experiment and sync with the reference
        sprintf(exp_name, "%i", exp_list[i]);
        strcpy(new_dataset_exp_path, new_dataset_path);
        strcat(new_dataset_exp_path, "/");
        strcat(new_dataset_exp_path, exp_name);
        strcpy(ref_dataset_exp_path, ref_dataset_path);
        strcat(ref_dataset_exp_path, "/");
        strcat(ref_dataset_exp_path, exp_name);

        // create experiment folder
        if (mkdir(new_dataset_exp_path, 0777) == -1){
            Proc_err(DEF_ERR_OPT, "failed to create new experiment, nbr %i, inside dataset", ref_dataset_exp_path);
            return EXIT_FAILURE;
        }

        // create and copy the reference files for the first level
        for (j = 0; j < 2; j++){
            // get the reference and new file paths
            strcpy(new_file_path, new_dataset_exp_path);
            strcat(new_file_path, "/");
            strcat(new_file_path, file_names[j]);
            strcpy(ref_file_path, ref_dataset_exp_path);
            strcat(ref_file_path, "/");
            strcat(ref_file_path, file_names[j]);
            
            // open files
            fptr_ref = fopen(ref_file_path, "r");
            fptr_dest = fopen(new_file_path, "w");

            if (fptr_ref == NULL || fptr_dest == NULL){
                Proc_err(DEF_ERR_OPT, "could not copy file, error opening file %s or %s, first level", ref_file_path, new_file_path);
                continue;
            }

            temp_ch = fgetc(fptr_ref); // this is the temporary char which gets filled as each new char is read, and read the first char of the reference file
            while (temp_ch != EOF){ // loop until reaching End Of File
                fputc(temp_ch, fptr_dest); // write the char into the destination file 
                temp_ch = fgetc(fptr_ref); // get next char in the reference file
            }

            // close files
            fclose(fptr_ref);
            fclose(fptr_dest);
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
            strcpy(new_file_path, new_dataset_exp_path);
            strcat(new_file_path, "/");
            strcat(new_file_path, file_names[j]);
            strcpy(ref_file_path, ref_dataset_exp_path);
            strcat(ref_file_path, "/");
            strcat(ref_file_path, file_names[j]);

            // open reference and destination files in read and write mode
            fptr_ref = fopen(ref_file_path,"r");
            fptr_dest = fopen(new_file_path,"w");
            
            // failsafe if the file could not be opened
            if (fptr_ref == NULL || fptr_dest == NULL){
                Proc_err(DEF_ERR_OPT, "could not copy file, error opening file %s or %s, second level", ref_file_path, new_file_path);
                continue;
                //return EXIT_FAILURE;
            }

            // print date in title file
            if (j == 5 && strcmp(ans, "y") == 0){
                fprintf(fptr_dest, "%s\n", exp_date);
            }

            // copy character by character until End Of File, see above for explanation
            temp_ch = fgetc(fptr_ref);
            while (temp_ch != EOF){
                fputc(temp_ch, fptr_dest);
                temp_ch = fgetc(fptr_ref);
            }

            // close reference and destination files
            fclose(fptr_ref);
            fclose(fptr_dest);
        }
    }
    
    return EXIT_SUCCESS;
}