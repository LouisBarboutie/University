/*
    Description:

    Author: Louis-Hendrik Barboutie
*/

AUERR = au_metabol_2(curdat);
QUITMSG("--- AU program au_metabol_2 finished ---")

int au_metabol_2(const char* curdat){
    
    char dataset_path[PATH_MAX];
    int d_list[64];
    char peak_freq[PATH_MAX];
    char command_string[PATH_MAX];
    char line[PATH_MAX];
    FILE *fptr;

    CPR_exec("pps", WAIT_TERM)
    CPR_exec("convertpeaklist peaklist", WAIT_TERM)

    strcpy(dataset_path, "dataset_path");
    GETSTRING("enter path to dataset (right click -> copy):", dataset_path);
    strcat(dataset_path, "/1/pdata/1/peaklist");

    fptr = fopen(dataset_path);
    while (fgets(line, sizeof(line), fptr) != NULL){
        if (sscanf(line, "%s", &peak_freq) == 1){
            break;
        } 
    }
    fclose(fptr);
    
    strcpy(command_string, "o1 ");
    strcat(command_string, peak_freq);

    CPR_exec(command_string, WAIT_TERM)

    return EXIT_SUCCESS;
}