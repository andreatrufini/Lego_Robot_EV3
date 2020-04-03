/***
ALL THIS FILE HAS BEEN DEVELOPED BY ---> Andrea TRUFINI
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

#endif

#ifndef MOTORS_H
#define MOTORS_H

typedef struct motor_Arguments_Rotation{
	int motor_PORT;
	uint8_t motor_ID;
	float angle;
	int left_right;
	int div_speed;
}motorArgumentsRotation;

typedef struct motor_Arguments{
	int motor_PORT;
	uint8_t motor_ID;
	int time_ms;
	int distance_cm;
	int forward_backward;
	int SpeedDivisor;
	int step;
	int stopValue;
}motorArguments;

void runMotor(motorArguments *motor);
void runMotorByAngle(motorArguments *motor);

#endif
