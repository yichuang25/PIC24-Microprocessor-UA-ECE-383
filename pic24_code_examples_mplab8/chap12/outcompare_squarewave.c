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
 * Generate a squarewave using output compare (OC1).
 * Define the following clock configuration if you wish higher accuracy.
 * Define CLOCK_CONFIG=PRIPLL_8MHzCrystal_40MHzFCY  in the MPLAB project.
 * Remove this macro if you wish to use the internal oscillator.
 *
 * This example is implemented fundamentally different on the PIC24E/dsPIC24E
*/

#ifndef SQWAVE_HALFPERIOD
#define SQWAVE_HALFPERIOD 2500   // desired half period, in us
#endif

void  configTimer2(void) {
  T2CON = T2_OFF | T2_IDLE_CON | T2_GATE_OFF
          | T2_32BIT_MODE_OFF
          | T2_SOURCE_INT
          | T2_PS_1_64 ;  //1 tick = 1.6 us at FCY=40 MHz
  PR2 = 0xFFFF;    //maximum period
  TMR2  = 0;       //clear timer2 value
}

uint16_t u16_sqwaveHPeriodTicks;
void _ISRFAST _OC1Interrupt() {
  _OC1IF = 0;
#if (defined(__dsPIC33E__) || defined(__PIC24E__))
//nothing to do here since OC1 has internal timer that is reset on OC1RS  match.
#else
  OC1R = OC1R + u16_sqwaveHPeriodTicks;
#endif
}

void configOutputCompare1(void) {
  T2CONbits.TON = 0;       //disable Timer when configuring Output compare

#if (defined(__dsPIC33E__) || defined(__PIC24E__))
  CONFIG_OC1_TO_RP(40);     //map OC1 to RP40/RB8 (RB2 RP pin is input only)
//use OC1R to set pulse high, OC1RS to set pulse low, timer reset on OC1RS match.
//assumes TIMER2 initialized before OC1 so PRE bits are set
  u16_sqwaveHPeriodTicks = usToU16Ticks(SQWAVE_HALFPERIOD, getTimerPrescale(T2CONbits));
  OC1R = u16_sqwaveHPeriodTicks;    //toggles output
  OC1RS = u16_sqwaveHPeriodTicks; //resets timer
  OC1CON1 = OC_TIMER2_SRC |      //Timer2 source
            OC_TOGGLE_PULSE;  //Continuous pulse mode;
  OC1CON2 = 0x001F;              //reset internal timer when OCxRS match occurs
#else
  CONFIG_OC1_TO_RP(2);     //map OC1 to RP2/RB2
  //initialize the compare register to 1/2 the squarewave period
  //assumes TIMER2 initialized before OC1 so PRE bits are set
  u16_sqwaveHPeriodTicks = usToU16Ticks(SQWAVE_HALFPERIOD, getTimerPrescale(T2CONbits));
  OC1R = u16_sqwaveHPeriodTicks;
  OC1CON = OC_TIMER2_SRC |     //Timer2 source
           OC_TOGGLE_PULSE;    //Toggle OC1 every compare event
#endif
  _OC1IF = 0;
  _OC1IP = 1;   //pick a priority
  _OC1IE = 1;   //enable
}

int main (void) {
  configBasic(HELLO_MSG);
  configTimer2();
  configOutputCompare1();
  T2CONbits.TON = 1;       //turn on Timer2 to start sqwave
  while (1) doHeartbeat(); //nothing to do, squarewave generated in hardware
}
