/***
ALL THIS FILE HAS BEEN DEVELOPED BY ---> Andrea TRUFINI
EURECOM - OS
***/

#ifndef GENERAL_H
#define GENERAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"

#define COLOR_COUNT  (( int )( sizeof( color ) / sizeof( color[ 0 ])))
#define Sleep( msec ) usleep(( msec ) * 1000 )

void hello();
void sensorAndMotorInit();

#endif
