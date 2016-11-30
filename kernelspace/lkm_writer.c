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
int len,temp;
char * world = "world";
char *msg;
char *output;
DEFINE_HASHTABLE(map, 3);


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


/** 
 * @filp 	pointer to the proc file
 * @buf	pointer to the char buffer to read to
 * @count 	number of bytes to copy
 * @offp 	the long offset
 * 
 * Returns msg which contains the value corresponding to the key which was 
 * written with the get command 
 */
ssize_t read_proc(struct file *filp,char *buf,size_t count,loff_t *offp ) 
{	
	if(count>temp)
	{
		count=temp;
	}
	temp=temp-count;
	copy_to_user(buf,output, count);
	if(count==0)
		temp=len;

	output = NULL;
	return count;
}

/* Reads mdata object from user space and does corresponding action depending 
 * on the mdata->code*/
ssize_t write_proc(struct file *filp,const char *buf,size_t count,loff_t *offp)
{
	printk(KERN_WARNING "In write_proc()\n");

	//Converts char* msg into a mdata struct
	copy_from_user(msg,buf,count);
	struct map_data *mdata = (struct map_data*)msg;

	printk(KERN_WARNING "Casted buf to map_data struct\n");

	size_t data_size = count - MAP_DATA_OFFSET;

	printk(KERN_WARNING "Declared data_size\n");

	uint8_t *tmp;
	struct hashmapEntry *entry = NULL;
	struct hashmapEntry *current_entry;

	printk(KERN_WARNING "Declared tmp, entry and current_entry\n");

	printk(KERN_WARNING "code: %u\n", mdata->code);
	switch ( mdata->code ) 
	{
		case GET:
			printk(KERN_WARNING "Inside switch GET\n");

			hash_for_each_possible(map, current_entry, next, mdata->key){
				if(current_entry->key == mdata->key)
					entry = current_entry;
			}
			if(entry!=NULL){
				printk(KERN_WARNING "entry != null\n");

				output = (char*)(entry->data);
				len = entry->data_size;
				printk("output is :%d\n",*(uint8_t *)entry->data );
				temp = len;
			} else{
				printk(KERN_WARNING "get found no matching key, returning 0\n");

				// Value wasn't found return 0
				return 0;
			}
			printk(KERN_WARNING "GET DONE\n");

			break;
 
		case PUT:
			printk(KERN_WARNING "Inside switch PUT\n");
			tmp = kmalloc(data_size, GFP_KERNEL);
			memcpy(tmp,mdata->data,data_size);
			
			entry = kmalloc(sizeof(struct hashmapEntry), GFP_KERNEL);
			entry->data = tmp;
			printk(KERN_WARNING "Value in put is %d\n",*(uint8_t *)entry->data );
			entry->data_size = data_size;
			entry->key = mdata->key;
			hash_add(map, &entry->next, mdata->key);
			printk(KERN_WARNING "PUT DONE\n");

			break;

		case REMOVE:
			printk(KERN_WARNING "Ínside removed\n");

			hash_for_each_possible(map, current_entry, next, mdata->key){
				if(current_entry->key == mdata->key)
					entry = current_entry;
			}
			if(entry!=NULL){
				
				printk(KERN_WARNING "entry removed\n");

				hash_del(&entry->next);

			} else{
				printk(KERN_WARNING "remove found no matching key\n");

				// Value wasn't found return 0
				return 0;
			}


			break;

		default:

			//What just happened?
			break;

	}

	return count;
	//Error handling? Do we have to return count or can we return an error number?
}

struct file_operations proc_fops = {
	read: read_proc,
	write: write_proc
};

void create_new_proc_entry(void) 
{
	proc_create("hashmap",0666,NULL,&proc_fops);
	msg=kmalloc(MAX_DATA_LENN*sizeof(char),GFP_KERNEL);

}


int proc_init (void) {
	create_new_proc_entry();
	hash_init(map);
	return 0;
}

void proc_cleanup(void) {
	struct hashmapEntry* current_entry;
	int bkt;
	printk(KERN_WARNING "Inside proc_cleanup()\n");
	remove_proc_entry("hashmap",NULL);

	/*Free all entries in the hashtable*/
	hash_for_each(map, bkt, current_entry, next){
		printk("key: %d\tbucket: %d\n", 1, bkt);
		kfree(current_entry->data);
		kfree(current_entry);
	}
	kfree(msg);
	printk(KERN_WARNING "proc_cleanup() done!\n");

}

MODULE_LICENSE("GPL"); 
module_init(proc_init);
module_exit(proc_cleanup);
