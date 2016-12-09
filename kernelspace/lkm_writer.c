#include "lkm_writer.h"

void clear_hashmap(void);

struct file_operations proc_fops = {
	read: read_proc,
	write: write_proc
};

int len,temp;
char * world = "world";
char *msg;
char *output;
DEFINE_HASHTABLE(map, 16);




/** 
 * @filp 	pointer to the proc file
 * @buf	pointer to the char buffer to read to
 * @count 	number of bytes to copy
 * @offp 	the long offset
 * 
 * Writes the value of output buffer into the proc file. 
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

/**
 * @key the key for the value in the map
 * Searches the hashmap, returns value if found, otherwise null
 */
struct hashmapEntry *get_entry(uint32_t key) 
{
	struct hashmapEntry *current_entry;
	hash_for_each_possible(map, current_entry, next, key){
		if(current_entry->key == key)
			return current_entry;
	}
	return NULL;

}

/**
 * @mdata struct read from the userspace from procfile
 * Copies the data in the hashmap_entry into the output buffer
 */
int get(struct map_data *mdata) 
{
	struct hashmapEntry *entry = get_entry(mdata->key);
	if(entry!=NULL){
		printk(KERN_WARNING "entry != null\n");

		output = (char*)(entry->data);
		len = entry->data_size;
		printk("output is :%d\n",*(uint8_t *)entry->data );
		temp = len;
	} else{
		printk(KERN_WARNING "get found no matching key, returning 0\n");
		return -1; //missing entry
	}
	return 0; //success
}

/**
 * @mdata struct read from userspace via procfile
 * @data_size the size of the data to be stored in hashmap
 * Copies the users key+value pair into the hashmap
 */
void put(struct map_data *mdata, size_t data_size) 
{
	/*Copy of "value" into memory*/
	struct hashmapEntry *entry;
	void *entryData = kmalloc(data_size, GFP_KERNEL);
	memcpy(entryData, mdata->data, data_size);

	entry = kmalloc(sizeof(struct hashmapEntry), GFP_KERNEL);
	entry->data = entryData;
	printk(KERN_WARNING "Value in put is %d\n",*(uint32_t *)entry->data );
	entry->data_size = data_size;
	entry->key = mdata->key;
	hash_add(map, &entry->next, mdata->key);

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

	size_t data_size = count - sizeof(struct map_data);

	printk(KERN_WARNING "Declared data_size\n");

	struct hashmapEntry *entry = NULL;

	printk(KERN_WARNING "Declared tmp, entry and current_entry\n");

	printk(KERN_WARNING "code: %u\n", mdata->code);
	switch ( mdata->code ) 
	{
		case GET:
			/*if no entry return 0
			  else return size of data in entry */
			printk(KERN_WARNING "Inside switch GET\n");
			if (get(mdata) == -1)
				return 0;
			else
				return len;
			printk(KERN_WARNING "GET DONE\n");

			break;

		case PUT:
			printk(KERN_WARNING "Inside switch PUT\n");

			put(mdata, data_size);

			printk(KERN_WARNING "PUT DONE\n");

			break;

		case REMOVE:
			printk(KERN_WARNING "Ínside removed\n");

			entry = get_entry(mdata->key);
			if(entry!=NULL){
				hash_del(&entry->next);
				kfree(entry->data);
				kfree(entry);
			} else{
				return 0; //value not found
			}
			break;

		case CLEAR:
			clear_hashmap();	

			break;
		default:

			//What just happened?
			break;

	}

	return count;
	//Error handling? Do we have to return count or can we return an error number?
}

/**
 * Removes all entries in the hashmap without removing the hashmap itself.
 */
void clear_hashmap()
{
	struct hashmapEntry* current_entry;
	int bkt;
	hash_for_each(map, bkt, current_entry, next){
		printk("key: %d\tbucket: %d\n", 1, bkt);
		hash_del(&current_entry->next);
		kfree(current_entry->data);
		kfree(current_entry);
	}

}


/**
 * Creates new proc file with name hashmap
 */
void create_new_proc_entry(void) 
{
	proc_create("hashmap",0666,NULL,&proc_fops);
	msg=kmalloc(MAX_DATA_LENN*sizeof(char),GFP_KERNEL);

}

/**
 * Inits hashmap and procfile
 */
