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
Program uses Timer0 and an external 32.768 kHZ crystal for timekeeping.
*/

volatile uint16_t u16_seconds = 0;

//Interrupt Service Routine for Timer1
void _ISRFAST _T1Interrupt (void) {
  u16_seconds++;
  _T1IF = 0;     //clear the timer interrupt bit
}

void  configTimer1(void) {
  T1CON = T1_OFF | T1_IDLE_CON | T1_GATE_OFF
          | T1_SYNC_EXT_OFF
          | T1_SOURCE_EXT
          | T1_PS_1_1 ;  //
  PR1 = 0x7FFF;                    //period is 1 second
  _T1IF = 0;                       //clear interrupt flag
  _T1IP = 1;                       //choose a priority
  _T1IE = 1;                       //enable the interrupt
  T1CONbits.TON = 1;               //turn on the timer
}

int main(void) {
  __builtin_write_OSCCONL(OSCCON | 0x02);    //    OSCCON.SOSCEN=1;
  configBasic(HELLO_MSG);   //say Hello!
  configTimer1();
  while (1) {
    outString("Seconds: ");
    outUint16Decimal(u16_seconds);
    outString("\n");
    while (!IS_TRANSMIT_COMPLETE_UART1());
    SLEEP();
  }
}
