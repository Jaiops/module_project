#include "hashtable.h"
#include <linux/types.h>

#define NULL ((void *)0)

void fail(){
	//printf("%s\n","Remove and reinsert the module to clear the table, please" );
	exit(1);
}
void putval(uint8_t key, uint8_t val){
	int err; 
	err = hashtable_put(key,&val,1); 
	if(err == -1){ 
		printf("%s\n","hashtable_put failed while putting to the hashtable" );
		fail(); 
	} 
}

//SHOULD ONLY BE USED ON EXISTING VALUES
uint8_t * getval(uint8_t key){
	uint8_t *ret;
	ret = hashtable_get(key);
	if(ret == NULL){
		printf("%s\n","error in getval, value does not exist!" );
		fail();
	}
	return ret;
}

void removval(uint8_t key){
	int err;
	err = hashtable_remove(key);
	if(err == -1){
		printf("%s\n","hashtable_remove failed while removing from the hashtable" );
		fail();
	}
}

void testPutOne(){
	putval(1,5);
	printf("%s\n","testPutOne succeded!" );
}

void testPutGetOne(){
	uint8_t * ret;
	putval(1,5);
	ret = getval(1);
	if(ret[0] != 5){
		printf("%s %d\n","testPutGetOne failed! was", ret[0] );
	}else{
		printf("%s\n","testPutGetOne succeded!" );

	}

}
void testNoObject(uint8_t key){
	uint8_t *ret;
	ret = hashtable_get(key);
	if(ret != NULL){
		printf("%s\n","testNoObject failed while getting value from the hashtable" );
		fail();
	}

		printf("%s\n","testNoObject successful!" );	

}

void testOneObject(){
	uint8_t val = 10;
	uint8_t * ret;
	uint8_t key = 1;
	putval(key,val);

	ret = getval(key);

	if(val != ret[0]){
		printf("%s%d%s%d\n","testOneObject failed while getting value from the hashtable, was ", ret[0]," should be ",val );
		fail();
	}

	removval(key);
	testNoObject(key);

	printf("%s\n","testOneObject successful!" );	
}


/*
void testPutRemoveOneObject(){
	uint8_t val = 10;
	uint8_t * ret;
	int err;
	err = hashtable_put(1,&val,1);
	if(err == -1){
		printf("%s\n","testOneObject failed while putting to the hashtable" );
		fail();
	}
	ret = hashtable_get(1);
	if(val != ret[0]){
		printf("%s\n","testOneObject failed while getting value from the hashtable, was %d should be %d",ret[0],val );
		fail();
	}
	err = hashtable_remove(1);
	if(err == -1){
		printf("%s\n","testOneObject failed while removing from the hashtable" );
		fail();
	}

	ret = hashtable_get(1);
	if(ret[0] != 0){
		printf("%s\n","testNoObject failed while getting value from the hashtable, should be 0 is %d",ret[0] );
		fail();
	}
}
void testTwoObject(){
	uint8_t val = 10;
	uint8_t val2 = 15;
	uint8_t * ret;
	int err;
	err = hashtable_put(1,&val,1);
	if(err == -1){
		printf("%s\n","testOneObject failed while putting to the hashtable" );
		fail();
	}
	ret = hashtable_get(1);
	if(val != ret[0]){
		printf("%s\n","testOneObject failed while getting value from the hashtable, was %d should be %d",ret[0],val );
		fail();
	}
	err = hashtable_remove(1);
	if(err == -1){
		printf("%s\n","testOneObject failed while removing from the hashtable" );
		fail();
	}
}
*/
int main() {
	printf("%s\n","STARTED TESTING!" );
	printf("%s\n","__TESTING testPutOne()" );

	//testPutOne();
	//printf("%s\n","__testPutOne() DONE!" );
	testOneObject();
	//testPutGetOne();
	printf("%s\n","__testPutGetOne() DONE!" );


	printf("%s\n","TESTING DONE!" );

	//testOneObject();
	//testNoObject();
	return 0;
}


