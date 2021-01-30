#include "pic24_all.h"
#include <stdio.h>

uint16_t fib (uint16_t n) {
  uint16_t f1;

  if (n == 0) return 0;
  if (n == 1) return 1;
  f1 = fib(n-1);
  f1 = fib(n-2) + f1;
  return (f1);
}

main (void) {

  uint16_t  k,j;
  j = 13;
  k = fib(j);

  printf (
    "fib(%d) is: %d\n",
    j,k);
}
