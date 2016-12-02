#include "hashtable.h"

#define MAX_PAYLOAD_SIZE 1000
struct map_data{
	uint8_t code;
	uint32_t key;

	uint8_t data[0];
};

ssize_t hashtable_get(uint32_t key, void **data)
{
	ssize_t valueSize;
	struct map_data m;
	m.code = 0;
	m.key = key;

	int fd = open("/proc/hashmap", O_RDWR);
	if(fd == -1){
		printf("%s\n","Error, can't open the hashmap file!" );
		return -1;
	}
	flock(fd, LOCK_EX);
	valueSize = write(fd, &m, sizeof(struct map_data));

	if(valueSize == -1){
		perror("write request");
		printf("%s\n","Error when writing to hashmap file!" );

		flock(fd, LOCK_UN);
		close(fd);
		return -1;

	}else if(valueSize == 0){

		flock(fd, LOCK_UN);
		close(fd);
		return 0;
	}

	*data = calloc(valueSize, 1);
	valueSize = read(fd, *data, valueSize);
	flock(fd, LOCK_UN);
	if (valueSize == -1) {
		perror("read answer");
	}
	close(fd);
	return valueSize;
}

int hashtable_put(uint32_t key, void *value, size_t dataSize)
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

/**
 * userspace call to clear all entries in the hashmap.
 */
int hashtable_clear()
{
	struct map_data *m = calloc(1, sizeof(struct map_data));
	m->code = 3;
	
	int fd = open("/proc/hashmap", 0_RDWR);
	if(fd == -1){
		printf("%s\n","Error, can't open the hashmap file!");
	}
	
	ssize_t err;
	
	flock(fd, LOCK_EX);

	err = write(fd, m, sizeof(struct map_data));

	flock(fd, LOCK_UN);
	close(fd);

	return 0;
}






