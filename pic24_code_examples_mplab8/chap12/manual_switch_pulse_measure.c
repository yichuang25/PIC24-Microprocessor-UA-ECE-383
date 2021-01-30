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
 * Measures the pulse width of a pushbutton switch using Timer2 without input capture.
 * CLOCK_CONFIG=PRIPLL_8MHzCrystal_40MHzFCY is defined in the MPLAB project.
 * Remove this macro if you wish to use the internal oscillator.
*/


void  configTimer2(void) {
  T2CON = T2_OFF | T2_IDLE_CON | T2_GATE_OFF
          | T2_32BIT_MODE_OFF
          | T2_SOURCE_INT
          | T2_PS_1_256 ;   //at 40 MHz, approx 420 ms period, 1 tick = 6.4 us
  PR2 = 0xFFFF;                    //maximum period
  TMR2  = 0;                       //clear timer2 value
  _T2IF = 0;                       //clear interrupt flag
  T2CONbits.TON = 1;               //turn on the timer
}

/// Switch1 configuration
inline void CONFIG_SW1()  {
  CONFIG_RB13_AS_DIG_INPUT();     //use RB13 for switch input
  ENABLE_RB13_PULLUP();           //enable the pullup
}
#define SW1              _RB13  //switch state
#define SW1_PRESSED()   SW1==0  //switch test
#define SW1_RELEASED()  SW1==1  //switch test


int main (void) {
  uint16_t u16_start, u16_delta;
  uint32_t u32_pulseWidth;
  configBasic(HELLO_MSG);
  CONFIG_SW1();    //use RB13
  configTimer2();
  while (1) {
    outString("Press button...");
    while (SW1_RELEASED())doHeartbeat();
    u16_start = TMR2;
    while (SW1_PRESSED())doHeartbeat();
    u16_delta = TMR2 - u16_start;  //works because using maximum PR2 value
    u32_pulseWidth = ticksToUs((uint32_t) u16_delta,getTimerPrescale(T2CONbits));
    printf(" %ld us\n",u32_pulseWidth);
  }
}
