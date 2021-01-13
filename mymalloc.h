#ifndef MYMALLOC_H
#define MYMALLOC_H
#include <string.h>
#include <sys/types.h>

void *nofree_malloc(size_t size);
struct block_meta {
  size_t size;
  struct block_meta *next;
  int free;
  int magic;    // For debugging only. TODO: remove this in non-debug mode.
};
#define META_SIZE sizeof(struct block_meta)
static void *global_base = NULL; //need string.h
struct block_meta *find_free_block(struct block_meta **last, size_t size);
struct block_meta *request_space(struct block_meta* last, size_t size);
struct block_meta *get_block_ptr(void *ptr);
void *mymalloc(size_t size);
void *mycalloc(size_t nelem, size_t elsize);
void *myrealloc(void *ptr, size_t size);
void myfree(void *ptr);
#endif