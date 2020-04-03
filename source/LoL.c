/*
 ============================================================================
 Name        : LoL.c
 Author      : ironmath
 Version     :
 Copyright   : ToBeAdded
 Description : Project of Operating System Course Group Composed by:
 	 	 	 	 -Matteo Guarrera
 	 	 	 	 -Riccardo Torres
 	 	 	 	 -Andra Trufini
 ============================================================================
 */


#include <stdio.h>
#include <stdlib.h>

#include "callMapping.h"

int main(void) {

	system("./removeImage.sh");
	callMapping();

	system("./convNormal.sh"); //function not secure but we don't care
	system("./beforeMerge.sh"); //function not secure but we don't care


	return EXIT_SUCCESS;
}
