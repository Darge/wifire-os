#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

TAILQ_HEAD(sb_head, super_block);
#define USABLE_SIZE(x) x - sizeof(super_block)
#define SIZE_WITH_SUPERBLOCK(x) x + sizeof(super_block)

typedef struct memory_range
{
	char* start; /* void*? but we can't increment it */
	size_t size; /* number of bytes which we can use */
	struct sb_head sb_head;
} memory_range;

typedef struct super_block
{
	size_t size; /* Including the super_block size. */
	TAILQ_ENTRY(super_block) sb_link;
} super_block;

void init_memory_range(memory_range* mr, void* start, size_t size)
{
	mr->start = start;
	mr->size = size;
	super_block* sb = (super_block*)mr;

	sb->size = size;
	TAILQ_INSERT_HEAD(&mr->sb_head, sb, sb_link);
}

super_block* find_entry(struct sb_head* sb_head, size_t total_size)
{
	super_block* current;

	TAILQ_FOREACH(current, sb_head, sb_link)
	{
		if (current->size >= total_size)
			return current;
	}

	return NULL;
}

// Insert in a sorted fashion
void insert_free_block(struct sb_head* sb_head, super_block* sb, size_t size)
{
	sb->size = size;
	if (TAILQ_EMPTY(sb_head))
	{
		TAILQ_INSERT_HEAD(sb_head, sb, sb_link);
	}
}

char* allocate(memory_range* mr, size_t requested_size)
{
	/* Search for the first entry in the list that has enough space */
	super_block* sb = find_entry(&mr->sb_head, SIZE_WITH_SUPERBLOCK(requested_size));
	
	if (!sb)
		return NULL; /* No entry has enough space */
	
	TAILQ_REMOVE(&mr->sb_head, sb, sb_link);
	size_t size_left = sb->size - SIZE_WITH_SUPERBLOCK(requested_size);
	
	if (size_left > sizeof(super_block))
	{
		super_block* new_sb = (super_block*)((char*)sb + SIZE_WITH_SUPERBLOCK(requested_size));
		insert_free_block(&mr->sb_head, new_sb, size_left);
		sb->size = SIZE_WITH_SUPERBLOCK(requested_size);
	}
	else
	{
		
	}

	return NULL;
}



int main()
{
	const int size = 1024 + 37;
	void* ptr = malloc(size);
	memory_range mr;
	init_memory_range(&mr, ptr, size);


	printf("Finished!\n");
	
	return 0;
}
