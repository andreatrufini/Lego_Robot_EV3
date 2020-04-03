/*
 * draw_funct.h
 *
 *  Created on: 9 gen 2020
 *      Author: ironmath
 */

#ifndef DRAW_FUNCT_H_
#define DRAW_FUNCT_H_

#define X_DIM 1201 //mm
#define Y_DIM 2001 //mm

#define LINE 0 // 1--> 10cm // 2 --> longline
#define TOPRINT 1

#if (TOPRINT==1)
	#define UNEXPLORED 0 	// 100
	#define FREE 80  		// 0
	#define OBJECTS 0 		// 100
	#define	ENEMY_FIELD 40 	// 0

#else


	#define UNEXPLORED 1//100 	// 0
	#define FREE  0//80  		// 0
	#define OBJECTS 1//0 		// 100
	#define	ENEMY_FIELD 2//40 	// 0

#endif

//void _drawPoint(fieldData *pt_fData,int x,int y,int typeOfField);
////
//void _drawLine(fieldData *pt_fData,int x1,int y1,int x2,int y2, int mode);
//
//void _drawTriangle(fieldData *pt_fData, int x_fixed, int y_fixed ,int x1,int y1,int x2,int y2);
//

#endif /* DRAW_FUNCT_H_ */
