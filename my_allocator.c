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
	if(mem_size == _length*2)
		mem_size = mem_size/2;
	main_block_addr = malloc(mem_size + 100000);
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
// Calculates log2 of number.  
long double Log2( double n )  
{  
    // log(n)/log(2) is log2.  
    return log( n ) / log( 2 );  
}

int split (int order){
	struct Header* temp = free_list[order];
	struct Header* prev = NULL;
	//look for block to split
	while(temp != NULL && !temp->is_free){
		prev = temp;
		temp = temp->next;
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
		free_list[order+1]->next = (Addr)temp + free_list[order+1]->size;
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
	free->next->next = (Addr)free->next + free->next->size;
	free->next->next->is_free = true;
	free->next->next->size = free->next->size;
	free->next->next->next = next;
	return 0;
}

int consolidate(){
	int i;
	for(i = free_list_size-1; i >= 0; --i){
		struct Header* temp = free_list[i];
		struct Header* prev = NULL;
		while(temp != NULL){
			int is_next = (temp->next != NULL && temp->next->next !=NULL);
			if(temp->next == NULL)
				break;
			if(temp->is_free && temp->next->is_free){
				if(prev == NULL)
					free_list[i] = temp->next->next;
				else
					prev->next = temp->next->next;
				struct Header* free = free_list[i-1];
				if(free == NULL){
					//insert
					free_list[i-1] = temp;
					free_list[i-1]->size = (mem_size/pow(2,i-1));
					free_list[i-1]->is_free = true;
					free_list[i-1]->next = NULL;
				}
				else{
					while(free < temp && free->next != NULL)
						free = free->next;
					struct Header* next = free->next;
					//insert
					free->next = temp;
					free->next->size = (mem_size/pow(2,i-1));
					free->next->is_free = true;
					free->next->next = next;
				}
				if(prev != NULL)
					temp = prev->next;
				else
					temp = free_list[i];
			}
			else
				temp = temp->next->next;
		}
	}
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

Addr find_free_node(int index){
	struct Header* temp = free_list[index];
  	while(temp != NULL && !temp->is_free){
  		temp = temp->next;
  	}
  	if(temp != NULL){
  		temp->is_free = false;
  		return(Addr)temp+sizeof(*temp);
  	}
  	else
  		return NULL;
}


extern Addr my_malloc(unsigned int _length) {
	int alloc_size = next_power_2(_length);
	if(alloc_size  == _length*2)
		alloc_size = mem_size/2;
	alloc_size = next_power_2(alloc_size + sizeof(struct Header));
  	consolidate();
  	int index = Log2(mem_size/alloc_size);
  	int offset = 0;
  	Addr return_addr = NULL;
  	while(return_addr == NULL){
	  	return_addr = find_free_node(index);
	  	if(return_addr != NULL){
	  		//print_free_lists();
	  		return return_addr;
	  	}
	  	split(offset);
	  	offset++;
	  	if(offset >= free_list_size){
	  		break;
	  	}
  	}
  	return NULL;
}

extern int my_free(Addr _a) {
  struct Header* temp = (struct Header*) (_a - sizeof(struct Header));
  temp->is_free = true;
  return 0;
}

