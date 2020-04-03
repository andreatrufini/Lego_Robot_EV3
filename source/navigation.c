/***
ALL THIS FILE HAS BEEN DEVELOPED BY ---> Andrea TRUFINI & Riccardo TORRES
EURECOM - OS
***/

#include "navigation.h"

///// START SECTION by: ANDREA TRUFINI /////

#define ROTATE_RIGHT_COEFF 1.85
#define ROTATE_LEFT_COEFF 1.89

extern sem_t sem_LT, sem_RT, sem_GYRO; // semaphores for right motor, left motor and gyroscope - USED TO ROTATE
sem_t sem_switch_motor; // semaphore for motor switching
sem_t sem_SONAR, sem_GYRO_SCAN; // semaphores used to synchronize sonar and gyroscope
sem_t sem_first_motor, sem_second_motor; // semaphores for right and left motors - USED TO MOVE FORWARD AND BACKWARD

int rx_done = 0;
int scanning_flag;
int rotationAngle;

/* Andrea TRUFINI:
  The 'move' function allows to the robot to do the forward and backward movements
*/
void move(generalArguments *general_arg, int straight_back, int distance){

  // threads for the two motors: right(RT) and left(LT)
  pthread_t thread_motor_RT, thread_motor_LT;

  static int motor_RT_LT = 0;
  sem_init(&sem_first_motor, 0, 1);
  sem_init(&sem_second_motor, 0, 0);

  if(straight_back==STRAIGHT){
    general_arg->motorArgRT.forward_backward = 0;
    general_arg->motorArgLT.forward_backward = 0;
  }else if(straight_back==BACK){
    general_arg->motorArgRT.forward_backward = 1;
    general_arg->motorArgLT.forward_backward = 1;
  }

  general_arg->motorArgRT.distance_cm = distance;
  general_arg->motorArgLT.distance_cm = general_arg->motorArgRT.distance_cm;

  /*
    With the only use of two threads to move straight or back there are some problems
    since the robot trajectory is not a "line". To solve this problem the idea was to
    use two semaphores in order to call one time the motors in the order RIGHT->LEFT
    and the next time LEFT->RIGHT, and so on. I thought it was a scheduling problem
    but this solution maybe helped but didn't solve completely the problem.
    In fact also different coefficients of motors speed have been used.
  */
  if (motor_RT_LT == 0){
    motor_RT_LT = 1;
    sem_wait(&sem_first_motor);
    pthread_create(&thread_motor_RT, NULL, moveStraightBack, &(general_arg->motorArgRT));
    sem_wait(&sem_second_motor);
    pthread_create(&thread_motor_LT, NULL, moveStraightBack, &(general_arg->motorArgLT));
  } else {
    motor_RT_LT = 0;
    sem_wait(&sem_first_motor);
    pthread_create(&thread_motor_LT, NULL, moveStraightBack, &(general_arg->motorArgLT));
    sem_wait(&sem_second_motor);
    pthread_create(&thread_motor_RT, NULL, moveStraightBack, &(general_arg->motorArgRT));
  }

  pthread_join(thread_motor_RT, NULL);
  pthread_join(thread_motor_LT, NULL);

}

/* Andrea TRUFINI:
  The 'rotate' function allows to the robot to rotate right or left using a certain speed divisor.
  It uses the gyroscope to reach the exact angle.
*/
void rotate(generalArguments *general_arg, int right_left, int angle, int div_speed){
  pthread_t thread_motor_RT, thread_motor_LT;
  pthread_t thread_gyroscope;

  /*
  Semaphores used to avoid problem of access to the same variable.
  In fact the gyroscope write is a variable in a structure and the motors read it.
  The synchronization has been done using these three Semaphores.
  */
  sem_init(&sem_GYRO, 0, 2);
	sem_init(&sem_RT, 0, 0);
	sem_init(&sem_LT, 0, 0);

  sem_init(&sem_switch_motor, 0, 1); // switch between left and right motor
  general_arg->motorArgRT_Rotate.div_speed = div_speed;
  general_arg->motorArgLT_Rotate.div_speed = div_speed;

  if (right_left==RIGHT){
    general_arg->rotate_RIGHT_LEFT=0;
    general_arg->motorArgRT_Rotate.left_right = 1;
    general_arg->motorArgLT_Rotate.left_right = 0;
  } else if (right_left==LEFT){
    general_arg->rotate_RIGHT_LEFT=1;
    general_arg->motorArgRT_Rotate.left_right = 0;
    general_arg->motorArgLT_Rotate.left_right = 1;
  }

  general_arg->motorArgRT_Rotate.angle = angle;
  general_arg->motorArgLT_Rotate.angle = (general_arg->motorArgRT_Rotate.angle);

  pthread_create(&thread_gyroscope, NULL, useGyroscope, &(general_arg->gyroscopeArg));
  pthread_create(&thread_motor_LT, NULL, moveRightLeft_wrap, general_arg);
  pthread_create(&thread_motor_RT, NULL, moveRightLeft_wrap, general_arg);

  pthread_join(thread_motor_RT, NULL);
  pthread_join(thread_motor_LT, NULL);
  pthread_join(thread_gyroscope, NULL);

  sem_destroy(&sem_switch_motor);

}

