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

/**
Implements a software filter for input pulses less than a specified duration.

*/

#define CONFIG_TOUT() CONFIG_RB9_AS_DIG_OUTPUT()
#define TOUT  _LATB9     //output state

#define TIN_RAW         _RB8     //raw test in
#define CONFIG_TIN()  CONFIG_RB8_AS_DIG_INPUT();


#define ISR_PERIOD        1      // in ms
#define MIN_STABLE       15      // in ms
#define MIN_STABLECOUNT  MIN_STABLE/ISR_PERIOD


uint16_t u16_stableCountTIN = 0;
uint8_t u8_rawTIN = 0;
uint8_t u8_oldrawTIN = 0;


//debounced switch value that is set in the timer ISR
//any variable written by an ISR, and accessed outside of the ISR
//should be declared volatile
volatile uint8_t u8_valueTIN = 0;

//Interrupt Service Routine for Timer3
void _ISRFAST _T3Interrupt (void) {
  u8_rawTIN = TIN_RAW;     //sample the switch
  if (u8_rawTIN != u8_oldrawTIN) {
    //changed values, zero the stability counter
    u16_stableCountTIN = 0;
    u8_oldrawTIN = u8_rawTIN;
  } else {
    u16_stableCountTIN++;
    if (u16_stableCountTIN >= MIN_STABLECOUNT) {
      //new value is ready!
      u8_valueTIN = u8_rawTIN;
    }
  }
  _T3IF = 0;                 //clear the timer interrupt bit
}



void  configTimer3(void) {
  //ensure that Timer2,3 configured as separate timers.
  T2CONbits.T32 = 0;     // 32-bit mode off
  //T3CON set like this for documentation purposes.
  T3CON = T3_OFF |T3_IDLE_CON | T3_GATE_OFF
          | T3_SOURCE_INT
          | T3_PS_1_1 ;
  PR3 = msToU16Ticks (ISR_PERIOD, getTimerPrescale(T3CONbits)) - 1;
  TMR3  = 0;                       //clear timer3 value
  _T3IF = 0;                       //clear interrupt flag
  _T3IP = 1;                       //choose a priority
  _T3IE = 1;                       //enable the interrupt
  T3CONbits.TON = 1;               //turn on the timer
}


uint8_t u8_oldvalueTIN = 0;

#define TPW  20     // in ms, pulsewidth of TOUT

int main (void) {
  configBasic(HELLO_MSG);
  TOUT = 0;
  // TOUT drives TIN
  CONFIG_TIN();
  CONFIG_TOUT();
  configTimer3();
  while (1) {
    TOUT = !TOUT;
    DELAY_MS(TPW);
    if (u8_valueTIN != u8_oldvalueTIN) {
      u8_oldvalueTIN = u8_valueTIN;
      outString("*");
    }
  }
}
