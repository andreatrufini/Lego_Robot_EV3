/*
 * config.h
 *
 *  Created on: 9 gen 2020
 *      Author: ironmath
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define MAXPATHLENGTH 1000 // CE NE SONO TROPPI METTILI TUTTI DA UN LATO

/*
 * Measurement unit:
 * Distance -> mm
 * Angle	-> deg °
 * Time 	->
*/

/* y_axis
 * ^---------
 * |		|
 * |		|
 * |		|
 * |		|
 * |		|
 * ---------- > x_axis
*/
#define X_DIM 1201 //mm
#define Y_DIM 2001 //mm

/*
 * The frame of reference is centered on the
 * axis of servo motor that has in charge the rotation
 * theta is the angle between the x_axis of the field
 * and the x_axis of the rover
 */
#define X_START 600	// mm
#define Y_START 100	// mm

#define THETA_START 0		// deg

/* The footprint is the space coverd by the rover
 * For semplicity right now is considered only behind
 * the axis of the servo
 */
#define FOOTPRINT_X 300
#define FOOTPRINT_Y 300

#define BUFFER_SIZE 1000

#define LINE 0 // 1--> 10cm // 2 --> longline


#define UNEXPLORED 1	//100
#define FREE  0			//80
#define OBJECTS 5		//0
#define	ENEMY_FIELD 2	//40


#define PI 3.14159265

#define DRAWFREESPACE 0
#define DRAWOCCUPIEDSPACE 1



#endif /* CONFIG_H_ */
