/*
 * obj_detect.c
 *
 *  Created on: 15 dic 2019
 *      Author: ironmath
 */

#include "obj_detect.h"

static float _stdInt( int  *vect, float mean,int idxstart,int idxfinish){
	int i;
	float sum_std=0;
	float std;
	int len=0;
	len=idxfinish-idxstart; //+1

	for(i=idxstart; i<=len; i++){ //stretto
		// correct std
		sum_std += pow(vect[i] - mean,2)/(len); //len -1to avoid ovf because big numbers
	}
	std=sqrt(sum_std);
	return std;
}

//IT WORKS
static int _sumColumns( char  *vect, int idx_start, int len){
	int i;
	int sum=0;
	for(i=idx_start; i<len; i++){
		sum += (int) vect[i];
	}
	return sum;
}
//IT WORKS
static float _meanInt( int  *vect, int idx_start, int len){
	int i;
	int sum=0;
	for(i=idx_start; i<len; i++){
		sum += vect[i];
	}
	return (float)sum/(len-idx_start);
}

static void _diffInt( int  *vect, int len){
	int i;
	int d;
	for(i=0; i<len-1; i++){
		d=vect[i+1]-vect[i];
		vect[i] = d;
	}

	return;
}

/*
static int _sumRows( char* vect, int idx_start, int len, int nColumns){
	int i;
	int sum=0;
	for(i=idx_start; i<len; i++){
		sum += (int) vect[i*nColumns];
	}
	return sum;
}
*/
static int _isItACube(int * verticalAng, int *verticalDistance){
	// printf("_isItACube\n");

	int idx=0;
	float mean_tmp;
	float avg_val_0_180,std_val_0_180;

    while(verticalAng[idx]<150 &&idx<200)
        idx=idx+1;


//  float _meanInt( int  *vect, int idx_start, int len);
    avg_val_0_180=_meanInt(verticalDistance,0,idx+1)-_meanInt(verticalDistance,0,2+1);
    mean_tmp=_meanInt(verticalDistance,0,idx+1);
//  float _stdInt( int  *vect, float mean,int len);
    std_val_0_180=_stdInt(verticalDistance,mean_tmp,0,idx);

    // printf("Mean of first %d values: %f\n",idx,avg_val_0_180);
    // printf("Std  of first %d values: %f\n",idx,std_val_0_180);
		// printf("Thresholds: Meanidx<0.2\tStdidx<2\n");

    if(abs(avg_val_0_180)<0.2 && std_val_0_180<2){
        printf("\t\t\t\t\tCube\n");
        return CUBE;
    }

	return UNIDENTIFIED;
};
static int _isItACilinder(int verticalAng[200], int verticalDistance[200]){
	// printf("_isItACilinder\n");

	int idx20=0;
	int idx120;
	int i;
	int verticalDistDiff[200]={0};
	int help_std[50]={0};
	float mean_tmp;
	float avg_diff_20_120,std_20_120;

	for(i=0;i<200;i++)
		verticalDistDiff[i]=verticalDistance[i];

	while(verticalAng[idx20]<20)
		idx20=idx20+1;

	idx120=idx20;

	while(verticalAng[idx120]<120)
		idx120=idx120+1;

	_diffInt( verticalDistDiff, 200); // modify the vector

	avg_diff_20_120=_meanInt(verticalDistDiff,idx20,idx120);//OK
	mean_tmp=_meanInt(verticalDistance, idx20, idx120+1); //OK
	//6.6626
	for(i=0;i<50;i++){
		help_std[i]=verticalDistance[idx20+i];
	}
	std_20_120=_stdInt(help_std,mean_tmp,0,idx120-idx20); // manca iniziare da 20


	// printf("Mean(diff) 20:120 values: %f\n",avg_diff_20_120 );
	// printf("Std        20:120 values: %f\n",std_20_120 );
	// printf("Thresholds: MeanDiff<0\tStd>5\n");
	if(avg_diff_20_120<0 && std_20_120>5){
		printf("\t\t\t\t\tCilinder for sure\n");
		return CILINDER;
	}
	return UNIDENTIFIED;
};

