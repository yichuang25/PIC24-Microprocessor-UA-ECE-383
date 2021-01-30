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
A 2-bit incremental Gray code rotary encoder example
*/

//clips cntr between 0 and max
uint8_t processRotaryData(volatile uint8_t u8_curr, volatile uint8_t u8_last,
                          volatile uint8_t *cntr, volatile uint8_t max) {
  int8_t delta = 0;
  //states listed in Gray code order for clarity
  switch (u8_curr) {
    case 0:
      if (u8_last == 1) delta = 1;
      else if (u8_last == 2) delta = -1;
      break;
    case 1:
      if (u8_last == 3) delta = 1;
      else if (u8_last == 0) delta = -1;
      break;
    case 3:
      if (u8_last == 2) delta = 1;
      else if (u8_last == 1) delta = -1;
      break;
    case 2:
      if (u8_last == 0) delta = 1;
      else if (u8_last == 3) delta = -1;
      break;
    default:
      break;
  }
  if (delta == 0) return(1); //error, illegal state
  //clip and update.
  if (( *cntr == 0 && delta == -1)
      || (*cntr == max && delta == 1)) return 0; //at limit
  (*cntr) = (*cntr) + delta;
  return 0;
}

#define ROT1_RAW _RB13
#define ROT0_RAW _RB12
#define GET_ROT_STATE() ((ROT1_RAW << 1) | ROT0_RAW)

/// ROT1 configuration
inline void configRotaryEncoder() {
  CONFIG_RB13_AS_DIG_INPUT();
  ENABLE_RB13_PULLUP();           //enable the pullup
  CONFIG_RB12_AS_DIG_INPUT();
  ENABLE_RB12_PULLUP();           //enable the pullup
  DELAY_US(1);                   //wait for pullups to settle
}

#define ROT_MAX  32            //arbitrary limit

volatile uint8_t u8_valueROT = 0;
volatile uint8_t u8_lastvalueROT = 0;
volatile uint8_t u8_errROT = 0;
volatile uint8_t u8_cntrROT = 0;

//Interrupt Service Routine for Timer3
void _ISRFAST _T3Interrupt (void) {
  u8_valueROT = GET_ROT_STATE(); //a value between 0 & 3
  if (u8_lastvalueROT != u8_valueROT) {
    u8_errROT = processRotaryData(u8_valueROT, u8_lastvalueROT, &u8_cntrROT, ROT_MAX);
    u8_lastvalueROT = u8_valueROT;
  }
  _T3IF = 0;                 //clear the timer interrupt bit
}


#define ISR_PERIOD     15                // in ms
void  configTimer3(void) {
  //ensure that Timer2,3 configured as separate timers.
  T2CONbits.T32 = 0;     // 32-bit mode off
  //T3CON set like this for documentation purposes.
  //could be replaced by T3CON = 0x0020
  T3CON = T3_OFF | T3_IDLE_CON | T3_GATE_OFF
          | T3_SOURCE_INT
          | T3_PS_1_64 ;  //results in T3CON= 0x0020
  PR3 = msToU16Ticks (ISR_PERIOD, getTimerPrescale(T3CONbits)) - 1;
  TMR3  = 0;                       //clear timer3 value
  _T3IF = 0;                       //clear interrupt flag
  _T3IP = 1;                       //choose a priority
  _T3IE = 1;                       //enable the interrupt
  T3CONbits.TON = 1;               //turn on the timer
}

int main (void) {
  uint8_t u8_lastCnt;
  configBasic(HELLO_MSG);
  /** PIO config ******/
  configRotaryEncoder();
  u8_valueROT = GET_ROT_STATE();
  u8_lastvalueROT = u8_valueROT;
  u8_lastCnt = u8_cntrROT;
  /** Configure the Timer */
  configTimer3();
  while (1) {
    if (u8_lastCnt != u8_cntrROT) {
      u8_lastCnt = u8_cntrROT;
      outUint8(u8_lastCnt);
      outString("\n");
      if  (u8_errROT) {
        outString("Rotary state error\n");
        u8_errROT = 0;
      }
    }
    doHeartbeat();     //indicate that we are alive
  } // end while (1)
}
