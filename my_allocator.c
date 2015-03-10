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
#include <math.h>
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
	free_list[0]->next = NULL;
	int i;
	for(i=1;i<free_list_size;++i)
		free_list[i] = NULL;
	
	return mem_size;
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
	struct Header* prev = NULL;
	//look for block to split
	while(temp != NULL && !temp->is_free){
		temp = temp->next;
		prev = temp;
	}
	if(temp == NULL)
		return 1;
	//remove first element of list
	if(prev == NULL){
		free_list[order] = temp->next;
	}
	//remove other element of list
	else{
		prev->next = temp->next;
	}
	struct Header* free = free_list[order+1];
	if(free == NULL){
		//insert left
		free_list[order+1] = temp;
		free_list[order+1]->size = (mem_size/pow(2,order+1));
		free_list[order+1]->is_free = true;
		//insert right
		free_list[order+1]->next = temp + (int)(mem_size/pow(2,order+1));
		free_list[order+1]->next->is_free = true;
		free_list[order+1]->next->size = free_list[order+1]->size;
		free_list[order+1]->next->next = NULL;
		return 0;
	}
	while(free < temp -(int)(mem_size/pow(2,order+1)) && free->next != NULL){
		free = free->next;
	}
	struct Header* next = free->next;
	//insert left
	free->next = temp;
	free->next->size = (mem_size/pow(2,order+1));
	free->next->is_free = true;
	//insert right
	free->next->next = temp + (int)(mem_size/pow(2,order+1));
	free->next->next->is_free = true;
	free->next->next->size = free->next->size;
	free->next->next->next = next;
	return 0;
}

int add_to_list(struct Header* h,int order){
	return 0;
}

struct Header* remove_from_list(int order){
	return 0;
}
void print_free_lists(){
	int i;
	for(i=0;i<free_list_size;++i){
		printf("List : %i \n",i);
		struct Header* temp = free_list[i];
		while(temp != NULL){
			printf("Size: %u Free: %i Addr:%p \n",temp->size,temp->is_free,temp);
			temp = temp->next;
		}
	}
	printf("\n");
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

