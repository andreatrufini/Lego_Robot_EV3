/***
ALL THIS FILE HAS BEEN DEVELOPED BY ---> Andrea TRUFINI
EURECOM - OS
***/

#include "matrix_navigation.h"
#include "callMapping.h"
#include "obj_detect.h"
#include "server.c"

struct localizationData lData;

extern float matrix_HScan[MATRIX_ROW][2]; // matrix used to save data of horizontal scanning
extern float matrix_VScan[MATRIX_ROW][2]; // matrix used to save data of vertical scanning

/*
  This function checks if, considering the actual position and the actual angle of the robot,
  in front of in there is an "empty space" equal to SECURE_DISTANCE in which it is possible to move.

  It also take into account that there should be also a secure distance on the left and right side
  of the robot, which is equal to SECURE_DISTANCE_LEFT_RIGHT.
*/
int checkSafeDistance(int map[ROWS][COLUMNS], matrixPosition *matrixPos){
    int i, j;
    int check=1;

    // scan the front of the robot depending on actual rotation and using a safe distance on left and right
    switch (matrixPos->rotation){
        case 0:
          for(j=0; j<SECURE_DISTANCE; j++){
            for(i = (matrixPos->x - ROBOT_WIDTH/2 - SECURE_DISTANCE_LEFT_RIGHT); i < (matrixPos->x + ROBOT_WIDTH/2 + SECURE_DISTANCE_LEFT_RIGHT); i++){
                if(map[matrixPos->y - ROBOT_HEIGHT/2 - j][i] == 1){
                    check=0;
                    return check;
                } else if(map[matrixPos->y - ROBOT_HEIGHT/2 - j][i] == 2){
                    check=2;
                    return check;
                }
            }
          }
          return check;
          break;

        case 90:
            if(COLUMNS - (matrixPos->x + ROBOT_HEIGHT/2) < SECURE_DISTANCE_FROM_BORDER){
                check=3;
                return check; // the function returns 2 if the robot is too near to the wall
            }

            for(j=0; j< SECURE_DISTANCE; j++){
              for(i = (matrixPos->y - ROBOT_WIDTH/2 - SECURE_DISTANCE_LEFT_RIGHT); i < (matrixPos->y + ROBOT_WIDTH/2 + SECURE_DISTANCE_LEFT_RIGHT); i++){
                  if(map[i][matrixPos->x + ROBOT_HEIGHT/2 + j] == 1){
                      check=0;
                      return check;
                  }
              }
            }
            return check;
            break;

        case 180:
            if(ROWS - (matrixPos->y + ROBOT_HEIGHT/2) < SECURE_DISTANCE_FROM_BORDER){
                check=3;
                return check; // the function returns 2 if the robot is too near to the wall
            }

            for(j=0; j< SECURE_DISTANCE; j++){
              for(i = (matrixPos->x - ROBOT_WIDTH/2 - SECURE_DISTANCE_LEFT_RIGHT); i < (matrixPos->x + ROBOT_WIDTH/2 + SECURE_DISTANCE_LEFT_RIGHT); i++){
                  if(map[matrixPos->y + ROBOT_HEIGHT/2 + j][i] == 1){
                      check=0;
                      return check;
                  }
              }
            }
            return check;
            break;

        case 270:
            if(matrixPos->x - ROBOT_HEIGHT/2 < SECURE_DISTANCE_FROM_BORDER){
                check=3;
                return check; // the function returns 2 if the robot is too near to the wall
            }

            for(j=0; j< SECURE_DISTANCE; j++){
              for(i = (matrixPos->y - ROBOT_WIDTH/2 - SECURE_DISTANCE_LEFT_RIGHT); i < (matrixPos->y + ROBOT_WIDTH/2 + SECURE_DISTANCE_LEFT_RIGHT); i++){
                  if(map[i][matrixPos->x - ROBOT_HEIGHT/2 - j] == 1){
                      check=0;
                      return check;
                  }
              }
            }
            return check;
            break;
    }
}

/*
  This function makes the robot move by 360° with a data acquisition with the sonar.
  After that the data are used to draw a real map.
  Some filter processing applied on the obtained map.
*/
void scanAndGenerateMap(int map[ROWS][COLUMNS], matrixPosition *matrixPos, generalArguments *general_arg){
    resetMatrix();  // reset the matrix with value of scanning
    refreshData(&lData);  // updates acquired data
    scanning(general_arg, RIGHT, 1);  // 360° robot scanning
    setPositionManually(&(lData.pData), matrixPos->x, ROWS-matrixPos->y, -matrixPos->rotation); // set the actual position of the robot
    loadDataInStructMat(&lData.sData, matrix_HScan); // load data of the vertical scanning
    refreshData(&lData);
    matrixcopyChar(map, &(lData.fData.field));  // copy the obtained map in the general map
    deletePoint(map); // filter points
    drawRobotPos(map, matrixPos); // draw the robot in the map
    drawOpponentField(map); // draw enemy field
    deleteCentralLine(map); // filter 1 on central line near the enemy field
    Sleep(1000);
}


