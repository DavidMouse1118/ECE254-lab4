/**
 * @file memory.c
 * @brief: ECE254 Lab: memory allocation algorithm comparison template file
 * @author: 
 * @date: 2015-11-20
 */

/* includes */
#include <stdio.h> 
#include <stdlib.h> 
#include "mem.h"

/* defines */

/* global varaiables */
struct Node_block {
	//1 is allocated, 0 is not allocated
	int allocated;
	size_t block_size;
	struct Node_block* next;
	struct Node_block* prev;
}

struct Node_block* best_head;


/* Functions */

/* memory initializer */
int best_fit_memory_init(size_t size)
{

	best_head = malloc(size);
	if(best_head == NULL){
		return -1;
	}
	best_head->allocated = 0;
	best_head->block_size = size;
	best_head->next = NULL;
	best_head->prev = NULL;
	return 0;

}

int worst_fit_memory_init(size_t size)
{

	// To be completed by students
	// You call malloc once here to obtain the memory to be managed.
	return 0;

}

/* memory allocators */
void *best_fit_alloc(size_t size)
{
	if(size == 0){
		return NULL;
	}
	Node_block* current_block = best_head;
	Node_block* best_block = best_head;
	//calculate size
	if(size % 4 != 0){
		size = size + (4 - size % 4);
	}

	while(current_block){
		if(current_block->allocated == 0){
			if(current_block->block_size >= size && current_block->block_size < best_block->block_size){
				best_block = current_block;
			}
		}

		current_block = current_block->next;
	}

	if(best_block->allocated == 1){
		return NULL;
	}
	//update node blocks
	if(size == best_block->block_size){
		best_block->allocated = 1;
	} else {
		Node_block* tmp = best_block;
		//updated new block
		best_block->allocated = 1;
		best_block->block_size = size;
		//updated parent block
		current_block = tmp + size;
		current_block->allocated = 0;
		current_block->block_size = tmp->block_size - size;
		best_block->next = current_block;
		tem->next->prev = current_block;
		current_block->prev = best_block;
		current_block->next = tmp->next;
	}


	return best_block;
}


void *worst_fit_alloc(size_t size)
{
	// To be completed by students
	return NULL;
}

/* memory de-allocator */
void best_fit_dealloc(void *ptr) 
{	
	Node_block* current_block = best_head;
	int found = 0;
	while(current_block) {
		if()
		current_block = current_block->next;
	}
	// To be completed by students
	return;
}

void worst_fit_dealloc(void *ptr) 
{

	// To be completed by students
	return;
}

/* memory algorithm metric utility function(s) */

/* count how many free blocks are less than the input size */ 
int best_fit_count_extfrag(size_t size)
{
	// To be completed by students
	return 0;
}

int worst_fit_count_extfrag(size_t size)
{
	// To be completed by students
	return 0;
}
