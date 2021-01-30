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
Illustrates a simple usage of the comparator module
*/

void configComparator(void) {
  CMCON = CMP_IDLE_STOP | CMP1_ENABLE | CMP2_DISABLE |
          CMP1_OUTPUT_DISABLE | CMP1_NORMAL_OUTPUT |
          CMP1_NEG_IP_VIN_POS | CMP1_POS_IP_CV_REF;
  CVRCON = CMP_VREF_ENABLE |
           CMP_VREF_OUTPUT_DISABLE |
           CMP_VRSRC_AVDD_AVSS |
           CMP_0P50_CVRR_1 ;
  DELAY_US(10) //wait for comparator to settle
  _C1EVT = 0; //clear C1 event flag
  _CMIF = 0;  //clear interrupt flag
}

int main (void) {
  uint8_t u8_i;
  configBasic(HELLO_MSG);
  configComparator();
  u8_i = _C1OUT;  //intial read to set trigger
  while (1) {
    do {
      doHeartbeat(); //wait for trigger
    } while (!_CMIF);
    _CMIF = 0;
    u8_i = _C1OUT;  //current value
    outString("\n Comparator fired: ");
    outUint8(u8_i);
    DELAY_MS(100);
  } //end while
}//end main