/*
  It makes the robot do a counterclockwise tour of the field.
  It consider the possible objects and obstacles in the field and allows to the robot to automatically
  navigate in the field scanning objects and coming back to initial position.
*/
void firstTour(int map[ROWS][COLUMNS], matrixPosition *matrixPos, generalArguments *general_arg){

    char path[MAXPATHLENGTH];
    getcwd(path, sizeof(path));
    localizationInit(&(lData.fData),&(lData.pData));
    int i;

    scanAndGenerateMap(map, matrixPos, general_arg);

    rotateRight(map, matrixPos); // ROBOT: rotate right
    scanning(general_arg, RIGHT, 0);  // MATRIX: rotate right
    // printMap(map);
    // printf("\n\n\n");
    Sleep(1000);

    if(checkSafeDistance(map, matrixPos) == 1) { // if there are no objects in front of the robot
        matrixMoveForward(map, matrixPos);  // MATRIX: move forward
        move(general_arg, STRAIGHT, MATRIX_STEP*1.5); // ROBOT: move forward
        // printMap(map);
        // printf("\n\n\n");
    } else if(checkSafeDistance(map, matrixPos) == 0) { // if there is an object
        if(checkSafeDistance(map, matrixPos) == 0){
          obstacleNavigation(map, matrixPos, general_arg);
        }
    }

    while(matrixPos->x != MATRIX_INIT_X || matrixPos->y != MATRIX_INIT_Y) { // continue until the initial position is reached

        if (matrixPos->y == MATRIX_INIT_Y && matrixPos->rotation == 180){ // when the robot is coming back to the initial pos with 180 degrees rotation
            rotateLeft(map, matrixPos);
            scanning(general_arg, LEFT, 0);
            // printMap(map);
            // printf("\n\n\n");
            Sleep(1000);

            scanAndGenerateMap(map, matrixPos, general_arg);
        }

        if(checkSafeDistance(map, matrixPos) == 1) { // if there are no objects
            matrixMoveForward(map, matrixPos);  // MATRIX: move forward
            move(general_arg, STRAIGHT, MATRIX_STEP*1.5); // ROBOT: move forward
            // printMap(map);
            // printf("\n\n\n");
        } else if(checkSafeDistance(map, matrixPos) == 2 || checkSafeDistance(map, matrixPos) == 3) {
            rotateLeft(map, matrixPos); // always turn left if the front of the robot is too near to the wall or to the opposite field
            scanning(general_arg, LEFT, 0); // ROBOT: turn left
            // printMap(map);
            // printf("\n\n\n");
            Sleep(1000);

            scanAndGenerateMap(map, matrixPos, general_arg); // 360 scanning with new map generation
        } else if(checkSafeDistance(map, matrixPos) == 0) {
            obstacleNavigation(map, matrixPos, general_arg); // navigate around obstacles
        }
    }
    rotateLeft(map, matrixPos); // MATRIX: rotate left
    scanning(general_arg, LEFT, 0); // ROBOT: rotate left
    // printMap(map);
    // printf("\n\n\n");
    Sleep(1000);

    // printEnvMap(path,&(lData.fData)); // print map on file - DEBUG
}

/*
  Considering the actual position of the robot it determines the position of the found object.
*/
void determineObjPos(matrixPosition *matrixPos, int *x_ObjPos, int *y_ObjPos){

  switch(matrixPos->rotation){
    case 0:
      *x_ObjPos = 2*(matrixPos->x);
      *y_ObjPos = 2*((ROWS - matrixPos->y) + ROBOT_HEIGHT / 2 + SECURE_DISTANCE + SHAPE_DIM/2);
      break;
    case 90:
      *x_ObjPos = 2*(matrixPos->x + ROBOT_HEIGHT / 2 + SECURE_DISTANCE + SHAPE_DIM/2);
      *y_ObjPos = 2*(ROWS - matrixPos->y);
      break;
    case 180:
      *x_ObjPos = 2*(matrixPos->x);
      *y_ObjPos = 2*((ROWS - matrixPos->y) - ROBOT_HEIGHT / 2 - SECURE_DISTANCE - SHAPE_DIM/2);
      break;
    case 270:
      *x_ObjPos = 2*(matrixPos->x - ROBOT_HEIGHT / 2 - SECURE_DISTANCE - SHAPE_DIM/2);
      *y_ObjPos = 2*(ROWS - matrixPos->y);
      break;
  }
}

