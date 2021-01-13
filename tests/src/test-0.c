#include <iostream>
#include <cstdlib>
#include <cstring>
#include <mymalloc.h>
using namespace std;

int main()
{
　　char *p;
　　p = (char *)mymalloc(10*sizeof(char));

　　memset(p,0,10*sizeof(char));

　　strcpy(p,"come on");
　　cout << "p: " << p << endl;

　　p =(char *)myrealloc(p,20*sizeof(char));
　　cout << "p: " << sizeof(p) << endl;

　　strcat(p,",baby!");
　　cout << "p: " << p << endl;

　　myfree(p);
   return 0;
}