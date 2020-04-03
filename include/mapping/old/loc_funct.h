/*
 * loc_funct.h
 *
 *  Created on: 23 nov 2019
 *      Author: ironmath
 */



#ifndef LOC_FUNCT_H_
#define LOC_FUNCT_H_

/*
 * Measurement unit:
 * Distance -> mm
 * Angle	-> deg °
 * Time 	->
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
//#define Y_START 1000	// mm

#define THETA_START 0		// deg
/* The footprint is the space coverd by the rover
 * For semplicity right now is considered only behind
 * the axis of the servo
 */
#define FOOTPRINT_X 300
#define FOOTPRINT_Y 300

#define BUFFER_SIZE 1000

#define LINE 0 // 1--> 10cm // 2 --> longline
#define TOPRINT 1

#if (TOPRINT==1)
	#define UNEXPLORED 0 	// 100
	#define FREE 80  		// 0
	#define OBJECTS 0 		// 100
	#define	ENEMY_FIELD 40 	// 0

#else


	#define UNEXPLORED 1//100 	// 0
	#define FREE  0//80  		// 0
	#define OBJECTS 1//0 		// 100
	#define	ENEMY_FIELD 2//40 	// 0

#endif
/* y_axis
 * ^---------
 * |		|
 * |		|
 * |		|
 * |		|
 * |		|
 * ---------- > x_axis
*/

#define PI 3.14159265

#define DRAWFREESPACE 0
#define DRAWOCCUPIEDSPACE 1

///////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
///////////////////////////////////////

const int eps_sonar; 	// Uncertainty of sonar measurement
						// 1 --> 0.1 cm all measurement are multiplied per 10
const int eps_angle;


typedef struct {
	int sonarDistance[BUFFER_SIZE]; // I declare a int vector to improve speed. The value returned from sonar is float
				// can be multiplied per 10 and casted
	float sonarClock[BUFFER_SIZE];

	int angle[BUFFER_SIZE]; 		// Angle alpha
	float angleClock[BUFFER_SIZE]; 	//

} sensorData;

// Euler Coordinates
typedef struct {
	int x;
	int y;
	int theta; // Degree

}positionData;


// Too big to define a new type
// Noo it is possible but it must be allocated in the static memory
typedef struct {
	//enemy field is f
	char field[X_DIM][Y_DIM];
}fieldData;


struct localizationData{
	fieldData fData;
	positionData pData;
	sensorData sData;

};



/*------------------------Public Functions------------------------*/
//fieldData localizationInit( fieldData *fData, positionData *pData);

//struct localizationData refreshData(struct localizationData *lData);

void localizationInit( fieldData *pt_fData, positionData *pt_pData);

void refreshData(struct localizationData *pt_lData);


void setPositionManually(positionData *pt_pData,int x, int y, int theta);


/*------------------------Static Functions------------------------*/
/*
// Padding of the matrix of with unexplored Value
static void _unexploredPadding(fieldData * pt_fData);

// Padding of the matrix of with enemyfield Value
static void _enemyfieldPadding();

// Read from sensorData and write to fieldData
static void _refreshEnvData();

// Read from sensorData and write to positionData
static void _refreshPositionData();
*/



#endif /* LOC_FUNCT_H_ */