static int _isItAPiramid(int verticalAng[200], int verticalDistance[200]){
	// printf("_isItAPiramid\n");

	int randomPiramid = (rand() % 4) + 2;
	printf("\t\t\t\t\tPiramid %d \n", randomPiramid);

	return randomPiramid;

	return UNIDENTIFIED;
};

//#define CUBE 		1
//#define CILINDER 	2
//#define PIRAMID4 	3
//#define PIRAMID3 	4
//#define PIRAMID4_UD	5
//#define PIRAMID3_UD	6

static int _isAnObject(int verticalAng[200], int verticalDistance[200]){
	float     mean_tmp=_meanInt(verticalDistance,0,150);
	// printf("Threshold: 1200, mean temp: %f\n",mean_tmp);
	if(mean_tmp>1200)
		return 0;
	else
		return 1;

}

int determineShape(float datas[400][2]){

	// printf("_determineShape\n");
	int verticalAng[200];
	int verticalDistance[200];
	int i;
	int shape=0;

	for(i=0;i<200;i++){
		verticalAng[i]=(int)datas[i][0];
		verticalDistance[i]=(int)datas[i][1];
	}

	if(_isAnObject(verticalAng,verticalDistance)){

		shape=_isItACube(verticalAng,verticalDistance);
		if(!shape)
			shape= _isItACilinder(verticalAng, verticalDistance);
		if(!shape)
			shape= _isItAPiramid(verticalAng, verticalDistance);
	}

	return shape;
}





void analyzeScanning(positionData *pt_pData){

	// If there is some data that we don't expect
	/*
	pt_pData->x;
	pt_pData->y;
	pt_pData->theta;
	dist=pt_lData->sData.sonarDistance[i];



	angleMeasDeg=pt_lData->sData.angle[i];
	angleFieldDeg =  90 + angleMeasDeg + theta;// 180 - (angleMeasDeg + theta + 90);

	x2_free= x1_free + dist*cos(degToRad(angleFieldDeg));
	y2_free= y1_free + dist*sin(degToRad(angleFieldDeg));
	*/

};



