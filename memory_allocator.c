#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"
//#include "include/queue.h"
//#include "include/memory_allocator.h"

#define USABLE_SIZE(x) (x - sizeof(super_block))
#define SIZE_WITH_SUPERBLOCK(x) ((x) + sizeof(super_block))
#define MOVE_BY_SUPERBLOCK_RIGHT(x) (super_block*)((char*)x + sizeof(super_block))
#define MOVE_BY_SUPERBLOCK_LEFT(x) (super_block*)((char*)x - sizeof(super_block))

TAILQ_HEAD(sb_head, super_block);

typedef struct memory_range
{
	void* start;
	size_t size; /* Number of bytes which we can use */
	struct sb_head sb_head;
} memory_range;

typedef struct super_block
{
	size_t size; /* Includes the super_block size. */
	TAILQ_ENTRY(super_block) sb_link;
} super_block;

void init_memory_range(memory_range* const mr, void* const start, size_t size)
{
	mr->start = start;
	mr->size = size;
	super_block* sb = (super_block*)start;

	sb->size = size;
	TAILQ_INIT(&mr->sb_head);
	TAILQ_INSERT_HEAD(&mr->sb_head, sb, sb_link);
}

super_block* find_entry(struct sb_head* const sb_head, size_t total_size)
{
	super_block* current;

	TAILQ_FOREACH(current, sb_head, sb_link)
	{
		if (current->size >= total_size)
			return current;
	}

	return NULL;
}

void merge_right(struct sb_head* const sb_head, super_block* const sb)
{
	super_block* next = TAILQ_NEXT(sb, sb_link);
	
	if (!next)
		return;
	
	char* sb_ptr = (char*)sb;
	if (sb_ptr + sb->size == (char*) next)
	{
		TAILQ_REMOVE(sb_head, next, sb_link);
		sb->size += next->size;
	}
}

/* Insert in a sorted fashion and merge. */
void insert_free_block(struct sb_head* const sb_head, super_block* const sb)
{
	if (TAILQ_EMPTY(sb_head))
	{
		TAILQ_INSERT_HEAD(sb_head, sb, sb_link);
		return;
	}

	super_block* current = NULL;
	super_block* best_so_far = NULL; /* sb can be inserted after this entry. */
	TAILQ_FOREACH(current, sb_head, sb_link)
	{
		if (current < sb)
			best_so_far = current;
	}

	if (!best_so_far)
	{
		TAILQ_INSERT_HEAD(sb_head, sb, sb_link);
		merge_right(sb_head, sb);
	}
	else
	{
		TAILQ_INSERT_AFTER(sb_head, best_so_far, sb, sb_link);
		merge_right(sb_head, sb);
		merge_right(sb_head, best_so_far);
	}
}

void* allocate(memory_range* mr, size_t requested_size)
{
	if (requested_size == 0)
		return NULL;

	/* Search for the first entry in the list that has enough space. */
	super_block* sb = find_entry(&mr->sb_head, SIZE_WITH_SUPERBLOCK(requested_size));
	
	if (!sb) /* No entry has enough space. */
		return NULL;
	
	TAILQ_REMOVE(&mr->sb_head, sb, sb_link);
	size_t size_left = sb->size - SIZE_WITH_SUPERBLOCK(requested_size);
	
	if (size_left > sizeof(super_block))
	{
		super_block* new_sb = (super_block*)((char*)sb + SIZE_WITH_SUPERBLOCK(requested_size));
		new_sb->size = size_left;
		insert_free_block(&mr->sb_head, new_sb);
		sb->size = SIZE_WITH_SUPERBLOCK(requested_size);
	}
	return MOVE_BY_SUPERBLOCK_RIGHT(sb);
}

void deallocate(memory_range* mr, void* memory_ptr)
{
	if (!memory_ptr)
		return;

	insert_free_block(&mr->sb_head, MOVE_BY_SUPERBLOCK_LEFT(memory_ptr));
}

void print_free_blocks(memory_range* mr)
{
	printf("printing the free blocks list:\n");
	super_block* current;
	TAILQ_FOREACH(current, &mr->sb_head, sb_link)
	{
		printf("%zu\n", current->size);
	}
	printf("\n");
}

int main()
{
	size_t size = 1063;
	void* ptr = malloc(size);
	memory_range mr;
	
	init_memory_range(&mr, ptr, size);
	
	print_free_blocks(&mr);
	
	void* ptr1 = allocate(&mr, 63);
	assert(ptr1 != NULL);

	print_free_blocks(&mr);
	
	void* ptr2 = allocate(&mr, 1000 - sizeof(super_block) + 1);
	assert(ptr2 == NULL);
	
	
	void* ptr3 = allocate(&mr, 1000 - 3*sizeof(super_block) - 1);
	assert(ptr3 != NULL);
	
	
	void* ptr4 = allocate(&mr, 1);
	assert(ptr4 != NULL);


	void* ptr5 = allocate(&mr, 1);
	assert(ptr5 == NULL);


	deallocate(&mr, ptr3);
	deallocate(&mr, ptr4);
	deallocate(&mr, ptr1);

	print_free_blocks(&mr);

	void* ptr6 = allocate(&mr, 1063 - sizeof(super_block));
	assert(ptr6 != NULL);


	printf("Finished!\n");
	
	return 0;
}
