/*
  Description:
  --------------------------------------------------------------------------------------------------------
  Author: Louis-Hendrik Barboutie
  Email: louis.barboutie@gmail.com
  --------------------------------------------------------------------------------------------------------
  Modifications:
  - 12/09/2022  created
*/

AUERR = au_multi_vt_static(curdat);
QUITMSG("---AU program au_multi_vt_static finished ---")

int au_multi_vt_static(const char* curdat){
    FILE *fptr_temperature_list;
    float temperature_list[128];
    int experiments_counter, v_spin, t_spin, i, t_equi;
    char line[PATH_MAX], temperature_list_name[PATH_MAX], temperature_list_path[PATH_MAX];

    /* input of the temperature list */
  (void) strcpy (temperature_list_name, "temperatureList");
  GETSTRING("enter name of temperature list:", temperature_list_name)
		
	/* get the file path */
	if (getParfileDirForRead(temperature_list_name, VT_DIRS, temperature_list_path) < 0){
    Proc_err(DEF_ERR_OPT, "VT list file %s: %s\n"
		"Please use 'edlist vt' to check for valid files.",
		temperature_list_name, temperature_list_path);
    ABORT
 	}

 	/* open the files and check for null */
	if ( (fptr_temperature_list = fopen (temperature_list_path,"r")) == NULL ){
  	Proc_err(DEF_ERR_OPT, "Could not open VT list file\n%s\n"
	  "Please use 'edlist vt' to check for valid files.",
	  temperature_list_path);
   	ABORT
  }	
  	
  /* store temperature and rotation speed settings in a list */
	experiments_counter = 0;
	while (fgets (line, sizeof(line), fptr_temperature_list) != NULL){
		if (sscanf(line, "%f", &temperature_list[experiments_counter]) != 1){ break; }
		experiments_counter ++;
	}
		
	fclose( fptr_temperature_list );

    v_spin = 3000;
    t_spin = 0;
    t_equi = 10;
    GETINT("enter spin rate (Hz) before aquisition:", v_spin)
    GETINT("Enter spin duration (sec):", t_spin)
    GETINT("enter equilibration duration (sec):", t_equi)

    STOREPAR("MASR", v_spin)
    STOREPAR("L 31", v_spin)
    MASR

    i = 0;
    TIMES(experiments_counter)
        SETCURDATA
        STOREPAR("TE", temperature_list[i])
        TESET
        TEREADY(t_equi,0.1)
        MASG(5)
        sleep(t_spin);
        MASH
        sleep(60);
        ZG_OVERWRITE
        IEXPNO
        i++;
    END

    return 0;
}