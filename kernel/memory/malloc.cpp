#include <stdlib.h>
#include <stdlib.h>
#include "malloc.h"

#define PAGE_SIZE 2048

void *malloc(size_t size)
{
	return __malloc(size);
}

// TODO: free function and fix busted code
size_t currentMmapAddr = 0x0000000001000000;
bool *isPageFree;
bool isPagesFreeListReady = false;
uint8_t pagesUsed = 0;
void *memmap(size_t size)
{
	if (size < 2048)
    {
		size = 2048;
	}
	else
    {
		size = (size + 7) &(-2048);
	}
	
    size_t oldMmapAddr = currentMmapAddr;
	currentMmapAddr += size;

	if (isPagesFreeListReady)
    {
		
	}
	
    pagesUsed += (size / 2048);
	return (void *)oldMmapAddr;
}

void mmapFree(void *ptr, size_t size)
{
	// TODO: Make this work
	return;
}

void spinLock(spinlock_t lock)
{
	return;
}

void spinUnlock(spinlock_t lock)
{
	return;
}


//stolen from TechflashOS https://github.com/techflashYT/Techflash-OS
/*						 Author: Benjamin David Lunt
 *										 Forever Young Software
 *										 Copyright (c) 1984-2022
 *	
 *	This code is donated to the Freeware communitee.	You have the
 *	 right to use it for learning purposes only.	You may not modify it
 *	 for redistribution for any other purpose unless you have written
 *	 permission from the author.
 *
 *	You may modify and use it in your own projects as long as they are
 *	 for non-profit only and if distributed, have these same requirements.
 *	Any project for profit that uses this code must have written 
 *	 permission from the author.
 *
 *	For more information:
 *		http://www.fysnet.net/osdesign_book_series.htm
 *	Contact:
 *		fys [at] fysnet [dot] net
 *
 * Last update:	4 May 2022 (May the 4th be with you!)
 *	(What is the core temperature of a Tauntaun?	--	Luke warm)
 *
 */

spinlock_t mallocSpinlock = 0;

// initialize the heap by simply allocating a single Bucket
//	with a single Pebble in it.	The pebble will be free.
HANDLE mallocInit(size_t size)
{
	struct S_MEMORY_BUCKET *bucket = createBucket(size);
	// Allocate enough RAM to store if up to 1TB of ram is free or not,
	// after that you're on your own.
	isPageFree = (bool*)malloc(524288 * sizeof(bool));
	// Set the pages used by the setup to "used"
	memset(isPageFree, false, pagesUsed * 2048);
	memset(isPageFree + (pagesUsed * 2048), true, ((524288 * sizeof(bool)) - (pagesUsed * sizeof(bool))));
	return bucket;
}

// allocates a linear block of memory, in 'size' bytes, and creates
//	a Bucket for this block, with one (free) Pebble.
struct S_MEMORY_BUCKET *createBucket(size_t size)
{

	// do we allocate a minimum?
#ifdef ALLOC_MIN	 
	if (size < ALLOC_MIN)
    {
		size = ALLOC_MIN;
	}
#endif
	
	// size must be a even number of pages
	size = (size + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1);

	struct S_MEMORY_BUCKET *bucket = (struct S_MEMORY_BUCKET *) memmap(size / PAGE_SIZE);
	if (bucket != NULL)
    {
		bucket->magic = MALLOC_MAGIC_BUCKET;
		bucket->lflags = BUCKET_FLAG_FIRST;
		bucket->size = size / PAGE_SIZE;	// count of pages used
		bucket->largest = size - sizeof(struct S_MEMORY_BUCKET) - sizeof(struct S_MEMORY_PEBBLE);

		bucket->prev = NULL;	// these will be assigned by the insertBucket() call
		bucket->next = NULL;
		
		struct S_MEMORY_PEBBLE *first = (struct S_MEMORY_PEBBLE *) ((uint8_t *) bucket + sizeof(struct S_MEMORY_BUCKET));
		bucket->first = first;

		first->magic = MALLOC_MAGIC_PEBBLE;
		first->sflags = MALLOC_FLAGS_VIRTUAL;
		first->lflags = PEBBLE_FLAG_FREE;
		first->padding = 0;
		first->size = bucket->largest;

		first->parent = bucket;
		first->prev = NULL;
		first->next = NULL;
	}

	return bucket;
}

// insert a bucket at destination
void insertBucket(struct S_MEMORY_BUCKET *bucket, void *destination)
{
	struct S_MEMORY_BUCKET *dest = (struct S_MEMORY_BUCKET *) destination;

	if (bucket && dest)
	UPDATE_NODE(bucket, dest);
}

// remove a bucket
void removeBucket(struct S_MEMORY_BUCKET *bucket)
{

	// don't remove the initial bucket
	if (bucket && (bucket != kernelHeap))
    {
		if (bucket->prev)
        {
			bucket->prev->next = bucket->next;
		}

		if (bucket->next)
        {
			bucket->next->prev = bucket->prev;
		}
		
        mmapFree(bucket, bucket->size);
	}
}