int proc_init (void) 
{
	loadFromFile();
	create_new_proc_entry();
	hash_init(map);
	return 0;
}

/**
 * Removes the proc file and frees the hashmap
 */
void proc_cleanup(void) 
{
	
	printk(KERN_WARNING "Inside proc_cleanup()\n");
	remove_proc_entry("hashmap",NULL);

	saveToFile();
	/*Free all entries in the hashtable*/
	clear_hashmap();
	kfree(msg);
	printk(KERN_WARNING "proc_cleanup() done!\n");
}

void saveToFile(void)
{
	char* dump_filename; //Set to the file you are targeting
	struct file *file;
	int i;
	void* data;  //Needs to be a kernel pointer, not userspace pointer
	int block_count; //Set me to something
	int block_size; //Set me to something
	loff_t fileoffset = 0;
	mm_segment_t old_fs;

	old_fs = get_fs();  //Save the current FS segment
	set_fs(get_ds());

	dump_filename = "/home/erik/hello.txt";
	block_count = 1;
	block_size = 9;


	struct hashmapEntry* current_entry;
	int bkt;

	file = filp_open(dump_filename, O_WRONLY|O_CREAT, 0644);

	if(file){
		hash_for_each(map, bkt, current_entry, next){
			block_size = 4 + 1 + 4 + 1 + current_entry->data_size + 1; //borde vara 1 mindre och inte avslutas med \n
			char *str = kmalloc(block_size, GFP_KERNEL);
			*(uint32_t*)str = current_entry->key;
			str[4] = ':';
			*(size_t*)(str+5) = current_entry->data_size;
			str[9] = ':';
			for (i = 10; i < block_size; i++) {
				str[i] = ((char*)(current_entry->data))[i-5];
				printk("%X ", ((uint8_t*)(current_entry->data))[i-5]);
			}
			printk("\n");
			str[block_size-1] = '\n';
			data = str;

			vfs_write(file, data, block_size, &fileoffset);
			fileoffset = fileoffset+block_size;
			kfree(str);

		}
		filp_close(file,NULL);
	}
	set_fs(old_fs); //Reset to save FS
	//kfree(dump_filename);

}

void loadFromFile(void)
{
	char* dump_filename; //Set to the file you are targeting
	struct file *file;
	int err;
	void* data;  //Needs to be a kernel pointer, not userspace pointer
	int block_size; //Set me to something
	loff_t fileoffset = 0;
	mm_segment_t old_fs;

	old_fs = get_fs();  //Save the current FS segment
	set_fs(get_ds());

	dump_filename = "/home/erik/hello.txt";

	file = filp_open(dump_filename, O_RDONLY, 0644);

	if(IS_ERR(file)) {
		err = PTR_ERR(file);
		if (err == ENOENT)
			printk(KERN_WARNING"File does not exist, start with empty table\n");
		else
			printk(KERN_WARNING"Error on opening file, start with empty table\n");
		return;
	}

	block_size = FILE_METADATA_OFFSET;
	data = kmalloc(block_size, GFP_KERNEL);
	if(file){
		struct path p;
		struct kstat ks;
		kern_path(dump_filename, 0, &p);
		vfs_getattr(&p, &ks);

		while(fileoffset < ks.size){
			struct hashmapEntry* entry  = kmalloc(sizeof(struct hashmapEntry), GFP_KERNEL);
			if (fileoffset + block_size >= ks.size) {
				printk(KERN_WARNING "block_size error, readsize larger than file size!\n");
				break;
			}
			vfs_read(file, data, block_size, &fileoffset);
			entry->key = *(uint32_t*)data;
			entry->data_size = *(size_t*)(data+sizeof(uint32_t)+1);
			entry->data = kmalloc(entry->data_size, GFP_KERNEL);
			fileoffset = fileoffset + block_size;
			if (fileoffset + entry->data_size >= ks.size) {
				printk(KERN_WARNING "entry-data error, readsize larger than file size!\n");
				break;
			}
			vfs_read(file, entry->data, entry->data_size, &fileoffset);
			fileoffset += (entry->data_size); //add one here for the \n
			hash_add(map, &entry->next, entry->key);
		}

		filp_close(file,NULL);
	}
	set_fs(old_fs); //Reset to save FS

}

MODULE_LICENSE("GPL"); 
module_init(proc_init);
module_exit(proc_cleanup);
