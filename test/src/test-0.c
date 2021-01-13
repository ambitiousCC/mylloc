#include <stdio.h>
#include <stdlib.h>
#include <mymalloc.h>

int main() {
  int *ptr = mymalloc(sizeof(int));
  if (ptr == NULL) { 
    printf("Failed to malloc a single int\n");
    return 1;
  }

  *ptr = 1;
  *ptr = 100;

  myfree(ptr);

  printf("malloc'd an int, assigned to it, and free'd it\n");

  int *ptr2 = mymalloc(sizeof(int));
  if (ptr2 == NULL) { 
    printf("Failed to malloc a single int\n");
    return 1;
  }

  *ptr2 = 2;
  *ptr2 = 200;

  myfree(ptr2);
  printf("malloc'd an int, assigned to it, and free'd it #2\n");

  malloc(1); // Screw up alignment.

  int *ptr3 = mymalloc(sizeof(int));
  if (ptr3 == NULL) { 
    printf("Failed to malloc a single int\n");
    return 1;
  }

  *ptr3 = 3;
  *ptr3 = 300;

  myfree(ptr3);
  printf("malloc'd an int, assigned to it, and free'd it #3\n");

  return 0;
}
