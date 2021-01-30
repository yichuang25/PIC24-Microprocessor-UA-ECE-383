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
 *  Demonstrates wakeup from sleep using the UART.
*/

volatile uint8_t u8_sleepFlag;

void _ISR _U1RXInterrupt (void) {
  if (u8_sleepFlag) {
    _U1RXIE = 0;              //disable the interrupt
    _U1RXIF = 0;              //ignore this garbled character
    u8_sleepFlag = 0;
  }

}

int main (void) {
  uint8_t u8_c;

  configBasic(HELLO_MSG);
  _U1RXIP = 1;  //choose a priority
  while (1) {
    outString("Entering Sleep mode. Enter character to wake\n");
    // Finish sending characters before sleeping
    WAIT_UNTIL_TRANSMIT_COMPLETE_UART1();
    _U1RXIF = 0;              //clear the flag
    u8_sleepFlag = 1;
    _U1RXIE = 1;              //enable the interrupt
    U1MODEbits.WAKE = 1;
    SLEEP();         //macro for asm("pwrsav #0")
    DELAY_MS(10);    //delay long enough for wake character to clear uart
    U1MODEbits.UARTEN = 0;  //disable UART and clear internal buffers
    configUART1(DEFAULT_BAUDRATE); //reconfigure it
    outString ("Awake! Now waiting for another character\n");
    u8_c = inChar();       //get  the character, this is garbage...
    printf ("First character is: %c (%x) \n", u8_c,u8_c);
    u8_c = inChar();       //get  the character, this is garbage...
    printf ("Second character is: %c (%x) \n", u8_c,u8_c);
    doHeartbeat();
  }
}
