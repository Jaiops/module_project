#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/hashtable.h>
#include <asm/uaccess.h>
#include <linux/slab.h>

#define MAX_DATA_LENN 1000
#define GET 0
#define PUT 1
#define REMOVE 2
int len,temp;
char * world = "world";
char *msg;
DEFINE_HASHTABLE(map, 3);


struct stored {
	void *data;
	size_t dataSize;
	struct hlist_node next ;
};



struct map_data{
	uint8_t code;
	uint8_t flag;
	uint32_t key;

	uint8_t data[];
};


/* 
@filp 	pointer to the proc file
@buf	pointer to the char buffer to read to
@count 	number of bytes to copy
@offp 	the long offset

Returns msg which contains the value corresponding to the key which was written with the get command */
ssize_t read_proc(struct file *filp,char *buf,size_t count,loff_t *offp ) 
{	
	if(count>temp)
	{
		count=temp;
	}
	temp=temp-count;
	copy_to_user(buf,msg, count);
	if(count==0)
		temp=len;

	return count;
}

/* Reads mdata object from user space and does corresponding action depending on the mdata->code*/
ssize_t write_proc(struct file *filp,const char *buf,size_t count,loff_t *offp)
{
	//Converts char* msg into a mdata struct
	struct map_data  *mdata = (struct map_data * )buf;
	size_t dataSize = count -6;
	int err;
	uint8_t * tmp;
	struct stored *s;
	printk("code: %u\n", mdata->code);
	switch ( mdata->code ) 
	{
		case GET:
			//TODO: Change msg into the value of the key
			
			break;
		case PUT:
			printk("putting!\n");
			tmp = kmalloc(dataSize, GFP_KERNEL);
			copy_from_user(tmp,buf+6,dataSize);
			
			s = kmalloc(sizeof(struct stored), GFP_KERNEL);
			s->data = tmp;
			s->dataSize = dataSize;

			hash_add(map, &s->next, mdata->key);
			break;

		case REMOVE:
			//TODO: Remove key:value pair
			break;

		default:

			//What just happened?
			break;

	}


	len=count;
	temp=len;
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
	struct stored* current_node;
	int bkt;
	printk("CLEANING!!?!?!?!?!?!?\n");
	remove_proc_entry("hashmap",NULL);

	/*Free all entries in the hashtable*/
	hash_for_each(map, bkt, current_node, next){
		printk("key: %d\tbucket: %d\n", 1, bkt);
		kfree(current_node->data);
		kfree(current_node);
	}
	kfree(msg);
}

MODULE_LICENSE("GPL"); 
module_init(proc_init);
module_exit(proc_cleanup);
