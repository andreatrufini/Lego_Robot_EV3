/*
 * draw_funct.c
 *
 *  Created on: 9 gen 2020
 *      Author: ironmath
 */


/*
 * DEVO FAR IN MODO DI STAMPARE ANCHE SE X2 E Y2 SONO FUORI DAL CAMPO
 * SE SONO FUORI DAL CAMPO STAMPO I PUNTI DENTRO E NON QUELLI FUORI
 * E POI AGGIUNGO UNA FUNZIONE CHE AGGIUNGE SPAZIO OCCUPATO DIETRO AI PUNTI DOVE TROVO UN OSTACOLO
 * */


#include <math.h>
#include <stdlib.h>

#include "draw_funct.h" // To include all the define that I need



// X_DIM Y_DIM
/* This Function aims to draw a point on frame buffer considering Point=(x,y)
 * It is not the mode actually but the value we want to write
 * 	UNEXPLORED 0 FREE 1 OBJECTS 2 ENEMY_FIELD 3
 */

//n-->j --> y Y_DIM
//m-->i --> x X_DIM

static void __drawPoint(char field[X_DIM][Y_DIM] ,int x,int y,int typeOfField){
		if ((x<0 || x>X_DIM  )||(  y<0 || y>Y_DIM )){  //uno o zero
				return ; // doesn't care about error
		}
//		printf("__drawPoint\n");
//		field[x][y]=field[x][y]+(char) typeOfField;
		field[x][y]=(char) typeOfField;
		return;
}

//
///* This Function aims to draw a line on frame buffer considering Point1=(x1,y1)
// * and Point2=(x2,y2) in order to connect them to create
// * a line, in 'm' mode: CLR means write 0, SET means write 1, XOR means invert bits.
// * Function modified by https://gist.github.com/bert/1085538 */
void _drawLine(char field[X_DIM][Y_DIM],int x1,int y1,int x2,int y2, int mode){

//	printf("_drawLine\n");

	//Check that it is not possible to "paint" the enviroment out of the border
//	if ((x1<0 || x1>X_DIM  )||( x2<0 || x2>X_DIM )||(  y1<0 || y1>Y_DIM )||( y2<0 || y2>Y_DIM )){  //uno o zero
//			return ; // doesn't care about error
//	}


	int dx =  abs (x1 - x2), sx = x2 < x1 ? 1 : -1;
	int dy = -abs (y1 - y2), sy = y2 < y1 ? 1 : -1;
	int err = dx + dy, e2; /* error value e_xy */
	int exitFlag=1;

	__drawPoint(field,x2,y2,OBJECTS);

	while(exitFlag){  /* loop */

		if ((x2 == x1 && y2 == y1) ||(abs(x2)>X_DIM*2 || abs(y2)>X_DIM*2) ) //Can be improved
			exitFlag=0;
		else{
			e2 = 2 * err;
			if (e2 >= dy) { err += dy; x2 += sx; } /* e_xy+e_x > 0 */
			if (e2 <= dx) { err += dx; y2 += sy; } /* e_xy+e_y < 0 */
		}

		if(mode == DRAWFREESPACE){
			__drawPoint(field,x2,y2,FREE);

		}else if(mode==DRAWOCCUPIEDSPACE){
			__drawPoint(field,x2,y2,OBJECTS);
		}

	}


	return ;
}


void _drawPolarLine(char field[X_DIM][Y_DIM],int x1,int y1,int rho,int thetaDeg, int mode){

//	printf("_drawLine\n");

	//Check that it is not possible to "paint" the enviroment out of the border
//	if ((x1<0 || x1>X_DIM  )||( x2<0 || x2>X_DIM )||(  y1<0 || y1>Y_DIM )||( y2<0 || y2>Y_DIM )){  //uno o zero
//			return ; // doesn't care about error
//	}
	int x2=  x1 + rho*cos(degToRad(thetaDeg));
	int y2=  y1 + rho*sin(degToRad(thetaDeg));


	int dx =  abs (x1 - x2), sx = x2 < x1 ? 1 : -1;
	int dy = -abs (y1 - y2), sy = y2 < y1 ? 1 : -1;
	int err = dx + dy, e2; /* error value e_xy */
	int exitFlag=1;

	__drawPoint(field,x2,y2,OBJECTS);

	while(exitFlag){  /* loop */

		if ((x2 == x1 && y2 == y1) ||(abs(x2)>X_DIM*2 || abs(y2)>X_DIM*2) ) //Can be improved
			exitFlag=0;
		else{
			e2 = 2 * err;
			if (e2 >= dy) { err += dy; x2 += sx; } /* e_xy+e_x > 0 */
			if (e2 <= dx) { err += dx; y2 += sy; } /* e_xy+e_y < 0 */
		}

		if(mode == DRAWFREESPACE){
			__drawPoint(field,x2,y2,FREE);

		}else if(mode==DRAWOCCUPIEDSPACE){
			__drawPoint(field,x2,y2,OBJECTS);
		}

	}


	return ;
}



