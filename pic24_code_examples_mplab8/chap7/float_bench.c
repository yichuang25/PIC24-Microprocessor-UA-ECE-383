#include "pic24_all.h"

/**
A simple program that benchmarks float vs u32 operations
The instruction cycle measurement is done in the simulator,
this program is not meant to run on hardware since configClock()
is not included.
*/


float f_a[10],f_b[10],f_c[10];
uint32_t u32_a[10],u32_b[10],u32_c[10];

//#define OP +
//#define OP *
#define OP /

void float_op (float *ptra, float *ptrb, float *ptrc, uint8_t u8_cnt) {

  uint8_t u8_i;
  for (u8_i=0; u8_i< u8_cnt; u8_i++) {
    *ptrc = *ptra OP *ptrb;   // add contents of two arrays
    ptra++;
    ptrb++;
    ptrc++;
  }
}

void uint32_t_op (uint32_t *ptra, uint32_t *ptrb, uint32_t *ptrc, uint8_t u8_cnt) {

  uint8_t u8_i;
  for (u8_i=0; u8_i< u8_cnt; u8_i++) {
    *ptrc = *ptra OP *ptrb;   // add contents of two arrays
    ptra++;
    ptrb++;
    ptrc++;
  }
}


int main(void) {

  f_a[0] = f_b[0]= 66666.6666666;
  f_a[1] = f_b[1]= 66666.6666666;
  f_a[2] = f_b[2]= 66666.6666666;
  f_a[3] = f_b[3]= 66666.6666666;
  f_a[4] = f_b[4]= 66666.6666666;
  f_a[5] = f_b[5]= 66666.6666666;
  f_a[6] = f_b[6]= 66666.6666666;
  f_a[7] = f_b[7]= 66666.6666666;
  f_a[8] = f_b[8]= 66666.6666666;
  f_a[9] = f_b[9]= 66666.6666666;
  f_b[0]= 33333.333333333;
  f_b[1]= 33333.333333333;
  f_b[2]= 33333.333333333;
  f_b[3]= 33333.333333333;
  f_b[4]= 33333.333333333;
  f_b[5]= 33333.333333333;
  f_b[6]= 33333.333333333;
  f_b[7]= 33333.333333333;
  f_b[8]= 33333.333333333;
  f_b[9]= 33333.333333333;

  u32_a[0] = u32_b[0]= 242142;
  u32_a[1] = u32_b[1]= 242142;
  u32_a[2] = u32_b[2]= 242142;
  u32_a[3] = u32_b[3]= 242142;
  u32_a[4] = u32_b[4]= 242142;
  u32_a[5] = u32_b[5]= 242142;
  u32_a[6] = u32_b[6]= 242142;
  u32_a[7] = u32_b[7]= 242142;
  u32_a[8] = u32_b[8]= 242142;
  u32_a[9] = u32_b[9]= 2421422;


  float_op(f_a,f_b,f_c,10);
  uint32_t_op(u32_a,u32_b,u32_c,10);

  while (1) {
  } // end while (1)

  // End program
  return 0;
}
