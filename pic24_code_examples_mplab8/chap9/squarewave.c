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
*  Generates a square wave using timer 2 and
*  an ISR.
*/


#define WAVEOUT _LATB2           //state
inline void CONFIG_WAVEOUT() {
  CONFIG_RB2_AS_DIG_OUTPUT();     //use RB2 for output
}

//Interrupt Service Routine for Timer2
void _ISRFAST _T2Interrupt (void) {
  WAVEOUT = !WAVEOUT;        //toggle output
  _T2IF = 0;                 //clear the timer interrupt bit
}

#define ISR_PERIOD  15    // in ms
void  configTimer2(void) {
  //T2CON set like this for documentation purposes.
  //could be replaced by T2CON = 0x0020
  T2CON = T2_OFF | T2_IDLE_CON | T2_GATE_OFF
          | T2_32BIT_MODE_OFF
          | T2_SOURCE_INT
          | T2_PS_1_64 ;  //results in T2CON = 0x0020
  //subtract 1 from ticks value assigned to PR2 because period is PRx + 1
  PR2 = msToU16Ticks (ISR_PERIOD, getTimerPrescale(T2CONbits)) - 1;
  TMR2  = 0;                       //clear timer2 value
  _T2IF = 0;                       //clear interrupt flag
  _T2IP = 1;                       //choose a priority
  _T2IE = 1;                       //enable the interrupt
  T2CONbits.TON = 1;               //turn on the timer
}

int main (void) {
  configBasic(HELLO_MSG);
  CONFIG_WAVEOUT();   //PIO Config
  configTimer2();      //TMR2 config
  //interrupt does work of generating the square wave
  while (1) {
    doHeartbeat(); //ensure that we are alive
  } // end while (1)

}