// run through the bucket and get the (possibly) new largest size
size_t bucketUpdateLargest(struct S_MEMORY_BUCKET *bucket)
{
	struct S_MEMORY_PEBBLE *p = bucket->first;
	size_t ret = 0;

	while (p != NULL)
    {
	if (p->size > ret)
		ret = p->size;
	p = p->next;
	}

	// update the value
	bucket->largest = ret;

	return ret;
}

// this takes an already created pebble and tries to place it in a bucket
// it is assumed that the caller has already checked that this bucket
//	isn't full and can hold the pebble, though we check anyway.
struct S_MEMORY_PEBBLE *placePebble(struct S_MEMORY_BUCKET *bucket, struct S_MEMORY_PEBBLE *pebble)
{
	struct S_MEMORY_PEBBLE *start = bucket->first;
	struct S_MEMORY_PEBBLE *best = NULL;
	size_t best_size = -1;
	
	if (bucket->lflags & BUCKET_FLAG_BEST) {
		// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		// BEST FIT method
		// scroll through all the pebbles until we find a free one
		//	large enough to insert our pebble, but the least sized free
		//	entry that satisfies our request.
		while (start != NULL)
        {
			if (PEBBLE_IS_FREE(start) && (start->size <= pebble->size))
            {
				if (start->size < best_size)
                {
					best = start;
					best_size = start->size;
				}
			}
			start = start->next;
		}
		// did we find one? Do we need to split it?
		if (best != NULL)
        {
			splitPebble(best, pebble->size);
			best->sflags = pebble->sflags;
			best->lflags = pebble->lflags;
		}
		start = best;
	}
	else
    {
		// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		// FIRST FOUND method
		// scroll through the pebbles until we find a free one
		//	large enough to insert our pebble in.	First one found, we use.
		while (start != NULL)
        {
			if (PEBBLE_IS_FREE(start) && (start->size >= pebble->size))
            {
				// we found one to use.	Do we need to split it?
				splitPebble(start, pebble->size);
				start->sflags = pebble->sflags;
				start->lflags = pebble->lflags;

				break;
			}
			start = start->next;
		}
	}
	return start;
}

// if the current pebble is large enough, will split a pebble into two
// else it returns NULL
struct S_MEMORY_PEBBLE *splitPebble(struct S_MEMORY_PEBBLE *pebble, size_t size)
{
	struct S_MEMORY_PEBBLE *newPebble = NULL;
	size_t newSize;
	
	if (SPLIT_PEBBLE(pebble->size, size))
    {
		newSize = (size + (PEBBLE_MIN_ALIGN - 1)) & ~(PEBBLE_MIN_ALIGN - 1);
		newPebble = (struct S_MEMORY_PEBBLE *)((uint8_t *) pebble + sizeof(struct S_MEMORY_PEBBLE) + newSize);

		memcpy(newPebble, pebble, sizeof(struct S_MEMORY_PEBBLE));

		newPebble->size = pebble->size - newSize - sizeof(struct S_MEMORY_PEBBLE);
		newPebble->prev = pebble;
		pebble->size = newSize;
		pebble->next = newPebble;
	}

	return newPebble;
}

// if this pebble is empty *and* if present, the next one is empty,
//	then absorb the next one, into this one.
struct S_MEMORY_PEBBLE *absorbNext(struct S_MEMORY_PEBBLE *pebble)
{
	if (pebble && pebble->next)
    {
		if (PEBBLE_IS_FREE(pebble) && PEBBLE_IS_FREE(pebble->next))
        {
			if (pebble->parent->first == pebble->next)
            {	// don't "delete" the Bucket->first pebble before we update it
				pebble->parent->first = pebble;
			}

			pebble->size += pebble->next->size + sizeof(struct S_MEMORY_PEBBLE);
			pebble->next = pebble->next->next;
			if (pebble->next)
            {
				pebble->next->prev = pebble;
			}
			bucketUpdateLargest(pebble->parent);
		}
	}
	return pebble;
}

// if this pebble is empty, *and* if present the last one is empty,
//	then let the last one absorb this one.
struct S_MEMORY_PEBBLE *meltPrev(struct S_MEMORY_PEBBLE *pebble)
{
	if (pebble && pebble->prev)
    {
		if (PEBBLE_IS_FREE(pebble) && PEBBLE_IS_FREE(pebble->prev))
        {
			if (pebble->parent->first == pebble)
            {	// don't "delete" the Bucket->first pebble before we update it
				pebble->parent->first = pebble->prev;
			}

			pebble->prev->size += pebble->size + sizeof(struct S_MEMORY_PEBBLE);
			pebble->prev->next = pebble->next;

			if (pebble->next)
            {
				pebble->next->prev = pebble->prev;
			}
			pebble = pebble->prev;
			bucketUpdateLargest(pebble->parent);
		}
	}
	return pebble;
}

