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
#include "reflow_oven.h"

/** \file
 *  Capstone- Reflow Oven control.
 *  Resources used:
 *   Timer2, Timer3, OC1, INT1, SPI
*/


static inline void CONFIG_ZEROCROSS_ENABLE() {
  CONFIG_RB6_AS_DIG_INPUT();
  ENABLE_RB6_PULLUP();
  CONFIG_INT1_TO_RP(6);   //RP6 shares RB63 pin
  DELAY_US(1);
  _INT1IF = 0;   //Clear the interrupt flag
  _INT1IP = 1;   //Choose a priority
  _INT1EP = 0;   //rising edge triggerred
  _INT1IE = 1;   //enable INT1 interrupt
}

void  configTimer2(void) {
  T2CON = T2_OFF | T2_IDLE_CON | T2_GATE_OFF
          | T2_32BIT_MODE_OFF
          | T2_SOURCE_INT
          | T2_PS_1_8 ;  //1 tick = 0.2 us at FCY=40 MHz
  PR2 =  0xFFFF;
  TMR2  = 0;       //clear timer2 value
}

#define TRIAC_PW_HIGH  100  //in microseconds, triac gate pulse width high
#define HALF_CYCLE 8000     //in microseconds, part of half-cycle to use for triggering
#define CYCLE_OFFSET 200    //don't start right at zero crossing
uint8_t u8_currPowerSetting;
uint16_t u16_halfCycleTicks;
uint16_t u16_triacPWHighTicks;
uint16_t u16_cycleOffsetTicks;

void configOutputCapture1(void) {
  T2CONbits.TON = 0;       //disable Timer when configuring Output compare
  TMR2 = 0;                //clear timer 2
  CONFIG_OC1_TO_RP(14);    //map OC1 to RP14/RB14
  OC1R = 1;                //want an immediate match so that OC1 is driven low
  OC1RS = 2;               //want an immediate match so that OC1 is driven low
//OC1 is initially off.
  OC1CON = OC_TIMER2_SRC |     //Timer2 source
           OC_SINGLE_PULSE;    //delayed one shot
  T2CONbits.TON = 1;       //enable the timer
}

//AC power is controlled by triac by sending a delayed fixed-width pulse
//to the triac's gate every half-cycle. The delay from zero-crossing
//before the pulse is sent controls the power, the longer the delay,
//the less the power. The output capture OC_SINGLE pulse mode is used to
//generate the pulse.
static inline void TRIAC_ON() {
  uint32_t u32_x;
  T2CONbits.TON = 0;    //disable Timer when configuring Output compare
  TMR2 = 0;             //clear timer 2
  //later in the cycle is less power
  u32_x = u16_halfCycleTicks;
  u32_x = (u32_x * u8_currPowerSetting)/MAX_POWER_SETTING;
  u32_x = u16_halfCycleTicks - u32_x;
  OC1R  =  u32_x+u16_cycleOffsetTicks; //time for rising edge of pulse
  OC1RS  = u32_x+u16_triacPWHighTicks+u16_cycleOffsetTicks;  //time for falling edge of pulse
  OC1CON = OC_TIMER2_SRC |     //Timer2 source
           OC_SINGLE_PULSE;    //delayed one shot
  T2CONbits.TON = 1;       //enable the timer
}

//use output compare single shot mode, active high
void configureTriacControl() {
  configTimer2();    //config Timer2
  configOutputCapture1();
  u16_halfCycleTicks = usToU16Ticks(HALF_CYCLE,getTimerPrescale(T2CONbits));
  u16_triacPWHighTicks = usToU16Ticks(TRIAC_PW_HIGH, getTimerPrescale(T2CONbits));
  u16_cycleOffsetTicks = usToU16Ticks(CYCLE_OFFSET, getTimerPrescale(T2CONbits));
}


//occurs at a 120 Hz rate, interrupted every time AC waveform crosses 0
void _ISRFAST _INT1Interrupt (void) {
  _INT1IF = 0;    //clear the interrupt bit
  if (u8_currPowerSetting) TRIAC_ON();     //turn on triac
} //end _INT1Interrupt

