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
		return NULL;
	}
	ssize_t err;
	flock(fd, LOCK_EX);
	err = write(fd, &m, sizeof(struct map_data));

	if(err == -1){
		perror("write request");
		printf("%s\n","Error when writing to hashmap file!" );

		flock(fd, LOCK_UN);
		close(fd);
		return NULL;

	}else if(err == 0){

		flock(fd, LOCK_UN);
		close(fd);
		return NULL;
	}

	uint8_t *returnData = calloc(MAX_PAYLOAD_SIZE, 1);
	err = read(fd, returnData, MAX_PAYLOAD_SIZE);
	flock(fd, LOCK_UN);
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
		return -1;
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
	flock(fd, LOCK_EX);
	err = write(fd, m, sizeof(struct map_data) + dataSize);
	flock(fd, LOCK_UN);
	if(err == -1){
		perror("write request");
		printf("%s\n","Error when writing to hashmap file!" );
		close(fd);
		return -1;
	}
	close(fd);

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

	flock(fd, LOCK_EX);

	err = write(fd, m, sizeof(struct map_data));


	flock(fd, LOCK_UN);


	if(err == -1){
		perror("write request");
		printf("%s\n","Error when writing to hashmap file!" );
		
		close(fd);
		return -1;
	}else if(err == 0){

		close(fd);
		return 1;
	}
	close(fd);

	return 0;
}
