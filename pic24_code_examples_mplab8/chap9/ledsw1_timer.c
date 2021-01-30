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
Demonstrates the use of a period interrupt to sample a switch
input, removes the need for debounce delays.
Uses a semaphore for press & release.
*/

/// LED1
#define CONFIG_LED1() CONFIG_RB14_AS_DIG_OUTPUT()
#define LED1  _LATB14     //led1 state

/// Switch1 configuration
inline void CONFIG_SW1()  {
  CONFIG_RB13_AS_DIG_INPUT();     //use RB13 for switch input
  ENABLE_RB13_PULLUP();           //enable the pullup
  DELAY_US(1);                    // Wait for pullup
}

#define SW1_RAW         _RB13             //raw switch value
#define SW1             u8_valueSW1       //switch state
#define SW1_PRESSED()   (SW1==0)          //switch test
#define SW1_RELEASED()  (SW1==1)          //switch test

typedef enum  {
  STATE_WAIT_FOR_PRESS = 0,
  STATE_WAIT_FOR_RELEASE,
} ISRSTATE;

volatile uint8_t u8_valueSW1  = 1;    //initially high
volatile uint8_t u8_pnrSW1 = 0;       ///< True when SW1 has been Pressed-N-Released

//Interrupt Service Routine for Timer3
void _ISRFAST _T3Interrupt (void) {
  static ISRSTATE e_isrState = STATE_WAIT_FOR_PRESS;
  u8_valueSW1 = SW1_RAW;     //sample the switch
  switch (e_isrState) {
    case STATE_WAIT_FOR_PRESS:
      if (SW1_PRESSED() && (u8_pnrSW1 == 0))
        e_isrState = STATE_WAIT_FOR_RELEASE;
      break;
    case STATE_WAIT_FOR_RELEASE:
      if (SW1_RELEASED()) {
        e_isrState = STATE_WAIT_FOR_PRESS;
        u8_pnrSW1 = 1; //set press & release semaphore
        break;
      }
    default:
      e_isrState = STATE_WAIT_FOR_RELEASE;
  }
  _T3IF = 0;                 //clear the timer interrupt bit
}


/// Switch2 configuration, does not have to be debounced
inline void CONFIG_SW2()  {
  CONFIG_RB12_AS_DIG_INPUT();     //use RB12 for switch input
  ENABLE_RB12_PULLUP();           //enable the pullup
  DELAY_US(1);                    //wait for pullup to drive pin high
}

#define SW2              _RB12  //switch state


typedef enum  {
  STATE_RESET = 0,
  STATE_WAIT_FOR_PNR1,
  STATE_WAIT_FOR_PNR2,
  STATE_BLINK,
  STATE_WAIT_FOR_RELEASE3
} STATE;

//print debug message for state when it changes
void printNewState (STATE e_currentState) {
  static STATE e_LastState = STATE_RESET;
  if (e_LastState != e_currentState) {
    switch (e_currentState) {
      case STATE_WAIT_FOR_PNR1:
        outString("STATE_WAIT_FOR_PNR1 - LED is off\n");
        break;
      case STATE_WAIT_FOR_PNR2:
        outString("STATE_WAIT_FOR_PNR2 - SW2 on goes to blink else go to PNR1\n");
        break;
      case STATE_BLINK:
        outString("STATE_BLINK - LED blinks, waiting for SW1 press\n");
        break;
      case STATE_WAIT_FOR_RELEASE3:
        outString("STATE_WAIT_FOR_RELEASE3 - LED is on\n");
        break;
      default:
        break;
    }
  }
  e_LastState = e_currentState;  //remember last state
}


#define ISR_PERIOD     15                // in ms
void  configTimer3(void) {
  //ensure that Timer2,3 configured as separate timers.
  T2CONbits.T32 = 0;     // 32-bit mode off
  //T3CON set like this for documentation purposes.
  //could be replaced by T3CON = 0x0020
  T3CON = T3_OFF |T3_IDLE_CON | T3_GATE_OFF
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
  STATE e_mystate;
  configBasic(HELLO_MSG);

  /** PIO config ******/
  CONFIG_SW1();        //configure switch
  CONFIG_SW2();        //configure switch
  CONFIG_LED1();       //config the LED
  /** Configure the Timer */
  configTimer3();
  /*****Toggle LED each time switch is pressed and released ******************************/
  e_mystate = STATE_WAIT_FOR_PNR1;

  /* Observe that use of a periodic interrupt for sampling the
  switch value means that all of the debounce delays can be removed
  */
  while (1) {
    printNewState(e_mystate);  //debug message when state changes
    switch (e_mystate) {
      case STATE_WAIT_FOR_PNR1:
        LED1 = 0; //turn off the LED
        if (u8_pnrSW1) {
          u8_pnrSW1 = 0; //clear semaphore
          e_mystate = STATE_WAIT_FOR_PNR2;
        }
        break;
      case STATE_WAIT_FOR_PNR2:
        LED1 = 1; //turn on the LED
        if (u8_pnrSW1) {
          u8_pnrSW1 = 0; //clear semaphore
          //decide where to go
          if (SW2) e_mystate = STATE_BLINK;
          else e_mystate = STATE_WAIT_FOR_PNR1;
        }
        break;
      case STATE_BLINK:
        LED1 = !LED1;    //blink while not pressed
        DELAY_MS(100);    //blink delay
        if (SW1_PRESSED()) e_mystate = STATE_WAIT_FOR_RELEASE3;
        break;
      case STATE_WAIT_FOR_RELEASE3:
        LED1 = 1;   //Freeze LED1 at 1
        //use u8_pnrSW1 instead of SW1_RELEASED because
        //u8_pnrSW1 is set on a release, and must be cleared.
        if (u8_pnrSW1) {
          u8_pnrSW1 = 0;
          e_mystate = STATE_WAIT_FOR_PNR1;
        }
        break;
      default:
        e_mystate = STATE_WAIT_FOR_PNR1;
    }//end switch(e_mystate)
    doHeartbeat();     //ensure that we are alive
  } // end while (1)
}