/* Andrea TRUFINI:
  Syncronize the two motors movements
*/
void moveRightLeft_wrap(generalArguments *general_arg){
  sem_wait(&sem_switch_motor);
  if(rx_done == 0){
    rx_done = 1;
    sem_post(&sem_switch_motor);
    moveRightLeft(general_arg, 0);
  }
  else{
    rx_done = 0;
    moveRightLeft(general_arg, 1);
  }
}

/* Andrea TRUFINI:
  Basic function used in the general function 'move' to move forward and backward
*/
void moveStraightBack(motorArguments *motor){
  if ( ev3_search_tacho_plugged_in(motor->motor_PORT, 0, &(motor->motor_ID), 0 )) {
		int max_speed, step, current_pos, i;
    static int speed_flag=0;
    FLAGS_T state;

    sem_post(&sem_second_motor);

		get_tacho_max_speed( motor->motor_ID, &max_speed );
		set_tacho_stop_action_inx( motor->motor_ID, TACHO_COAST );
		do {
			get_tacho_state_flags( motor->motor_ID, &state );
		} while ( state );

		set_tacho_ramp_up_sp( motor->motor_ID, 0 );
		set_tacho_ramp_down_sp( motor->motor_ID, 0 );

		/// Setting the initial position of the Motor
		get_tacho_position(motor->motor_ID, &current_pos);

    if(motor->forward_backward == 0){
	 			step= (motor->distance_cm) * 360 / 17,278756;
    } else {
		  	step= (-1)*(motor->distance_cm) * 360 / 17,278756;
		}

		/// Moving the Motor
    if(speed_flag==0){
      speed_flag=1;
      set_tacho_speed_sp( motor->motor_ID, max_speed / 5.85 );
    }else{
      speed_flag=0;
      set_tacho_speed_sp( motor->motor_ID, max_speed / 6 );
    }

		set_tacho_position_sp( motor->motor_ID, step );

		set_tacho_command_inx( motor->motor_ID, TACHO_RUN_TO_REL_POS );
		} else {
  			printf( "LEGO_EV3_M_MOTOR %d is NOT found\n", motor->motor_PORT );
  	}

    Sleep(100*(motor->distance_cm));

    pthread_exit(NULL);
}


/* Andrea TRUFINI:
  Basic function used in the general function 'rotate' to rotate right and left
*/
void moveRightLeft(generalArguments *general_arg, int which_motor){
  motorArgumentsRotation *motor;
  int counter;

  // the same function is used from the two motor threads - Choose which motor
  if (which_motor == 0){
    motor= &(general_arg->motorArgRT_Rotate);
  }else{
    motor= &(general_arg->motorArgLT_Rotate);
  }

  // if motor found
  if ( ev3_search_tacho_plugged_in(motor->motor_PORT, 0, &(motor->motor_ID), 0 )) {
		int max_speed;
		int step;
		int current_pos;
		int i;
		FLAGS_T state;

		get_tacho_max_speed( motor->motor_ID, &max_speed ); // get max speed
		set_tacho_stop_action_inx( motor->motor_ID, TACHO_COAST );
		do {
			get_tacho_state_flags( motor->motor_ID, &state );
		} while ( state );

		set_tacho_ramp_up_sp( motor->motor_ID, 0 );
		set_tacho_ramp_down_sp( motor->motor_ID, 0 );

		/// Setting the initial position
		get_tacho_position(motor->motor_ID, &current_pos);

    // set the step positive or negative in order to move right or left
    if(motor->left_right == 0){
	 			step=3;
    } else {
				step=-3;
		}


    if (which_motor == 0){
      sem_wait(&sem_RT);
    }else{
      sem_wait(&sem_LT);
    }

		/// Moving the Motor
		set_tacho_speed_sp( motor->motor_ID, max_speed / motor->div_speed );
    while(abs(general_arg->gyroscopeArg.value) <= motor->angle){

      if (which_motor == 0){
        sem_post(&sem_GYRO);
      }else{
        sem_post(&sem_GYRO);
      }

      set_tacho_position_sp( motor->motor_ID, step );
      set_tacho_command_inx( motor->motor_ID, TACHO_RUN_TO_REL_POS );

      if (which_motor == 0){
        sem_wait(&sem_RT);
      }else{
        sem_wait(&sem_LT);
      }
    }
    if (which_motor == 0){
      sem_post(&sem_GYRO);
    }else{
      sem_post(&sem_GYRO);
    }

    /*
      Since the gyroscope was not really precise I implemented this "function"
      which allows to the robot to do small steps with a lower speed around
      the bias angle, i.e. the angle that the robot shall reach since it has
      been passed to this function.
    */
    set_tacho_speed_sp( motor->motor_ID, max_speed/10);
    for(counter=0; counter<10; counter++){
      if (which_motor == 0){
        sem_wait(&sem_RT);
      }else{
        sem_wait(&sem_LT);
      }
      if(abs(general_arg->gyroscopeArg.value) != (abs(general_arg->motorArgRT_Rotate.angle))){
        if((general_arg->gyroscopeArg.value) < 0 && motor->left_right == 0){
          step=-(abs(general_arg->gyroscopeArg.value) - abs(general_arg->motorArgRT_Rotate.angle));
          set_tacho_position_sp( motor->motor_ID, step );
          set_tacho_command_inx( motor->motor_ID, TACHO_RUN_TO_REL_POS );
          Sleep(100);
        }else if(abs(general_arg->gyroscopeArg.value) >= 0 && motor->left_right == 0){
          step=(abs(general_arg->motorArgRT_Rotate.angle) - abs(general_arg->gyroscopeArg.value));
          set_tacho_position_sp( motor->motor_ID, step );
          set_tacho_command_inx( motor->motor_ID, TACHO_RUN_TO_REL_POS );
          Sleep(100);
        }else{
        }
      }
      if (which_motor == 0){
        sem_post(&sem_GYRO);
      }else{
        sem_post(&sem_GYRO);
      }
    }
    //printf("Final ANGLE: %f\n", general_arg->gyroscopeArg.value);

		} else {
  			printf( "LEGO_EV3_M_MOTOR %d is NOT found\n", motor->motor_PORT );
  	}
    general_arg->gyroscopeArg.terminate++;
    pthread_exit(NULL);
}

