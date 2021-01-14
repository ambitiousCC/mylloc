#include <iostream>
#include <stdio.h>
#include <stdlib.h>
extern "C"{
#include "./lib/mymalloc.h"
}
using namespace std;
int main() {
  int *ptr = (int*)mymalloc(sizeof(int));
  if (ptr == NULL) { 
    cout<<"Failed to malloc a single int"<<endl;
    return 1;
  }

  *ptr = 1;
  *ptr = 100;

  myfree(ptr);

  cout<<"malloc'd an int, assigned to it, and free'd it"<<endl;

  int *ptr2 = (int*)mymalloc(sizeof(int));
  if (ptr2 == NULL) { 
    cout<<"Failed to malloc a single int\n"<<endl;
    return 1;
  }

  *ptr2 = 2;
  *ptr2 = 200;

  myfree(ptr2);
  cout<<"malloc'd an int, assigned to it, and free'd it #2"<<endl;

  malloc(1); // Screw up alignment.

  int *ptr3 = (int*)mymalloc(sizeof(int));
  if (ptr3 == NULL) { 
    cout<<"Failed to malloc a single int"<<endl;
    return 1;
  }

  *ptr3 = 3;
  *ptr3 = 300;

  myfree(ptr3);
  cout<<"malloc'd an int, assigned to it, and free'd it #3"<<endl;

  return 0;
}
