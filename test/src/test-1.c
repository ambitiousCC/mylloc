#include <stdio.h>
#include <stdlib.h>
#include <mymalloc.h>

#define RUNS 10000

int main() {
  mymalloc(1);

  int i;
  int **arr = mymalloc(RUNS * sizeof(int *));

  if (arr == NULL) {
    printf("Memory failed to allocate!\n");
    return 1;
  }

  for (i = 0; i < RUNS; i++) {
    arr[i] = mymalloc(sizeof(int));
    if (arr[i] == NULL) {
      printf("Memory failed to allocate!\n");
      return 1;
    }
    
    *(arr[i]) = i+1;
  }

  for (i = 0; i < RUNS; i++) {
    if (*(arr[i]) != i+1) {
      printf("Memory failed to contain correct data after many allocations!\n");
      return 2;
    }
  }

  for (i = 0; i < RUNS; i++) {
    myfree(arr[i]);
  }
  
  myfree(arr);
  printf("Memory was allocated, used, and freed!\n");	
  return 0;
}
