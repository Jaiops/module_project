#include <hashtable.h>
#include <stdio.h>
struct map_data{
	uint8_t code;
	uint8_t flag;
	uint32_t key;

	uint8_t data[1000];
}

uint8_t  * hashtable_get(uint32_t key)
{
	struct map_data m;
	m.code = 0;
	m.key = key;

	FILE *f = fopen("/proc/hashmap","wr");
	if(f == NULL){
		printf("%s\n","Error, can't open the hashmap file!" );
	}
	int err;
	err = fputs((char * )m,f);
	if(err == EOF){
		printf("%s\n","Error when writing to hashmap file!" );

	}
	fgets(&m.data,1000,f);
	fclose(f);
	return m.data;
}

int hashtable_put(uint32_t key, uint8_t * value)
{
	struct map_data m;
	m.code = 1;
	m.key = key;
	m.data = value;

	FILE *f = fopen("/proc/hashmap","wr");
	if(f == NULL){
		printf("%s\n","Error, can't open the hashmap file!" );
	}
	int err;
	err = fputs((char * )m,f);
	if(err == EOF){
		printf("%s\n","Error when writing to hashmap file!" );
	}
	fgets(&m.data,1000,f);
	fclose(f);
}

int hashtable_remove(uint32_t key)
{
	struct map_data m;
	m.code = 2;
	m.key = key;

	FILE *f = fopen("/proc/hashmap","wr");
	if(f == NULL){
		printf("%s\n","Error, can't open the hashmap file!" );
	}
	int err;
	err = fputs((char * )m,f);
	if(err == EOF){
		printf("%s\n","Error when writing to hashmap file!" );
	}
	fgets(&m.data,1000,f);
	fclose(f);
}