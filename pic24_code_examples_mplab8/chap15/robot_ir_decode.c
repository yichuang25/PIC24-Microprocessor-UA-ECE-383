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
 * Decodes bi-phase bitstream from IR remote control as output by IR receiver
 * Protocol is Phillips VCR control, 13 bit command (start bit, toggle bit, 5-bit address, 6-bit data)
 * Timer2 divider must be set such that one bit time does not exceed the timer period.
*/

#define IR_FIFO_SIZE 32
volatile uint8_t au8_irFIFO[32];
volatile uint16_t u16_irFifoHead = 0;
volatile uint16_t u16_irFifoTail = 0;

void irFifoWrite(uint8_t u8_x) {
  u16_irFifoHead++;
  if (u16_irFifoHead == IR_FIFO_SIZE) u16_irFifoHead = 0;
  au8_irFIFO[u16_irFifoHead] = u8_x;
}
uint8_t irFifoRead(void) {
  while (u16_irFifoHead == u16_irFifoTail) {
    doHeartbeat();
  }
  u16_irFifoTail++;
  if (u16_irFifoTail == IR_FIFO_SIZE) u16_irFifoTail = 0;
  return au8_irFIFO[u16_irFifoTail];
}

uint8_t irFifoDataRdy(void) {
  return(u16_irFifoHead != u16_irFifoTail);
}



#define PWM_PERIOD 20000   //in microseconds  

/* no interrupt for Timer2, must be configured so that one bit time does
not exceed the Timer2 period. Will use 20 ms since this will also be
used for servo control
*/
void  configTimer2(void) {
  T2CON = T2_OFF | T2_IDLE_CON | T2_GATE_OFF
          | T2_32BIT_MODE_OFF
          | T2_SOURCE_INT
          | T2_PS_1_64 ;   //at 40 MHz, approx 104 ms max, 1 tick = 1.6 us
  PR2 = usToU16Ticks(PWM_PERIOD, getTimerPrescale(T2CONbits)) - 1;
  TMR2  = 0;                       //clear timer2 value
  _T2IF = 0;                        //clear interrupt flag
  T2CONbits.TON = 1;               //turn on the timer
}

#define TWOTHIRDS_PERIOD_US 1100      //two thirds expected bit period, in microseconds
#define COMMAND_LENGTH  13            //number of bits expected in IR command
#define IR_INPUT _RB7                 //using RB9 for IR input

volatile uint16_t u16_lastCapture, u16_thisCapture,u16_delta, u16_twoThirdsPeriodTicks;
volatile uint8_t u8_bitCount,u8_bitCountTotal,u8_currentByte;
//some one-bit flags
typedef struct tagFLAGBITS {
unsigned u1_bitEdge:
  1;
unsigned u1_bitValue:
  1;
} FLAGBITS;
static volatile FLAGBITS flags;

typedef enum  {
  STATE_START_PULSE_FALL = 0,
  STATE_START_PULSE_RISE,
  STATE_BIT_CAPTURE,
  STATE_LAST_EDGE,
} ICSTATE;

ICSTATE e_isrICState;

void _ISRFAST _IC1Interrupt() {
  _IC1IF = 0;
  u16_thisCapture = IC1BUF ;  //always read buffer to prevent overflow
  u16_delta = computeDeltaTicks(u16_lastCapture,u16_thisCapture,PR2);
  u16_lastCapture = u16_thisCapture;
  switch (e_isrICState) {
    case STATE_START_PULSE_FALL:
      e_isrICState = STATE_START_PULSE_RISE;
      break;
    case STATE_START_PULSE_RISE:
      if (u16_delta > u16_twoThirdsPeriodTicks) {
        //error, unexpected long pulse, reset back to start state
        e_isrICState = STATE_START_PULSE_FALL;
      } else {
        //received start pulse, start accumulating bits
        flags.u1_bitEdge = 1;   //next edge contains a bit
        u8_bitCount = 0;
        u8_currentByte = 0;
        flags.u1_bitValue = 1; //first bit is always a '1'
        u8_bitCountTotal = 0;
        e_isrICState = STATE_BIT_CAPTURE;
      }
      break;
    case STATE_BIT_CAPTURE:
      if ((u16_delta > u16_twoThirdsPeriodTicks) || flags.u1_bitEdge) {
        //record this bit
        if ((u16_delta > u16_twoThirdsPeriodTicks)) {
          //bit value has changed if wide pulse
          flags.u1_bitValue = !flags.u1_bitValue;
        }
        if (u8_bitCount != 0)u8_currentByte = u8_currentByte << 1;;
        if (flags.u1_bitValue) u8_currentByte = u8_currentByte | 0x01;
        u8_bitCount++;
        u8_bitCountTotal++;
        flags.u1_bitEdge = 1;  //this was a bit edge
        if (u8_bitCount == 7) {  //received start, toggle, address
          irFifoWrite(u8_currentByte);
          u8_currentByte = 0;
          u8_bitCount = 0;
        }
      }
      flags.u1_bitEdge = !flags.u1_bitEdge;  //next edge is opposite
      if (u8_bitCountTotal == COMMAND_LENGTH) {
        if (u8_bitCount != 0) irFifoWrite(u8_currentByte);  //save last byte
        if (IR_INPUT) e_isrICState = STATE_START_PULSE_FALL;
        else e_isrICState = STATE_LAST_EDGE;   //one more edge to come
      }
      break;
    case STATE_LAST_EDGE:
      e_isrICState = STATE_START_PULSE_FALL;
      break;

    default:
      e_isrICState = STATE_START_PULSE_FALL;
  }
}


//configure input capture.
void configInputCapture1(void) {
  CONFIG_RB7_AS_DIG_INPUT();   //use RB7 for IR Input (must be 5 V tolerant)
  CONFIG_IC1_TO_RP(7);         //map IC1 to RP7/R7
  e_isrICState = STATE_START_PULSE_FALL;
  u16_irFifoHead = 0;
  u16_irFifoTail = 0;
  u16_twoThirdsPeriodTicks = usToU16Ticks(TWOTHIRDS_PERIOD_US, getTimerPrescale(T2CONbits));
  IC1CON = IC_TIMER2_SRC |     //Timer2 source
           IC_INT_1CAPTURE |   //Interrupt every capture
           IC_EVERY_EDGE;      //Interrupt every edge
  _IC1IF = 0;
  _IC1IP = 1;
  _IC1IE = 1;   //enable
}

