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


#include "draw.h"

/* Initialization of the map with all 0 */
void mapInitialization(int map[ROWS][COLUMNS]){
    int i,j;

    for(i=0; i<ROWS; i++){
        for(j=0; j<COLUMNS; j++) {
            map[i][j] = 0;
        }
    }
}

/* Draw enemy field with 2 */
void drawOpponentField(int map[ROWS][COLUMNS]){
    int i,j;

    for(i=0; i<ROWS/2; i++){
        for(j=0; j<COLUMNS; j++) {
            map[i][j] = 2;
        }
    }
}

/*
Print the map using different color to indicate the different componets:
  - 1: objects - blue;
  - 2: enemy field - red;
  - 3: robot - green;
  - 4: front of the robot - yellow;
*/
void printMap(int map[ROWS][COLUMNS]){
    int i,j;

    for(i=0; i<ROWS; i++){
        for(j=0; j<COLUMNS; j++){
            if(map[i][j]==1)
                printf(ANSI_COLOR_BLUE "%d " ANSI_COLOR_RESET, map[i][j]);
            else if(map[i][j]==2)
                printf(ANSI_COLOR_RED "%d " ANSI_COLOR_RESET, map[i][j]);
            else if(map[i][j]==3)
                printf(ANSI_COLOR_GREEN "%d " ANSI_COLOR_RESET, map[i][j]);
            else if(map[i][j]==4)
                printf(ANSI_COLOR_YELLOW "%d " ANSI_COLOR_RESET, map[i][j]);
            else
                printf("%d ", map[i][j]);

        }
        printf("\n");
    }
}

/* Draw the robot in the map in the initial position */
void drawRobotInit(int map[ROWS][COLUMNS]) {
    int i, j;

    for (i=ROWS-ROBOT_HEIGHT-DISTANCE_FROM_BACK; i<ROWS-DISTANCE_FROM_BACK; i++) {
        for (j = COLUMNS / 2 - ROBOT_WIDTH / 2; j < COLUMNS / 2 - ROBOT_WIDTH / 2 + ROBOT_WIDTH; j++) {
            map[i][j] = 3;
        }
    }

    // draw the front of the robot with a different number, i.e. 4
    i = ROWS-ROBOT_HEIGHT-DISTANCE_FROM_BACK;
    for (j = COLUMNS / 2 - ROBOT_WIDTH / 2; j < COLUMNS / 2 - ROBOT_WIDTH / 2 + ROBOT_WIDTH; j++) {
        map[i][j] = 4;
    }
}

/* It allows to delete a 1 surrounded by 0 - It behaves as a filter and it allows to have a more cleaned map */
void deletePoint(int map[ROWS][COLUMNS]){
  int i,j;

  for(i=1; i<ROWS-1; i++){
    for(j=1; j<COLUMNS-1; j++){
        if(map[i][j]==1 && map[i-1][j-1]==0 && map[i-1][j]==0 && map[i-1][j+1]==0 && map[i][j-1]==0 &&
           map[i][j+1]==0 && map[i+1][j-1]==0 && map[i+1][j]==0 && map[i+1][j+1]==0){
             map[i][j]=0;
        }
    }
  }
}

