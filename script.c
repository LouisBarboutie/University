/*
	AUTHOR: Louis-Hendrik Barboutie
	CONTACT: louis.barboutie@gmail.com

	CHANGELOG:
	- 08/02/2023	Basic functionality done
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int  main(){

	// ================= //
	// === VARIABLES === //
	// ================= //

	// --- GENERIC COUNTERS ---
	int i, j, k, h;

	// --- DATA ---
	char **temperatureTimeList;
	char **phTimeList;
	float *temperatureValues;
	float **phValues;
	int N; // number of temperature values
	int M; // number of pH values

	// --- FILE HANDLING ---
	FILE *fptrRefTemperature, *fptrRefPh, *fptrDest;
	char *filePathTemperature, *filePathPh, *filePathDest;
	char *line;
	int lineSize;

	lineSize = 2048;



	// ========================= //
	// === MEMORY ALLOCATION === //
	// ========================= //

	filePathTemperature = (char *)malloc(2048 * sizeof(char));
	filePathPh 					= (char *)malloc(2048 * sizeof(char));
	filePathDest 				= (char *)malloc(2048 * sizeof(char));
	line 								= (char *)malloc(lineSize * sizeof(char));



	// ============================== //
	// === SPECIFY FILE LOCATIONS === //
	// ============================== //

	strcpy(filePathTemperature, "temperature.txt");
	strcpy(filePathPh, "pH.txt");
	strcpy(filePathDest, "MajaOutput.txt");
	printf("%s\n%s\n%s\n", filePathTemperature, filePathPh, filePathDest );



	// ================== //
	// === OPEN FILES === //
	// ================== //

	fptrRefTemperature = fopen(filePathTemperature, "r");
	fptrRefPh 				 = fopen(filePathPh, "r");
	fptrDest 					 = fopen(filePathDest, "w");



	// =================== //
	// === ERROR CHECK === //
	// =================== //

	if (fptrRefTemperature == NULL || fptrRefPh == NULL || fptrDest == NULL){
		return 1;
	}



	// ==================================== //
	// === COUNT THE TEMPERATURE VALUES === //
	// ==================================== //

	N = 0;
	while (fgets(line, lineSize, fptrRefTemperature) != NULL){
		N++;
	}
	printf("N = %i\n", N);

	M = 0;
	while (fgets(line, lineSize, fptrRefPh) != NULL){
		M++;
	}
	printf("M = %i\n", M);

  // --- GO BACK TO BEGINNING OF FILE ---
	rewind(fptrRefTemperature);
	rewind(fptrRefPh);



	// =========================================== //
	// === MEMORY ALLOCATION ELECTRIC BOOGALOO === //
	// =========================================== //

	temperatureTimeList = (char **)malloc(N * sizeof(char *));
	for (i = 0; i < N; i++){
		temperatureTimeList[i] = (char *)malloc(16 * sizeof(char));
	}

	temperatureValues = (float *)malloc(N * sizeof(float));

	phTimeList = (char **)malloc(M * sizeof(char *));
	for (i = 0; i < M; i++){
		phTimeList[i] = (char *)malloc(16 * sizeof(char));
	}

	phValues = (float **)malloc(24 * sizeof(float *));
	for (i = 0; i < 24; i++){
		phValues[i] = (float *)malloc(M * sizeof(float));
	}



	// =================== //
	// === GET DA DATA === //
	// =================== //

	i = 0;
	while (fgets(line, lineSize, fptrRefTemperature) != NULL){
		sscanf(line, "%s %f", temperatureTimeList[i], &temperatureValues[i]);
		i++;
	}

	i = 0;
	while (fgets(line, lineSize, fptrRefPh) != NULL){
		sscanf(line, "%s %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", phTimeList[i], &phValues[0][i], &phValues[1][i], &phValues[2][i], &phValues[3][i], &phValues[4][i], &phValues[5][i], &phValues[6][i], &phValues[7][i], &phValues[8][i], &phValues[9][i], &phValues[10][i], &phValues[11][i], &phValues[12][i], &phValues[13][i], &phValues[14][i], &phValues[15][i], &phValues[16][i], &phValues[17][i], &phValues[18][i], &phValues[19][i], &phValues[20][i], &phValues[21][i], &phValues[22][i], &phValues[23][i]);
		i++;
	}




	// ===================================== //
	// === COMPARATOR LOOP GO BRRRRRRRRR === //
	// ===================================== //

	// --- reset the counters ---
	i = 0;
	for (;;){
		iterations++;
		if(i >= M){
				break;
		}
		for(j = 0; j < M; j++){
			if (strcmp(phTimeList[j], temperatureTimeList[i]) == 0){
				fprintf(fptrDest, "%s ", phTimeList[j]);
				for (k = 0; k < 24; k++){
					fprintf(fptrDest, "%.2f ", phValues[k][j]);
				}
				fprintf(fptrDest, "\n");
			}
		}
		i++;
	}


	// =================== //
	// === CLOSE FILES === //
	// =================== //

	fclose(fptrRefTemperature);
	fclose(fptrRefPh);
	fclose(fptrDest);



	// ====================== //
	// === MEMORY FREEING === //
	// ====================== //

	for (i = 0; i < N; i++){
		free(temperatureTimeList[i]);
	}
	free(temperatureTimeList);

	free(temperatureValues);

	for (i = 0; i < M; i++){
		free(phTimeList[i]);
	}
	free(phTimeList);

	for (i = 0; i < 24; i++){
		free(phValues[i]);
	}
	free(phValues);

	free(filePathTemperature);
	free(filePathPh);
	free(filePathDest);
	free(line);

	printf("\nsuccess\n");
  return 0;
}
