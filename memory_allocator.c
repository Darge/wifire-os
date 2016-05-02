#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

TAILQ_HEAD(sb_head, super_block);


typedef struct memory_range
{
	char* start; /* void*? but we can't increment it */
	size_t size; /* number of bytes which we can use */
	struct sb_head sb_head;
} memory_range;

typedef struct super_block
{
	size_t size;
	TAILQ_ENTRY(super_block) sb_link;
} super_block;

void init_memory_range(memory_range* mr, void* start, size_t size)
{
	mr->start = start;
	mr->size = size;
	super_block* sb_ptr = (super_block*)mr;
	


	
}

char* allocate(memory_range* mr, size_t requested_size)
{
	return NULL;	
}



int main()
{
	const int size = 1024*1024 + 37;
	void* ptr = malloc(size);
	memory_range mr;
	init_memory_range(&mr, ptr, size);


	printf("Finished!\n");
	
	return 0;
}
