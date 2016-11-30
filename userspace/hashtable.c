#include "hashtable.h"

#define MAX_PAYLOAD_SIZE 1000
struct map_data{
	uint8_t code;
	uint8_t flag;
	uint32_t key;

	uint8_t data[0];
};

uint8_t  * hashtable_get(uint32_t key)
{
	struct map_data m;
	m.code = 0;
	m.key = key;

	int fd = open("/proc/hashmap", O_RDWR);
	if(fd == -1){
		printf("%s\n","Error, can't open the hashmap file!" );
	}
	ssize_t err;
	err = write(fd, &m, sizeof(struct map_data));
	printf("%ld\n",err );
	if(err == -1){
		perror("write request");
		printf("%s\n","Error when writing to hashmap file!" );

	}else if(err == 0){
		printf("%s\n","No value was found" );
		return NULL;
	}

	uint8_t *returnData = calloc(MAX_PAYLOAD_SIZE, 1);
	err = read(fd, returnData, MAX_PAYLOAD_SIZE);
	if (err == -1) {
		perror("read answer");
	}
	close(fd);
	return returnData;
}

int hashtable_put(uint32_t key, uint8_t * value, size_t dataSize)
{
	if (dataSize > MAX_PAYLOAD_SIZE) {
		fprintf(stderr, "Size of data bigger than allowed\n");
	}

	struct map_data *m = calloc(1, sizeof(struct map_data) + dataSize);
	m->code = 1;
	m->key = key;
	memcpy(m->data, value, dataSize);

	int fd = open("/proc/hashmap", O_RDWR);
	if(fd == -1){
		printf("%s\n","Error, can't open the hashmap file!" );
		return -1;
	}
	ssize_t err;
	err = write(fd, m, sizeof(struct map_data) + dataSize);
	if(err == -1){
		perror("write request");
		printf("%s\n","Error when writing to hashmap file!" );
		return -1;
	}
	return 0;
}

int hashtable_remove(uint32_t key)
{
	struct map_data *m = calloc(1, sizeof(struct map_data));
	m->code = 2;
	m->key = key;

	int fd = open("/proc/hashmap", O_RDWR);
	if(fd == -1){
		printf("%s\n","Error, can't open the hashmap file!" );
		return -1;
	}
	ssize_t err;
	err = write(fd, m, sizeof(struct map_data));
	if(err == -1){
		perror("write request");
		printf("%s\n","Error when writing to hashmap file!" );
		return -1;
	}

	close(fd);
	return 0;
}
