#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/**
 *File:  Hashtable.h
 * 
 * This file gives a interface to the linux kernel module lkm_writer which
 * Is a hashmap in kernelspace.
 * Communications between user - kernelspace is done through /proc/hashmap
 */

/** 
 * @key key for the element to get
 * 
 * Tries to get a value from the hashmap.
 *
 * return: pointer to the data, NULL when a element does not exist for the given key.
 */
ssize_t hashtable_get(uint32_t key, void **data);

/**
 * @key 	key for the element to insert.
 * @value 	the value to insert into the hashmap.
 * @dataSize size of @value to insert.
 *
 * Inserts a value into the hashmap.
 *
 * return 0 on success, else -1.
 */
int hashtable_put(uint32_t key, uint8_t * value, size_t dataSize);

/** 
 * @key key for the element to remove
 * 
 * Tries to remove a value from the hashmap.
 *
 * return: 0 on succes, 1 on value not found, -1 on error.
 */
int hashtable_remove(uint32_t key);
