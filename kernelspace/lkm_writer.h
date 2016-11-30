
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/hashtable.h>
#include <asm/uaccess.h>
#include <linux/slab.h>

// number of bytes in the mapdata struct before the data array
#define MAP_DATA_OFFSET 6
#define MAX_DATA_LENN 1000

/* Defines what the "code" variable mean regarding which hashmap interface 
 * function should be used */
#define GET 0
#define PUT 1
#define REMOVE 2

struct hashmapEntry {
	void *data;
	uint32_t key;
	size_t data_size;
	struct hlist_node next ;
};



struct map_data{
	uint8_t code;
	uint8_t flag;
	uint32_t key;

	uint8_t data[0];
};


struct hashmapEntry *get_entry(uint32_t key);

int get(struct map_data *mdata);

void put(struct map_data *mdata, size_t data_size);

ssize_t write_proc(struct file *filp,const char *buf,size_t count,loff_t *offp);

ssize_t read_proc(struct file *filp,char *buf,size_t count,loff_t *offp );

void create_new_proc_entry(void);

int proc_init (void);

void proc_cleanup(void);
