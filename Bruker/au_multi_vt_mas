/*
  Description:
  This AU programm allows variable temperature and variable speed acquisitions.
  The parameters for the aquisitions need to be passed in in a .txt file, stored in the vt list directory.
  Example of a parameter list: temperature (K) [whitespace] rotation speed (Hz)
    280 2000
    275 10000
    275 15000
  If datasets do not yet exist, the current dataset and its parameters are copied. 
  If the data sets already exist, then the experiments are executed as they are.
  Temperature and rotation speed are set according to the values in the parameter list.
  You will be asked for the temperature equilibration	time. 
  The AU program will wait at as long as defined by this time for the temperature to settle.
  --------------------------------------------------------------------------------------------------------
  Author: Louis-Hendrik Barboutie
  Email: louis.barboutie@gmail.com
  --------------------------------------------------------------------------------------------------------
  Modifications:
  - 12/09/2022  changed parameter list format
  - 09/09/2022  created
*/

AUERR = au_multi_vt_mas(curdat);
QUITMSG("--- AU program au_multi_vt_mas finished ---")

#include <inc/exptUtil>

int au_multi_vt_mas(const char* curdat)
{
  int i;
  int masr_list[128], t_spin, t_before, t_after, experiments_counter, t_equi;
  float temp_list[128];
  char parameter_list_name[PATH_MAX], line[PATH_MAX], parameter_list_path[PATH_MAX];
  FILE *fptr_parameter;
    
  /* input of the temperature and masr lists */
  (void) strcpy (parameter_list_name, "parameterList");
  GETSTRING("enter name of parameter list:", parameter_list_name)
		
	/* get the file path */
	if (getParfileDirForRead(parameter_list_name, VT_DIRS, parameter_list_path) < 0){
    Proc_err(DEF_ERR_OPT, "VT list file %s: %s\n"
		"Please use 'edlist vt' to check for valid files.",
		parameter_list_name, parameter_list_path);
    ABORT
 	}

 	/* open the files and check for null */
	if ( (fptr_parameter = fopen (parameter_list_path,"r")) == NULL ){
  	Proc_err(DEF_ERR_OPT, "Could not open VT list file\n%s\n"
	  "Please use 'edlist vt' to check for valid files.",
	  parameter_list_path);
   	ABORT
  }	
  	
  /* store temperature and rotation speed settings in a list */
	experiments_counter = 0;
	while (fgets (line, sizeof(line), fptr_parameter) != NULL){
		if (sscanf(line, "%f %i", &temp_list[experiments_counter], &masr_list[experiments_counter]) != 2){ break; }
		experiments_counter ++;
	}
		
	fclose( fptr_parameter );
    
  GETINT("enter thermal equilibration time (sec):", t_equi)
    
  /* test aquisition sequence */
  i = 0;
	TIMES(experiments_counter)
	  SETCURDATA
    STOREPAR("TE",temp_list[i]) // set the temperature parameter
    STOREPAR("MASR",masr_list[i]) // set the rotation speed parameter
    STOREPAR("L 31",masr_list[i]) // set the rotation speed parameter
    TESET // set the target temperature
    MASR // set the target rotation speed
    MASG(5) //start the rotation
    TEREADY(t_equi,0.1) // start the temperature change and wait t_equi after stabilization
    ZG_OVERWRITE // do the acquisition
    IEXPNO // increment the experiment
    i++; 
	END
	MASH // halt the rotation
	
  return 0;
}