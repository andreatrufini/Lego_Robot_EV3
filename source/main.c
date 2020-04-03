/***
ALL THIS FILE HAS BEEN DEVELOPED BY ---> Andrea TRUFINI
EURECOM - OS
***/

#include "sensors.h"
#include "motors.h"
#include "navigation.h"

// MATTEO
#include "debugFunctions.h"
#include "loc_funct.h"
#include "obj_detect.h"

// ANDREA
#include "draw.h"
#include "header.h"
#include "matrix_movements.h"
#include "matrix_navigation.h"
#include "general.h"

#define MAXPATHLENGTH 1000

sem_t sem_RT, sem_LT, sem_GYRO; // semaphores used for the right and left motors and for the gyroscope

matrixPosition matrixPos; // struct with robot matrix data

float matrix_VScan[MATRIX_ROW][2]; // TORRES: matrix in which the data of vertical scanning are saved
float matrix_HScan[MATRIX_ROW][2];  // TORRES: matrix in which the data of horizontal scanning are saved

struct localizationData lData;

static bool _check_pressed( uint8_t sn ) {
 	int val;

 	if ( sn == SENSOR__NONE_ ) {
		return ( ev3_read_keys(( uint8_t *) &val ) && ( val & EV3_KEY_UP ));
	}
	return ( get_sensor_value( 0, sn, &val ) && ( val != 0 ));
}


/*** MAIN ***/
int main( int argv, char **argc )
{
	uint8_t motor_RT, motor_LT, motor_SONAR;
	uint8_t sn_light_sensor;

	pthread_t thread_sendMatrix;
	pthread_t thread_gyroscope;
	pthread_t thread_light_sensor;

  int i;

  //GENERAL STRUCTURE DEFINITION
  generalArguments *generalArg = malloc (sizeof (generalArguments));

  int map[ROWS][COLUMNS]; // Definition of general map

  /* Initialization of first robot position and angle*/
  matrixPos.x = MATRIX_INIT_X;
  matrixPos.y = MATRIX_INIT_Y;
  matrixPos.rotation = MATRIX_INIT_ANGLE;

	//RIGHT MOTOR INITIALIZATION
	generalArg->motorArgRT.motor_PORT = OUTPUT_B;
	generalArg->motorArgRT.forward_backward = 0;
	generalArg->motorArgRT_Rotate.motor_PORT = OUTPUT_B;
	generalArg->motorArgRT_Rotate.left_right=0;

	//LEFT MOTOR INITIALIZATION
	generalArg->motorArgLT.motor_PORT = OUTPUT_C;
	generalArg->motorArgLT.forward_backward = 0;
	generalArg->motorArgLT_Rotate.motor_PORT = OUTPUT_C;
	generalArg->motorArgLT_Rotate.left_right=0;

	//SONAR MOTOR INITIALIZATION
	generalArg->motorArgSonar.motor_PORT = OUTPUT_A;
  generalArg->motorArgSonar.time_ms = 1000;

  //SONAR INITIALIZATION
  generalArg->sonarArg.sonar_PORT = LEGO_EV3_US;
  generalArg->sonarArg.from = 0;

	//GYROSCOPE INITIALIZATION
	generalArg->gyroscopeArg.gyroscope_PORT = LEGO_EV3_GYRO;
	generalArg->gyroscopeArg.from = 0;

  // Setup and say "Hello"
  hello();

  // Sensors and motors initialization
	sensorAndMotorInit();


  // ######################## MAIN #############################

  waitSTART();

  mapInitialization(map); // initialize map
  drawOpponentField(map); // draw the enemy field in the map
  drawRobotInit(map); // draw the robot in the map

  firstTour(map, &matrixPos, generalArg); // make a tour of the field
  // ###################### END ANDREA NAVIGATION ###########################


	ev3_uninit();
	printf( "*** ( EV3 ) Bye! ***\n" ); // terminate

	return ( 0 );
}
