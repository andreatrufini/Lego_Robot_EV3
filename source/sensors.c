/***
ALL THIS FILE HAS BEEN DEVELOPED BY ---> Andrea TRUFINI & Riccardo TORRES
EURECOM - OS
***/

#include "sensors.h"
#include "header.h"

extern float matrix_VScan[MATRIX_ROW][2];
extern float matrix_HScan[MATRIX_ROW][2];

extern sem_t sem_LT, sem_RT, sem_GYRO;

extern int scanning_flag;
extern sem_t sem_SONAR, sem_GYRO_SCAN;
extern rotationAngle;

extern matrixPosition matrixPos;

///// RICCARDO TORRES////

/* This function is used in conjunction with sonar to perform vertical scans.
Specifically, it saves the value of the position of the motor, which will
then be used to understand the shape of the object. */

//Read Position
void readPosition(motorArguments *motor){
	int current_pos;
	float offset;
	int i=0;
	motor->stopValue = 0; //Value to exit from the thread

	if ( ev3_search_tacho_plugged_in(motor->motor_PORT, 0, &(motor->motor_ID), 0 )) {

		get_tacho_position(motor->motor_ID, &current_pos);
		offset=current_pos;

	} else {
		printf( "LEGO_EV3_M_MOTOR %d is NOT found\n", motor->motor_PORT );
	}

	while(motor->stopValue != 1){
			get_tacho_position(motor->motor_ID, &current_pos);
			if(offset>0){
			 	current_pos-=offset;
			} else {
			 	current_pos+=offset;
			}
			matrix_VScan[i][0]=current_pos;
			i++;
			Sleep( 50 );
	}
	pthread_exit(NULL);
}

/* Riccardo TORRES:
 This function is (of course) used for all the scans that are made in the field.
In fact it can be called together with the engine, for a vertical scan (ObjectShape function),
or together with the gyroscope, for a horizontal scan (Scanning function).
Depending on the case, it writes its values inside a matrix that will
then be used in the mapping phase.

	Andrea TRUFINI:
	Implementation on semaphores
*/
void readSonar(sonarArguments *sonar){

	float value;
	int i=0;
	sonar->stopValue = 0; // Value to exit from the thread

	while(sonar->stopValue != 1){
			if (ev3_search_sensor(sonar->sonar_PORT, &(sonar->sonar_ID), sonar->from)){
				if ( !get_sensor_value0(sonar->sonar_ID, &value)) {
					value = 0;
				}
				fflush( stdout );
			}

			if (sonar->vertical_scan==0){
				if (scanning_flag == 1){
					sem_wait(&sem_GYRO_SCAN);
					if(sonar->stopValue != 1){
						matrix_HScan[i][1]=value;
						Sleep( 50 );
						i++;
						fflush( stdout );
					}
					sem_post(&sem_SONAR);
				}
			} else {
				if(sonar->stopValue != 1){
					matrix_VScan[i][1]=value;
					Sleep( 50 );
					i++;
					fflush( stdout );
				}
			}
		}
		pthread_exit(NULL);
}


///// END RICCARDO TORRES////



///// START SECTION by: Andrea TRUFINI /////
/* Used to read the gyroscope value - Synchronization with semaphores */
void useGyroscope(gyroscopeArguments *gyroscope) {
 float value;
 float offset;
 int i=0;
 gyroscope->terminate = 0;

 if (ev3_search_sensor(gyroscope->gyroscope_PORT, &(gyroscope->gyroscope_ID), gyroscope->from)){
  if ( !get_sensor_value0(gyroscope->gyroscope_ID, &value)) {
   value = 0;
  }
  offset=value;
  fflush( stdout );
 }

	while (gyroscope->terminate != 2) {
		 if (ev3_search_sensor(gyroscope->gyroscope_PORT, &(gyroscope->gyroscope_ID), gyroscope->from)){
			 if ( !get_sensor_value0(gyroscope->gyroscope_ID, &value)) {
			  value = 0;
			 }

			if(offset>0){
				value -= offset;
			} else {
				value += offset;
			}

			sem_wait(&sem_GYRO);
			sem_wait(&sem_GYRO);

			gyroscope->value = value;
			if(scanning_flag == 1){
				sem_wait(&sem_SONAR);

				matrix_HScan[i][0]=(int)value + matrixPos.rotation;
				i++;
				fflush( stdout );
				sem_post(&sem_GYRO_SCAN);
			}
			fflush( stdout );

			sem_post(&sem_RT);
			sem_post(&sem_LT);
		}
	}
	pthread_exit(NULL);
}

///// START SECTION by: Andrea TRUFINI /////

///// RICCARDO TORRES////

/* Unfortunately below is an initialization of the color sensor,
 which we then decided not to use. */

//Use Light Sensor
/*void uselight_sensor(light_sensorArguments *light_sensor) {
	float value;

	while (true) {
		if (ev3_search_sensor(light_sensor->light_sensor_PORT, &(light_sensor->light_sensor_ID),light_sensor->from)){
    //  printf("light_sensor found, reading light_sensor...\n");
      if ( !get_sensor_value0(light_sensor->light_sensor_ID, &value)) {
        value = 0;
      }
      printf( "light_sensor Value: (%f) \n", value);
      fflush( stdout );
    }
	}
}*/


/*
This function has been widely used in debugging and has evolved over time.
Its main purpose is to observe and use the values present in the matrix,
printing a file that was then used by Guarrera in the mapping phase.
In the final phase has two different prints depending on the type of scan
that has been made: horizontal or vertical.
*/
void sendValue(int vertical_scan) {

	int row;
	int columns;
	static int cnt_v;
	static int cnt_h;
	int flag=0;
	FILE *fp;


	if(vertical_scan==0){

		float value_H[MATRIX_ROW][2];

		char filename_title[100]="/home/robot/ric_t/horizontal_output";
		char filename[100];

			sprintf(filename, "%s%d", filename_title, cnt_h);
			fp = fopen(filename,"w");
			memcpy(value_H, matrix_HScan, sizeof(float)*MATRIX_ROW*2);
			for (row=0; row<MATRIX_ROW; row++) {
    		for(columns=0; columns<2; columns++) {
					if (flag==0) {
						fprintf(fp, "%d,", (int)matrix_HScan[row][columns]);
						flag=1;
					} else {
						fprintf(fp, "%d,\n", (int)matrix_HScan[row][columns]);
						flag=0;
					}
      	}
 			}
			cnt_h++;

	} else {

		float value_V[MATRIX_ROW][2];

		char filename_title[100]="/home/robot/ric_t/vertical_output";
		char filename[100];

			sprintf(filename, "%s%d", filename_title, cnt_v);
			fp = fopen(filename,"w");
			memcpy(value_V, matrix_VScan, sizeof(float)*MATRIX_ROW*2);
			for (row=0; row<MATRIX_ROW; row++) {
    		for(columns=0; columns<2; columns++) {
					if (flag==0) {
						fprintf(fp, "%d,", (int)matrix_VScan[row][columns]);
						flag=1;
					} else {
						fprintf(fp, "%d,\n", (int)matrix_VScan[row][columns]);
						flag=0;
					}
      	}
 			}
			cnt_v++;
	}
	fclose(fp);

}

/*Simple function to reset the matrix, widely used in the debugging phase*/
void resetMatrix(){
	int i;

	for(i=0; i<MATRIX_ROW; i++){
		matrix_HScan[i][0]=0;
		matrix_HScan[i][1]=0;
	}
}

///// END RICCARDO TORRES////
