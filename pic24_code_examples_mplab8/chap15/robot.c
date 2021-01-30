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
Capstone project for a small 3-wheeled robot with
IR control and collision detection.
*/

extern uint8_t irFifoRead(void);
extern void  configTimer2(void);
extern void configInputCapture1(void);
extern void configOutputCapture1(void);
extern uint8_t irFifoDataRdy(void);

#define WHEEL_PWM_STEPS 10        //number of PWM steps
#define WHEEL_PWM_STEPSIZE 500    //in microseconds
#define MAN_DRIVE 0
#define AUTO_DRIVE 1

#define BLOCKAGE_PRESENT     560   //1.8 V
#define BLOCKAGE_CLEAR       434   //1.4 V


uint8_t u8_leftDC, u8_rightDC;  //left, right duty cycles, left period
uint8_t u8_leftPeriod,u8_rightPeriod ; //left/right period counters
uint16_t u16_adcVal;


typedef struct tagFLAGBITS {
unsigned u1_mode:
  1;
unsigned u1_obstacle:
  1;
unsigned u1_irCmdFlag:
  1;
} FLAGBITS;
static volatile FLAGBITS flags;


#define WHEEL_LEFT_1A _LATB1
#define WHEEL_LEFT_2A _LATB3
#define WHEEL_LEFT_EN _LATB2
#define WHEEL_RIGHT_1A  _LATB14
#define WHEEL_RIGHT_2A  _LATB12
#define WHEEL_RIGHT_EN  _LATB13


#define WHEEL_LEFT_FORWARD()  { WHEEL_LEFT_1A = 1; WHEEL_LEFT_2A = 0; }
#define WHEEL_LEFT_REVERSE()  { WHEEL_LEFT_1A = 0; WHEEL_LEFT_2A = 1; }
#define WHEEL_LEFT_STOP()     { WHEEL_LEFT_1A = 0;  WHEEL_LEFT_2A = 0; }
#define WHEEL_RIGHT_FORWARD() { WHEEL_RIGHT_1A = 1; WHEEL_RIGHT_2A = 0; }
#define WHEEL_RIGHT_REVERSE() { WHEEL_RIGHT_1A = 0;  WHEEL_RIGHT_2A = 1; }
#define WHEEL_RIGHT_STOP()    { WHEEL_RIGHT_1A = 0;  WHEEL_RIGHT_2A = 0; }

static inline void configLeftWheel(void) {
  CONFIG_RB1_AS_DIG_OUTPUT();
  CONFIG_RB2_AS_DIG_OUTPUT();
  CONFIG_RB3_AS_DIG_OUTPUT();
}

static inline void configRightWheel(void) {
  CONFIG_RB12_AS_DIG_OUTPUT();
  CONFIG_RB13_AS_DIG_OUTPUT();
  CONFIG_RB14_AS_DIG_OUTPUT();
}


void _ISRFAST _T3Interrupt (void) {
  uint16_t u16_temp;
  _T3IF = 0;                 //clear the timer interrupt bit
  if (u8_rightDC) {
    if ( u8_rightPeriod == WHEEL_PWM_STEPS || !u8_rightPeriod) {
      u8_rightPeriod = 0;
      WHEEL_RIGHT_EN = 1;
    } else if (u8_rightPeriod >= u8_rightDC) WHEEL_RIGHT_EN = 0;
    u8_rightPeriod++;
  } else {
    WHEEL_RIGHT_EN = 0;
    u8_rightPeriod = 0;
  }
  if (u8_leftDC) {
    if ( u8_leftPeriod == WHEEL_PWM_STEPS || !u8_leftPeriod) {
      u8_leftPeriod = 0;
      WHEEL_LEFT_EN = 1;
    } else if (u8_leftPeriod >= u8_leftDC) WHEEL_LEFT_EN = 0;
    u8_leftPeriod++;
  } else {
    WHEEL_LEFT_EN = 0;
    u8_rightPeriod = 0;
  }
  u16_temp = ADC1BUF0;
  if (u16_temp > BLOCKAGE_PRESENT ) flags.u1_obstacle = 1;
  else if (u16_temp < BLOCKAGE_CLEAR) flags.u1_obstacle = 0;
  AD1CON1bits.SAMP = 1; //start next ADC conversion for next interrupt
}

void  configTimer3(void) {
  //ensure that Timer2,3 configured as separate timers.
  T2CONbits.T32 = 0;     // 32-bit mode off
  T3CON = T3_OFF |T3_IDLE_CON | T3_GATE_OFF
          | T3_SOURCE_INT
          | T3_PS_1_8 ;
  PR3 = usToU16Ticks (WHEEL_PWM_STEPSIZE, getTimerPrescale(T3CONbits)) - 1;
  TMR3  = 0;                       //clear timer3 value
  _T3IF = 0;                       //clear interrupt flag
  _T3IP = 1;                       //choose a priority
  _T3IE = 1;                       //enable the interrupt
  T3CONbits.TON = 1;               //turn on the timer
}

/*
Configure Timer3 for a WHEEL_PWM_STEPSIZE periodic interrupt, do
manual PWM on both wheels
*/

void configWheels(void) {
  u8_leftDC = 0;
  u8_rightDC = 0;
  u8_leftPeriod = 0;
  u8_rightPeriod = 0;
  configLeftWheel();
  configRightWheel();
  WHEEL_LEFT_FORWARD();
  WHEEL_RIGHT_FORWARD();
  configTimer3();
}

