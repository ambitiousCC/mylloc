#include <stdio.h>
#include <stdlib.h>
#include <mymalloc.h>

#define TOTAL_ALLOCS 200000
#define ALLOC_SIZE 1024*1024

int main() {
  mymalloc(1);
	
  int i;
  void *ptr = NULL;

  for (i = 0; i < TOTAL_ALLOCS; i++) {
    ptr = mymalloc(ALLOC_SIZE);
    if (ptr == NULL) {
      printf("Memory failed to allocate!\n");
      return 1;
    }
		
    myfree(ptr);
  }

  printf("Memory was allocated and freed!\n");	
  return 0;
}
