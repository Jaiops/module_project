#include "hashtable.h"
#include <linux/types.h>

#define NULL ((void *)0)

int main() {
	uint8_t t = 99;
	hashtable_put(1, &t, 1);
	hashtable_put(152, &t, 1);
	hashtable_put(90, &t, 1);
	hashtable_put(44, &t, 1);
	hashtable_put(12, &t, 1);
	hashtable_put(95, &t, 1);
	return 0;
}
