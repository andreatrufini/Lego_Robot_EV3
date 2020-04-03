/*
 * obj_detect.h
 *
 *  Created on: 15 dic 2019
 *      Author: ironmath
 *
 *1 trovare la posizione di N oggetti
 *		a) Disegnare un cerchio di protezione attorno a ognuno di essi
 *2 avvicinarsi fino a un cm
 *3 trovare l'altezza
 *4 classificare in base all'altezza in 3 classi:
 *      a) Cilindro
 *      b) Cubo
 *      c) Piramide rovesciata
 *      d) Piramidi
 *
 *
 */

#ifndef OBJ_DETECT_H_
#define OBJ_DETECT_H_


#define UNIDENTIFIED 0
#define CUBE 		1
#define CILINDER 	6
#define PIRAMID4 	2
#define PIRAMID3 	3
#define PIRAMID4_UD	4
#define PIRAMID3_UD	5

#include <time.h>
#include <stdlib.h>
#include "loc_funct.h"
#include "config.h"

// Cube 		footprint 15cm*15cm
// Piramid4 	footprint 15cm*15cm
// Piramid3 	footprint
// Cilinder 	footprint


int determineShape(float datas[400][2]);


void findObjPosition(fieldData *pt_fData, int num );

int isThereObj(fieldData *pt_fData);


#endif /* OBJ_DETECT_H_ */
