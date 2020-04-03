/*
 * loc_funct.c
 *
 *  Created on: 23 nov 2019
 *      Author: ironmath
 */


// Custom Include
#include "loc_funct.h"



/* Use a batch of data to fill the matrix drawing lines and points */
static void _fillTheMap(struct localizationData * pt_lData){

	// printf("_fillTheMap\n");

	int i,tempx,tempy;
	int x1_free = pt_lData->pData.x;
	int y1_free = pt_lData->pData.y;
	int theta= pt_lData->pData.theta;

	int x1_sonar, y1_sonar;

	int x2_free, y2_free;
	int x2_free_prev, y2_free_prev;
	int x2_fill, y2_fill;
	int expected_dist, dist_half_field;

	int x2,y2;
	int dist,angleMeasDeg; // for readability
	int dist_prev;
	int angleFieldDeg;
	int angleFieldDeg_prev;

	int updateDist, noPrint;
	int ang_A_half, ang_B_half, ang_C_half, ang_D_half;
	int ang_A, ang_B;
	int zone_half;
	int zone_full;

	//counter of unexpected in one zone;
	int counter[4]={0};

	/* Calculate where to draw the lines*/
	for(i=0;i<1500;i++){
		dist=pt_lData->sData.sonarDistance[i];

		dist=dist*RESCALELENGHT;

		angleMeasDeg=pt_lData->sData.angle[i];

		angleFieldDeg =  90 + angleMeasDeg + theta;// 180 - (angleMeasDeg + theta + 90);

		while(angleFieldDeg<0)
			angleFieldDeg+=360;
		while(angleFieldDeg>360)
			angleFieldDeg-=360;

		//12 cm between the rotating axes of the robot and the sonar
		x1_sonar=x1_free + (int)12*cos(degToRad((float)angleFieldDeg));
		y1_sonar=y1_free + (int)12*sin(degToRad((float)angleFieldDeg));

		x2_free= x1_sonar + (int)dist*cos(degToRad((float)angleFieldDeg));
		y2_free= y1_sonar + (int)dist*sin(degToRad((float)angleFieldDeg));


		//printf("PositionRobotSonar: ");

//
//		ang_D=radToDeg(atan(y1_sonar/x1_sonar));
//		ang_A=radToDeg(atan(((Y_DIM/2)-y1_sonar)/x1_sonar));

//		ang_B=radToDeg(atan(((Y_DIM/2)-y1_sonar)/(X_DIM-x1_sonar)));
//		ang_C=radToDeg(atan((y1_sonar)/(X_DIM-x1_sonar)));
		//		printf("ANGLES:\t%d\t%d\t%d\t%d\t%d\t%d\n",ang_A,ang_B,ang_A_half,ang_B_half,ang_C_half,ang_D_half);
		//		printf("ANGLES:\t%f\t%f\t%f\t%f\t%f\t%f\n",help[0],help[1],help[2],help[3],help[4],help[5]);


		ang_A = radToDeg(atan2(Y_DIM-y1_sonar,x1_sonar));
		ang_B = radToDeg(atan2(Y_DIM-y1_sonar,X_DIM-x1_sonar));

		ang_C_half = radToDeg(atan2(y1_sonar,X_DIM-x1_sonar));
		ang_D_half = radToDeg(atan2(y1_sonar,x1_sonar));

		ang_A_half = radToDeg(atan2(Y_DIM/2-y1_sonar,x1_sonar));
		ang_B_half = radToDeg(atan2(Y_DIM/2-y1_sonar,X_DIM-x1_sonar));

		updateDist=0;
		noPrint=0; //useless

		//check if ther is something unespected
		if(dist>0 && x2_free>0 && y2_free>0){
			//zone1 half
			if( angleFieldDeg>180-ang_A_half && angleFieldDeg< 180+ang_D_half ){
				zone_half=1; zone_full=1;

				expected_dist=x1_sonar/cos(degToRad(180-angleFieldDeg));
				if(expected_dist-dist > UNCERTAINTY_OBJ ){
					if(dist<THRESHOLD_OBJ)
						//printf("ZONE[%d] OBJ IN X2: %d\tY2: %d\t\t dist: (r)%d (e)%d \tcounter[1]: %d\n",zone_half,x2_free+X_OFFSET1, y2_free,dist,expected_dist,counter[0]);

					counter[0]+=1;

				}else if (dist-expected_dist< UNCERTAINTY_OBJ){
					dist=expected_dist;
				}else if(dist-expected_dist > UNCERTAINTY_OBJ){
//					noPrint=1;
				}

			//zone2 half
			}else if (angleFieldDeg> ang_B_half && angleFieldDeg<180-ang_A_half){
				zone_half=2;

				//expected_dist_half>((Y_DIM/2)-y1_sonar))/cos(degToRad(angleFieldDeg-90)))
				dist_half_field =( int) ((Y_DIM/2)-y1_sonar)/cos(degToRad(angleFieldDeg-90));
				// zone2 half--> zona1 full
				if(angleFieldDeg> ang_B_half && angleFieldDeg<ang_B){
					zone_full=1;
					expected_dist = (int) (X_DIM-x1_sonar)/cos(degToRad(angleFieldDeg-90));

				// zone2 half--> zona2 full
				}else if(angleFieldDeg>ang_B &&  angleFieldDeg<180-ang_A){
					zone_full=2;
					expected_dist = (int) (Y_DIM-y1_sonar)/cos(degToRad(angleFieldDeg-90));

				// zone2 half--> zona3 full
				}else if(angleFieldDeg>180-ang_A && angleFieldDeg<180-ang_A_half){
					zone_full=3;
					expected_dist = (int) (x1_sonar)/cos(degToRad(180-angleFieldDeg));

				}


				if(dist_half_field-dist > UNCERTAINTY_OBJ){
					if(dist<THRESHOLD_OBJ)
						//printf("ZONE[%d] OBJ IN X2: %d\tY2: %d\t\t dist: (r)%d (e)%d \tcounter[2]: %d\n",zone_half,x2_free, y2_free+Y_OFFSET2,dist,expected_dist,counter[1]);

					counter[1]+=1;

				}else if (dist>expected_dist){
//					noPrint=1;
				}else if (dist>dist_half_field){
					dist=dist_half_field;

				}
//				else if(dist-dist_half_field > UNCERTAINTY_OBJ){
//					noPrint=1;
//					dist=dist_half_field;
//
//				}

			//zone3 half
			}else if (angleFieldDeg> 360- ang_C_half && angleFieldDeg< ang_B_half+360){
				zone_half=3;

				//expected_dist>(x1_sonar/cos(degToRad(angleFieldDeg)))
				expected_dist=(int) (X_DIM-x1_sonar)/cos(degToRad(angleFieldDeg));
				if(expected_dist-dist > UNCERTAINTY_OBJ ){
					if(dist<THRESHOLD_OBJ)
						//printf("ZONE[%d] OBJ IN X2: %d\tY2: %d\t\t dist: (r)%d (e)%d \tcounter[3]: %d\n",zone_half,x2_free+X_OFFSET3,y2_free,dist,expected_dist,counter[2]);

					counter[2]+=1;

				}else if (dist-expected_dist< UNCERTAINTY_OBJ){
					dist=expected_dist;
				}else if(dist-expected_dist > UNCERTAINTY_OBJ){
//					noPrint=1;
				}

			//zone4 half
			}else if (angleFieldDeg>180+ang_D_half && angleFieldDeg<360-ang_C_half){
				zone_half=4;

				//expected_dist>(y1_sonar/cos(degToRad(angleFieldDeg-270)))
				expected_dist=(int) (y1_sonar/cos(degToRad(angleFieldDeg-270)));
				if(expected_dist-dist > UNCERTAINTY_OBJ ){
					if(dist<THRESHOLD_OBJ)
						//printf("ZONE[%d] OBJ IN X2: %d\tY2: %d\t\t dist: (r)%d (e)%d \tcounter[4]: %d\n",zone_half,x2_free,y2_free+Y_OFFSET4,dist,expected_dist,counter[3]);

					counter[3]+=1;

				}else if (dist-expected_dist< UNCERTAINTY_OBJ){
					dist=expected_dist;

				}else if(dist-expected_dist > UNCERTAINTY_OBJ){
//					noPrint=1;

				}
			}
		}

#if CLEAN_BORDER_FIELD==1
			//REUPDATE --> x2 y2 with new dist
		x2_free= x1_sonar + dist*cos(degToRad(angleFieldDeg));
		y2_free= y1_sonar + dist*sin(degToRad(angleFieldDeg));
//		if(updateDist==1){
//			x2_free= x1_sonar + expected_dist*cos(degToRad(angleFieldDeg));
//			y2_free= y1_sonar + expected_dist*sin(degToRad(angleFieldDeg));
//		}else{
//
//		}
#endif

		/*START:  Fill the space between two different line*/
		if(!noPrint){
			if(!i){
				_drawLine((pt_lData->fData.field), x1_sonar, y1_sonar, x2_free, y2_free, DRAWFREESPACE);

			}
			else{

		#if FILLEDFIELD ==1
				_drawTriangle((pt_lData->fData.field), x1_sonar, y1_sonar, x2_free_prev, y2_free_prev,x2_free, y2_free );
		#else
				_drawLine((pt_lData->fData.field), x1_sonar, y1_sonar, x2_free, y2_free, DRAWFREESPACE);
		#endif

			}

		/*END: 	  Fill the space between two different line*/


#if LINE==1
			x2= x1_free - (dist+100)*cos(_degToRad(angleFieldDeg));
			y2= y1_free + (dist+100)*sin(_degToRad(angleFieldDeg));
			_drawLine(&(pt_lData->fData), x2_free, y2_free, x2, y2, DRAWOCCUPIEDSPACE); //
#endif


			x2_free_prev=x2_free;
			y2_free_prev=y2_free;
			dist_prev=dist;
			angleFieldDeg_prev=angleFieldDeg;

		}
	}

	return;
}


