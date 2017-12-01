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

/* global varaiables */
struct Node_block {
	//1 is allocated, 0 is not allocated
	int allocated;
	size_t block_size;
	struct Node_block* next;
	struct Node_block* prev;
};

Node_block* best_head;


/* Functions */

/* memory initializer */
int best_fit_memory_init(size_t size)
{
	//size must be greater than sizeof(Node_block) + 4
	if(size <= (sizeof(Node_block) + 4)){
		printf("Size too small");
		return -1;
	}
	best_head = (Node_block*)malloc(size);
	if(!best_head){
		return -1;
	}
	printf("allocated memory block start from %d\n", best_head);
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
		printf("size is too small, cannot be zero\n");
		return NULL;
	}
	Node_block* current_block;
	current_block = best_head;

	Node_block* best_block = NULL;
	Node_block* new_block;

	//calculate size
	if(size % 4 != 0){
		size = size + (4 - size % 4);
	}
	size = size + sizeof(Node_block);

	//Find best block
	while(current_block){
		if(current_block->allocated == 0){
			if(current_block->block_size >= size){
				if(!best_block){
					best_block = current_block;
				}
				if (current_block->block_size < best_block->block_size){
					best_block = current_block;
				}
			}
		}

		current_block = current_block->next;
	}
	//if best node doesnt exit, return
	if(!best_block){
		return NULL;
	}

	printf("allocted size %d in block %d\n", size, best_block);
	//exact allocatikon
	if(best_block->block_size == size){
		best_block->allocated = 1;
	}
	//internal fragementation
	else if((best_block->block_size - size) <= sizeof(Node_block)){
		printf("fragementation\n");
		//updated old block
		best_block->allocated = 1;
	}
	//split with new block
	else {
		//create new block
		new_block = (Node_block*)((size_t)best_block + size);
		new_block->allocated = 0;
		new_block->block_size = best_block->block_size - size;
		//insert at the end of the linked list
		if(best_block->next == NULL){
			new_block -> next = NULL;
			new_block -> prev = best_block;
			best_block->next = new_block;
		} 
		//insert in the middle
		else {
			new_block -> next = best_block -> next;
			new_block -> prev = best_block;
			best_block -> next -> prev = new_block;
		}
		//updated old block
		best_block->allocated = 1;
		best_block->block_size = size;
		printf("best_block\n");
		printf("%d\n", (int)best_block-(int)best_head);
		printf("%d\n", best_block->allocated);
		printf("%d\n", best_block->block_size);
		printf("new_block\n");
		printf("%d\n", (int)new_block-(int)best_head);
		printf("%d\n", new_block->allocated);
		printf("%d\n", new_block->block_size);
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
	Node_block* prev_block;
	Node_block* next_block;
	int found = 0;
	while(current_block) {
		if(current_block->allocated == 1 && (size_t)current_block == (size_t)ptr){
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
	current_block -> allocated = 0;
	printf("old_block\n");
		printf("%d\n", current_block);
		printf("%d\n", current_block->allocated);
		printf("%d\n", current_block->block_size);

	if(next_block != NULL && next_block -> allocated == 0){
		current_block -> next = next_block -> next;
		current_block -> block_size += next_block -> block_size;
		if(next_block -> next != NULL){
			next_block -> next -> prev = current_block;
		}
	}
	if(prev_block != NULL && prev_block -> allocated == 0){
		current_block -> prev  = prev_block -> prev;
		current_block -> block_size += prev_block -> block_size;
		if(prev_block -> prev != NULL){
			prev_block -> prev -> next = current_block;
		}
	}
	printf("new_block\n");
		printf("%d\n", current_block);
		printf("%d\n", current_block->allocated);
		printf("%d\n", current_block->block_size);
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
	size = size + sizeof(Node_block);
	int count = 0;
	Node_block* current_block = best_head;
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
	// To be completed by students
	return 0;
}

void print_all_nodes_information_best(){
	// sets the head_bf node
	Node_block* current_block;
	current_block = best_head;
	
	printf("\n========================= PRINTING ALL NODES AND INFO ===========================\n\n");
	
	int node_count = 0;
	
	// traverse the list
	while(current_block != 0){
		node_count++;
		printf("========== Information about Node %d ========\n ", node_count);
		printf("Current address: %d\n", current_block);
		printf("Previous node's' address: %d\n", current_block->prev);
		printf("Next node's' address: %d\n", current_block->next);
		printf("size_of_this_memory block: %d\n", current_block->block_size);
		printf("is_allocated: %d\n", current_block->allocated);

		current_block = current_block->next;
	}
	printf("========== General Stuff ========\n ");
	printf("Total number of nodes: %d\n", node_count);
	printf("Size of a struct node: %d\n\n", sizeof(Node_block));
}


int main(int argc, char *argv[])
{
	// int num = 0;
	// int algo = 0; // default algorithm to test is best fit  
	// void *p, *q;

	// if (argc != 2) {
	// 	fprintf(stderr, "Usage: %s <0/1>. 0 for best fit and 1 for worst fit \n", argv[0]);
	// 	exit (1);
	// } else if (!strcmp(argv[1], "1") || !strcmp(argv[1], "0")) {
	// 	algo = atoi(argv[1]);
	// } else {
	// 	fprintf(stderr, "Invalid argument, please specify 0 or 1\n");
	// 	exit(1);
	// }
	// if ( algo == 0 ) {
	// 	best_fit_memory_init(1024);	// initizae 1KB, best fit
	// 	int a;
	// 	for( a = 0; a < 20; a++ ){
	// 		p = best_fit_alloc(a);		// allocate 8B
	// 		//printf("best fit: p=%p\n", p);
	// 		printf("best fit: p=%d\n", p);
	// 	}
	// 	num = best_fit_count_extfrag(4);
	// } else if ( algo == 1 ) {
	// 	worst_fit_memory_init(1024);	// initizae 1KB, worst fit

	// 	q = worst_fit_alloc(8);		// allocate 8B
	// 	printf("worst fit: q=%p\n", q);
	// 	if ( q != NULL ) {
	// 		worst_fit_dealloc(q);	
	// 	}
	// 	num = worst_fit_count_extfrag(4);
	// } else {
	// 	fprintf(stderr, "Should not reach here!\n");
	// 	exit(1);
	// }

	// printf("num = %d\n", num);

	// return 0;

	best_fit_memory_init(1024);
	
while (1) {

	void* four_1 =	best_fit_alloc(5);
	printf("The return value of four_1 is: %d\n", four_1);
	if (four_1 == NULL) {  break; }
	
	
	void* two_five_six_1 = best_fit_alloc(256);
  	printf("The return value of two_five_six_1 is: %d\n", two_five_six_1);
	if (two_five_six_1 == NULL) { break; }
	
	void* eight_1 = best_fit_alloc(9);
	printf("The return value of eight_1 is: %d\n", eight_1);
	if (eight_1 == NULL) { break; }
	
	void* one_two_eight_1 = best_fit_alloc(128);
	printf("The return value of one_two_eight_1 is: %d\n", one_two_eight_1);
	if (one_two_eight_1 == NULL) { break; }
	
 	void* sixteen_1 = best_fit_alloc(17);
 	printf("The return value of sixteen_1 is: %d\n", sixteen_1);
	if (sixteen_1 == NULL) { break; }

	void* sixty_four_1 = best_fit_alloc(65);
 	printf("The return value of sixty_four_1 is: %d\n", sixty_four_1);
	if (sixty_four_1 == NULL) { break; }
	
 	void* thirty_two_1 = best_fit_alloc(33);
 	printf("The return value of thirty_two_1 is: %d\n", thirty_two_1);
	if (thirty_two_1 == NULL) { break; }


	best_fit_dealloc(eight_1);
	best_fit_dealloc(thirty_two_1);
	best_fit_dealloc(one_two_eight_1);
}

print_all_nodes_information_best();

int four_bf, eight_bf, sixteen_bf, thirtytwo_bf, sixtyfour_bf, onetwentyeight_bf, twofiftysix_bf;

four_bf = best_fit_count_extfrag(4);
eight_bf = best_fit_count_extfrag(8);
sixteen_bf = best_fit_count_extfrag(16);
thirtytwo_bf = best_fit_count_extfrag(32);
sixtyfour_bf = best_fit_count_extfrag(64);
onetwentyeight_bf = best_fit_count_extfrag(128);
twofiftysix_bf = best_fit_count_extfrag(256);

printf("the extern frag for best fit for four bytes is %d\n", four_bf);
printf("the extern frag for best fit for eight bytes is %d\n", eight_bf);
printf("the extern frag for best fit for 16 bytes is %d\n", sixteen_bf);
printf("the extern frag for best fit for 32 bytes is %d\n", thirtytwo_bf);
printf("the extern frag for best fit for 64 bytes is %d\n", sixtyfour_bf);
printf("the extern frag for best fit for 128 bytes is %d\n", onetwentyeight_bf);
printf("the extern frag for best fit for 256 bytes is %d\n", twofiftysix_bf);


	return 0;
}

