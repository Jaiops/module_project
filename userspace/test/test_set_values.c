#include "test_util.h"

int main(){
	printf("%s\n","STARTED TESTING!" );

	printf("\n%s\n","INSERTING OBJECTS:" );
	for (int i = 0; i < 10; ++i){
	
		putval(i,i+2);
	}

	return 0;
}
