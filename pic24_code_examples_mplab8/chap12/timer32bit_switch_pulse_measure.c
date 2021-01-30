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
 * Measures the pulse width of pushbutton switch using Timer2/3 in 32-bit mode with INT1 for edge detection.
 * For more accuracy,  use an external crystal and define
 * CLOCK_CONFIG=PRIPLL_8MHzCrystal_40MHzFCY in your MPLAB project.
 * Typical crystal accuracy for through hole is +/-20 pmm, so for a 100000 us
 * pulse width measurement this is +/- 2 us.
*/

typedef enum  {
  STATE_WAIT_FOR_FALL_EDGE = 0,
  STATE_WAIT_FOR_RISE_EDGE,
} INT1STATE;

INT1STATE e_isrINT1State = STATE_WAIT_FOR_FALL_EDGE;
volatile uint8_t u8_captureFlag = 0;
volatile union32 u32_lastCapture; //union32 declared in stdint.h
volatile union32 u32_thisCapture;
volatile int32_t u32_delta;
volatile int32_t u32_pulseWidth;

//Interrupt Service Routine for INT1
void _ISRFAST _INT1Interrupt (void) {
  _INT1IF = 0;    //clear the interrupt bit
  switch (e_isrINT1State) {
    case STATE_WAIT_FOR_FALL_EDGE:
      if (u8_captureFlag == 0) {
        u32_lastCapture.u16.ls16 = TMR2;
        u32_lastCapture.u16.ms16 = TMR3HLD;
        _INT1EP = 0;  //configure for rising edge
        e_isrINT1State = STATE_WAIT_FOR_RISE_EDGE;
      }
      break;
    case STATE_WAIT_FOR_RISE_EDGE:
      u32_thisCapture.u16.ls16 = TMR2;
      u32_thisCapture.u16.ms16 = TMR3HLD;
      u32_delta = u32_thisCapture.u32 - u32_lastCapture.u32;
      u32_pulseWidth = ticksToUs(u32_delta, getTimerPrescale(T2CONbits));
      u8_captureFlag = 1;
      _INT1EP = 1;     //configure for falling edge
      e_isrINT1State = STATE_WAIT_FOR_FALL_EDGE;
      break;
    default:
      e_isrINT1State= STATE_WAIT_FOR_FALL_EDGE;
  }
}

/// Switch1 configuration, use RB13
inline void CONFIG_SW1()  {
  CONFIG_RB13_AS_DIG_INPUT();   //use RB13 for switch input
  ENABLE_RB13_PULLUP();         //enable the pullup
#if (defined(__dsPIC33E__) || defined(__PIC24E__))
  CONFIG_INT1_TO_RP(45);   //map INT1 to RP45/RB13
#else
  CONFIG_INT1_TO_RP(13);   //map INT1 to RP13/RB13
#endif
  DELAY_US(1);             //Wait for pullup
  /** Configure INT1 interrupt  */
  _INT1IF = 0;     //Clear the interrupt flag
  _INT1IP = 1;     //Choose a priority
  _INT1EP = 1;     //negative edge triggerred
  _INT1IE = 1;     //enable INT1 interrupt
}

//Timer2/3 used as single 32-bit timer, control word of Timer2 controls timer,
//interrupt status of Timer3 used for the combined timer
void  configTimer23(void) {
  T2CON = T2_OFF | T2_IDLE_CON | T2_GATE_OFF
          | T2_32BIT_MODE_ON
          | T2_SOURCE_INT
          | T2_PS_1_1 ;
  PR2 = 0xFFFF;                    //maximum period
  PR3 = 0xFFFF;                    //maximum period
  TMR3HLD = 0;                     //write MSW first
  TMR2  = 0;                       //then LSW
  _T3IF = 0;                       //clear interrupt flag
  T2CONbits.TON = 1;               //turn on the timer
}


int main (void) {
  configBasic(HELLO_MSG);
  CONFIG_SW1();    //use RB13
  configTimer23();
  while (1) {
    outString("Press button...");
    while (!u8_captureFlag) doHeartbeat();
    printf(" %ld us\n",u32_pulseWidth);
    u8_captureFlag = 0;
  }
}

