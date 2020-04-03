/*
 * loc_funct.h
 *
 *  Created on: 23 nov 2019
 *      Author: ironmath
 */



#ifndef LOC_FUNCT_H_
#define LOC_FUNCT_H_


///////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
///////////////////////////////////////
#include "config.h"
#include "draw_funct.h"

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



