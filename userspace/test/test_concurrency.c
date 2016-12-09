#include "test_util.h"
#include <stdlib.h>
#include <stdio.h>
int main( int argc, char *argv[]){

	printf("%s\n","starting test_conc application" );
	if(argc != 4){
		printf("%s%d%s%d\n","invalid args! was",argc, " expected ",4 );
		exit(1);
	}
	int start = atoi(argv[1]);
	int size = atoi(argv[2]);
	int appnr = atoi(argv[3]);
	printf("%s%d%s\n","Test starting ", appnr," ..." );
	for (int i = 0; i < 3; ++i){
		
		for (int j = 0; j < size; ++j){
				putval(j+start,j+start);

		}

		for (int j = 0; j < size; ++j){

			int * val = (int*)getval(j+start);
			if(*val != j+start){
				printf("%s%d%s%d\n","Error, getval returned wrong value. Was ",val[0]," expected ",j+start );
				printf("%s%d%s\n","...application ",appnr, " failed." );
				exit(1);
			}
		}

		for (int j = 0; j < size; ++j){
			removval(j+start);

		}

	}
	printf("%s%d%s\n","Test with application ",appnr, " successful!" );	

}