#include <stdlib.h>
#include <stdint.h>
#include "mem.h"
//stolen from TechflashOS https://github.com/techflashYT/Techflash-OS
/*             Author: Benjamin David Lunt
 *                     Forever Young Software
 *                     Copyright (c) 1984-2022
 *  
 *  This code is donated to the Freeware communitee.  You have the
 *   right to use it for learning purposes only.  You may not modify it
 *   for redistribution for any other purpose unless you have written
 *   permission from the author.
 *
 *  You may modify and use it in your own projects as long as they are
 *   for non-profit only and if distributed, have these same requirements.
 *  Any project for profit that uses this code must have written 
 *   permission from the author.
 *
 *  For more information:
 *    http://www.fysnet.net/osdesign_book_series.htm
 *  Contact:
 *    fys [at] fysnet [dot] net
 *
 * Last update:  4 May 2022 (May the 4th be with you!)
 *
 */

void *malloc(size_t size);
void free(void *ptr);

#define MALLOC_FLAGS_VIRTUAL		(1 << 0)	 // any memory, physical or linear, non contiguous, any location
#define MALLOC_FLAGS_PHYSICAL	 (1 << 1)	 // must be physical address and contiguous, inclusive
#define MALLOC_FLAGS_CLEAR			(1 << 2)	 // clear the memory on return
#define MALLOC_FLAGS_LOW1MEG		(1 << 3)	 // must be before the 1 Meg mark, inclusive
#define MALLOC_FLAGS_LOW16MEG	 (1 << 4)	 // must be before the 16 Meg mark, inclusive
#define MALLOC_FLAGS_LOW4GIG		(1 << 5)	 // must be before the 4 Gig mark, inclusive
#define MALLOC_FLAGS_ALIGNED		(1 << 6)	 // must be aligned.	The aligned parameter is now used, else it is ignored.
#define MALLOC_HARDWARE32			 (MALLOC_FLAGS_LOW4GIG | MALLOC_FLAGS_PHYSICAL | MALLOC_FLAGS_CLEAR)
#define MALLOC_HARDWARE64			(											 MALLOC_FLAGS_PHYSICAL | MALLOC_FLAGS_CLEAR)
typedef void* HANDLE;
typedef void* spinlock_t;
#ifndef MALLOC_H
#define MALLOC_H

// this is the minimum allocation size
#define ALLOC_MIN  (65536 + sizeof(struct S_MEMORY_BUCKET) + sizeof(struct S_MEMORY_PEBBLE))

// magic value for a tag
#define MALLOC_MAGIC_BUCKET 'BUCK'
#define MALLOC_MAGIC_PEBBLE 'ROCK'

// local flags for a bucket (only applies to this bucket)
#define BUCKET_FLAG_FIRST   (0 <<  0)  // if clear, use first find method
#define BUCKET_FLAG_BEST    (1 <<  0)  // if set, use best fit method

// local flags for a pebble
#define PEBBLE_FLAG_FREE    (0 <<  0)  // if set, is in use, if clear, free for use
#define PEBBLE_FLAG_IN_USE  (1 <<  0)  //  ...


#define PEBBLE_MIN_ALIGN 64  // minimum power of 2 to align the next pebble (1 or a power of 2)
#define PEBBLE_MIN_SIZE  64  // a pebble must be at least this size
#if PEBBLE_MIN_SIZE > PEBBLE_MIN_ALIGN
	#error "PEBBLE_MIN_ALIGN must be at least PEBBLE_MIN_SIZE"
#endif

// macro to see if the free chunk is large enough to split
//                                 if ceil(current pebble size, 64)              >   new pebble size with a remainder < (sizeof(PEBBLE) + PEBBLE_MIN_SIZE)
#define SPLIT_PEBBLE(s0, s1) ((((s0) + PEBBLE_MIN_ALIGN - 1) & ~PEBBLE_MIN_SIZE) > ((s1) + sizeof(struct S_MEMORY_PEBBLE) + PEBBLE_MIN_SIZE))

#define PEBBLE_IS_FREE(p) (((p)->lflags & PEBBLE_FLAG_IN_USE) == PEBBLE_FLAG_FREE)


#pragma pack(push, 1)

struct S_MEMORY_PEBBLE {
	uint32_t magic;         // a pebble in a bucket
	uint32_t lflags;        // local flags for this pebble
	uint32_t sflags;        // sent flags for this pebble
	uint32_t padding;       // padding/alignment
	size_t size;          // count of bytes requested

	struct S_MEMORY_BUCKET *parent; // parent bucket of this pebble

	// linked list of pebbles
	struct S_MEMORY_PEBBLE *prev;
	struct S_MEMORY_PEBBLE *next;
};

struct S_MEMORY_BUCKET {
	uint32_t magic;     //  a bucket full of pebbles
	uint32_t lflags;    //  local flags for this bucket
	size_t size;      //  count of 4096 pages used for this bucket
	size_t largest;   //  largest free block in this bucket

	// linked list of buckets
	struct S_MEMORY_BUCKET *prev;
	struct S_MEMORY_BUCKET *next;

	struct S_MEMORY_PEBBLE *first;
};

#pragma pack(pop)

#define UPDATE_NODE(p0, p1) {\
	(p0)->next = (p1)->next;\
	(p1)->next = (p0);\
	(p0)->prev = (p1);\
	if ((p0)->next) {\
		(p0)->next->prev = (p0);\
	}\
}


#define __malloc(s)      kmalloc((s), 0, MALLOC_FLAGS_VIRTUAL)
#define __calloc(s)      kmalloc((s), 1, MALLOC_FLAGS_CLEAR)
#define __amalloc(s, a)  kmalloc((s), (a), ~MALLOC_FLAGS_CLEAR)
#define __acalloc(s, a)  kmalloc((s), (a), MALLOC_FLAGS_CLEAR)

// local functions
struct S_MEMORY_BUCKET *createBucket(size_t size);
struct S_MEMORY_PEBBLE *placePebble(struct S_MEMORY_BUCKET *bucket, struct S_MEMORY_PEBBLE *pebble);
struct S_MEMORY_PEBBLE *splitPebble(struct S_MEMORY_PEBBLE *pebble, size_t size);

// public data/functions
extern HANDLE kernelHeap;

HANDLE mallocInit(size_t size);

void *kmalloc(size_t size, uint64_t alignment, uint32_t flags);
void *realloc(void *ptr, size_t size); // The standard function.

#endif   // MALLOC_H