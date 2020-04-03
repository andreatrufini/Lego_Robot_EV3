/***
ALL THIS FILE HAS BEEN DEVELOPED BY ---> Andrea TRUFINI
EURECOM - OS
***/

#ifndef DRAW_H
#define DRAW_H

#define ROBOT_WIDTH 7
#define ROBOT_HEIGHT 11

#define SHAPE_DIM 11

#define DISTANCE_FROM_BACK 5 // distance of the robot from the back "wall" of the field

#include "header.h"

void mapInitialization(int map[ROWS][COLUMNS]);
void drawOpponentField(int map[ROWS][COLUMNS]);
void drawRobotInit(int map[ROWS][COLUMNS]);
void drawSquare(int map[ROWS][COLUMNS], int x, int y);
void drawCircle(int map[ROWS][COLUMNS], float xpos, float ypos);
void printMap(int map[ROWS][COLUMNS]);
void drawRobotPos(int map[ROWS][COLUMNS], matrixPosition *matrixPos);
void deletePoint(int map[ROWS][COLUMNS]);
void deleteCentralLine(int map[ROWS][COLUMNS]);

#endif
