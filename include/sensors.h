/***
ALL THIS FILE HAS BEEN DEVELOPED BY ---> Andrea TRUFINI & Riccardo TORRES
EURECOM - OS
***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"

#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

/// WIN32
#ifdef __WIN32__
#include <Windows.h>

/// UNIX
#else
#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

#endif

#define MATRIX_ROW 400

#include "motors.h"

#ifndef SENSORS_H
#define SENSORS_H

typedef struct sonar_Arguments{
	int sonar_PORT;
	uint8_t sonar_ID;
	int from;
	int stopValue;
	int vertical_scan;
}sonarArguments;

typedef struct gyroscope_Arguments{
	int gyroscope_PORT;
	uint8_t gyroscope_ID;
	int from;
	float value;
	int terminate;
	int stopValue;
}gyroscopeArguments;

typedef struct light_sensor_Arguments{
	int light_sensor_PORT;
	uint8_t light_sensor_ID;
	int from;
}light_sensorArguments;

void readPosition(motorArguments *motor);
void readSonar(sonarArguments *sonar);
void readGyroscope(gyroscopeArguments *gyroscope);
void sendValue(int flag_scan);
void useGyroscope(gyroscopeArguments *gyroscope);
void uselight_sensor(light_sensorArguments *light_sensor);
void resetMatrix();
#endif
