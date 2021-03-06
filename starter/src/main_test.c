/**
  * ECE254 Linux Dynamic Memory Management Lab
  * @file: main_test.c
  * @brief: The main file to write tests of memory allocation algorithms
  */ 

/* includes */
/* system provided header files. You may add more */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
/* non-system provided header files. 
   Do not include more user-defined header files here
 */
#include "mem.h"

void print_all_nodes(int type);
void* get_head(int type);

int main(int argc, char *argv[])
{
	int algo = 0; // default algorithm to test is best fit  
	void *p, *q;
	void* head;
	srand(time(NULL));//random generator seed

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <0/1>. 0 for best fit, 1 for worst fit and 2 for external fragmentation\n", argv[0]);
		exit (1);
	} else if (!strcmp(argv[1], "2") || !strcmp(argv[1], "1") || !strcmp(argv[1], "0")) {
		algo = atoi(argv[1]);
	} else {
		fprintf(stderr, "Invalid argument, please specify 0 or 1\n");
		exit(1);
	}
	if ( algo == 0 ) {
		printf("\n========================= Best fit test 1: Input too small, fail to init memory ===========================\n\n");
		if(best_fit_memory_init(32) == -1){
			printf("Best fit test 1 Passed\n");
		}

		printf("\n========================= Best fit test 2: Input too big, fail to allocate block ===========================\n\n");
		best_fit_memory_init(1024);
		head = get_head(algo);
		if(best_fit_alloc(2048) == NULL){
			printf("Best fit test 2 Passed\n");
		}

		printf("\n========================= Best fit test 3: Allocate succeed, deallocate succeed ===========================\n\n");
		p = best_fit_alloc(5);
		if((size_t)p == (size_t)head + 32){
			printf("Best fit test 3 Passed\n");
		}
		best_fit_dealloc(p);


		printf("\n========================= Best fit test 4: Allocate in the smallest available block ===========================\n\n");
		p = best_fit_alloc(5);
		q = best_fit_alloc(5);
		best_fit_dealloc(p);
		p = best_fit_alloc(5);
		if((size_t)p == (size_t)head + 32){
			printf("Best fit test 4 Passed\n");
		}
		print_all_nodes(algo);

		printf("\n========================= Best fit test 5: Block not found, failed to deallocate ===========================\n\n");
		best_fit_memory_init(1024);
		p = best_fit_alloc(5);
		//wrong block
		q = (void*)((size_t)p +1);
		best_fit_dealloc(q);
		//block that has is not allocated, so dealloc will fail
		q = (void*)((size_t)p + 32 + 5);
		best_fit_dealloc(q);
		printf("Best fit test 5 Passed\n");
	} else if ( algo == 1 ) {
		printf("\n========================= Worst fit test 1: Input too small, fail to init memory ===========================\n\n");
		if(worst_fit_memory_init(32) == -1){
			printf("Worst fit test 1 Passed\n");
		}

		printf("\n========================= Worst fit test 2: Input too big, fail to allocate block ===========================\n\n");
		worst_fit_memory_init(1024);
		head = get_head(algo);
		if(worst_fit_alloc(2048) == NULL){
			printf("Worst fit test 2 Passed\n");
		}

		printf("\n========================= Worst fit test 3: Allocate succeed, deallocate succeed ===========================\n\n");
		p = worst_fit_alloc(5);
		if((size_t)p == (size_t)head + 32){
			printf("Worst fit test 3 Passed\n");
		}
		worst_fit_dealloc(p);


		printf("\n========================= Worst fit test 4: Allocate in the largest available block ===========================\n\n");
		p = worst_fit_alloc(5);
		q = worst_fit_alloc(5);
		worst_fit_dealloc(p);
		p = worst_fit_alloc(5);
		if((size_t)p == (size_t)head + 40 + 40 + 32){
			printf("Worst fit test 4 Passed\n");
		}
		print_all_nodes(algo);

		printf("\n========================= Worst fit test 5: Block not found, failed to deallocate ===========================\n\n");
		worst_fit_memory_init(1024);
		p = worst_fit_alloc(5);
		//wrong block
		q = (void*)((size_t)p +1);
		worst_fit_dealloc(q);
		//block that has is not allocated, so dealloc will fail
		q = (void*)((size_t)p + 32 + 5);
		worst_fit_dealloc(q);
		printf("Worst fit test 5 Passed\n");
	} else if(algo == 2){
		printf("\n========================= External fragmentation test ===========================\n\n");
		//Initialize 102400 bytes memory space. 
		//Using a while loop to allocate and deallocate memory blocks repeatedly. More specifically, in each iteration, we allocate 8 memory blocks with a random size from 1 - 512 bytes and deallocate one every two blocks of memory. The while loop will break when one allocation failed.
		//Check the external fragmentation for memory block size less than 4 bytes, 8 bytes, 16 bytes, 32 bytes, 64 bytes, 128 bytes, 256 bytes, 512 bytes. These will be the experimental data.

		best_fit_memory_init(102400);
		while(1){
			//Alloc
			void* best_block[8];
			best_block[0] =	best_fit_alloc((size_t)(rand() % 512 + 1));
			if (best_block[0] == NULL) {  break; }
			best_block[1] =	best_fit_alloc((size_t)(rand() % 512 + 1));
			if (best_block[1] == NULL) {  break; }
			best_block[2] =	best_fit_alloc((size_t)(rand() % 512 + 1));
			if (best_block[2] == NULL) {  break; }
			best_block[3] =	best_fit_alloc((size_t)(rand() % 512 + 1));
			if (best_block[3] == NULL) {  break; }
			best_block[4] =	best_fit_alloc((size_t)(rand() % 512 + 1));
			if (best_block[4] == NULL) {  break; }
			best_block[5] =	best_fit_alloc((size_t)(rand() % 512 + 1));
			if (best_block[5] == NULL) {  break; }
			best_block[6] =	best_fit_alloc((size_t)(rand() % 512 + 1));
			if (best_block[6] == NULL) {  break; }
			best_block[7] =	best_fit_alloc((size_t)(rand() % 512 + 1));
			if (best_block[7] == NULL) {  break; }

			//Dealloc
			best_fit_dealloc(best_block[0]);
			best_fit_dealloc(best_block[2]);
			best_fit_dealloc(best_block[4]);
			best_fit_dealloc(best_block[6]);
		}

		worst_fit_memory_init(102400);
		while(1){
			//Alloc
			void* worst_block[8];
			worst_block[0] = worst_fit_alloc((size_t)(rand() % 512 + 1));
			if (worst_block[0] == NULL) {  break; }
			worst_block[1] = worst_fit_alloc((size_t)(rand() % 512 + 1));
			if (worst_block[1] == NULL) {  break; }
			worst_block[2] = worst_fit_alloc((size_t)(rand() % 512 + 1));
			if (worst_block[2] == NULL) {  break; }
			worst_block[3] = worst_fit_alloc((size_t)(rand() % 512 + 1));
			if (worst_block[3] == NULL) {  break; }
			worst_block[4] = worst_fit_alloc((size_t)(rand() % 512 + 1));
			if (worst_block[4] == NULL) {  break; }
			worst_block[5] = worst_fit_alloc((size_t)(rand() % 512 + 1));
			if (worst_block[5] == NULL) {  break; }
			worst_block[6] = worst_fit_alloc((size_t)(rand() % 512 + 1));
			if (worst_block[6] == NULL) {  break; }
			worst_block[7] = worst_fit_alloc((size_t)(rand() % 512 + 1));
			if (worst_block[7] == NULL) {  break; }

			//Dealloc
			worst_fit_dealloc(worst_block[0]);
			worst_fit_dealloc(worst_block[2]);
			worst_fit_dealloc(worst_block[4]);
			worst_fit_dealloc(worst_block[6]);
		}

		printf("\n========================= Best fit external fragmentation ===========================\n\n");
		printf("The external fragmentation memory blocks less than 4 bytes is %d\n", best_fit_count_extfrag(4));
		printf("The external fragmentation memory blocks less than 8 bytes is %d\n", best_fit_count_extfrag(8));
		printf("The external fragmentation memory blocks less than 16 bytes is %d\n", best_fit_count_extfrag(16));
		printf("The external fragmentation memory blocks less than 32 bytes is %d\n", best_fit_count_extfrag(32));
		printf("The external fragmentation memory blocks less than 64 bytes is %d\n", best_fit_count_extfrag(64));
		printf("The external fragmentation memory blocks less than 128 bytes is %d\n", best_fit_count_extfrag(128));
		printf("The external fragmentation memory blocks less than 256 bytes is %d\n", best_fit_count_extfrag(256));
		printf("The external fragmentation memory blocks less than 512 bytes is %d\n", best_fit_count_extfrag(512));

		printf("\n========================= Worst fit external fragmentation ===========================\n\n");
		printf("The external fragmentation memory blocks less than 4 bytes is %d\n", worst_fit_count_extfrag(4));
		printf("The external fragmentation memory blocks less than 8 bytes is %d\n", worst_fit_count_extfrag(8));
		printf("The external fragmentation memory blocks less than 16 bytes is %d\n", worst_fit_count_extfrag(16));
		printf("The external fragmentation memory blocks less than 32 bytes is %d\n", worst_fit_count_extfrag(32));
		printf("The external fragmentation memory blocks less than 64 bytes is %d\n", worst_fit_count_extfrag(64));
		printf("The external fragmentation memory blocks less than 128 bytes is %d\n", worst_fit_count_extfrag(128));
		printf("The external fragmentation memory blocks less than 256 bytes is %d\n", worst_fit_count_extfrag(256));
		printf("The external fragmentation memory blocks less than 512 bytes is %d\n", worst_fit_count_extfrag(512));
	} else {
		fprintf(stderr, "Should not reach here!\n");
		exit(1);
	}

	//printf("num = %d\n", num);

	return 0;
}

