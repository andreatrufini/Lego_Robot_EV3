/***
ALL THIS FILE HAS BEEN DEVELOPED BY ---> Andrea TRUFINI
EURECOM - OS
***/

/***
General REFERNCE SYSTEM:
                0°
                ^
                |
                |
                |
                |
 270° ---------------------> 90°
                |
                |
                |
                |
               180°
***/

#include "matrix_movements.h"

/*
  Custom function to copy a matrix into another one.
*/
void matrixcopy (int destmat[ROWS][COLUMNS], int srcmat[ROWS][COLUMNS]){
    int i, j;
    for (i=0; i<ROWS; i++)
        for (j=0; j<COLUMNS; j++)
            destmat[i][j] = srcmat[i][j];
}


/*
  Since the job has been divided in different parts done by different people,
  this function has been used to copy one matrix into another one, considering
  different reference systems.
*/
void matrixcopyChar (int destmat[ROWS][COLUMNS], char srcmat[COLUMNS][ROWS]){
    int i, j;
    for (i=0; i<ROWS; i++)
        for (j=0; j<COLUMNS; j++)
            destmat[ROWS-i][COLUMNS-j] = (int)srcmat[j][i];
}

/* Rotate clockwise the robot matrix in the map by 90 from the position 0° or 180° */
void rotate90Clockwise_from_0_180(int map[ROWS][COLUMNS], matrixPosition *matrixPos){

        int i,j, supp;
        int output[ROWS][COLUMNS];
        matrixcopy(output, map);

        for (i=matrixPos->y-ROBOT_HEIGHT/2; i<matrixPos->y+ROBOT_HEIGHT/2 +1; i++)
            for (j=matrixPos->x-ROBOT_WIDTH/2; j<matrixPos->x+ROBOT_WIDTH/2 +1; j++) {
                output[i][j] = 0;
            }

        for (i=matrixPos->y-ROBOT_HEIGHT/2; i<matrixPos->y+ROBOT_HEIGHT/2 +1; i++)
            for (j=matrixPos->x-ROBOT_WIDTH/2; j<matrixPos->x+ROBOT_WIDTH/2 +1; j++) {
                supp = map[i][j];
                output[matrixPos->y - matrixPos->x + j][matrixPos->x + matrixPos->y - i] = supp;
            }

        matrixcopy(map, output);
}

/* Rotate clockwise the robot matrix in the map by 90 from the position 90° or 270° */
void rotate90Clockwise_from_90_270(int map[ROWS][COLUMNS], matrixPosition *matrixPos){

    int i,j, supp;
    int output[ROWS][COLUMNS];
    matrixcopy(output, map);

    for (i=matrixPos->y-ROBOT_WIDTH/2; i<matrixPos->y+ROBOT_WIDTH/2 +1; i++)
        for (j=matrixPos->x-ROBOT_HEIGHT/2; j<matrixPos->x+ROBOT_HEIGHT/2 +1; j++) {
            output[i][j] = 0;
        }

    for (i=matrixPos->y-ROBOT_WIDTH/2; i<matrixPos->y+ROBOT_WIDTH/2 +1; i++)
        for (j=matrixPos->x-ROBOT_HEIGHT/2; j<matrixPos->x+ROBOT_HEIGHT/2 +1; j++) {
            supp = map[i][j];
            output[matrixPos->y - matrixPos->x + j][matrixPos->x + matrixPos->y - i] = supp;
        }

    matrixcopy(map, output);
}

/* General functions called to rotate the robot matrix clockwise by 90° */
void rotateRight(int map[ROWS][COLUMNS], matrixPosition *matrixPos){
    if (matrixPos->rotation==0 || matrixPos->rotation==180){
        rotate90Clockwise_from_0_180(map, matrixPos);
    }

    if (matrixPos->rotation==90 || matrixPos->rotation==270){
        rotate90Clockwise_from_90_270(map, matrixPos);
    }

    if(matrixPos->rotation < 270){
        matrixPos->rotation += 90;
    }else{
        matrixPos->rotation = 0; //adding 90 to 270 the result will be 360 which is equal to 0
    }

}

/* General functions called to rotate the robot matrix counterclockwise by 90° */
void rotateLeft(int map[ROWS][COLUMNS], matrixPosition *matrixPos){
    if (matrixPos->rotation==0 || matrixPos->rotation==180){
        rotate90Clockwise_from_0_180(map, matrixPos);
        rotate90Clockwise_from_90_270(map, matrixPos);
        rotate90Clockwise_from_0_180(map, matrixPos);
    }

    if (matrixPos->rotation==90 || matrixPos->rotation==270){
        rotate90Clockwise_from_90_270(map, matrixPos);
        rotate90Clockwise_from_0_180(map, matrixPos);
        rotate90Clockwise_from_90_270(map, matrixPos);
    }

    if(matrixPos->rotation > 0){
        matrixPos->rotation -= 90;
    }else{
        matrixPos->rotation = 270; //if the actual pos is 0, with the subtraction of 90, the angle will be 270
    }

}