/*
  Considering the actual position and actual angle of the robot this function
  allows to navigate around the object doing vertical scanning, recognizing
  the shape of the scanned object.
*/
void obstacleNavigation(int map[ROWS][COLUMNS], matrixPosition *matrixPos, generalArguments *general_arg){
    int move_counter=0;
    int total_counter=0;
    int starting_rotation;
    int shape;
    int x_ObjPos, y_ObjPos;
    int cnt;

    starting_rotation = matrixPos->rotation;
    ObjectShape(general_arg);
    shape=determineShape(matrix_VScan);
    if(shape!=0){ // if the data obtained from the scanning corresponds to an object
      determineObjPos(matrixPos, &x_ObjPos, &y_ObjPos);
        if(x_ObjPos < 120 && y_ObjPos < 100){
          printf("X: %d, Y: %d", x_ObjPos, y_ObjPos);
          sendObject(0, x_ObjPos, y_ObjPos, shape);
        }
    }


    rotateLeft(map, matrixPos); // MATRIX: rotate left
    scanning(general_arg, LEFT, 0); // ROBOT: rotate left
    // printMap(map);
    // printf("\n\n\n");
    Sleep(1000);
    cnt=0;
    /* while the distance is not safe, i.e. until the robot doesn't have the
    space to continue the navigation, continue to do the vertical scanning
    */
    while(checkSafeDistance(map, matrixPos) == 1){
        if (cnt==3){
          cnt=0;
          scanAndGenerateMap(map, matrixPos, general_arg);
        }
        cnt++;

        move(general_arg, STRAIGHT, MATRIX_STEP*1.5);
        matrixMoveForward(map, matrixPos);
        // printMap(map);
        // printf("\n\n\n");
        rotateRight(map, matrixPos);
        scanning(general_arg, RIGHT, 0);
        Sleep(1000);
        ObjectShape(general_arg);
        shape=determineShape(matrix_VScan);
        if(shape!=0){
          determineObjPos(matrixPos, &x_ObjPos, &y_ObjPos);
          printf("X: %d, Y: %d", x_ObjPos, y_ObjPos);
        }
        sendObject(0, x_ObjPos, y_ObjPos, shape);

        // printMap(map);
        // printf("\n\n\n");
        total_counter++;
        if(checkSafeDistance(map, matrixPos) == 1){
            move_counter++;
            if(starting_rotation == 0){
                if(move_counter==2){
                    // to be improved - Condition to verify that the robot is able to turn of 180 degrees and come back
                    if(matrixPos->y - ROWS/2 > ROBOT_HEIGHT + SECURE_DISTANCE){
                        break;
                    }else {
                        // SONAR SCAN
                        ObjectShape(general_arg);
                        shape=determineShape(matrix_VScan);
                        if(shape!=0){
                          determineObjPos(matrixPos, &x_ObjPos, &y_ObjPos);
                          printf("X: %d, Y: %d", x_ObjPos, y_ObjPos);
                        }
                        sendObject(0, x_ObjPos, y_ObjPos, shape);

                        rotateLeft(map, matrixPos);
                        scanning(general_arg, LEFT, 0);
                        // printMap(map);
                        // printf("\n\n\n");
                        rotateLeft(map, matrixPos);
                        scanning(general_arg, LEFT, 0);
                        // printMap(map);
                        // printf("\n\n\n");
                        break;
                    }
                }
            } else if (starting_rotation == 90 || starting_rotation == 270){
                if(move_counter==1){
                    break;
                }
            } else if (starting_rotation == 180){
                if(move_counter == 2){
                    break;
                } else if (matrixPos->y == MATRIX_INIT_Y){
                    rotateLeft(map, matrixPos);
                    scanning(general_arg, LEFT, 0);
                    // printMap(map);
                    // printf("\n\n\n");
                    break;
                }
            }
        }else{
            if (matrixPos->y == MATRIX_INIT_Y && starting_rotation == 180){
                rotateLeft(map, matrixPos);
                scanning(general_arg, LEFT, 0);
                // printMap(map);
                // printf("\n\n\n");
                break;
            }
            rotateLeft(map, matrixPos);
            scanning(general_arg, LEFT, 0);
            // printMap(map);
            // printf("\n\n\n");
        }
    }

}
