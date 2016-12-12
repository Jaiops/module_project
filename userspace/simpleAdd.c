#include "hashtable.h"

void putval(int key, int val){
	int err; 
	err = hashtable_put(key,&val,sizeof(int)); 
	if(err == -1){ 
		printf("%s\n","...hashtable_put failed while putting to the hashtable" );
	} 
}

int main() {
	putval(16, 50005);
	putval(89, 5055);
	putval(616, 1337);
	putval(666, 777);
	return 0;
}
