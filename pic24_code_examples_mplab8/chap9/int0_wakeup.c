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
 *  Demonstrates the use of int0 interrupt
 *  to wake from Sleep mode.
 *  Pushbutton on an int0 pin is used to wake from Sleep mode.
 *
 *
 */

/// Interrupt Service Routine for INT0
void _ISRFAST _INT0Interrupt (void) {
  _INT0IF = 0;    //clear the interrupt bit
}

/// Switch1 configuration, RB7 shares INT0 pin on PIC24HGP202
inline void CONFIG_SW1()  {
  CONFIG_RB7_AS_DIG_INPUT();     //use RB7 for switch input
  ENABLE_RB7_PULLUP();           //enable the pullup
}

int main (void) {
  configBasic(HELLO_MSG);
  /** Configure the switch ***********/
  CONFIG_SW1();    //enables individual CN interrupt also
  /** Configure INT0 interrupt  */
  _INT0IF = 0;     //Clear the interrupt flag
  _INT0IP = 2;     //Choose a priority
  _INT0EP = 1;     //negative edge triggerred
  _INT0IE = 1;     //enable INT0 interrupt
  while (1) {
    outString("Entering Sleep mode, press button to wake.\n");
    // Finish sending characters before sleeping
    WAIT_UNTIL_TRANSMIT_COMPLETE_UART1();
    SLEEP();        //macro for asm("pwrsav #0")
  }
}
