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


struct mystruct {
	int data;
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

Copies msg into buf which is in the user space */
ssize_t read_proc(struct file *filp,char *buf,size_t count,loff_t *offp ) 
{	
	//TODO: Stop returning the msg string and instead return the data in the hashmap
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
	struct map_data  *mdata = (struct map_data * )msg;
	int err;
	char * tmp;
	//Reads the mdata struct from user space
	copy_from_user(msg,buf,count);
	switch ( mdata->code ) 
	{
		case GET:
			//TODO: Return the value at key
			break;
		case PUT:
			//TODO: Store data into the hashmap
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
	remove_proc_entry("hashmap",NULL);
	kfree(msg);
}

MODULE_LICENSE("GPL"); 
module_init(proc_init);
module_exit(proc_cleanup);
