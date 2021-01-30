#include "pic24_all.h"
#include <stdio.h>

void upcase (char *p) {
  while (*p != 0) {
    //convert to upper case
    if (*p > 0x60 &&
        *p < 0x7B)  {
      //lowercase ‘a’-’z’, so
      //convert to ‘A’-’Z’
      *p = *p - 0x20;
    }
    p++;  //advance to next char
  }
}

char s1[]="Hello";
char s2[]="UPPER/lower";

int main (void) {
  upcase(s1);
  upcase(s2);
  while (1);
}
