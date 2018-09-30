#include <stdio.h>

int main(){
  int b,c;
  register int a asm ("16");
  a = 24;
  b = 3;
  c = 4;
  asm volatile
  (

    "spmal   %[z], %[x], %[y]\n\t"
    : [z] "=r" (a)
    : [x] "r" (b), [y] "r" (c)
  );

  if ( c != 1 ){
     printf("\n[[FAILED]]\n");
     return -1;
  }

  printf("\n[[PASSED]]\n");

  return 0;
}
