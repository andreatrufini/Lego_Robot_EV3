/*
 * angles_funct.c
 *
 *  Created on: 9 gen 2020
 *      Author: ironmath
 */
#include "angles_funct.h"

//GOOD
/* convert degree to radians */
double degToRad(double deg){

//	printf("_degToRad\n");

	double rad;
	if (deg<0){
			deg=deg+360;
	}else if(deg>360){
		deg=deg-360;
	}
	return rad=deg/180*PI;
}

double radToDeg(double rad){

//	printf("_degToRad\n");

	double deg;
	deg=rad/PI*180;

	if (deg<0){
		deg=deg+360;
	}else if(deg>360){
		deg=deg-360;
	}
	return deg;
}