// shrink the pebble from the current size to a new smaller size
//	if the size is now small enough to split the pebble, we do it
struct S_MEMORY_PEBBLE *shrinkPebble(struct S_MEMORY_PEBBLE *pebble, size_t size)
{
	struct S_MEMORY_PEBBLE *ret = NULL;

	if (pebble)
    {
		splitPebble(pebble, size);
		ret = pebble;
	}

	return ret;
}


void *kmalloc(size_t size, uint64_t alignment, uint32_t flags)
{
	void *ret = NULL;

	// minimum amount of memory we allocate to the caller
	if (size < PEBBLE_MIN_SIZE)
    {
		size = PEBBLE_MIN_SIZE;
	}

	struct S_MEMORY_PEBBLE pebble;
	pebble.magic = MALLOC_MAGIC_PEBBLE;
	pebble.sflags = flags;
	pebble.lflags = PEBBLE_FLAG_IN_USE;
	pebble.padding = 0;
	pebble.size = (size + (PEBBLE_MIN_ALIGN - 1)) & ~(PEBBLE_MIN_ALIGN - 1);

	spinLock(&mallocSpinlock);

	struct S_MEMORY_BUCKET *bucket = (struct S_MEMORY_BUCKET *)kernelHeap;
	while (bucket != NULL)
    {
		if (bucket->largest >= pebble.size)
        {
			ret = placePebble(bucket, &pebble);
			bucketUpdateLargest(bucket);
			if (ret != NULL)
			ret = (uint8_t *) ret + sizeof(struct S_MEMORY_PEBBLE);
			break;
		}
		bucket = bucket->next;
	}

	// if ret == NULL, we didn't find a bucket large enough, or with enough empty space.
	//	so allocate another bucket
	if (ret == NULL)
    {
		size_t newSize = pebble.size + (sizeof(struct S_MEMORY_BUCKET) + sizeof(struct S_MEMORY_PEBBLE));
		bucket = createBucket(newSize);
		if (bucket)
        {
			insertBucket(bucket, kernelHeap);
			ret = placePebble(bucket, &pebble);
			bucketUpdateLargest(bucket);
			if (ret != NULL)
			ret = (uint8_t *) ret + sizeof(struct S_MEMORY_PEBBLE);
		}
	}

	spinUnlock(&mallocSpinlock);

	// if we are to clear the memory, do it now
	if (ret && (flags & MALLOC_FLAGS_CLEAR))
    {
		memset(ret, 0, size);
	}
	
	return ret;
}

void *realloc(void *ptr, size_t size)
{
	struct S_MEMORY_PEBBLE *pebble;
	void *ret = NULL;
	
	if (size == 0) {
		free(ptr);
		return NULL;
	}
	
	if (ptr == NULL) {
		return kmalloc(size, 0 /* not used */, MALLOC_FLAGS_VIRTUAL);
	}

	spinLock(&mallocSpinlock);

	pebble = (struct S_MEMORY_PEBBLE *) ((uint8_t *) ptr - sizeof(struct S_MEMORY_PEBBLE));

	spinUnlock(&mallocSpinlock);

	if (size <= pebble->size) {
		ret = shrinkPebble(pebble, size);
	}
	else {
		if (pebble->sflags & MALLOC_FLAGS_ALIGNED) {
			ret = NULL;
		}
		else {
			// the new requested size is larger than the current pebble, so allocate a new space
			ret = kmalloc(size, 0 /* not used */, pebble->sflags);
			if (ret)
			memcpy(ret, ptr, size);
			free(ptr);
		}
	}

	return ret;
}

// free a pebble, possibly merging it with a neighbor(s), and possible removing this
//	now empty Bucket.
void free(void *ptr)
{

	if (ptr == NULL)
	return;

	spinLock(&mallocSpinlock);

	struct S_MEMORY_PEBBLE *pebble = (struct S_MEMORY_PEBBLE *) ((uint8_t *) ptr - sizeof(struct S_MEMORY_PEBBLE));
	
	// check that it actually is a pebble
	if (pebble->magic != MALLOC_MAGIC_PEBBLE)
    {
		spinUnlock(&mallocSpinlock);
		return;
	}
	
	// mark it as free
	pebble->lflags = PEBBLE_FLAG_FREE;
	
	// see if we can absorb any of the neighbors
	pebble = meltPrev(pebble);
	absorbNext(pebble);

	// if this empties the bucket, shall we remove the bucket?
	struct S_MEMORY_BUCKET *bucket = pebble->parent;
	if (PEBBLE_IS_FREE(bucket->first) && (bucket->first->prev == NULL) && (bucket->first->next == NULL))
    {
		removeBucket(bucket);
	}
	else
    {
		bucketUpdateLargest(bucket);
	}
	
	spinUnlock(&mallocSpinlock);
}