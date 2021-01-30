#include "pic24_all.h"
#include <stdio.h>

uint8_t countOnes (uint16_t u16_v) {
  uint8_t u8_cnt, u8_i;

  u8_cnt = 0;
  for (u8_i=0; u8_i<16; u8_i++) {
    if (u16_v & 0x0001) u8_cnt++;
    u16_v = u16_v >> 1;
  }
  return(u8_cnt);
}


int main (void) {

  uint16_t u16_k;
  uint8_t  u8_j;

  u16_k = 0xA501;
  u8_j = countOnes(u16_k);
  printf (
    "Number of one bits in %x is %d\n",
    u16_k, u8_j);
  while (1);
}
