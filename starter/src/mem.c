/**
 * @file memory.c
 * @brief: ECE254 Lab: memory allocation algorithm comparison template file
 * @author: 
 * @date: 2015-11-20
 */

/* includes */
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include "mem.h"

/* defines */
typedef struct Node {
	//1 is allocated, 0 is not allocated
	int allocated;
	size_t block_size;
	struct Node* next;
	struct Node* prev;
} Node_block;

/* global varaiables */
//The node head of the block linked list (best fit)
Node_block* best_head;
//The node head of the block linked list (worst fit)
Node_block* worst_head;


/* Functions */

/* memory initializer */
int best_fit_memory_init(size_t size)
{
	//size must be greater than sizeof(Node_block) + 4
	if(size <= (sizeof(Node_block) + 4)){
		printf("size too small, can not allocated\n");
		return -1;
	}

	//Allocated memory
	best_head = (Node_block*)malloc(size);
	//Allocation failed, return -1
	if(!best_head){
		return -1;
	}
	
	//initialze the linked list (best_head)
	best_head->allocated = 0;
	best_head->block_size = size;
	best_head->next = NULL;
	best_head->prev = NULL;
	return 0;

}

int worst_fit_memory_init(size_t size)
{
	//size must be greater than sizeof(Node_block) + 4
	if(size <= (sizeof(Node_block)+4)){
		printf("Size too small,  can not allocated\n");
		return -1;
	}

	//Allocated memory
	worst_head = (Node_block*)malloc(size);
	//Allocation failed, return -1
	if(!worst_head){
		return -1;
	}
	
	//initialze the linked list (worst_head)
	worst_head -> allocated = 0;
	worst_head -> block_size = size;
	worst_head -> next = NULL;
	worst_head -> prev =NULL;
	return 0;

}

/* memory allocators */
void *best_fit_alloc(size_t size)
{
	if(size <= 0){
		printf("size is too small, cannot be zero or negative\n");
		return NULL;
	}
	Node_block* current_block = best_head;
	Node_block* best_block = NULL;
	Node_block* new_block;

	//Calculate size, increase the size to a multiple of 4
	if(size % 4 != 0){
		size = size + (4 - size % 4);
	}

	//Add node size into size, so the size will be the same idea of the block size
	size = size + sizeof(Node_block);

	//Find best block
	while(current_block){
		//The required block is not allocated, its block size is greater than the size plus node size or its block size is equal to the size
		if(current_block->allocated == 0 && (current_block->block_size > size + sizeof(Node_block) || current_block->block_size == size)){
			//initialize the best block
			if(!best_block){
				best_block = current_block;
			}
			//update the best block, with a smaller block
			if (current_block->block_size < best_block->block_size){
				best_block = current_block;
			}
		}
		current_block = current_block->next;
	}
	//if best node doesn't exit, return
	if(!best_block){
		printf("No space for the size\n");
		return NULL;
	}

	//printf("allocted size %d in block %d\n", size, (size_t)best_block);
	//exact allocatikon
	if(best_block->block_size == size){
		best_block->allocated = 1;
	}
	else {
		//split the best block into best block and new block
		//create new block
		new_block = (Node_block*)((size_t)best_block + size);
		new_block->allocated = 0;
		new_block->block_size = best_block->block_size - size;
		//insert at the end of the linked list
		if(best_block->next == NULL){
			new_block -> next = NULL;
			best_block -> next = new_block;
			new_block -> prev = best_block;
		} 
		//insert in the middle
		else {
			new_block -> next = best_block -> next;
			best_block -> next = new_block;
			new_block -> prev = best_block;
			new_block -> next -> prev = new_block;
		}
		//updated best block
		best_block->allocated = 1;
		best_block->block_size = size;
	}
	//return the pointer pointers to the allocated memory
	return (Node_block*)((size_t)best_block + sizeof(Node_block)) ;
}


void *worst_fit_alloc(size_t size)
{
	if(size == 0){
		printf("size is too small, cannot be zero\n");
		return NULL;
	}
	Node_block* current_block = worst_head;
	Node_block* worst_block = NULL;
	Node_block* new_block;

	//Calculate size, increase the size to a multiple of 4
	if(size % 4 != 0){
		size = size + (4 - size % 4);
	}
	//Add node size into size, so the size will be the same idea as block size
	size = size + sizeof(Node_block);

	//Find worst block
	while(current_block){
		//The required block is not allocated and its block size is greater than the size plus node size or its block size is equal to the size
		if(current_block->allocated == 0 && (current_block->block_size > size + sizeof(Node_block) || current_block->block_size == size)){
			//initialize the worst block
			if(!worst_block){
				worst_block = current_block;
			}
			//update the worst block with a larger block
			if (current_block->block_size > worst_block->block_size){
				worst_block = current_block;
			}
		}
		current_block = current_block->next;
	}

	//if worst node doesnt exit, return
	if(!worst_block){
		printf("No space for the size\n");
		return NULL;
	}

	//printf("allocted size %d in block %d\n", size, worst_block);
	//exact allocatikon
	if(worst_block->block_size == size){
		worst_block->allocated = 1;
	}
	else {
		//split the worst block into worst block and new block
		//create new block
		new_block = (Node_block*)((size_t)worst_block + size);
		new_block->allocated = 0;
		new_block->block_size = worst_block->block_size - size;
		//insert at the end of the linked list
		if(worst_block->next == NULL){
			new_block -> next = NULL;
			worst_block -> next = new_block;
			new_block -> prev = worst_block;
		} 
		//insert in the middle
		else {
			new_block -> next = worst_block -> next;
			worst_block -> next = new_block;
			new_block -> prev = worst_block;
			new_block -> next -> prev = new_block;
		}
		//updated worst block
		worst_block->allocated = 1;
		worst_block->block_size = size;
	}
	//return the pointer to the allocated memory
	return (Node_block*)((size_t)worst_block + sizeof(Node_block)) ;
}

