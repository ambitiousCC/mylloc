#include <stdio.h>
#include <stdlib.h>
#include <mymalloc.h>

int main() {
  int *ptr = mymalloc(sizeof(int));
  if (ptr == NULL) { 
    printf("给int类型分配失败\n");
    return 1;
  }

  for(int i=0;i<)
  myfree(ptr);

  printf("给int类型分配成功并释放成功\n");
  return 0;
}
