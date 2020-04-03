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
#define CM 20

#define PRINTFOOTPRINT 0
#define RESCALELENGHT 0.03 //0.1
#define UNCERTAINTY_OBJ 8
#define THRESHOLD_OBJ 30

//Option to draw the field
#define FILLEDFIELD 1
#define CLEAN_BORDER_FIELD 1

// CM=10 means resolution 1cm
// CM=1 means resolution 1mm
// CM=20 means resolution 2cm

#if CM==1
	#define X_DIM 1211 //mm
	#define Y_DIM 2011 //mm

	/*
	 * The frame of reference is centered on the
	 * axis of servo motor that has in charge the rotation
	 * theta is the angle between the x_axis of the field
	 * and the x_axis of the rover
	 */
	#define X_START 600	// mm
	#define Y_START 250	// mm

	#define THETA_START 0		// deg

	/* The footprint is the space coverd by the rover
	 * For semplicity right now is considered only behind
	 * the axis of the servo
	 */
	#define FOOTPRINT_X 300
	#define FOOTPRINT_Y 300
	#define RADIUS	150

	#define X_OFFSET1 -70 //Xaxes
	#define Y_OFFSET2 70 //Yaxes
	#define X_OFFSET3 70 //Xaxes
	#define Y_OFFSET4 -70 //Yaxes

#elif CM==10

	#define X_DIM 121 //cm
	#define Y_DIM 201 //cm

	#define X_START 60	// cm
	#define Y_START 25	// cm

	#define THETA_START 0		// deg

	#define FOOTPRINT_X 30	//cm
	#define FOOTPRINT_Y 30	//cm
	#define RADIUS	15

	#define X_OFFSET1 -7 //Xaxes
	#define Y_OFFSET2 7 //Yaxes
	#define X_OFFSET3 7 //Xaxes
	#define Y_OFFSET4 -7 //Yaxes

#elif CM==20

	#define X_DIM 61 //cm
	#define Y_DIM 101 //cm

	#define X_START 30	// cm
	#define Y_START 12	// cm

	#define THETA_START 0		// deg

	#define FOOTPRINT_X 15	//cm
	#define FOOTPRINT_Y 15	//cm
	#define RADIUS	15

	#define X_OFFSET1 -3 //Xaxes
	#define Y_OFFSET2 3 //Yaxes
	#define X_OFFSET3 3 //Xaxes
	#define Y_OFFSET4 -3 //Yaxes

#endif


#define BUFFER_SIZE 1000

#define LINE 0 // 1--> 10cm // 2 --> longline


#define UNEXPLORED 0	//100
#define FREE  0			//80
#define OBJECTS 1		//0
#define	ENEMY_FIELD 2	//40


#define PI 3.14159265

#define DRAWFREESPACE 0
#define DRAWOCCUPIEDSPACE 1



#endif /* CONFIG_H_ */
