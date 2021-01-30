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
Demonstrates a PWM DAC - connect an RC filter on the OC1
output and vary the pulse width of the PWM signal, and monitor
the DC value on the capacitor. The RC time constant should
be at least 10x greater than the PWM period. Example values
used for testing were R=6.8k, C = 1.0u, PWM period= 500 us.
Measured ripple was 80 mv, time for the DAC to change voltage
from 1.0 V to 3.0 V and vice versa was ~ 30 ms (about 60 PWM periods)
For more accuracy, use an external crystal and define
CLOCK_CONFIG=PRIPLL_8MHzCrystal_40MHzFCY in the MPLAB project.
Remove this macro if you wish to use the internal oscillator.
*/

#ifndef PWM_PERIOD
#define PWM_PERIOD 500  // desired period, in us
#endif

void  configTimer2(void) {
  T2CON = T2_OFF | T2_IDLE_CON | T2_GATE_OFF
          | T2_32BIT_MODE_OFF
          | T2_SOURCE_INT
          | T2_PS_1_64 ;  //1 tick = 1.6 us at FCY=40 MHz
  PR2 = usToU16Ticks(PWM_PERIOD, getTimerPrescale(T2CONbits)) - 1;
  TMR2  = 0;       //clear timer2 value
  _T2IF = 0;
  _T2IP = 1;
  _T2IE = 1;    //enable the Timer2 interrupt
}

void configOutputCompare1(void) {
  T2CONbits.TON = 0;       //disable Timer when configuring Output compare
  OC1R = 0;
  OC1RS = 0;  //initially off
#if (defined(__dsPIC33E__) || defined(__PIC24E__))
  CONFIG_OC1_TO_RP(35);        //map OC1 to RP35/RB3
//turn on the compare toggle mode using Timer2
  OC1CON1 = OC_TIMER2_SRC |     //Timer2 source
            OC_PWM_CENTER_ALIGN;  //PWM
  OC1CON2 = 0x000C;           //sync source is Timer2.
#else
  CONFIG_RB3_AS_DIG_OUTPUT();
  CONFIG_OC1_TO_RP(3);        //map OC1 to RP3/RB3
//assumes TIMER2 initialized before OC1 so PRE bits are set

//turn on the compare toggle mode using Timer2
  OC1CON = OC_TIMER2_SRC |     //Timer2 source
           OC_PWM_FAULT_PIN_DISABLE;  //PWM, no fault detection
#endif
}

volatile uint8_t u8_updateFlag = 0;
volatile uint16_t u16_newOC1RS;

void _ISR _T2Interrupt(void) {
  _T2IF = 0;    //clear the timer interrupt bit
  if (u8_updateFlag) {
    OC1RS =  u16_newOC1RS;  //update pulse width value
    u8_updateFlag = 0;
    _LATB9 = 1;
    DELAY_US(1);
    _LATB9 = 0;
  }
}

char sz1[32];

int main(void) {
  uint16_t u16_mv;
  float f_tmp;
  configBasic(HELLO_MSG);
  CONFIG_RB9_AS_DIG_OUTPUT();
  _LATB9 = 0;
  configTimer2();
  configOutputCompare1();
  T2CONbits.TON = 1;       //turn on Timer2 to start PWM
  while (1) {
    outString("Input voltage 0 to 3300 (mv): \n");
    inStringEcho(sz1,30);
    sscanf(sz1,"%d",(int *) &u16_mv);
    f_tmp = u16_mv;
    f_tmp = f_tmp/3300 * PR2;
    u16_newOC1RS = f_tmp;
    u8_updateFlag = 1;
  }
}
