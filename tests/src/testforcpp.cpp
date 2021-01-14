#include <iostream>
#include <stdio.h>
#include <stdlib.h>
extern "C"{
#include "../lib/mymalloc.h"
}
using namespace std;
int main() {
  int *ptr = (int*)mymalloc(sizeof(int));
  if (ptr == NULL) { 
    cout<<"给int类型分配失败"<<endl;
    return 1;
  }

  int i;
  for(i=0;i<100;i++)
    *ptr = i;
  myfree(ptr);

  cout<<"给int类型分配成功并释放成功"<<endl;
  return 0;
}