static void _fillTheRover(struct localizationData * pt_lData){
	//SUPPONGO CHE IL ROVER E' SEMPRE UN QUADRATO ORIENTATO CON I BORDI PARALLELI

	int x_motor_axes= pt_lData->pData.x;
	int y_motor_axes = pt_lData->pData.y;
	//int theta_rover = pt_lData->pData.theta;
	// printf("%d %d %d %d",x_motor_axes-FOOTPRINT_X/2,y_motor_axes-FOOTPRINT_Y/2,x_motor_axes+FOOTPRINT_X/2, y_motor_axes+FOOTPRINT_Y/2);

	_filledCircle ((pt_lData->fData.field),x_motor_axes, y_motor_axes, RADIUS, DRAWOCCUPIEDSPACE); // 15 cm of radius

	//_drawSquare((pt_lData->fData.field), x_motor_axes-FOOTPRINT_X/4, y_motor_axes-FOOTPRINT_Y/2, x_motor_axes+FOOTPRINT_X/2, y_motor_axes+FOOTPRINT_Y/2 ,DRAWOCCUPIEDSPACE);

	return;
}

// Read from sensorData and write to fieldData
// Positional data are used to orient the upgrade of the field
// Sensor data to refresh "Field data"
static void _refreshEnvData(struct localizationData * pt_lData){

	// printf("_refreshEnvData\n");


	_fillTheMap(pt_lData);

	//TO BE COMPLETED
	#if PRINTFOOTPRINT ==1
		_fillTheRover(pt_lData);
	#endif
	// Zero angle found
	//printf("%d",lData.sData.angle[i]);

	return;
}