#define CMD_STOP         0x36  //VCR stop button
#define CMD_SPEEDUP      0x29  //VCR pause button
#define CMD_SLOWDOWN     0x37  //VCR record button
#define CMD_FORWARD      0x35  //VCR play button
#define CMD_TURN_LEFT    0x32  //VCR rewind button
#define CMD_TURN_RIGHT   0x34  //VCR fast forward button
#define CMD_REVERSE      0x05  //VCR numeral 5
#define CMD_SPIN         0x09  //VCR numeral 9
#define CMD_MODESWAP     0x01  //VCR numeral 1 
#define CMD_NOP          0x00

#define DEFAULT_SPEED    4
#define DEFAULT_SPEED    4
#define BACK_SPEED       6

void doSpeedUp(uint8_t u8_speed) {
  u8_leftDC = 0;
  u8_rightDC = 0;
  do {
    u8_leftDC++;
    u8_rightDC++;
    DELAY_MS(100);
  } while (u8_leftDC < u8_speed);
}

void allStop(void) {
  //slow to a stop
  while (u8_leftDC || u8_rightDC) {
    if (u8_leftDC) u8_leftDC--;
    if (u8_rightDC) u8_rightDC--;
    DELAY_MS(100);
  }
}

void goForward(uint8_t u8_speed) {
  WHEEL_LEFT_FORWARD();
  WHEEL_RIGHT_FORWARD();
  doSpeedUp(u8_speed);
}

void goReverse(uint8_t u8_speed) {
  WHEEL_LEFT_REVERSE();
  WHEEL_RIGHT_REVERSE();
  doSpeedUp(u8_speed);
}

void processCmd(uint8_t u8_cmd) {
  switch (u8_cmd) {
    case CMD_SPEEDUP:
      if (u8_leftDC < WHEEL_PWM_STEPS) u8_leftDC += 1;
      if (u8_rightDC < WHEEL_PWM_STEPS) u8_rightDC += 1;
      break;
    case CMD_SLOWDOWN:
      if (u8_leftDC) u8_leftDC -= 1;
      if (u8_rightDC) u8_rightDC -= 1;
      break;
    case CMD_STOP:
      allStop();
      break;
    case CMD_FORWARD:
      allStop();
      goForward(DEFAULT_SPEED);
      break;
    case CMD_REVERSE:
      allStop();
      goReverse(DEFAULT_SPEED);
      break;
    case CMD_TURN_LEFT:
      if (u8_leftDC) u8_leftDC -= 1;
      break;
    case CMD_TURN_RIGHT:
      if (u8_rightDC) u8_rightDC -= 1;
      break;
    case CMD_SPIN:
      allStop();
      WHEEL_LEFT_FORWARD();
      WHEEL_RIGHT_REVERSE();
      doSpeedUp(DEFAULT_SPEED);
      break;
    case CMD_MODESWAP:
      allStop();
      flags.u1_mode = !flags.u1_mode;
      break;
    default:  //do nothing
      break;
  }
}

uint8_t getIRCMD(void) {
  uint8_t u8_x, u8_cmd;
  u8_x = irFifoRead();
  u8_cmd = irFifoRead();
  if (u8_x & 0x20) outString("Toggle = 1, ");
  else outString("Toggle = 0, ");
  outString("Addr: ");
  outUint8(u8_x & 0x1F);
  outString(",Cmd: ");
  outUint8(u8_cmd);
  outString("\n");
  if (flags.u1_irCmdFlag) flags.u1_irCmdFlag = 0;
  else flags.u1_irCmdFlag = 1;
  if (flags.u1_irCmdFlag) return u8_cmd;
  else return CMD_NOP;
}

void checkIR(void) {
  uint8_t u8_cmd;
  if (irFifoDataRdy()) {
    u8_cmd = getIRCMD();
    processCmd(u8_cmd);
  }
}

void doTurn(uint8_t u8_ltarg, uint8_t u8_rtarg) {
  u8_leftDC = 0;
  u8_rightDC = 0;
  while ((u8_leftDC != u8_ltarg) ||
         (u8_rightDC != u8_rtarg) ) {
    if (u8_leftDC != u8_ltarg) u8_leftDC++;
    if (u8_rightDC != u8_rtarg) u8_rightDC++;
    DELAY_MS(80);
  }
}


void autoDrive(void) {
  if (flags.u1_obstacle) {
    allStop();
    //backup, pivot
    WHEEL_LEFT_REVERSE();
    WHEEL_RIGHT_REVERSE();
    doSpeedUp(BACK_SPEED);
    DELAY_MS(300);
    u8_leftDC = 2;  //slow down wheel to turn
    DELAY_MS(800);  //turning
    allStop();
    DELAY_MS(200);
    WHEEL_LEFT_FORWARD();
    WHEEL_RIGHT_FORWARD();
    doTurn(BACK_SPEED,2);
    DELAY_MS(600);
    doSpeedUp(DEFAULT_SPEED);
  } else if (u8_leftDC != DEFAULT_SPEED ||
             u8_rightDC != DEFAULT_SPEED ) {
    WHEEL_LEFT_FORWARD();
    WHEEL_RIGHT_FORWARD();
    doSpeedUp(DEFAULT_SPEED);
  }
}

int main (void) {
  configBasic(HELLO_MSG);
  CONFIG_AN1_AS_ANALOG();
  configADC1_ManualCH0( ADC_CH0_POS_SAMPLEA_AN1, 31, 1 );
  SET_SAMP_BIT_ADC1();   //start first conversion
  configTimer2();
  configInputCapture1();
  configWheels();
  while (1) {
    checkIR();
    if (flags.u1_mode == AUTO_DRIVE) autoDrive();
    doHeartbeat();
  } //end while(1)
}
