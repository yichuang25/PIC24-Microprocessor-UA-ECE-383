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
A program that uses a finite state machine approach for
toggling an LED whenever a pushbutton switch is pressed
and released. Demonstrates the use of debounce delays when
polling a switch input.
*/

/// LED1
#define CONFIG_LED1() CONFIG_RB14_AS_DIG_OUTPUT()
#define LED1  _LATB14     //led1 state

/// Switch1 configuration
inline void CONFIG_SW1()  {
  CONFIG_RB13_AS_DIG_INPUT();     //use RB10 for switch input
  ENABLE_RB13_PULLUP();           //enable the pullup
}


#define SW1              _RB13       //switch state
#define SW1_PRESSED()   (SW1==0)  //switch test
#define SW1_RELEASED()  (SW1==1)  //switch test

typedef enum  {
  STATE_RESET = 0,
  STATE_WAIT_FOR_PRESS,
  STATE_WAIT_FOR_RELEASE
} STATE;

STATE e_lastState = STATE_RESET;
//print debug message for state when it changes
void printNewState (STATE e_currentState) {
  if (e_lastState != e_currentState) {
    switch (e_currentState) {
      case STATE_WAIT_FOR_PRESS:
        outString("STATE_WAIT_FOR_PRESS\n");
        break;
      case STATE_WAIT_FOR_RELEASE:
        outString("STATE_WAIT_FOR_RELEASE\n");
        break;
      default:
        outString("Unexpected state\n");
        break;
    }
  }
  e_lastState = e_currentState;  //remember last state
}

int main (void) {
  STATE e_mystate;

  configBasic(HELLO_MSG);      // Set up heartbeat, UART, print hello message and diags
  /** GPIO config ***************************/
  CONFIG_SW1();        //configure switch
  CONFIG_LED1();       //config the LED
  DELAY_US(1);         //give pullups a little time
  /*****Toggle LED each time switch is pressed and released ******************************/
  e_mystate = STATE_WAIT_FOR_PRESS;

  while (1) {
    printNewState(e_mystate);  //debug message when state changes
    switch (e_mystate) {
      case STATE_WAIT_FOR_PRESS:
        if (SW1_PRESSED()) e_mystate = STATE_WAIT_FOR_RELEASE;
        break;
      case STATE_WAIT_FOR_RELEASE:
        if (SW1_RELEASED()) {
          LED1 = !LED1;   //toggle LED
          e_mystate = STATE_WAIT_FOR_PRESS;
        }
        break;
      default:
        e_mystate = STATE_WAIT_FOR_PRESS;
    }//end switch(e_mystate)
    DELAY_MS(DEBOUNCE_DLY);      //Debounce
    doHeartbeat();     //ensure that we are alive
  } // end while (1)
}
