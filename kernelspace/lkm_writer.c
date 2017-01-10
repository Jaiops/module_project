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

		if(output!=NULL)
			kfree(output);

		output = (char*)(entry->data);
		len = entry->data_size;
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
	entry->data_size = data_size;
	entry->key = mdata->key;
	hash_add(map, &entry->next, mdata->key);

}

/* Reads mdata object from user space and does corresponding action depending 
 * on the mdata->code*/
ssize_t write_proc(struct file *filp,const char *buf,size_t count,loff_t *offp)
{
	struct map_data *mdata;
	struct hashmapEntry *entry;
	ssize_t data_size;

	//Converts char* msg into a mdata struct
	if(msg!=NULL)
		kfree(msg);

	msg=kmalloc(count*sizeof(char),GFP_KERNEL);

	copy_from_user(msg,buf,count);
	mdata = (struct map_data*)msg;

	data_size = count - sizeof(struct map_data);
	entry = NULL;
	switch ( mdata->code ) 
	{
		case GET:
			/*if no entry return 0
			  else return size of data in entry */
			if (get(mdata) == -1)
				return 0;
			else
				return len;
			break;

		case PUT:
			put(mdata, data_size);
			break;

		case REMOVE:
			entry = get_entry(mdata->key);
			if(entry!=NULL){
				hash_del(&entry->next);
				kfree(entry->data);
				kfree(entry);
			} else
				return 0; //value not found
			
			break;

		case CLEAR:
			clear_hashmap();	

			break;
		case SAVE:

			//Free old output.
			if(output!=NULL)
				kfree(output);
			//size of the hash array + 2 for OP code and '&' divider and
			//a uint32_t to imitate a map_data struct.
			len = hash_size()+2+sizeof(uint32_t);
			temp = len;
			output = create_save_array(hash_size()); 
			break;

		case LOAD:
			parse_input_data(msg, count);
			break;
		default:
			//What just happened? Should we return some error code?
			printk(KERN_WARNING "Key-Store LKM: Unknown OP code in write_proc.");
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

}

/**
 * Inits hashmap and procfile
 */
int proc_init (void) 
{
	hash_init(map);
	create_new_proc_entry();
	return 0;
}

/**
 * Removes the proc file and frees the hashmap
 */
void proc_cleanup(void) 
{
	
	remove_proc_entry("hashmap",NULL);

	/*Free all entries in the hashtable*/
	clear_hashmap();
	kfree(msg);
	kfree(output);
}

/**
 * Calculates the total size of the hashmaps, including entries and metadata in bytes.
 * This size is used when saving to disc.
 * Returns the size or 0 if the hashmap is empty.
 */
size_t hash_size(void)
{
	int bkt;
	struct hashmapEntry *current_entry;
	size_t size = 0;

	hash_for_each(map, bkt, current_entry, next){
		size += (FILE_METADATA_OFFSET + current_entry->data_size);
	}
	return size;
}

/**
 * Creates an array with each hashmap entry including
 * the key, datasize and the data for the entry.
 * The Array is initiated with bytes corresponding to the struct map_data.
 * The operation code for loading the data back into the hashmap and the map_data key, padded.
 *
 * Returns a pointer to the array.
 */
void *create_save_array(size_t array_size)
{
	ssize_t filesize = array_size + 2 + sizeof(size_t);
	int bkt;
	int i;
	struct hashmapEntry *current_entry;
	ssize_t offset = 0;
	uint8_t *array = kmalloc(filesize, GFP_KERNEL);
	array[offset] = 5;
	offset++;
	*(uint32_t*)(array+offset) = 0;
	offset += sizeof(uint32_t);
	array[offset] = '&';
	offset++;

	hash_for_each(map, bkt, current_entry, next){

		*(uint32_t*)(array+offset) = current_entry->key;
		offset += sizeof(current_entry->key);
		array[offset] = ':';
		offset++;
		*(size_t*)(array+offset) = current_entry->data_size;
		offset += sizeof(current_entry->data_size);

	
		array[offset] = ':';
		offset++;

		for (i = 0; i < current_entry->data_size; i++, offset++) 
		{
			array[offset] = ((uint8_t*)(current_entry->data))[i];
		}
		printk("value = %u", *(uint32_t*)(array+(offset-current_entry->data_size)));
		printk("\n");

	}

	return array;
}

/**
 *
 */
void parse_input_data(void* array, size_t size)
{
	int i;
	struct hashmapEntry* entry;
	size_t offset = 2+ sizeof(uint32_t);

	while(offset < size)
	{
		entry  = kmalloc(sizeof(struct hashmapEntry), GFP_KERNEL);
		if (offset + FILE_METADATA_OFFSET > size) 
		{
			printk(KERN_WARNING "block_size error, readsize larger than array size!\n");
			break;
		}

		entry->key = *(uint32_t*)(array+offset);
		offset += sizeof(uint32_t)+1; //add one to skip the :
		
		entry->data_size = *(size_t*)(array+offset); 

		offset += sizeof(size_t) + 1; //add one to skip the :
	
		if (offset + entry->data_size > size) 
		{
			printk(KERN_WARNING "entry-data error, readsize larger than array size!\n");
			break;
		}

		entry->data = kmalloc(entry->data_size, GFP_KERNEL);
		for (i = 0; i < entry->data_size; i++, offset++) 
		{
			((uint8_t*)entry->data)[i] = ((uint8_t*)array)[offset];
		}	
		printk("Read from file. Key: %d, Data: %u\n", entry->key, *(int*)entry->data);
		hash_add(map, &entry->next, entry->key);
	}
}

MODULE_LICENSE("GPL"); 
module_init(proc_init);
module_exit(proc_cleanup);
