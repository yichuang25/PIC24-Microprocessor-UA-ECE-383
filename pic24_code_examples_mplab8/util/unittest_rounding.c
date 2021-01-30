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

#define NUM_TESTS 10

int main (void) {
  float f_x[] = {0.0,10.1, 10.5, 100.99, 0.000001, 0.99999999, 10.49999, 10.500001, 1.3, 1.8};
  uint16_t u16_y;
  uint32_t u32_y;
  uint8_t u8_i;

  configBasic(HELLO_MSG);

  for (u8_i=0; u8_i< NUM_TESTS; u8_i++) {
    u32_y = roundFloatToUint32(f_x[u8_i]);
    u16_y = roundFloatToUint16(f_x[u8_i]);
    printf("f: %3.9f  u16: %d, u32: %ld\n",
           (double) f_x[u8_i], u16_y, u32_y);
  }

  while (1) {
    doHeartbeat(); //ensure that we are alive
  } // end while (1)

}