// Read from sensorData and write to positionData
//static positionData* _refreshPositionData(positionData *pt_pData,sensorData *pt_sData){
static void _refreshPositionData(positionData *pt_pData,sensorData *pt_sData){
	/*
	printf("_refreshPositionData\n");

	int i = 0;
	int idxAngle0;

	int alpha_deg, beta_deg;
	int x_new1,x_new3,y_new2,y_new4;

	int inc=10; //some kind of uncertainty to be estimated
				// different for x and y ??

	float dist;
	double beta_rad; // must be double to match with trigonometric functions
	// first implementation
	// look for the zero angle
	if(pt_sData->angle[i]) // if zero do else find zero
		while(pt_sData->angle[i])
			i++;

	idxAngle0=i;

	// Zero angle found
	//printf("%d",sData.angle[i]);
	for(i=0;i<181;i++){

		alpha_deg = pt_sData->angle[idxAngle0+i]; //called alpha
		beta_deg = alpha_deg+pt_pData->theta;
		dist = (float)pt_sData->sonarDistance[idxAngle0+i];
		// esiste la funzione deg to rad
		if (beta_deg<0)
			beta_deg=beta_deg+360;
		//x_new[]=
		beta_rad=beta_deg/180*PI;
		///PSEUDO CODICE DI QUELLO CHE DEVE FARE PER AGGIORNARE LA POSIZIONE

		//Da implementare i controlli con gli uguali
		 *
		 *
		 *
		if(beta_deg>315 || beta_deg <45){ 			// "I" in the graph --> refresh x
			x_new1=X_DIM-dist*cos(beta_rad); //CORRECT

		}else if (beta_deg>45 && beta_deg <135){ 	// "II" in the graph --> try to refresh y
			y_new2=Y_DIM-dist*sin(beta_rad); // TO BE CHECKED

		}else if(beta_deg>135 && beta_deg <225){	// "III" in the graph --> refresh x
			x_new3=dist*cos(180-beta_rad);	 //CORRECT

		}else if(beta_deg>225 && beta_deg <315){	// "IV" in the graph  --> refresh y
			y_new4=dist*cos(90-beta_rad);	 //CORRECT
		}

	}

		//PSUEDOCODICE DI
	if( x_new1 + x_new3 < X_DIM - inc ){
		//objDetected;
		printf("object detected\n");

	}else{
		printf("upgrade x position\n");
		//upgrade x position
	}

	if( y_new2 + y_new4 < Y_DIM -inc){
		printf("object detected\n");

	}else{
		printf("upgrade y position\n");

	}

	*/


	//return pt_pData;
	return;
};


