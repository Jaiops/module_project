#include <stdio.h>
#include "../hashtable.h"

void fail(void);

void putval(int key, int val);

uint8_t * getval(int key);

void removval(uint8_t key);

int compareStrings(uint8_t * v1 , uint8_t *v2, ssize_t len);