//use 16-bit mode
void configSPI1(void) {
  //spi clock = 40MHz/1*64 = 40MHz/64 = 625 kHz
  SPI1CON1 = SEC_PRESCAL_1_1 |     //1:1 secondary prescale
             PRI_PRESCAL_64_1 |     //64:1 primary prescale
             CLK_POL_ACTIVE_HIGH   | //clock active high (CKP = 0)
             SPI_CKE_OFF          | //out changes inactive to active (CKE=0)
             SPI_MODE16_ON         | //16-bit mode
             MASTER_ENABLE_ON;     //master mode
  //Only need SDI, SCLK since Thermocouple sensor is output only
  CONFIG_SCK1OUT_TO_RP(7);   //use RP7 for SCLK
  CONFIG_SDI1_TO_RP(5);      //use RP5 for SDI
  CONFIG_SLAVE_ENABLE();     //chip select
  SLAVE_DISABLE();           //disable the chip select
  SPI1STATbits.SPIEN = 1;    //enable SPI mode
}

uint16_t readMAX6675(void) {
  uint16_t u16_x;
  SLAVE_ENABLE();            //assert chipselect
  u16_x = ioMasterSPI1(0);   //read theromouple
  SLAVE_DISABLE();
  return(u16_x);
}

float getCelsiusFloatTemp(void) {
  uint16_t u16_x;
  float f_tempC;
  u16_x = readMAX6675();
  u16_x = u16_x >> 3;     //drop status bits
  f_tempC = u16_x;        //convert to float
  f_tempC = f_tempC/4;     //10.2 format, divide by 4
  return(f_tempC);
}

//Round to nearest degree
int16_t getCelsiusI16Temp(void) {
  uint16_t u16_x;
  uint16_t u16_frac;

  u16_x = readMAX6675();
  u16_frac = (u16_x & 0x1F) >> 3; //mask integer, drop status bits
  u16_x = u16_x >> 5;
  if (u16_frac >= 2) u16_x++; //round up
  return((int16_t) u16_x);
}



uint16_t volatile u16_tenthSeconds = 0;

void _ISRFAST _T3Interrupt (void) {
  _T3IF = 0;           //clear the timer interrupt bit
  u16_tenthSeconds++;  //coarse timekeeper 0.1 second, all we need.
}

#define ISR_PERIOD     100            // in ms
void  configTimer3(void) {
  //ensure that Timer2,3 configured as separate timers.
  T2CONbits.T32 = 0;     // 32-bit mode off
  T3CON = T3_OFF |T3_IDLE_CON | T3_GATE_OFF
          | T3_SOURCE_INT
          | T3_PS_1_256 ;
  PR3 = msToU16Ticks (ISR_PERIOD, getTimerPrescale(T3CONbits)) - 1;
  TMR3  = 0;                       //clear timer3 value
  _T3IF = 0;                       //clear interrupt flag
  _T3IP = 1;                       //choose a priority
  _T3IE = 1;                       //enable the interrupt
  T3CONbits.TON = 1;               //turn on the timer
}


void decrementPower(void) {
  if (!u8_currPowerSetting) return;
  u8_currPowerSetting--;
}

void incrementPower(void) {
  if (u8_currPowerSetting == MAX_POWER_SETTING) return;
  u8_currPowerSetting++;
}

void setPower(uint8_t newPower) {
  u8_currPowerSetting = newPower;
}

uint8_t getPower(void) {
  return(u8_currPowerSetting);
}


void doMainMenu(void) {
  uint8_t u8_c;
  printf("Current profile: %s\n", getProfileDesc(u8_currentProfile));
  printf("Reflow oven menu:\n");
  printf("   'S' - start oven for current profile  \n");
  printf("   'p' - reflow profile menu \n");
  printf("   'd' - debug menu\n");
  printf("Enter character: ");
  u8_c = inCharEcho();
  printf("\n");
  if (u8_c == 'd') doDebugMenu();
  if (u8_c == 'p') doProfileMenu();
  if (u8_c == 'S') doReflow();
  printf("\n");
}


int main (void) {
  configBasic(HELLO_MSG);
  configureTriacControl();
  CONFIG_ZEROCROSS_ENABLE();
  configSPI1();
  configTimer3();
  doRead(&fdata); //read temperature calibration data

  while (1) {
    doMainMenu();
  }
}
