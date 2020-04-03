/*
 * draw_funct.h
 *
 *  Created on: 9 gen 2020
 *      Author: ironmath
 */

#ifndef DRAW_FUNCT_H_
#define DRAW_FUNCT_H_

#include "config.h"
#include "angles_funct.h"

//void _drawPoint(char field [X_DIM][Y_DIM],int x,int y,int typeOfField);
//
void _drawLine(char field [X_DIM][Y_DIM],int x1,int y1,int x2,int y2, int mode);

void _drawPolarLine(char field[X_DIM][Y_DIM],int x1,int y1,int rho,int thetaDeg, int mode);

void _drawTriangle(char field [X_DIM][Y_DIM], int x_fixed, int y_fixed ,int x1,int y1,int x2,int y2);

void _drawPolarTriangleColor(char field [X_DIM][Y_DIM],int mmToAdd, int x_fixed, int y_fixed ,int rho1,int thetaDeg1,int rho2,int thetaDeg2,int mode);

void _drawSquare(char field[X_DIM][Y_DIM], int x1, int y1 ,int x2,int y2, int mode);

void _filledCircle (char field[X_DIM][Y_DIM] ,int cx, int cy, int radius, int typeOfField);

//

#endif /* DRAW_FUNCT_H_ */
