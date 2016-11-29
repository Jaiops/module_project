#include "hashmap.h"

struct hashmap_t *hashmap_setup()
{
	struct hashmap_t *map = kmalloc(sizeof(struct hashmap_t));
	map->storage= kmalloc(sizeof(struct node*)*10);
	map->size = 0;
	map->stored = 0;

	return map;
}

uint8_t[] hashmap_get(struct hashmap_t *map, uint32_t key)
{
	return NULL;
}

uint8_t hasshmap_put()
{
	return HMAP_OK;
}

uint8_t hashmap_remove()
{
	return HMAP_OK;
}
