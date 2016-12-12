#include "test_util.h"

void fail(){
	printf("%s\n","The test suit failed, please remove the module and re-insert to clear possible data!" );
	exit(1);
}

void putval(int key, int val){
	int err; 
	err = hashtable_put(key,&val,sizeof(int)); 
	if(err == -1){ 
		printf("%s\n","...hashtable_put failed while putting to the hashtable" );
		fail(); 
	} 
}

//SHOULD ONLY BE USED ON EXISTING VALUES
uint8_t * getval(int key){
	uint8_t *val;
	ssize_t ret;
	ret = hashtable_get(key,(void** )&val);
	printf("key: %d\n", key);
	if(ret == 0){
		printf("%s\n","...error in getval, value does not exist!" );
		fail();
	}else if (ret == -1){
		printf("%s\n","...error in getval, error code returned from hashtable_get" );
		fail();
	}
	return val;
}

void removval(uint8_t key){
	int err;
	err = hashtable_remove(key);
	if(err == -1){
		printf("%s\n","...hashtable_remove failed while removing from the hashtable" );
		fail();
	}
}

int compareStrings(uint8_t * v1 , uint8_t *v2, ssize_t len){

	for (int i = 0; i < len; ++i){

		if (v1[i] != v2[i]){
			
			printf("%s%c%s%c\n","...compareStrings v1: ",v1[i], " v2: ", v2[i] );
			return -1;
		}
		
	}
	return 0;
}
