#ifndef MYMALLOC_H
#define MYMALLOC_H
#include <string.h>
#include <sys/types.h>
void *nofree_malloc(size_t size);
struct my_block {
  size_t size;
  struct my_block *next; //链表形式存储
  int free; //0，标识不可用；1，标识可用
};
static size_t META_SIZE = sizeof(struct my_block);
static void *g_base = NULL; //need string.h
struct my_block *find_free_block(struct my_block **last_block, size_t size);
struct my_block *request_space(struct my_block* last_block, size_t size);
struct my_block *get_block_ptr(void *ptr);
void *mymalloc(size_t size);
void *mycalloc(size_t nelem, size_t elsize);
void *myrealloc(void *ptr, size_t size);
void myfree(void *ptr);
#endif