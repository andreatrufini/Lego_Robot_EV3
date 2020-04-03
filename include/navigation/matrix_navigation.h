/***
ALL THIS FILE HAS BEEN DEVELOPED BY ---> Andrea TRUFINI
EURECOM - OS
***/

#ifndef NAVIGATION_TEST_MATRIX_NAVIGATION_H
#define NAVIGATION_TEST_MATRIX_NAVIGATION_H

#include "matrix_movements.h"
#include "navigation.h"

#define SECURE_DISTANCE 2*MATRIX_STEP+1
#define SECURE_DISTANCE_LEFT_RIGHT MATRIX_STEP+1
#define SECURE_DISTANCE_FROM_BORDER 6

void firstTour(int map[ROWS][COLUMNS], matrixPosition *matrixPos, generalArguments *general_arg);
int checkSafeDistance(int map[ROWS][COLUMNS], matrixPosition *matrixPos);
void obstacleNavigation(int map[ROWS][COLUMNS], matrixPosition *matrixPos, generalArguments *general_arg);
void determineObjPos(matrixPosition *matrixPos, int *x_ObjPos, int *y_ObjPos);
void scanAndGenerateMap(int map[ROWS][COLUMNS], matrixPosition *matrixPos, generalArguments *general_arg);

#endif //NAVIGATION_TEST_MATRIX_NAVIGATION_H
