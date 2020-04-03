/***
ALL THIS FILE HAS BEEN DEVELOPED BY ---> Andrea TRUFINI
EURECOM - OS
***/

#ifndef HEADER_H
#define HEADER_H

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define ROWS 101    // original dim is 201
#define COLUMNS 61  // original dim is 121

#define MATRIX_INIT_X COLUMNS/2
#define MATRIX_INIT_Y ROWS-ROBOT_HEIGHT/2-DISTANCE_FROM_BACK-1
#define MATRIX_INIT_ANGLE 0

typedef struct matrix_position{
    int x;
    int y;
    int rotation;
}matrixPosition;

#endif
