#include "test_util.h"

int main(){

	printf("\n%s\n","GETTING OBJECTS:" );
	for (int i = 0; i < 10; ++i){

		uint8_t * v = getval(i);
		
		if( v[0] != i+2){
			printf("%s%d%s%d\n","...TEST FAILED, was",v[0],"expected",i+2 );
			fail();
		}
	}

	printf("%s\n","TEST SUCCESSFUL!" );

}