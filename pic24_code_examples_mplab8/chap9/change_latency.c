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
Measure latency using change notification.
RB2 drives RB3 to generate an CN interrupt.
Need to run this with a slow clock to avoid
external loading effecting CN triggering point.
With FCY = 4 MHz, got 8 FYCs high period, 8 FYCs low period.
*/

//Interrupt Service Routine for Change Notification
void _ISRFAST _CNInterrupt (void) {
  _LATB2 = 0;   //set back low
  NOP();        //give the CN time to propagate so
  NOP();        //clearing the flag will actually clear it.
  NOP();
  _CNIF = 0;    //clear the change notification interrupt bit
}

int main (void) {
  configBasic(HELLO_MSG);
  /** PIO ***********/
  CONFIG_RB2_AS_DIG_OUTPUT();
  _LATB2 = 0;
  CONFIG_RB3_AS_DIG_INPUT();
  ENABLE_RB3_CN_INTERRUPT();
  /** Configure Change Notification general interrupt  */
  _CNIF = 0;         //Clear the interrupt flag
  _CNIP = 2;         //Choose a priority
  _CNIE = 1;         //enable the Change Notification general interrupt
  while (1) {
    _LATB2 = 1;       //trigger interrupt by bringing high
    NOP();            //give the CN time to propagate
    NOP();
  }
}
