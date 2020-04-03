/***
ALL THIS FILE HAS BEEN DEVELOPED BY ---> Andrea TRUFINI & Riccardo TORRES
EURECOM - OS
***/

#include "motors.h"
#include "navigation.h"

/* This function is used for the operation of all motors within the system,
which is why many of its parameters are defined via the structure.
I (Riccardo Torres) used this function in ObjectShape() to make the motor
connected to the sonar move for vertical scans. */

//Run Motors using angles
void runMotorByAngle(motorArguments *motor){

	if ( ev3_search_tacho_plugged_in(motor->motor_PORT, 0, &(motor->motor_ID), 0 )) {
		int max_speed;
		int current_pos;
		int step;
		int i;
		FLAGS_T state;
		int actual_pos;
		int start_position;

		get_tacho_max_speed( motor->motor_ID, &max_speed );

      do {
        get_tacho_state_flags( motor->motor_ID, &state );
      } while ( state );

      // printf( "run to relative position...\n" );
      set_tacho_speed_sp( motor->motor_ID, max_speed / motor->SpeedDivisor);
      set_tacho_ramp_up_sp( motor->motor_ID, 0 );
      set_tacho_ramp_down_sp( motor->motor_ID, 0 );
      get_tacho_position(motor->motor_ID, &actual_pos);

			start_position=actual_pos;
      step=motor->step;

      set_tacho_position_sp( motor->motor_ID, step );

      set_tacho_command_inx( motor->motor_ID, TACHO_RUN_TO_REL_POS );
      get_tacho_position(motor->motor_ID, &actual_pos);

			Sleep(1000);

			get_tacho_position(motor->motor_ID, &actual_pos);

			Sleep(200);

		} else {
      printf( "LEGO_EV3_M_MOTOR %d is NOT found\n", motor->motor_ID );
    }
		pthread_exit(NULL);
}
