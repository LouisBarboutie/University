/* 
    Für Elise

    Description:
    This script creates a new dataset, which is a copy of an already existing dataset. 
    It copies all the parameter files but no raw or processed data.
    Experiments numbering does not have to be continuous.
    Processing number is assumed to be 1 for every experiment.
    Title file is written automatically with given parameters.

    Author: Louis-Hendrik Barboutie

    Changelog:
    - 21/09/2022    added title file creation
    - 20/09/2022	implemented directory reading, automatic date setting
    - 19/09/2022    created
*/
AUERR = au_metabol_1();
QUITMSG("--- AU program au_metabol_1 finished ---")

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

int au_metabol_1(void){
    int i, j, n_exp, spinner_nbr;
    // char exp_nbr_str[64][4];
    char exp_name_list[64][4];
    char file_names[6][20]; // list of the names of files to copy
    char new_dir_path_default[PATH_MAX]; // path to the default directory containing all datasets
    char ref_dataset_name[PATH_MAX]; // name of the reference dataset
    char ref_dataset_path[PATH_MAX]; // path to the reference dataset
    char new_dataset_name[PATH_MAX]; // name of the new dataset
    char new_dataset_path[PATH_MAX]; // path to the new dataset
    char temp_string[PATH_MAX]; // temporary string
    char new_dataset_exp_path[PATH_MAX]; // path to an experiment of the new dataset
    char ref_dataset_exp_path[PATH_MAX]; // path to an experiment of the reference dataset
    char ref_file_path[PATH_MAX]; // path to the reference file
    char new_file_path[PATH_MAX]; // path to the new file
    char temp_ch; // one character as copy buffer
    char exp_date[PATH_MAX];
    char exp_date_title[PATH_MAX];
    char metabolite_name[PATH_MAX];
    char metabolite_concentration[PATH_MAX];
    FILE *fptr_ref; // pointer to open the reference files
    FILE *fptr_dest; // pointer to open the destination files 
    DIR *ref_dataset_folder; // pointer to the reference dataset folder
    struct dirent *entry; 
    time_t T = time(NULL);
    struct tm dt = *localtime(&T);

    // get the current date for printing in title file and dataset name
    getdate(&dt);
    sprintf(exp_date, "%02d/%02d/%04d", dt.tm_mday, dt.tm_mon + 1, dt.tm_year + 1900);
    sprintf(exp_date_title, "%02d%02d%04d", dt.tm_mday, dt.tm_mon + 1, dt.tm_year + 1900);

    // define the list of file names to copy
    strcpy(file_names[0], "acqu");
    strcpy(file_names[1], "acqus");
    strcpy(file_names[2], "outd");
    strcpy(file_names[3], "proc");
    strcpy(file_names[4], "procs");
    strcpy(file_names[5], "title");
    	
    // define default directory path
    strcpy(new_dir_path_default, "/opt/topspin3.6.3.b.11/data/jesus/nmr/");

    // ask user for name parameters of new dataset
    strcpy(metabolite_name, "metabolite_name");
    GETSTRING("enter name of metabolite:", metabolite_name)
    strcpy(metabolite_concentration, "metabolite_concentration");
    GETSTRING("enter concentration of metabolite in mM:", metabolite_concentration)
    GETINT("enter spinner number:", spinner_nbr)
    
    // construct dataset name
    strcpy(new_dataset_name, "MDB_");
    strcat(new_dataset_name, metabolite_name);
    strcat(new_dataset_name, "_");
    strcat(new_dataset_name, metabolite_concentration);
    strcat(new_dataset_name, "mM_");
    strcat(new_dataset_name, exp_date_title);

    // ask for a reference dataset to copy
    strcpy(ref_dataset_name, "MDB_REFERENCE_XXXmM_21092022");
    GETSTRING("enter name of experiment to copy (must be for same user):", ref_dataset_name)

    // store the path to the reference dataset
    strcpy(temp_string, new_dir_path_default);
    strcat(temp_string, ref_dataset_name);
    strcpy(ref_dataset_path, temp_string);

    // store the path to the new dataset
    strcpy(temp_string, new_dir_path_default);
    strcat(temp_string, new_dataset_name);
    strcpy(new_dataset_path, temp_string);

    // create new dataset 
    if (mkdir(new_dataset_path, 0777) == -1){
        Proc_err(DEF_ERR_OPT, "failed to create new dataset");
    }

    // check the amount and names of experiments to create
    n_exp = 0;
    ref_dataset_folder = opendir(ref_dataset_path);

    // failsafe
    if (ref_dataset_folder == NULL){
        Proc_err(DEF_ERR_OPT, "error opening the folder");
        return EXIT_FAILURE;
    }

    // store the experiment names in a list
    while (entry = readdir(ref_dataset_folder)){
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;
        }
        else {
            strcpy(exp_name_list[n_exp], entry->d_name);
            n_exp++;
        }
    }
    closedir(ref_dataset_folder);	

    // check for the correct value of n_exp
    Proc_err(DEF_ERR_OPT, "THIS IS NOT AN ERROR: n_exp = %i, should be 7", n_exp);
    
    for (j = 0; j < n_exp; j++)
    {
        // sprintf(exp_nbr_str[j], "%s", exp_name_list[j]);    
        
        // store path to experiment and sync with the reference
        strcpy(new_dataset_exp_path, new_dataset_path);
        strcat(new_dataset_exp_path, "/");
        strcat(new_dataset_exp_path, exp_name_list[j]);
        strcpy(ref_dataset_exp_path, ref_dataset_path);
        strcat(ref_dataset_exp_path, "/");
        strcat(ref_dataset_exp_path, exp_name_list[j]);


        // create first experiment
        if (mkdir(new_dataset_exp_path, 0777) == -1){
            Proc_err(DEF_ERR_OPT, "failed to create new experiment, nbr %i, inside dataset", ref_dataset_exp_path);
        }

        // create and copy the reference files for the first level
        for (i = 0; i < 2; i++){
            // get the reference and new file paths
            strcpy(new_file_path, new_dataset_exp_path);
            strcat(new_file_path, "/");
            strcat(new_file_path, file_names[i]);
            strcpy(ref_file_path, ref_dataset_exp_path);
            strcat(ref_file_path, "/");
            strcat(ref_file_path, file_names[i]);
            
            // open files
            fptr_ref = fopen(ref_file_path, "r");
            fptr_dest = fopen(new_file_path, "w");

            if (fptr_ref == NULL || fptr_dest == NULL){
                Proc_err(DEF_ERR_OPT, "error opening file %s or %s, first level", ref_file_path, new_file_path);
                return EXIT_FAILURE;
            }

            temp_ch = fgetc(fptr_ref);
            while (temp_ch != EOF){
                fputc(temp_ch, fptr_dest);
                temp_ch = fgetc(fptr_ref);
            }

            // close files
            fclose(fptr_ref);
            fclose(fptr_dest);
        }
        
        // create and copy the reference files and folders for the second level

        // create first subfolder
        strcat(new_dataset_exp_path, "/pdata");
        strcat(ref_dataset_exp_path, "/pdata");
        if (mkdir(new_dataset_exp_path, 0777) == -1){
            Proc_err(DEF_ERR_OPT, "failed to create new directory: %s", ref_dataset_exp_path);
        }

        // create second subfolder
        strcat(new_dataset_exp_path,"/1");
        strcat(ref_dataset_exp_path, "/1");
        if (mkdir(new_dataset_exp_path, 0777) == -1){
            Proc_err(DEF_ERR_OPT, "failed to create new directory: %s", ref_dataset_exp_path);
        }

        // copy files loop
        for (i = 2; i < 6; i++){
            // get the path for the files to copy
            strcpy(new_file_path, new_dataset_exp_path);
            strcat(new_file_path, "/");
            strcat(new_file_path, file_names[i]);
            strcpy(ref_file_path, ref_dataset_exp_path);
            strcat(ref_file_path, "/");
            strcat(ref_file_path, file_names[i]);

            // open reference and destination files in read and write mode
            fptr_ref = fopen(ref_file_path,"r");
            fptr_dest = fopen(new_file_path,"w");
            
            // failsafe if the file could not be opened
            if (fptr_ref == NULL || fptr_dest == NULL){
                Proc_err(DEF_ERR_OPT, "error opening file %s or %s, second level", ref_file_path, new_file_path);
                return EXIT_FAILURE;
            }
            
            // print the title file
            if (i == 5){
                fprintf(fptr_dest, "%s\n", exp_date);
                if ( strcmp(exp_name_list[j], "10") == 0){
                    fprintf(fptr_dest, "%s", "d1 = 1 ms\n");
                }
                if ( strcmp(exp_name_list[j], "11") == 0){
                    fprintf(fptr_dest, "%s", "d1 = 10 s\n");
                }
                if ( strcmp(exp_name_list[j], "12") == 0){
                    fprintf(fptr_dest, "%s", "d1 = 20 s\n");
                }
                if ( strcmp(exp_name_list[j], "13") == 0){
                    fprintf(fptr_dest, "%s", "d1 = 40 s\n");
                }
                if ( strcmp(exp_name_list[j], "100") == 0){
                    fprintf(fptr_dest, "%s", "parameters set for shimming\n");
                }
                fprintf(fptr_dest,"%s %s mM\nD2O, TSP 5 mM\ninsert HR-MAS 25 uL\nspinner number %i\n277 K\nP1 4.50 us\nPLW 1 14.65 W\n", metabolite_name, metabolite_concentration, spinner_nbr);
            }

            // copy character by character until End Of File
            else{
            	temp_ch = fgetc(fptr_ref);
            	while (temp_ch != EOF){
                	fputc(temp_ch, fptr_dest);
              	  temp_ch = fgetc(fptr_ref);
            	}
            }
        
            // close reference and destination files
            fclose(fptr_ref);
            fclose(fptr_dest);
        }
    }
    
    return EXIT_SUCCESS;
}