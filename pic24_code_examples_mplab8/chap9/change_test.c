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

/** \file
 *  A simple test of the change notification feature.
 *  A momentary pushbutton is connected to RB13, with
 *  the change notication interrupt for that port is enabled.
 *  A message is printed each time the change notification
 *  interrupt occurs.
*/

volatile uint8_t u8_cFlag = 0;

//Interrupt Service Routine for Change Notification
void _ISRFAST _CNInterrupt (void) {
  _CNIF = 0;    //clear the change notification interrupt bit
  u8_cFlag = 1;
}

/// Switch1 configuration
inline void CONFIG_SW1()  {
  CONFIG_RB13_AS_DIG_INPUT();     //use RB13 for switch input
  ENABLE_RB13_PULLUP();           //enable the pullup
  ENABLE_RB13_CN_INTERRUPT();     //CN13IE = 1
}

int main (void) {
  configBasic(HELLO_MSG);
  /** Configure the switch ***********/
  CONFIG_SW1();  //enables individual CN interrupt also
  DELAY_US(1);   //delay for pullup
  /** Configure Change Notification general interrupt  */
  _CNIF = 0;         //Clear the interrupt flag
  _CNIP = 2;         //Choose a priority
  _CNIE = 1;         //enable the Change Notification general interrupt
  while (1) {
    if (u8_cFlag) {
      DELAY_MS(15);  //wait for switch bounce to settle
      u8_cFlag = 0;
      outString("Change notification interrupt occurred\n");
    }
    doHeartbeat();
  }
}
