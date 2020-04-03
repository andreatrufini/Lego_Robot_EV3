/*
 * debugFunction.h
 *
 *  Created on: 24 nov 2019
 *      Author: ironmath
 */

#ifndef DEBUGFUNCTIONS_H_
#define DEBUGFUNCTIONS_H_


#define BUFFER_SIZE 1000


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "loc_funct.h"
#include "config.h"


// Print the environment in file with a matrix format
void printEnvMap(char* path,fieldData  *pt_fData);

void loadEnvMap(char* path,fieldData  *pt_fData);

// read the file to load sensor data
int readTestDataFile(char *filenameOfSonarData);

// after having loaded the demo data in a temporary vector,
// it load the sensor data in the real structure
void loadDataInStruct(sensorData* pt_sDataDebug);

void loadDataInStructMat(sensorData *pt_sDataDebug, float matrix[400][2]);



#endif /* DEBUGFUNCTIONS_H_ */