void findObjPosition(fieldData *pt_fData, int num ){
	/* Y_DIM is often used as Y_DIM/2 because the search
	 * of the obj is done in one part of the field
	 */

	// printf("findObjPosition\n");
	FILE *fp;
	fp=fopen("axisData.txt","w");
	if(fp==NULL){
		// printf("Unable to open the file");
		return;
	}

	int k_factor=1.5;

	int i, j;
	volatile int sum;
	float mean_x, mean_y;
	float std_x, std_y;

	double sum_tot_x,sum_tot_y;


	double th_max_x,th_max_y;
	int obj_dim_x;
	int obj_dim_y;

	int count;

	int max_x_idx=0;
	int max_y_idx=0;


	int x[X_DIM]={0};
	int y[Y_DIM]={0};

	/////////////////////////X analisys//////////////////////////////

	sum_tot_x=0;
	for(i=0; i<X_DIM; i++){

		sum = _sumColumns( pt_fData->field[i], 0, Y_DIM/2); // sum until our field (Y_DIM) to search for the obj

		x[i]=sum;
		if(i && x[max_x_idx]<sum)
			max_x_idx=i;

		sum_tot_x+=sum;
	}

	mean_x= (int) sum_tot_x/X_DIM;

	//STD DEVIATION
	std_x=_stdInt(x, mean_x, 0,X_DIM); // x doesn't change

	//th_max_x=0.1*x[max_x_idx]+std_x*k_factor;
	th_max_x=std_x*k_factor;
	count=0;

	// SUBTRACT 3*STD and padding
	for(i=0; i<X_DIM; i++){
//			x[i] = x[i] - std_x*k_factor;
			if(x[i]<th_max_x){
				x[i]=0;
				if(i>4 && (x[i-1] || x[i-2] || x[i-3] || x[i-4]))
					count++;
			}else{
				count++;
				//x[i]=1; //SE C'È UN OGGETTO METTI UNO PER TROVARE GLI OSTACOLI
			}
	}
	obj_dim_x=count;

	/////////////////////////Y analisys//////////////////////////////

	sum_tot_y=0;
	for(j=0; j<Y_DIM/2; j++){  // sum until our field to search for the obj
		sum=0;

		for(i=0; i<X_DIM; i++){
			sum+=(int)pt_fData->field[i][j];
		}
		y[j]=sum;

		if(j && y[max_y_idx]<sum)
			max_y_idx=j;

		sum_tot_y+=sum;
	}

	mean_y=(int)sum_tot_y/Y_DIM;

	// SUBTRACT 3*STD and padding
	std_y=_stdInt(y, mean_y, 0,Y_DIM); //y doesn't change


	//th_max_y=0.1*y[max_y_idx]+std_y*k_factor;
	th_max_y=std_y*k_factor;
	count=0;
	for(j=0; j<Y_DIM/2; j++){
//		y[j] = y[j] - std_y*k_factor;
		if(y[j]<th_max_y){ // oppure check con 0
			y[j]=0;
			if(j>4 && (y[j-1] || y[j-2] || y[j-3] || y[j-4]))
							count++;
		}else{
			count++;


			//SE C'È UN OGGETTO METTI UNO PER TROVARE GLI OSTACOLI
//			y[j]=1; // per semplicità di visualizzazione poi si deve cambiare per riconoscere le forme
		// sto perdendo un sacco di dati
		}
	}
	obj_dim_y=count;



	//CLEAN DATA
	// remember on MATLAB I subtract the avg 2 times
	// --> remove the avg
	// --> after few simulation the best compromise to detect an obj is padding of zero, the data that are below a threshold
	// --> threshold set as k*std, k=3;
	// --> after this look for the max
	// --> another threshold to 1/4 of the max
	// --> look for continuos datas over the threshold to determine the position



	fprintf(fp,"Mean_x: %f\t Max_x_idx: %d\tStd_x: %d\n",mean_x, max_x_idx, std_x);
	fprintf(fp,"Obj_dim_x: %d\n",obj_dim_x);

	// printf("Mean_x: %f\t Max_x_idx: %d\tStd_x: %d\n",mean_x, max_x_idx, std_x);
	// printf("Obj_dim_x: %d\n",obj_dim_x);

	for(i=0; i<X_DIM; i++){
		fprintf(fp,"%d ",x[i]);
	}

	fprintf(fp,"\nMean_y: %f\t Max_y_idx: %d\tStd_y: %d\n",mean_y, max_y_idx, std_y);
	fprintf(fp,"Obj_dim_y: %d\n",obj_dim_y);

	// printf("\nMean_y: %f\t Max_y_idx: %d\tStd_y: %d\n",mean_y, max_y_idx, std_y);
	// printf("Obj_dim_y: %d\n",obj_dim_y);


	for(j=0; j<Y_DIM; j++){
		fprintf(fp,"%d ",y[j]);
	}
	fclose(fp);
	return;
}


int isThereObj(fieldData *pt_fData){

//	printf("isThereObj\n");

//	FILE *fp;
//	fp=fopen("axisData.txt","w");
//
//	if(fp==NULL){
//		printf("Unable to open the file");
//		return -1;
//	}

//	int k_factor=2;
//
//	int i, j;
//	int sum;
//	double sum_tot_x,sum_tot_y;
//	double sum_std_x,sum_std_y;
//	int mean_x, mean_y;
//	double std_x, std_y;
//	double th_max_x,th_max_y;
//	int obj_dim_x;
//	int obj_dim_y;
//
//	int count;
//
//	int max_x_idx=0;
//	int max_y_idx=0;
//
//
//	int x[X_DIM];
//	int y[Y_DIM];
//
//	sum_tot_y=0;
//	for(j=0;j<Y_DIM/2; j++){
//		sum=0;
//		for(i=0; i<X_DIM; i++){
//			sum+=pt_fData->field[i][j];
//		}
//		y[j]=sum;
//		if(i && x[max_x_idx]<sum)
//			max_x_idx=i;
//
//		sum_tot_y+=sum;
//	}
//
//	mean_x=(int)sum_tot_x/(X_DIM/2);
//
//	printf("mean_x");

//	if(mean_x>)


}
