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
 * Measures the square wave frequency using input capture and Timer2
 * This projects uses an external crystal for accuracy.
 * CLOCK_CONFIG=PRIPLL_8MHzCrystal_40MHzFCY is defined in the MPLAB project.
 * Remove this macro if you wish to use the internal oscillator.
*/

uint8_t getPeriodAdjust (uint8_t ICMbits) {
  if (ICMbits == IC_EVERY_16_RISE_EDGE) return 16;
  else if (ICMbits == IC_EVERY_4_RISE_EDGE) return 4;
  else return 1;
}

volatile uint8_t u8_captureFlag = 0;
volatile uint16_t u16_lastCapture;
volatile uint16_t u16_thisCapture;
volatile uint32_t u32_period;

void _ISRFAST _IC1Interrupt() {
  _IC1IF = 0;
  u16_thisCapture = IC1BUF;  //always read the buffer to prevent overflow
  if (u8_captureFlag == 0) {
    u32_period = (uint32_t) computeDeltaTicks(u16_lastCapture,u16_thisCapture,PR2);
    u32_period = ticksToNs (u32_period, getTimerPrescale(T2CONbits));
    //adjust period if necessary
#if (defined(__dsPIC33E__) || defined(__PIC24E__))
    u32_period = u32_period/getPeriodAdjust(IC1CON1bits.ICM);
#else
    u32_period = u32_period/getPeriodAdjust(IC1CONbits.ICM);
#endif
    u8_captureFlag = 1;
  }
  u16_lastCapture = u16_thisCapture;
}

void configInputCapture1(void) {
  CONFIG_RB13_AS_DIG_INPUT();
#if (defined(__dsPIC33E__) || defined(__PIC24E__))
  CONFIG_IC1_TO_RP(45);        //map IC1 to RP45/RB13
  IC1CON1 = IC_TIMER2_SRC |     //Timer2 source
            IC_INT_1CAPTURE |   //Interrupt every capture
            IC_EVERY_EDGE;      //Capture every edge
  IC1CON2 = 0x000C;            //sync to timer2
#else
  CONFIG_IC1_TO_RP(13);        //map IC1 to RP13/RB13
  IC1CON = IC_TIMER2_SRC |     //Timer2 source
           IC_INT_1CAPTURE |        //Interrupt every capture
           IC_EVERY_16_RISE_EDGE;   //capture every 16th edge
#endif
  _IC1IF = 0;
  _IC1IP = 1;   //pick a priority
  _IC1IE = 1;   //enable
}

void  configTimer2(void) {
  T2CON = T2_OFF | T2_IDLE_CON | T2_GATE_OFF
          | T2_32BIT_MODE_OFF
          | T2_SOURCE_INT
          | T2_PS_1_64 ;  //1 tick = 1.6 us at FCY=40 MHz
  PR2 = 0xFFFF;                    //maximum period
  TMR2  = 0;                       //clear timer2 value
  _T2IF = 0;                       //clear interrupt flag
  T2CONbits.TON = 1;               //turn on the timer
}

int main (void) {
  uint32_t u32_maxPeriodNs;
  configBasic(HELLO_MSG);
  configTimer2();
  configInputCapture1();
#if (defined(__dsPIC33E__) || defined(__PIC24E__))
  u32_maxPeriodNs = ticksToNs (65536, getTimerPrescale(T2CONbits))/getPeriodAdjust(IC1CON1bits.ICM);
#else
  u32_maxPeriodNs = ticksToNs (65536, getTimerPrescale(T2CONbits))/getPeriodAdjust(IC1CONbits.ICM);
#endif
  printf("Maximum period is %ld ns\n",u32_maxPeriodNs);
  while (1) {
    outString("Press button...");
    while (!u8_captureFlag) doHeartbeat();
    printf(" %ld ns\n",u32_period);
    u8_captureFlag = 0;
  }
}
