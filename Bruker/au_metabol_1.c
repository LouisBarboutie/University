/* 
    Für Elise
    Description:
    This script creates a new dataset, which is a copy of an already existing dataset. 
    It copies all the parameter files but no raw or processed data.
    Experiments numbering does not have to be continuous.
    Processing number is assumed to be 1 for every experiment.
    Title file is written automatically with given parameters.
    Author: Louis-Hendrik Barboutie
    e-mail: louis.barboutie@gmail.com
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

void assemble_path(char *path, char *prefix, char *suffix);
void file_copy(char *ref_file_path, char *new_file_path);
void create_title(char *ref_file_path, char *new_file_path, char *exp_date, char *metabolite_name, char *metabolite_concentration, char *metabolite_volume, char *solvent, int spinner_nbr, int exp);

int au_metabol_1(void){
    int i, j, n_exp, spinner_nbr, d1;
    char exp_name_list[64][8];
    char exp_int_list[64];
    char *file_names[7] = { "acqu", "acqus", "popt.array", "outd", "proc", "procs", "title"}; // list of the names of files to copy
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
    char metabolite_volume[PATH_MAX];
    char solvent[PATH_MAX];
    char *ptr;
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
    	
    // define default directory path
    strcpy(new_dir_path_default, "/opt/nmrdata/users/jesus/ux_data/nmr");

    // ask user for name parameters of new dataset
    strcpy(metabolite_name, "metabolite_name");
    GETSTRING("enter name of metabolite:", metabolite_name)
    strcpy(metabolite_concentration, "metabolite_concentration");
    GETSTRING("enter concentration of metabolite in mM:", metabolite_concentration)
    strcpy(metabolite_volume, "metabolite_volume");
    GETSTRING("enter metabolite volume (uL)", metabolite_volume)
    strcpy(solvent, "solvent");
    GETSTRING("enter solvent name:", solvent)
    GETINT("enter spinner number:", spinner_nbr)
    
    // construct dataset name
    strcpy(new_dataset_name, "MDB_");
    strcat(new_dataset_name, metabolite_name);
    strcat(new_dataset_name, "_");
    strcat(new_dataset_name, metabolite_concentration);
    strcat(new_dataset_name, "mM_");
    strcat(new_dataset_name,metabolite_volume),
    strcat(new_dataset_name, "uL_");
    strcat(new_dataset_name, solvent);
    strcat(new_dataset_name, "_");
    strcat(new_dataset_name, exp_date_title);

    // ask for a reference dataset to copy
    strcpy(ref_dataset_name, "MDB_REFERENCE_XXXmM_21092022");
    GETSTRING("enter name of experiment to copy (must be for same user):", ref_dataset_name)

    // store the path to the reference dataset
    assemble_path(ref_dataset_path, new_dir_path_default, ref_dataset_name);
    assemble_path(new_dataset_path, new_dir_path_default, new_dataset_name);

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
            exp_int_list[n_exp] = strtol(entry->d_name,&ptr, 10);
            n_exp++;
        }
    }
    closedir(ref_dataset_folder);	

    // check for the correct value of n_exp
    Proc_err(DEF_ERR_OPT, "THIS IS NOT AN ERROR: n_exp = %i, should be 7", n_exp);
    
    for (j = 0; j < n_exp; j++)
    { 
        // store path to experiment and sync with the reference
        assemble_path(new_dataset_exp_path, new_dataset_path, exp_name_list[j]);
        assemble_path(ref_dataset_exp_path, ref_dataset_path, exp_name_list[j]);
        
        // create first experiment
        if (mkdir(new_dataset_exp_path, 0777) == -1){
            Proc_err(DEF_ERR_OPT, "failed to create new experiment, nbr %i, inside dataset", ref_dataset_exp_path);
        }

        // create and copy the reference files for the first level
        for (i = 0; i < 3; i++){
            // get the reference and new file paths
            assemble_path(new_file_path, new_dataset_exp_path, file_names[i]);
            assemble_path(ref_file_path, ref_dataset_exp_path, file_names[i]);

            // copy file by file, but popt.array only for experiment 1
            if ( i == 2 && strcmp(exp_name_list[j], "1") == 1){
                continue;
            }
            else{
                file_copy(ref_file_path, new_file_path);
            }
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
        for (i = 3; i < 7; i++){
            // get the path for the files to copy
            assemble_path(new_file_path, new_dataset_exp_path, file_names[i]);
            assemble_path(ref_file_path, ref_dataset_exp_path, file_names[i]);
            
            // print the title file, shitty way, maybe use list to set d1
            if (i == 6){
                create_title(ref_file_path, new_file_path, exp_date, metabolite_name, metabolite_concentration, metabolite_volume, solvent, spinner_nbr, exp_int_list[j]);   
            }
            else{
        	    file_copy(ref_file_path, new_file_path);
            }
        }
        
    }
        
    return EXIT_SUCCESS;
}

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

void create_title(char *ref_file_path, char *new_file_path, char *exp_date, char *metabolite_name, char *metabolite_concentration, char *metabolite_volume, char *solvent, int spinner_nbr, int exp){
    FILE *fptr_ref, *fptr_dest;
    int d1 = 0;
    
    // open files
    fptr_ref = fopen(ref_file_path, "r");
    fptr_dest = fopen(new_file_path, "w");

    if (fptr_ref == NULL || fptr_dest == NULL){
        Proc_err(DEF_ERR_OPT, "error opening file %s or %s", ref_file_path, new_file_path);
    }

    fprintf(fptr_dest, "%s\n", exp_date);
    if (exp == 10){
        fprintf(fptr_dest, "d1 = %i ms\n", 1);
    }
    if (exp > 10 && exp < 100){
        d1 = 10 * pow(2,exp-11);
        fprintf(fptr_dest, "d1 = %i s\n", d1);
    }
    if (exp == 100){
        fprintf(fptr_dest, "parameters set for shimming\n");
    }
    fprintf(fptr_dest,"%s %s mM\nVolume %s uL\n%s, TSP 5 mM\ninsert HR-MAS\nspinner number %i\n277 K\nPLW 1 14.65 W\n", metabolite_name, metabolite_concentration, metabolite_volume, solvent, spinner_nbr);
            

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
