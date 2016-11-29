/* Return values */
#define HMAP_OK 0
#define HMAP_OCCUPIED 1
#define HMAP_NOEXIST 2

/* Flags for put */
#define HMAP_IGNORE 16

struct node{
	struct node* next;
	uint32_t *key; //should be char[]
	uint8_t *value;
} node;

struct {
	uint32_t size;
	uint32_t stored;
	struct node* storage[];
} hashmap_t;

struct hashmap_t *hashmap_setup();

uint8_t[] hashmap_get(struct hashmap_t map, uint32_t key);

uint8_t hashmap_put();

uint8_t hashmap_remove();
