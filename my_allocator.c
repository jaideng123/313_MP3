/* 
    File: my_allocator.c

    Author: Jaiden Gerig + Andrew Schlotzhauer
            Department of Computer Science
            Texas A&M University
    Date  : <date>

    Modified: 

    This file contains the implementation of the module "MY_ALLOCATOR".

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include<stdlib.h>
#include "my_allocator.h"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/

unsigned int init_allocator(unsigned int _basic_block_size,unsigned int _length){
	mem_size = next_power_2(_length);
	if(mem_size != _length)
		mem_size = mem_size/2;
	main_block_addr = malloc(mem_size);
	block_size = next_power_2(_basic_block_size);
	int counter = 1;
	unsigned int size = mem_size;
	while(size > block_size){
		counter++;
		size = size/2;
	}
	free_list_size = counter;
	free_list = malloc(free_list_size*sizeof(struct Header *));
	free_list[0] = (struct Header *)main_block_addr;
	free_list[0]->size = mem_size;
	free_list[0]->is_free = true;
	free_list[0]->is_left = true;
	free_list[0]->next = NULL;
	free_list[0]->prev = NULL;
	free_list[0]->mem_block = main_block_addr+sizeof(struct Header *);
	int i;
	for(i=1;i<free_list_size;++i)
		free_list[i] = NULL;
	
	return block_size;
}

int release_allocator(){
	free(main_block_addr);
	free(free_list);
	return 0;
}

//taken from http://graphics.stanford.edu/~seander/bithacks.html
unsigned int next_power_2(unsigned int v){
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	return v;
}

int split (int order){
	struct Header* temp = free_list[order];
	//look for block to split
	while(temp != NULL && !temp->is_free){
		temp = temp->next;
	}
	if(temp == NULL)
		return 1;
	
}

int add_to_list(struct Header* h,int order){
	//empty list
	if (free_list[order] == NULL){
		free_list[order] = h;
		h->next = NULL;
		h->prev = NULL;
	}
	else{
		struct Header* temp = free_list[order];
		//search for end
		while(temp->next != NULL)
			temp = temp->next;
		temp->next = h;
		h->next = NULL;
		h->prev = temp;
	}
	return 0;
}

struct Header* remove_from_list()(int order){
	
	
}


extern Addr my_malloc(unsigned int _length) {
  /* This preliminary implementation simply hands the call over the 
     the C standard library! 
     Of course this needs to be replaced by your implementation.
  */
  return malloc((size_t)_length);
}

extern int my_free(Addr _a) {
  /* Same here! */
  free(_a);
  return 0;
}