/*
  The actions did with this function are:
  - raise up the sonar block
  - move straight 10 cm
  - raise down the sonar block catching the ball
*/
void catchTheBall(generalArguments *general_arg, int nleft_right){

  general_arg->motorArgSonar.step=75;
  runMotorByAngle(&(general_arg->motorArgSonar));

  Sleep(2000);

  move(general_arg, STRAIGHT, 10);

  general_arg->motorArgSonar.step=-75;
  runMotorByAngle(&(general_arg->motorArgSonar));

}

///// SECTION END by: Andrea TRUFINI /////



///// RICCARDO TORRES /////

/*This function is used for horizontal scanning, also this function has
undergone  several final changes.
Its functionality is to scan the surrounding environment using both gyroscope
and scanner at the same time. The values you can pass it refer to whether or
not you want to perform a 360 degree scan.*/

void scanning(generalArguments *general_arg, int pos,int scanning_360){
  general_arg->sonarArg.vertical_scan=0;
  scanning_flag=1;
  sem_init(&sem_SONAR, 0, 1);
  sem_init(&sem_GYRO_SCAN, 0, 0);

  pthread_t thread_Sonar;

  pthread_create(&thread_Sonar, NULL, readSonar, &(general_arg->sonarArg));

  if(scanning_360==0){
    if(pos==RIGHT){
      rotate(general_arg, RIGHT, 87, 5);
    } else {
      rotate(general_arg, LEFT, 87, 5);
    }
  } else {
    rotate(general_arg, RIGHT, 360, 10);
  }

  general_arg->sonarArg.stopValue=1;

  scanning_flag=0;

  sem_post(&sem_GYRO_SCAN);
  pthread_join(thread_Sonar, NULL);
  sendValue(0);
}

/*This function is used for object recognition and uses an engine together
with sonar. In fact you can see that at the end of the function the
"stop values" are raised.*/

void ObjectShape(generalArguments *general_arg){

  pthread_t thread_MotorVerticalScan;
  pthread_t thread_SonarVerticalScan;
  pthread_t thread_SonarVerticalScanPosition;

  general_arg->sonarArg.vertical_scan=1;


  pthread_create(&thread_SonarVerticalScan, NULL, readSonar, &(general_arg->sonarArg));
  pthread_create(&thread_SonarVerticalScanPosition, NULL, readPosition, &(general_arg->motorArgSonar));

  general_arg->motorArgSonar.step=400;
  general_arg->motorArgSonar.SpeedDivisor = 25;
  pthread_create(&thread_MotorVerticalScan, NULL, runMotorByAngle, &(general_arg->motorArgSonar));
  pthread_join(thread_MotorVerticalScan, NULL);

  Sleep(4000);

  general_arg->motorArgSonar.step=-400;
  pthread_create(&thread_MotorVerticalScan, NULL, runMotorByAngle, &(general_arg->motorArgSonar));
  pthread_join(thread_MotorVerticalScan, NULL);

  Sleep(4000);

  general_arg->sonarArg.stopValue=1;
  general_arg->motorArgSonar.stopValue=1;

  pthread_join(thread_SonarVerticalScan, NULL);
  pthread_join(thread_SonarVerticalScanPosition, NULL);

  sendValue(1);

  Sleep(100);
  resetMatrix();
}

/* Primordial function to kick the ball, it is based on the fact that
 it must be called when you are five centimetres away from it. Not used
 in the final design. */

void sendBall(generalArguments *general_arg){

  general_arg->motorArgSonar.step=300;
  general_arg->motorArgSonar.SpeedDivisor = 5;
  runMotorByAngle(&(general_arg->motorArgSonar));

  move(general_arg, BACK, 5);
  move(general_arg, STRAIGHT, 10);

}

///// END RICCARDO TORRES /////
