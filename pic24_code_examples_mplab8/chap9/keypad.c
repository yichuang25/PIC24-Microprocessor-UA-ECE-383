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
Implements a 4x3 key scanned keypad interface.
A periodic timer interrupt is used to poll the keypad. Thanks goes
to David Weaver for suggestions on improvements to the scan function.
*/

#define C0 _RB9
#define C1 _RB8
#define C2 _RB7
#define C3 _RB6

static inline void CONFIG_COLUMN() {
  CONFIG_RB9_AS_DIG_INPUT();
  ENABLE_RB9_PULLUP();
  CONFIG_RB8_AS_DIG_INPUT();
  ENABLE_RB8_PULLUP();
  CONFIG_RB7_AS_DIG_INPUT();
  ENABLE_RB7_PULLUP();
  CONFIG_RB6_AS_DIG_INPUT();
  ENABLE_RB6_PULLUP();
}

#define R0 _LATB5
#define R1 _LATB4
#define R2 _LATB3

#define CONFIG_R0_DIG_OUTPUT() CONFIG_RB5_AS_DIG_OUTPUT()
#define CONFIG_R1_DIG_OUTPUT() CONFIG_RB4_AS_DIG_OUTPUT()
#define CONFIG_R2_DIG_OUTPUT() CONFIG_RB3_AS_DIG_OUTPUT()

void CONFIG_ROW() {
  CONFIG_R0_DIG_OUTPUT();
  CONFIG_R1_DIG_OUTPUT();
  CONFIG_R2_DIG_OUTPUT();
}

static inline void DRIVE_ROW_LOW() {
  R0 = 0;
  R1 = 0;
  R2 = 0;
}

static inline void DRIVE_ROW_HIGH() {
  R0 = 1;
  R1 = 1;
  R2 = 1;
}

void configKeypad(void) {
  CONFIG_ROW();
  DRIVE_ROW_LOW();
  CONFIG_COLUMN();
  DELAY_US(1);     //wait for pullups to stabilize inputs
}

//drive one row low
void setOneRowLow(uint8_t u8_x) {
  switch (u8_x) {
    case 0:
      R0 = 0;
      R1 = 1;
      R2 = 1;
      break;
    case 1:
      R0 = 1;
      R1 = 0;
      R2 = 1;
      break;
    default:
      R0 = 1;
      R1 = 1;
      R2 = 0;
      break;
  }
}
#define NUM_ROWS 3
#define NUM_COLS 4
const uint8_t au8_keyTable[NUM_ROWS][NUM_COLS] = { {'1', '4', '7', '*'},
  {'2', '5', '8', '0'},
  {'3', '6', '9', '#'}
};

#define KEY_PRESSED() (!C0 || !C1 || !C2 || !C3)   //any low
#define KEY_RELEASED() (C0 && C1 && C2 && C3)  //all high

uint8_t doKeyScan(void) {
  uint8_t u8_row, u8_col;
  //determine column
  if (!C0) u8_col = 0;
  else if (!C1) u8_col = 1;
  else if (!C2) u8_col = 2;
  else if (!C3) u8_col = 3;
  else return('E'); //error
  //determine row
  for (u8_row = 0; u8_row < NUM_ROWS; u8_row++) {
    setOneRowLow(u8_row); //enable one row low
    if (KEY_PRESSED()) {
      DRIVE_ROW_LOW(); //return rows to driving low
      return(au8_keyTable[u8_row][u8_col]);
    }
  }
  DRIVE_ROW_LOW(); //return rows to driving low
  return('E'); //error
}


typedef enum  {
  STATE_WAIT_FOR_PRESS = 0,
  STATE_WAIT_FOR_PRESS2,
  STATE_WAIT_FOR_RELEASE,
} ISRSTATE;

ISRSTATE e_isrState = STATE_WAIT_FOR_PRESS;
volatile uint8_t u8_newKey = 0;

//Interrupt Service Routine for Timer3
void _ISR _T3Interrupt (void) {
  switch (e_isrState) {
    case STATE_WAIT_FOR_PRESS:
      if (KEY_PRESSED() && (u8_newKey == 0)) {
        //ensure that key is sampled low for two consecutive interrupt periods
        e_isrState = STATE_WAIT_FOR_PRESS2;
      }
      break;
    case STATE_WAIT_FOR_PRESS2:
      if (KEY_PRESSED()) {
        // a key is ready
        u8_newKey = doKeyScan();
        e_isrState = STATE_WAIT_FOR_RELEASE;
      } else e_isrState = STATE_WAIT_FOR_PRESS;
      break;

    case STATE_WAIT_FOR_RELEASE:
      //keypad released
      if (KEY_RELEASED()) {
        e_isrState = STATE_WAIT_FOR_PRESS;
      }
      break;
    default:
      e_isrState = STATE_WAIT_FOR_PRESS;
      break;
  }
  _T3IF = 0;                 //clear the timer interrupt bit
}

#define ISR_PERIOD     15      // in ms

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
  configBasic(HELLO_MSG);
  /** PIO config ******/
  configKeypad();
  /** Configure the Timer */
  configTimer3();
  while (1) {
    if (u8_newKey) {
      outChar(u8_newKey);
      u8_newKey = 0;
    }
    doHeartbeat();     //ensure that we are alive
  } // end while (1)
}
