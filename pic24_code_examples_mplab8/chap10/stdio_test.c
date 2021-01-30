/*
 * "Copyright (c) 2008 Robert B. Reese, Bryan A. Jones, J. W. Bruce ("AUTHORS")"
 * All rights reserved.
 * (R. Reese, reese_AT_ece.msstate.edu, Mississippi State University)
 * (B. A. Jones, bjones_AT_ece.msstate.edu, Mississippi State University)
 * (J. W. Bruce, jwbruce_AT_ece.msstate.edu, Mississippi State University)
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice, the following
 * two paragraphs and the authors appear in all copies of this software.
 *
 * IN NO EVENT SHALL THE "AUTHORS" BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE "AUTHORS"
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE "AUTHORS" SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE "AUTHORS" HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Please maintain this header in its entirety when copying/modifying
 * these files.
 *
 *
 */
#include "pic24_all.h"
#include <stdio.h>

/** \file
 *  This file demonstrates use of 'scanf' forms, requires inclusion of "pic24_stdio_uart.c".
 */



#define BUFSIZE 63
char  sz_1[BUFSIZE+1];

int main (void) {
  int32_t i32_i;
  uint32_t ui32_k;
  double d_n;
  float f_n;

  configBasic(HELLO_MSG);
  printf ("This tests 'scanf' stdio; all entries must be terminated by a newline\n");
  printf("Input a string (< 63 characters: ");
  scanf("%s",sz_1);          //get a string from the console
  printf("\n");
  printf("Entered: '%s' \n",sz_1);

  printf("Input an unsigned decimal number: ");
  printf("\n");
  scanf("%lu",&ui32_k);
  printf("The number is: %lu\n",ui32_k);
  printf("Input a signed decimal number: ");
  printf("\n");
  scanf("%ld",&i32_i);
  printf("The number is: %ld\n",i32_i);
#if 1
  printf("Input a floating point number (single precision): ");
  scanf("%f",&f_n);
  printf("The number is: %f\n",(double) f_n);
#endif
#if 1
  printf("Input a floating point number (double precision): ");
  scanf("%lf",&d_n);
  printf("The number is: %lf\n",d_n);
#endif
  printf ("This concludes the 'scanf' stdio test\n");
  while (1) {
    doHeartbeat();
  }
}