void _drawTriangle(char field[X_DIM][Y_DIM], int x_fixed, int y_fixed ,int x1,int y1,int x2,int y2){

//	printf("_drawLine\n");


	//Check that it is not possible to "paint" the enviroment out of the border
//	if ((x1<0 || x1>X_DIM  )||( x2<0 || x2>X_DIM )||( x_fixed<0 || x_fixed>X_DIM )||(  y1<0 || y1>Y_DIM )||( y2<0 || y2>Y_DIM ) || ( y_fixed<0 || y_fixed>Y_DIM )){  //uno o zero
//			return ; // doesn't care about error
//	}

	/* Draw the free space */
	_drawLine(field,x_fixed, y_fixed,x1,y1, DRAWFREESPACE);
	_drawLine(field,x_fixed, y_fixed,x2,y2, DRAWFREESPACE);

	/* Draw the occupied space*/
	_drawLine(field,x1, y1,x2,y2, DRAWOCCUPIEDSPACE);


	int dx =  abs (x1 - x2), sx = x2 < x1 ? 1 : -1;
	int dy = -abs (y1 - y2), sy = y2 < y1 ? 1 : -1;
	int err = dx + dy, e2; /* error value e_xy */
	int exitFlag=1;

	__drawPoint(field,x2,y2,OBJECTS);



	while(exitFlag){  /* loop */

//		cnt+=1;
		if ((x2 == x1 && y2 == y1)||(abs(x2)>X_DIM*2 || abs(y2)>X_DIM*2) ) //Can be improved
			exitFlag=0;
		else{
			e2 = 2 * err;
			if (e2 >= dy) { err += dy; x2 += sx; } /* e_xy+e_x > 0 */
			if (e2 <= dx) { err += dx; y2 += sy; } /* e_xy+e_y < 0 */
		}

		_drawLine(field,x_fixed, y_fixed,x2,y2, DRAWFREESPACE);


	}


	return ;
}


void _drawPolarTriangleColor(char field [X_DIM][Y_DIM],int mmToAdd, int x_fixed, int y_fixed ,int rho1,int thetaDeg1,int rho2,int thetaDeg2,int mode){


	int x1=  x_fixed + (rho1+mmToAdd)*cos(degToRad(thetaDeg1));
	int y1=  y_fixed + (rho1+mmToAdd)*sin(degToRad(thetaDeg1));

	int x2=  x_fixed + (rho2+mmToAdd)*cos(degToRad(thetaDeg2));
	int y2=  y_fixed + (rho2+mmToAdd)*sin(degToRad(thetaDeg2));



//	/* Draw the free space */
//	_drawLine(field,x_fixed, y_fixed,x1_near,y1_near, DRAWFREESPACE);
//	_drawLine(field,x_fixed, y_fixed,x2_near,y2_near, DRAWFREESPACE);
//
//	_drawLine(field,x_fixed, y_fixed,x1_near,y1_near, DRAWFREESPACE);
//	_drawLine(field,x_fixed, y_fixed,x2_near,y2_near, DRAWFREESPACE);
//
//	/* Draw the occupied space*/
//	_drawLine(field,x1, y1,x2,y2, DRAWOCCUPIEDSPACE);


	int dx =  abs (x1 - x2), sx = x2 < x1 ? 1 : -1;
	int dy = -abs (y1 - y2), sy = y2 < y1 ? 1 : -1;
	int err = dx + dy, e2; /* error value e_xy */
	int exitFlag=1;

	//TO BE IMPROVED
	if(mode == DRAWFREESPACE){
		__drawPoint(field,x2,y2,FREE);

	}else if(mode==DRAWOCCUPIEDSPACE){
		__drawPoint(field,x2,y2,OBJECTS);
	}



	while(exitFlag){  /* loop */

//		cnt+=1;
		if ((x2 == x1 && y2 == y1)||(abs(x2)>X_DIM*2 || abs(y2)>X_DIM*2) ) //Can be improved
			exitFlag=0;
		else{
			e2 = 2 * err;
			if (e2 >= dy) { err += dy; x2 += sx; } /* e_xy+e_x > 0 */
			if (e2 <= dx) { err += dx; y2 += sy; } /* e_xy+e_y < 0 */
		}

		_drawLine(field,x_fixed, y_fixed,x2,y2, mode);


	}


	return ;



}