/* memory de-allocator */
void best_fit_dealloc(void *ptr) 
{	
	Node_block* current_block = best_head;
	Node_block* prev_block;
	Node_block* next_block;
	int found = 0;
	//loop through the linked list and see it if the (input pointer - node size) pointes to a memory block
	while(current_block) {
		//check if (input pointer - node size) pointes to a memory block and the memory block is allocated
		if(current_block->allocated == 1 && (size_t)current_block == (size_t)ptr - sizeof(Node_block)){
			found = 1;
			break;
		}
		current_block = current_block->next;
	}
	//Didnt find the block, return
	if(found == 0){
		printf("blocks not found, failed to dealloc\n");
		return;
	}
	//start dealloc
	prev_block = current_block -> prev;
	next_block = current_block -> next;
	//update the current block
	current_block -> allocated = 0;

	//merge current_block and next_block into current block, if next_block is not allocated 
	if(next_block != NULL && next_block -> allocated == 0){
		current_block -> block_size += next_block -> block_size;
		current_block -> next = next_block -> next;
		if(next_block -> next != NULL){
			next_block -> next -> prev = current_block;
		}
	}

	//merge prev_block and current_block into prev_block, if prev_block is not allocated
	if(prev_block != NULL && prev_block -> allocated == 0){
		prev_block -> block_size += current_block -> block_size;
		prev_block -> next = current_block -> next;
		if(current_block -> next != NULL){
			current_block -> next -> prev = prev_block;
		}
	}
	return;
}

void worst_fit_dealloc(void *ptr) 
{
	Node_block* current_block = worst_head;
	Node_block* prev_block;
	Node_block* next_block;
	int found = 0;
	//loop through the linked list and see it if the (input pointer - node size) pointes to a memory block
	while(current_block) {
		//check if (input pointer - node size) pointes to a memory block and the memory block is allocated
		if(current_block->allocated == 1 && (size_t)current_block == (size_t)ptr - sizeof(Node_block)){
			found = 1;
			break;
		}
		current_block = current_block->next;
	}
	//Didnt find the block, return
	if(found == 0){
		printf("blocks not found, failed to dealloc\n");
		return;
	}
	//start dealloc
	prev_block = current_block -> prev;
	next_block = current_block -> next;
	//update the current block
	current_block -> allocated = 0;

	//merge current_block and next_block into current block, if next_block is not allocated 
	if(next_block != NULL && next_block -> allocated == 0){
		current_block -> block_size += next_block -> block_size;
		current_block -> next = next_block -> next;
		if(next_block -> next != NULL){
			next_block -> next -> prev = current_block;
		}
	}

	//merge prev_block and current_block into prev_block, if prev_block is not allocated
	if(prev_block != NULL && prev_block -> allocated == 0){
		prev_block -> block_size += current_block -> block_size;
		prev_block -> next = current_block -> next;
		if(current_block -> next != NULL){
			current_block -> next -> prev = prev_block;
		}
	}
	return;

}

/* memory algorithm metric utility function(s) */

/* count how many free blocks are less than the input size */ 
int best_fit_count_extfrag(size_t size)
{
	//add node size into size, so size is the same idea as block size
	size = size + sizeof(Node_block);
	int count = 0;
	Node_block* current_block = best_head;
	//loop thourgh the linked list and count for memory blocks that are not allocated and its block size is less then size
	while(current_block){
		if(current_block -> allocated == 0 && current_block -> block_size < size){
			count++;
		}
		current_block = current_block->next;
	}
	return count;
}

int worst_fit_count_extfrag(size_t size)
{
	//add node size into size, so size is the same idea as block size
	size = size + sizeof(Node_block);
	int count = 0;
	Node_block* current_block = worst_head;
	//loop thourgh the linked list and count for memory blocks that are not allocated and its block size is less then size
	while(current_block){
		if(current_block -> allocated == 0 && current_block -> block_size < size){
			count++;
		}
		current_block = current_block->next;
	}
	return count;
}

//Print all nodes from the linked list
//if the input type is 0, it is best fit
//if the input type is 1, it is worst fit
void print_all_nodes(int type){
	Node_block* head;
	if(type == 0){
		head = best_head;
	} else {
		head = worst_head;
	}
	Node_block* current_block = head;

	printf("\n************ ALL NODES BLOCKS ************\n\n");
	
	int count = 0;
	while(current_block){
		printf("Node %d : %d - %d , size %d, is_allocated %d\n", count, (int)((size_t)current_block - (size_t)head), (int)((size_t)current_block - (size_t)head + current_block->block_size - 1), (int)(current_block->block_size), current_block->allocated);
		current_block = current_block->next;
		count++;
	}
}

//Function that return the head address of the linked list
//if the input type is 0, it is best fit
//if the input type is 1, it is worst fit
void* get_head(int type){
	if(type == 0){
		return best_head;
	} else {
		return worst_head;
	}
}
