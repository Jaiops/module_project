#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

uint8_t  * hashtable_get(uint32_t key);

int hashtable_put(uint32_t key, uint8_t * value, size_t dataSize);

int hashtable_remove(uint32_t key);