void _drawSquare(char field[X_DIM][Y_DIM], int x1, int y1 ,int x2,int y2, int mode){

//	printf("_drawSquare\n");


	//Check that it is not possible to "paint" the enviroment out of the border
//	if ((x1<0 || x1>X_DIM  )||( x2<0 || x2>X_DIM )||( x_fixed<0 || x_fixed>X_DIM )||(  y1<0 || y1>Y_DIM )||( y2<0 || y2>Y_DIM ) || ( y_fixed<0 || y_fixed>Y_DIM )){  //uno o zero
//			return ; // doesn't care about error
//	}
	int i,j,tmp_x,tmp_y,dx;

	if(x1<x2){
		tmp_x=x1;
		tmp_y=y1;

		x1=x2;
		y1=y2;

		x2=tmp_x;
		y2=tmp_y;
	}
	dx=x1-x2;

	/* Draw the occupied space */
	for (i=0;i<dx;i++){
		 _drawLine(field, x1+i, y1, x1+i, y2, mode);
	}


	return ;
}


static void __drawPlot4points (char field[X_DIM][Y_DIM], int cx, int cy, int x, int y, int typeOfField){
	// The '(x != 0 && y != 0)' test in the last line of this function
	// may be omitted for a performance benefit if the radius of the
	// circle is known to be non-zero.

  __drawPoint (field, cx + x, cy + y, typeOfField);
  if (x != 0) __drawPoint(field, cx - x, cy + y, typeOfField);
  if (y != 0) __drawPoint(field, cx + x, cy - y, typeOfField);
  if (x != 0 && y != 0) __drawPoint(field, cx - x, cy - y, typeOfField);
}


static void __drawPlot8points (char field[X_DIM][Y_DIM],int cx, int cy, int x, int y, int typeOfField){

	__drawPlot4points(field, cx, cy, x, y,typeOfField);
  if (x != y) __drawPlot4points(field, cx, cy, y, x,typeOfField);
}


// 'cx' and 'cy' denote the offset of the circle centre from the origin.
void _drawCircle (char field[X_DIM][Y_DIM],int cx, int cy, int radius, int typeOfField){

  int error = -radius;
  int x = radius;
  int y = 0;

  // The following while loop may altered to 'while (x > y)' for a
  // performance benefit, as long as a call to 'plot4points' follows
  // the body of the loop. This allows for the elimination of the
  // '(x != y') test in 'plot8points', providing a further benefit.
  //
  // For the sake of clarity, this is not shown here.
  while (x >= y)
  {
	__drawPlot8points(field, cx, cy, x, y,typeOfField);
    error += y;
    ++y;
    error += y;
    // The following test may be implemented in assembly language in
    // most machines by testing the carry flag after adding 'y' to
    // the value of 'error' in the previous step, since 'error'
    // nominally has a negative value.
    if (error >= 0)
    {
      --x;
      error -= x;
      error -= x;
    }
  }
}


void _filledCircle (char field[X_DIM][Y_DIM] ,int cx, int cy, int radius, int mode){
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x = 0;
	int y = radius;


	_drawLine(field,cx, cy + radius,cx, cy - radius,mode);
	_drawLine(field,cx+ radius, cy ,cx- radius, cy ,mode);

	while (x < y)	{
		// ddF_x == 2 * x + 1;
		// ddF_y == -2 * y;
		// f == x*x + y*y - radius*radius + 2*x - y + 1;
		if (f >= 0) {
		  y--;
		  ddF_y += 2;
		  f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		_drawLine(field,cx - x, cy + y,cx + x, cy + y,mode);
		_drawLine(field,cx - x, cy - y,cx + x, cy - y,mode);
		_drawLine (field,cx - y, cy + x,cx + y, cy + x,mode);
		_drawLine (field,cx - y, cy - x,cx + y, cy - x,mode);

	}
}







/* EOF */
