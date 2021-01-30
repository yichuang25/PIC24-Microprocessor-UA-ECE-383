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
 *  Demonstrates use of a periodic interrupt to flash an LED.
 *  Timer3 is configured for a 300 ms interrupt to flash an
 *  LED on RB14.
*/

/// LED1
#define CONFIG_LED1() CONFIG_RB14_AS_DIG_OUTPUT()
#define LED1  _LATB14     //led1 state

//Interrupt Service Routine for Timer3
void _ISRFAST _T3Interrupt (void) {
  LED1 = !LED1; //toggle the LED
  _T3IF = 0;    //clear the timer interrupt bit
}

#if (defined(__dsPIC33E__) || defined(__PIC24E__))
//for these families, need shorter period because of potentially faster clock
//which will overflow 16-bit timer
#define ISR_PERIOD     150      // in ms
#else
#define ISR_PERIOD     300      // in ms
#endif

void  configTimer3(void) {
  //ensure that Timer2,3 configured as separate timers.
  T2CONbits.T32 = 0;     // 32-bit mode off
  //T3CON set like this for documentation purposes.
  //could be replaced by T3CON = 0x0030
  T3CON = T3_OFF | T3_IDLE_CON | T3_GATE_OFF
          | T3_SOURCE_INT
          | T3_PS_1_256 ;  //results in T3CON= 0x0030
  PR3 = msToU16Ticks (ISR_PERIOD, getTimerPrescale(T3CONbits)) - 1;
  TMR3  = 0;                       //clear timer3 value
  _T3IF = 0;                       //clear interrupt flag
  _T3IP = 1;                       //choose a priority
  _T3IE = 1;                       //enable the interrupt
  T3CONbits.TON = 1;               //turn on the timer
}

int main (void) {
  configBasic(HELLO_MSG);
  /** GPIO config ***************************/
  CONFIG_LED1();       //config the LED
  LED1 = 1;
  /** Config the Timer, use Timer3***********/
  configTimer3();
  while (1) {
    //enter idle mode while waiting for timer to go off
    //Timer interrupt will wake us from idle mode
    IDLE();  //macro for  __asm__ volatile ("pwrsav #1")
  }
  // End program
  return 0;
}
