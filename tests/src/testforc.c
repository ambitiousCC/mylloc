#include <stdio.h>
#include <stdlib.h>
#include <mymalloc.h>

int main() {
  int *ptr = (int*)mymalloc(sizeof(int));
  if (ptr == NULL) { 
    printf("给int类型分配失败\n");
    return 1;
  }

  int i;
  for(i=0;i<100;i++)
    *ptr = i;
  myfree(ptr);

  printf("给int类型分配成功并释放成功\n");
  return 0;
}