//GOOD
// Padding of the matrix of with enemyfield Value
static void _enemyfieldPadding(fieldData *pt_fData){

	// printf("_enemyfieldPadding\n");
	int i,j;
	const int startEnemyField = (int) Y_DIM/2;

	for(i=0; i<X_DIM;i++){
		for(j=startEnemyField; j<Y_DIM; j++)	// approximatelly half field is the enemy field
										// this value can be updated
		{
			pt_fData->field[i][j] = ENEMY_FIELD;
		}
	}

	return;
};

//GOOD
// Delete every information except the object ones
static void _onlyBorderPadding(fieldData *pt_fData){

	// printf("_onlyBorderPadding\n");
	int i,j;

	for(i=0; i<X_DIM;i++){
		for(j=0; j<Y_DIM; j++){	// approximatelly half field is the enemy field
			if(pt_fData->field[i][j]!=OBJECTS)
				pt_fData->field[i][j] = UNEXPLORED;
		}
	}

	return;
};


static void _fieldSeparator(fieldData *pt_fData){

	// printf("_fieldSeparator\n");
	int i;

	for(i=0; i<X_DIM;i++)
		pt_fData->field[i][Y_DIM/2]=ENEMY_FIELD;


	return;
};

//GOOD
// Padding of the matrix of with unexplored Value
static void _unexploredPadding(fieldData  *pt_fData){

	// printf("_unexploredPadding\n");
	int i,j;

	for(i=0; i<X_DIM;i++){
		for(j=0; j<Y_DIM; j++)
		{
			pt_fData->field[i][j] = UNEXPLORED;
		}
	}

	return;
};


//GOOD
void localizationInit( fieldData *pt_fData, positionData *pt_pData){

	// printf("localizationInit\n");

	pt_pData->x = X_START;
	pt_pData->y = Y_START;
	pt_pData->theta= THETA_START ;


	_unexploredPadding(pt_fData);
	_enemyfieldPadding(pt_fData);


	return;
};




//GOOD
void refreshData(struct localizationData *pt_lData){

	// printf("refreshData\n");


	_refreshEnvData(pt_lData);
	_refreshPositionData(&(pt_lData->pData),&(pt_lData->sData));
	_fieldSeparator(&(pt_lData->fData));
	// _onlyBorderPadding(&(pt_lData->fData));

	//only if we wanna delete the enemy field information but we are already analyzing only our field
	//	_enemyfieldPadding(&(pt_lData->fData));

	return;
};



void setPositionManually(positionData *pt_pData,int x, int y, int theta){

	pt_pData->x = x;
	pt_pData->y = y;
	if(theta <0)
		theta=theta+360;

	pt_pData->theta= theta ;

	return;
};
