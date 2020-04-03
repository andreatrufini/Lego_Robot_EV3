/***
ALL THIS FILE HAS BEEN DEVELOPED BY ---> Andrea TRUFINI
EURECOM - OS
***/

#ifndef NAVIGATION_TEST_MATRIX_MOVEMENTS_H
#define NAVIGATION_TEST_MATRIX_MOVEMENTS_H

#include "header.h"
#include "draw.h"

#define MATRIX_STEP 3

void matrixcopy (int destmat[ROWS][COLUMNS], int srcmat[ROWS][COLUMNS]);
void matrixcopyChar (int destmat[ROWS][COLUMNS], char srcmat[COLUMNS][ROWS]);
void rotateRight(int map[ROWS][COLUMNS], matrixPosition *matrixPos);
void rotateLeft(int map[ROWS][COLUMNS], matrixPosition *matrixPos);
void rotate90Clockwise_from_0_180(int map[ROWS][COLUMNS], matrixPosition *matrixPos);
void rotate90Clockwise_from_90_270(int map[ROWS][COLUMNS], matrixPosition *matrixPos);
void matrixMoveForward(int map[ROWS][COLUMNS], matrixPosition *matrixPos);
void matrixMoveBackward(int map[ROWS][COLUMNS], matrixPosition *matrixPos);

#endif //NAVIGATION_TEST_MATRIX_MOVEMENTS_H
