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
Demonstrates the use of semaphore variable that is
set in an interrupt service routine and read by
the main loop. In this case, the semaphore indicates
that a button press & release IO event has been received.
The main loop toggles an LED when the semaphore
is set.
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

#define SW1             _RB13               //switch value
#define SW1_PRESSED()   (SW1==0)            //switch test
#define SW1_RELEASED()  (SW1==1)            //switch test

//semaphore variable
volatile uint8_t u8_pressAndRelease = 0;    //initially cleared

typedef enum  {
  STATE_RESET = 0,
  STATE_WAIT_FOR_PRESS,
  STATE_WAIT_FOR_RELEASE
} STATE;

volatile STATE e_mystate = STATE_RESET;

//Interrupt Service Routine for Timer3
void _ISRFAST _T3Interrupt (void) {
  if (!u8_pressAndRelease) {
    //semaphore is cleared, watch for another press & release
    switch (e_mystate) {
      case STATE_WAIT_FOR_PRESS:
        if (SW1_PRESSED()) {
          e_mystate = STATE_WAIT_FOR_RELEASE;
        }
        break;
      case STATE_WAIT_FOR_RELEASE:
        if (SW1_RELEASED()) {
          //have received a complete Press & Release.
          //Set the semaphore
          u8_pressAndRelease = 1;
          e_mystate = STATE_WAIT_FOR_PRESS;
        }
        break;
      default:
        e_mystate = STATE_WAIT_FOR_PRESS;
    }
  }

  _T3IF = 0;                 //clear the timer interrupt bit
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
  configBasic(HELLO_MSG);
  /** GPIO config ***************************/
  CONFIG_SW1();        //configure switch
  CONFIG_LED1();       //config the LED
  /* Timer configuration */
  configTimer3();      //use Timer3 to periodically sample the switch input

  /*****Toggle LED each time switch is pressed and released ******/

  while (1) {
    //wait for press & release
    if (!u8_pressAndRelease) {
      doHeartbeat();     //ensure that we are alive
    } else {
      //semaphore received (IO event has occurred!)
      //Toggle the LED, then clear the semaphore
      LED1 = !LED1;
      u8_pressAndRelease = 0;
    }
  }// end while (1)

  // End program
  return 0;
}