/*
  Move the robot matrix forward with a step equal to MATRIX_STEP considering the
  actual position and the actual angle of the robot
*/
void matrixMoveForward(int map[ROWS][COLUMNS], matrixPosition *matrixPos){
    int i, j;
    int output[ROWS][COLUMNS];
    matrixcopy(output, map);

    switch(matrixPos->rotation){
        case 0:
            for (i=matrixPos->y-ROBOT_HEIGHT/2; i<matrixPos->y+ROBOT_HEIGHT/2 +1; i++)
                for (j=matrixPos->x-ROBOT_WIDTH/2; j<matrixPos->x+ROBOT_WIDTH/2 +1; j++) {
                    output[i][j] = 0;
                }

            for (i=matrixPos->y-ROBOT_HEIGHT/2; i<matrixPos->y+ROBOT_HEIGHT/2 +1; i++)
                for (j=matrixPos->x-ROBOT_WIDTH/2; j<matrixPos->x+ROBOT_WIDTH/2 +1; j++) {
                    //output[i][j] = 0;
                    output[i-MATRIX_STEP][j] = map[i][j];
                }

            matrixPos->y -= MATRIX_STEP;
            matrixcopy(map, output);
            break;

        case 90:
            for (i=matrixPos->y-ROBOT_WIDTH/2; i<matrixPos->y+ROBOT_WIDTH/2 +1; i++)
                for (j=matrixPos->x-ROBOT_HEIGHT/2; j<matrixPos->x+ROBOT_HEIGHT/2 +1; j++) {
                    output[i][j] = 0;
                }

            for (i=matrixPos->y-ROBOT_WIDTH/2; i<matrixPos->y+ROBOT_WIDTH/2 +1; i++)
                for (j=matrixPos->x-ROBOT_HEIGHT/2; j<matrixPos->x+ROBOT_HEIGHT/2 +1; j++) {
                    //output[i][j] = 0;
                    output[i][j+MATRIX_STEP] = map[i][j];
                }

            matrixPos->x += MATRIX_STEP;
            matrixcopy(map, output);
            break;

        case 180:
            for (i=matrixPos->y-ROBOT_HEIGHT/2; i<matrixPos->y+ROBOT_HEIGHT/2 +1; i++)
                for (j=matrixPos->x-ROBOT_WIDTH/2; j<matrixPos->x+ROBOT_WIDTH/2 +1; j++) {
                    output[i][j] = 0;
                }

            for (i=matrixPos->y-ROBOT_HEIGHT/2; i<matrixPos->y+ROBOT_HEIGHT/2 +1; i++)
                for (j=matrixPos->x-ROBOT_WIDTH/2; j<matrixPos->x+ROBOT_WIDTH/2 +1; j++) {
                    //output[i][j] = 0;
                    output[i+MATRIX_STEP][j] = map[i][j];
                }

            matrixPos->y += MATRIX_STEP;
            matrixcopy(map, output);
            break;

        case 270:
            for (i=matrixPos->y-ROBOT_WIDTH/2; i<matrixPos->y+ROBOT_WIDTH/2 +1; i++)
                for (j=matrixPos->x-ROBOT_HEIGHT/2; j<matrixPos->x+ROBOT_HEIGHT/2 +1; j++) {
                    output[i][j] = 0;
                }

            for (i=matrixPos->y-ROBOT_WIDTH/2; i<matrixPos->y+ROBOT_WIDTH/2 +1; i++)
                for (j=matrixPos->x-ROBOT_HEIGHT/2; j<matrixPos->x+ROBOT_HEIGHT/2 +1; j++) {
                    //output[i][j] = 0;
                    output[i][j-MATRIX_STEP] = map[i][j];
                }

            matrixPos->x -= MATRIX_STEP;
            matrixcopy(map, output);
            break;
    }
}

/*
  Move the robot matrix backward with a step equal to MATRIX_STEP considering the
  actual position and the actual angle of the robot
*/
void matrixMoveBackward(int map[ROWS][COLUMNS], matrixPosition *matrixPos){
    int i, j;
    int output[ROWS][COLUMNS];
    matrixcopy(output, map);

    switch(matrixPos->rotation){
        case 0:
            for (i=matrixPos->y-ROBOT_HEIGHT/2; i<matrixPos->y+ROBOT_HEIGHT/2 +1; i++)
                for (j=matrixPos->x-ROBOT_WIDTH/2; j<matrixPos->x+ROBOT_WIDTH/2 +1; j++) {
                    output[i][j] = 0;
                    output[i+MATRIX_STEP][j] = map[i][j];
                }

            matrixPos->y += MATRIX_STEP;
            matrixcopy(map, output);
            break;

        case 90:
            for (i=matrixPos->y-ROBOT_WIDTH/2; i<matrixPos->y+ROBOT_WIDTH/2 +1; i++)
                for (j=matrixPos->x-ROBOT_HEIGHT/2; j<matrixPos->x+ROBOT_HEIGHT/2 +1; j++) {
                    output[i][j] = 0;
                    output[i][j-MATRIX_STEP] = map[i][j];
                }

            matrixPos->x -= MATRIX_STEP;
            matrixcopy(map, output);
            break;

        case 180:
            for (i=matrixPos->y-ROBOT_HEIGHT/2; i<matrixPos->y+ROBOT_HEIGHT/2 +1; i++)
                for (j=matrixPos->x-ROBOT_WIDTH/2; j<matrixPos->x+ROBOT_WIDTH/2 +1; j++) {
                    output[i][j] = 0;
                    output[i-MATRIX_STEP][j] = map[i][j];
                }

            matrixPos->y -= MATRIX_STEP;
            matrixcopy(map, output);
            break;

        case 270:
            for (i=matrixPos->y-ROBOT_WIDTH/2; i<matrixPos->y+ROBOT_WIDTH/2 +1; i++)
                for (j=matrixPos->x-ROBOT_HEIGHT/2; j<matrixPos->x+ROBOT_HEIGHT/2 +1; j++) {
                    output[i][j] = 0;
                    output[i][j+MATRIX_STEP] = map[i][j];
                }

            matrixPos->x += MATRIX_STEP;
            matrixcopy(map, output);
            break;
    }
}
