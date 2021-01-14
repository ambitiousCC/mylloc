#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "../include/mymalloc.h"

struct my_block *find_free_block(struct my_block **last_block, size_t size) {
  struct my_block *current = g_base; //当前的内存块
  while (current && !(current->free && current->size >= size)) { //当前current存在，不满足(当前找到的内存块空闲且大小大于需要分配的大小)条件继续寻找
    *last_block = current; // 当前
    current = current->next; //下一个内存块
  }
  return current; //找到的结果（需要判断具体的内容是否可用）
}

struct my_block *find_new_space(struct my_block* last_block, size_t size) {
  struct my_block *block;
  block = sbrk(0);
  /******************只是证明可以分配*******************/
  void *request = sbrk(size + META_SIZE); //size是当前想要分配的内存，需要加上META_SIZE是已经存过的内存
  assert((void*)block == request); //如果线程不安全，停止
  if (request == (void*) -1) return NULL; //分配失败
  /**************************************************/
  
  /******************开始分配的语句*********************/
  if (last_block) last_block->next = block; //第一次请求时last_block==NULL跳过此步，之后每一次使用指针链接当前到指定block，也就是内存的开头
  block->size = size;
  block->next = NULL;
  block->free = 0; //标识占用
  return block;
}

/**
1. 如果我们能够找到一个空闲块，就使用它
2. 如果不能，继续调用find_new_space请求空间
*/
void *mymalloc(size_t size) {
  struct my_block *block;

  if (size <= 0) return NULL; //1. 非法输入

  if (!g_base) {
    block = find_new_space(NULL, size);
    if (!block) return NULL; //2. 无法分配
    g_base = block;
  } else {
    struct my_block *last_block = g_base;
    block = find_free_block(&last_block, size); //在内存空间中找
    if (!block) {
      block = find_new_space(last_block, size); //请求新的内存空间
      if (!block) return NULL; //3. 请求无效
    } else {
      //值得注意的：考虑冲突的快
      block->free = 0; //标识block已经被占用
    }
  }
  
  return(block+1); //4. 请求成功，返回分配成功的内存块的下一个指针
}

void *mycalloc(size_t nelem, size_t elsize) {
  size_t size = nelem * elsize;
  void *ptr = mymalloc(size); //分配
  memset(ptr, 0, size); //初始化调用malloc分配的空间（设置为0)
  return ptr;
}

struct my_block *get_block_ptr(void *ptr) {
  return (struct my_block*)ptr - 1; //因为malloc中返回的是分配成功的内存块的下一个指针，这里需要-1才能指向真正分配的内存块的指针
}

void myfree(void *ptr) {
  if (!ptr) return; //不能释放不存在的指针

  struct my_block* block_ptr = get_block_ptr(ptr); //分配的内存块指针
  assert(block_ptr->free == 0); //如果这个内存块并没有被占用，实际上是没有成功的
  block_ptr->free = 1; //内存标识为1，空闲啦
}

void *myrealloc(void *ptr, size_t size) {
  if (!ptr) return mymalloc(size); //如果未分配，直接分配一个malloc

  struct my_block* block_ptr = get_block_ptr(ptr);
  if (block_ptr->size >= size) return ptr;

  //需要malloc一个新地址，然后是否原来的旧空间并且干掉野指针，记得偷偷复制原来的数据，对用户透明
  void *new_ptr;
  new_ptr = mymalloc(size); 
  if (!new_ptr) return NULL;
  memcpy(new_ptr, ptr, block_ptr->size); //从原块到目的块
  myfree(ptr);  
  return new_ptr;
}
