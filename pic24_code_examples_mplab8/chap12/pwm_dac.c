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
be at least 10x greater than the PWM period. Examples values
used for testing were R=6.8k, C = 1.0u, PWM period= 500 us
For better accuracy, use an external crystal and define
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
          | T2_PS_1_8 ;  //1 tick = 1.6 us at FCY=40 MHz
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
//assumes TIMER2 initialized before OC1 so PRE bits are set
#if (defined(__dsPIC33E__) || defined(__PIC24E__))
  CONFIG_OC1_TO_RP(35);        //map OC1 to RP35/RB3
//turn on the compare toggle mode using Timer2
  OC1CON1 = OC_TIMER2_SRC |     //Timer2 source
            OC_PWM_CENTER_ALIGN;  //PWM
  OC1CON2 = 0x000C;           //sync source is Timer2.
#else
  CONFIG_RB3_AS_DIG_OUTPUT();
  CONFIG_OC1_TO_RP(3);        //map OC1 to RP3/RB3
//turn on the compare toggle mode using Timer2
  OC1CON = OC_TIMER2_SRC |     //Timer2 source
           OC_PWM_FAULT_PIN_DISABLE;  //PWM, no fault detection
#endif
}

void _ISR _T2Interrupt(void) {
  uint32_t u32_temp;
  _T2IF = 0;    //clear the timer interrupt bit
  //update the PWM duty cycle from the ADC value
  u32_temp = ADC1BUF0;  //use 32-bit value for range
  //compute new pulse width that is 0 to 99% of PR2
  // pulse width (PR2) * ADC/1024
  u32_temp = (u32_temp * (PR2))>> 10 ;  // >>10 is same as divide/1024
  OC1RS = u32_temp;  //update pulse width value
  AD1CON1bits.SAMP = 1; //start next ADC conversion for next interrupt
}

int main(void) {
  uint16_t u16_oc1rs;
  uint32_t u32_pw;
  float f_dacV;
  configBasic(HELLO_MSG);
  configTimer2();
  configOutputCompare1();
  CONFIG_AN0_AS_ANALOG();
  configADC1_ManualCH0( ADC_CH0_POS_SAMPLEA_AN0, 31, 0 );
  SET_SAMP_BIT_ADC1();      //start sampling and conversion
  T2CONbits.TON = 1;       //turn on Timer2 to start PWM
  while (1) {
    u16_oc1rs = OC1RS;
    u32_pw= ticksToUs(u16_oc1rs, getTimerPrescale(T2CONbits));
    f_dacV = u16_oc1rs;
    f_dacV = f_dacV * 3.3/(PR2+1);
#ifdef SMALLRAM
    {
      uint16_t ui16_dacValmv;
      ui16_dacValmv = f_dacV * 1000;
      printf("PWM PW (us): %ld, PWM DAC voltage: %d\n",u32_pw, ui16_dacValmv);
    }
#else
    printf("PWM PW (us): %ld, PWM DAC voltage: %4.2f\n",u32_pw, (double) f_dacV);
#endif
    DELAY_MS(100);
  }
}
