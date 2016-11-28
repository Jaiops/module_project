#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/slab.h>

#define MAX_DATA_LENN 1000
#define GET 0
#define PUT 1
#define REMOVE 2
int len,temp;
char * world = "world";
char *msg;

hasmdata_t *map;




struct map_data{
	uint8_t code;
	uint8_t flag;
	uint32_t key;

	uint8_t data[];
}



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

ssize_t write_proc(struct file *filp,const char *buf,size_t count,loff_t *offp)
{
	
	copy_from_user(msg,buf,count);
	struct map_data  *mdata = (map_data * )msg;
	int err;
	char * tmp;
	switch ( mdata->code ) 
	{
		case GET:
			tmp = hashmap_get(map,mdata->key);
			if(tmp != NULL){
				msg = tmp;
				err = 0;
			}
			else{

				err = 1;
			}
			break;
		case PUT:
			err = hashmap_put(map,mdata->key,mdata->flag, mdata->data);
			break;

		case REMOVE:
			err = hashmap_put_remove(map,mdata->key);

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
	return 0;
}

void proc_cleanup(void) {
	remove_proc_entry("hashmap",NULL);
	hasmdata_free(map);
	kfree(msg);
}

MODULE_LICENSE("GPL"); 
module_init(proc_init);
module_exit(proc_cleanup);