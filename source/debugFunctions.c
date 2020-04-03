/*
 * debugFunction.c
 *
 *  Created on: 24 nov 2019
 *      Author: ironmath
 */

#include "debugFunctions.h"

// Global Variable
/*
struct fieldData fDataDebug;

sensorData sDataDebug;

struct positionData pDataDebug;
*/


int sonarDistanceVect[BUFFER_SIZE]; // for the string I'm going to read from file
int angleVect[BUFFER_SIZE];

// GOOD
/* Print the char matrix in a file and add a special code to easily convert the matrix into an jpg image*/
void printEnvMap(char* path,fieldData  *pt_fData){
	printf("printEnv\n");

	FILE *fp;
	int lenPath=strlen(path);
	int i,j;
	int valueToPrint;

	char filename[7] = "map.txt" ;
	char fullpath[strlen(filename)+lenPath];

	strncpy(fullpath, path , lenPath);
	fullpath[lenPath] = '/' ;

	strncpy(fullpath+lenPath+1, filename , strlen(filename));
	fullpath[strlen(filename)+lenPath+1]='\0';

	printf("%s\n",fullpath);
	fp=fopen(fullpath, "w");

	fprintf(fp,"P2\n%d %d\n255\n",Y_DIM,X_DIM); //for the image conversion


	for(i=0;i<X_DIM;i++){
		for(j=0;j<Y_DIM;j++){
			valueToPrint=(int)pt_fData->field[i][j];
			//to convert with good colors for visualization
			switch (valueToPrint) {

				// case UNEXPLORED:
				// 	valueToPrint=100;
			  //  	    break;

			 	case FREE:
			 		valueToPrint=70;
			 		break;

			 	case OBJECTS:
			 		valueToPrint=0;
			 		break;

			 	case ENEMY_FIELD:
			 		valueToPrint=40;
			 	   	break;

			 	default:
			 		valueToPrint=0;
			 		break;
			}
			fprintf(fp,"%d ", valueToPrint);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	return;

};





//GOOD
/*Load a test set of sonar data from a file with the following format
 * "%d,%d,", &angle, &sonarDistance */
int readTestDataFile(char *filenameOfSonarData){
	printf("readTestDataFile\n");

	FILE *fp;
	int lenPath;

	int sonarDistance; // for the string I'm going to read from file
	int angle;
	int i;
	int offset=0;

    fp = fopen(filenameOfSonarData,"r");

    if (fp == NULL){
        printf("Error! opening file");
        // Program exits if the file pointer returns NULL.
        return (-1);
    }


    i=0;
    while (fscanf(fp, "%d,%d,", &angle, &sonarDistance) == 2) // expect 2 successful conversion
    {
      // process buffer
//    	if(!i)
//    		offset=angle;

    	angle=angle-offset;
    	angleVect[i]=angle;
//    	printf("%d ",angle);
   		sonarDistanceVect[i]=(int) (sonarDistance/CM); // @mm->cm

    	i+=1;
    }
    if (feof(fp))
    {
      // hit end of file
    }
    else
    {
      // some other error interrupted the read
    }

    fclose(fp);

    return 0;
};


//GOOD
/*Load in the real data struct the test data read from the file*/
void loadDataInStruct(sensorData *pt_sDataDebug){
	printf("loadDataInStruct\n");
	int i;

	for(i=0;i<BUFFER_SIZE; i++){
		pt_sDataDebug->sonarDistance[i]=sonarDistanceVect[i];
		pt_sDataDebug->angle[i]=angleVect[i];
	}
	return;
};


void loadDataInStructMat(sensorData *pt_sDataDebug, float matrix[400][2]){

	printf("loadDataInStructMat\n");
	int i;

	for(i=0;i<400; i++){   /// VALORE DA SISTEMARE
		pt_sDataDebug->sonarDistance[i]=(int)matrix[i][1];
		pt_sDataDebug->angle[i]=(int)matrix[i][0];
		// printf("%d\n", (int)matrix[i][1]);
		//
		// if((int)matrix[i][0]>90 || (int)matrix[i][0]< -90){
		// 	pt_sDataDebug->sonarDistance[i]=0;
		// 	pt_sDataDebug->angle[i]=0;
		// }else{
		//
		// }
	}
	return;
};


void loadEnvMap(char* path,fieldData  *pt_fData){
	printf("printEnv\n");

	FILE *fp;
	int lenPath=strlen(path);
	int i,j;
	int valueToPrint;

	int x_line[X_DIM];

    char * line = NULL;
    size_t len = 0;
    ssize_t read;

	char filename[7] = "map.txt" ;
	char fullpath[strlen(filename)+lenPath];

	strncpy(fullpath, path , lenPath);
	fullpath[lenPath] = '/' ;

	strncpy(fullpath+lenPath+1, filename , strlen(filename));
	fullpath[strlen(filename)+lenPath+1]='\0';

	printf("%s\n",fullpath);
	fp=fopen(fullpath, "r");

    read = getline(&line, &len, fp);
	read = getline(&line, &len, fp);
	read = getline(&line, &len, fp);

	for(i=0;i<X_DIM;i++){
		for(j=0;j<Y_DIM-1;j++){
			sscanf(fp,"%d ",&valueToPrint);
			printf("%d",valueToPrint);

			pt_fData->field[i][j]=(char)valueToPrint;
		}
		sscanf(fp,"%d\n",valueToPrint);
		pt_fData->field[i][j]=(char)valueToPrint;

	}
	fclose(fp);
	return;

};


void fieldGenerator(fieldData  *pt_fData){


	_drawCircle((pt_fData->field), 30,10, 7, OBJECTS );
	_drawCircle((pt_fData->field), 50,10, 7, OBJECTS );
	_drawCircle((pt_fData->field), 10,80, 7, OBJECTS );


}