/*
  It draws the robot in the map considering the actual position and actual angle
  Used to print in real-time the robot on the map
*/
void drawRobotPos(int map[ROWS][COLUMNS], matrixPosition *matrixPos){
  int i,j;

  switch (matrixPos->rotation){
    case 0:
      for (i=matrixPos->y - ROBOT_HEIGHT/2; i<matrixPos->y + ROBOT_HEIGHT/2; i++) {
          for (j = matrixPos->x - ROBOT_WIDTH / 2; j < matrixPos->x + ROBOT_WIDTH/2 +1; j++) {
              map[i][j] = 3;
          }
      }

      // draw the front of the robot with a different number
      i = matrixPos->y - ROBOT_HEIGHT/2;
      for (j = matrixPos->x - ROBOT_WIDTH / 2; j < matrixPos->x - ROBOT_WIDTH / 2 + ROBOT_WIDTH; j++) {
          map[i][j] = 4;
      }
      break;

    case 90:
      for (i=matrixPos->y-ROBOT_WIDTH/2; i<matrixPos->y+ROBOT_WIDTH/2; i++) {
          for (j = matrixPos->x - ROBOT_HEIGHT/2; j < matrixPos->x + ROBOT_HEIGHT/2; j++) {
              map[i][j] = 3;
          }
      }

      // draw the front of the robot with a different number
      j = matrixPos->x + ROBOT_HEIGHT/2;
      for (i = matrixPos->y -ROBOT_WIDTH/2 ; i < matrixPos->x + ROBOT_WIDTH/2; i++) {
          map[i][j] = 4;
      }
      break;

    case 180:
      for (i=matrixPos->y - ROBOT_HEIGHT/2; i<matrixPos->y + ROBOT_HEIGHT/2; i++) {
          for (j = matrixPos->x - ROBOT_WIDTH / 2; j < matrixPos->x + ROBOT_WIDTH/2 + 1; j++) {
              map[i][j] = 3;
          }
      }

      // draw the front of the robot with a different number
      i = matrixPos->y + ROBOT_HEIGHT/2;
      for (j = matrixPos->x - ROBOT_WIDTH / 2; j < matrixPos->x - ROBOT_WIDTH / 2 + ROBOT_WIDTH; j++) {
          map[i][j] = 4;
      }
      break;

    case 270:
      for (i=matrixPos->y-ROBOT_WIDTH/2; i<matrixPos->y+ROBOT_WIDTH/2; i++) {
          for (j = matrixPos->x - ROBOT_HEIGHT/2; j < matrixPos->x + ROBOT_HEIGHT/2; j++) {
              map[i][j] = 3;
          }
      }

      // draw the front of the robot with a different number
      j = matrixPos->x - ROBOT_HEIGHT/2;
      for (i = matrixPos->y -ROBOT_WIDTH/2 ; i < matrixPos->x + ROBOT_WIDTH/2; i++) {
          map[i][j] = 4;
      }
      break;
  }

}

/*
  It deletes the central line near the enemy field writing 0.
  It is useful because with the mapping of the field some 1 could be written near the line of
  the enemy field and than the navigation alghoritm could interpret these as the presence of an object.
*/
void deleteCentralLine(int map[ROWS][COLUMNS]){
  int i;

  for(i=0; i<COLUMNS; i++){
    map[ROWS/2+1][i]=0;
    map[ROWS/2+2][i]=0;
  }
}

/*
  Draw a square in the map in X,Y
  Used just to test the navigation alghoritm.
*/
void drawSquare(int map[ROWS][COLUMNS], int x, int y){
    int i,j;

    for(i=x; i< x + SHAPE_DIM; i++){
        for(j=y; j< y + SHAPE_DIM; j++){
            map[i][j]=1;
        }
    }

}

/*
  Draw a circle in the map in X,Y
  Used just to test the navigation alghoritm.
*/
void drawCircle(int map[ROWS][COLUMNS], float xpos, float ypos){
    int i,j;
    int matsize=SHAPE_DIM+1;
    float x = 0, y = 0, radius = 0, rsqrd = 0, rcheck = 0;

    radius=SHAPE_DIM/2;
    rsqrd = radius*radius; //rsqrd is equal to radius squared.
    x -= (matsize/2); //with this I make sure that the x and y values start from the top right corner of the matrix, so that each x, y value corresponds to the correct cell position (i, j)
    y = matsize/2;

    for(i = xpos; i < xpos+matsize; i++)
    {
        for(j = ypos; j < ypos+matsize; j++)
        {
            rcheck = ((y)*(y)) + ((x)*(x)); // calculating the equation of the circle with the x and y values taking the offset into account
            if(rcheck <= rsqrd)
            {
                map[i][j] = 1;
            }

            x = x+1; //stepping the values of x and y so they stay with the corresponding cell
        }
        y = y-1;
        x -= matsize ;   // <-- Reset x to start of row
    }
}
