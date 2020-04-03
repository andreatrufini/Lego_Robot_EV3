/*
 * callMapping.c
 *
 *  Created on: 9 gen 2020
 *      Author: ironmath
 */



#include "callMapping.h"
struct localizationData lData;
float matrix[400][2]={0};

void callMapping(){

	char path[MAXPATHLENGTH];
	getcwd(path, sizeof(path));
	localizationInit(&(lData.fData),&(lData.pData));

	//printEnvMap(path,&(lData.fData));
//	loadEnvMap(path,&(lData.fData));



//CUBO
//	SIMULO la ricezione di dati       x   y  theta
	refreshData(&lData); /// #########################################


	//readTestDataFile("/home/ironmath/git/legooflegend/demo_maps/measure2/output");
	//fieldGenerator(&lData.fData); //to modify the field and add object


	setPositionManually(&(lData.pData),X_START,Y_START,0); //ROVER
	readTestDataFile("/home/ironmath/git/legooflegend/demo_maps/measure2/3602/output0");
	loadDataInStructMat(&lData.sData, matrix[400][2]){
	loadDataInStruct(&lData.sData); //load sensor data
	refreshData(&lData);
//
//
	setPositionManually(&(lData.pData),X_START+34/2,Y_START,0); //ROVER
	readTestDataFile("/home/ironmath/git/legooflegend/demo_maps/measure2/3602/output1");
	loadDataInStruct(&lData.sData); //load sensor data
	refreshData(&lData);

	setPositionManually(&(lData.pData),X_START+34/2,Y_START+27/2,0); //ROVER
	readTestDataFile("/home/ironmath/git/legooflegend/demo_maps/measure2/3602/output2");
	loadDataInStruct(&lData.sData); //load sensor data
	refreshData(&lData);
////
	setPositionManually(&(lData.pData),X_START+34/2,Y_START+(27+27)/2,0); //ROVER
	readTestDataFile("/home/ironmath/git/legooflegend/demo_maps/measure2/3602/output3");
	loadDataInStruct(&lData.sData); //load sensor data
	refreshData(&lData);



//	loadDataInStructMat(&lData.sData); //load sensor data



	findObjPosition(&(lData.fData), 1 );

	printEnvMap(path,&(lData.fData));

};
