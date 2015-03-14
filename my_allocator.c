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
	add_to_list(temp,order+1);
	struct Header* right = (Addr)temp + (int)(mem_size/pow(2,order+1));
	add_to_list(right,order+1);
	//print_free_lists();
	return 0;
}

int consolidate(){
	int i;
	for(i = free_list_size-1; i >= 0; --i){
		struct Header* temp = free_list[i];
		struct Header* next = NULL;
		struct Header* prev = NULL;
		//print_free_lists();
		while(temp != NULL){
			if(temp->next == NULL)
				break;
			next = temp->next->next;
			if(temp->is_free && temp->next->is_free){
				if(prev == NULL)
					free_list[i] = next;
				else
					prev->next = next;
				add_to_list(temp,i-1);
				temp = next;
			}
			else{
				prev = temp->next;
				temp = next;
			}
		}
	}
	return 0;
}

int add_to_list(struct Header* h,int order){
	struct Header* temp = free_list[order];
	struct Header* prev = NULL;
	//list is empty
	if(temp == NULL){
		free_list[order] = h;
		h->size = (mem_size/pow(2,order));
		h->is_free = true;
		h->next = NULL;
		return 0;
	}
	//search for node to insert in
	while(temp < h && temp->next != NULL){
		prev = temp;
		temp = temp->next;
	}
	if(prev == NULL){
		if(temp > h){
			free_list[order] = h;
			free_list[order]->size = (mem_size/pow(2,order));
			free_list[order]->is_free = true;
			free_list[order]->next = temp;
		}
		else if(temp < h){
			free_list[order]->next = h;
			free_list[order]->next->size = (mem_size/pow(2,order));
			free_list[order]->next->is_free = true;
			free_list[order]->next->next = NULL;
		}
		else
			printf("Ya Done Fucked up\n");

	}
	else{
		if(temp < h && temp->next == NULL){
			temp->next = h;
			temp->next->is_free = true;
			temp->next->size = (mem_size/pow(2,order));
			temp->next->next = NULL;
			
		}
		else{
		prev->next = h;
		prev->next->size = (mem_size/pow(2,order));
		prev->next->is_free = true;
		prev->next->next = temp;
		}
	}
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
  	//printf("%i\n",alloc_size );
  	while(return_addr == NULL){
	  	return_addr = find_free_node(index);
	  	if(return_addr != NULL){
	  		return return_addr;
	  	}
	  	if(offset == index)
	  		break;
	  	split(offset);
	  	offset++;
	  	//print_free_lists();
	  	if(offset >= free_list_size){
	  		break;
	  	}
  	}
  	return NULL;
}

extern int my_free(Addr _a) {
	if(_a == NULL)
		return 1;
  struct Header* temp = (_a - sizeof(struct Header));
  temp->is_free = true;
  return 0;
}

