#include "hashtable.h"
#include <linux/types.h>

#define NULL ((void *)0)

void fail(){
	printf("%s\n","The test suit failed, please remove the module and re-insert to clear possible data!" );
	exit(1);
}
void putval(uint8_t key, uint8_t val){
	int err; 
	err = hashtable_put(key,&val,1); 
	if(err == -1){ 
		printf("%s\n","...hashtable_put failed while putting to the hashtable" );
		fail(); 
	} 
}

//SHOULD ONLY BE USED ON EXISTING VALUES
uint8_t * getval(uint8_t key){
	uint8_t *ret;
	ret = hashtable_get(key);
	if(ret == NULL){
		printf("%s\n","...error in getval, value does not exist!" );
		fail();
	}
	return ret;
}

void removval(uint8_t key){
	int err;
	err = hashtable_remove(key);
	if(err == -1){
		printf("%s\n","...hashtable_remove failed while removing from the hashtable" );
		fail();
	}
}

void testPutRemoveOne(){
	putval(1,5);
	removval(1);
	printf("%s\n","...testPutRemoveOne successful!" );
}

void testNoObject(uint8_t key){
	uint8_t *ret;
	ret = hashtable_get(key);
	if(ret != NULL){
		printf("%s\n","...testNoObject failed while getting value from the hashtable" );
		fail();
	}

	printf("%s\n","...testNoObject successful!" );	
}

void testOneObject(){
	uint8_t val = 10;
	uint8_t * ret;
	uint8_t key = 1;
	putval(key,val);

	ret = getval(key);

	if(val != ret[0]){
		printf("%s%d%s%d\n","...testOneObject failed while getting value from the hashtable, was ", ret[0]," should be ",val );
		fail();
	}

	removval(key);

	testNoObject(key);

	printf("%s\n","...testOneObject successful!" );	
}

void testTwoObjects(){
	uint8_t val = 10;
	uint8_t val2 = 15;
	uint8_t * ret;
	uint8_t * ret2;


	putval(1,val);
	putval(2,val2);

	ret = getval(1);
	ret2 = getval(2);

	if(ret[0] != val){
		printf("%s %d %s %d\n","...testTwoObject failed. Was",ret[0],"expected",val );
		fail();
	}
	if(ret2[0] != val2){
		printf("%s %d %s %d\n","...testTwoObject failed. Was",ret2[0],"expected",val );
		fail();
	}

	removval(1);
	removval(2);

	printf("%s\n","...tesTwoObjects successful!" );	

}

void testSeveralGetCalls(){
	int count = 10;
	for (int i = 0; i < count; ++i)
	{
		putval(i+1, i+2);
	}

	uint8_t * ret;

	for (int i = 0; i < count; ++i)
	{
		ret = getval(i+1);
		if(ret[0] != i+2){
			printf("%s %d %s %d\n","...testSeveralGetCalls failed. Was",ret[0],"expected",i+2 );
			fail();
		}
	}
	for (int i = 0; i < count; ++i)
	{
		removval(i+1);
	}
	printf("%s\n","..testSeveralGetCalls successful!" );

}

void testRemoveNonExisting(){
	int err;
	err = hashtable_remove(1);

	if(err!=1){
		printf("%s\n","...removeNonExisting actually removed something!" );
		fail();
	}

	printf("%s\n","...removeNonExisting successful!" );
}


void testOverwriteValue(){
	uint8_t * ret;
	uint8_t * ret2;
	uint8_t * ret3;

	putval(2,10);
	ret = getval(2);

	if(10 != ret[0]){
		printf("%s%d%s%d\n","...testOneObject failed while getting value from the hashtable, was ", ret[0]," should be ",10 );
		fail();
	}

	putval(2,15);
	ret2 = getval(2);

	if(15 != ret2[0]){
		printf("%s%d%s%d\n","...testOneObject failed while getting value from the hashtable, was ", ret[0]," should be ",15 );
		fail();
	}

	removval(2);
	ret3 = getval(2);

	if(10 != ret3[0]){
		printf("%s%d%s%d\n","...testOneObject failed while getting value from the hashtable, was ", ret[0]," should be ",10 );
		fail();
	}

	removval(2);

	printf("%s\n","...overwriteValue() successful" );
}

int main() {
	printf("%s\n","STARTED TESTING!" );


	printf("\n%s\n","TESTING testNoObject:" );
	testNoObject(1);

	printf("\n%s\n","TESTING testRemoveNonExisting:" );
	testRemoveNonExisting();

	printf("\n%s\n","TESTING testPutRemoveOne:" );
	testPutRemoveOne();

	printf("\n%s\n","TESTING testOneObject:" );
	testOneObject();

	printf("\n%s\n","TESTING testTwoObjects:" );
	testTwoObjects();

	printf("\n%s\n","TESTING testSeveralGetCalls:" );
	testSeveralGetCalls();

	printf("\n%s\n","TESTING testOverwriteValue:" );

	testOverwriteValue();


	printf("\n%s\n","TESTING DONE!" );

	return 0;
}


