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
#define Sleep( msec ) usleep(( msec ) * 1000 )

#endif

#include "motors.h"
#include "sensors.h"


#ifndef NAVIGATION_H
#define NAVIGATION_H

#define STRAIGHT 1
#define BACK 2
#define RIGHT 3
#define LEFT 4

typedef struct general_Arguments{
  motorArguments motorArgRT;
  motorArguments motorArgLT;
  motorArguments motorArgSonar;
  sonarArguments sonarArg;
  gyroscopeArguments gyroscopeArg;
  light_sensorArguments light_sensorArg;
  motorArgumentsRotation motorArgRT_Rotate;
  motorArgumentsRotation motorArgLT_Rotate;
  int motor_right_left_rotate;
  int stopValue;
  int rotate_RIGHT_LEFT;
}generalArguments;

void Gyroscope(generalArguments *general_arg);
void moveStraightBack(motorArguments *motor);
void moveRightLeft(generalArguments *general_arg, int which_motor);
int SendMinValue(generalArguments *general_arg);
void rotate(generalArguments *general_arg, int right_left, int angle, int div_speed);
void move(generalArguments *general_arg, int straight_back, int distance);
void scanning(generalArguments *general_arg, int pos,int all);
void catchBall(generalArguments *general_arg, int nright_left);
void ObjectShape(generalArguments *general_arg);
void scanningVertical(generalArguments *general_arg); // Riccardo TORRES
void sendBall(generalArguments *general_arg); // Riccardo TORRES
void moveRightLeft_wrap(generalArguments *general_arg); // Riccardo TORRES
#endif
