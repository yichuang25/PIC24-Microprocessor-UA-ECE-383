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
#include "stdio.h"

// uncomment the next line to setup this project for a 12-bit ADC
#define USE_12BIT_ADC

#ifdef  USE_12BIT_ADC
#define   ADC_LEN           12
#define   ADC_NSTEPS        4096
#define   ADC_12BIT_FLAG    1
#else
#define   ADC_LEN           10
#define   ADC_NSTEPS        1024
#define   ADC_12BIT_FLAG    0
#endif

// prototypes provided for each DAC we support in this little example
void configDAC(void);
void writeDAC(uint16_t u16_x, uint16_t u16_y);

volatile uint8_t    u8_per, u8_amp;
volatile uint16_t   u16_per;

const uint8_t au8_sinetbl[] = {127,133,139,146,152,158,164,170,176,181, \
                               187,192,198,203,208,212,217,221,225,229,233,236,239,242,244,247,249,250, \
                               252,253,253,254,254,254,253,253,252,250,249,247,244,242,239,236,233,229, \
                               225,221,217,212,208,203,198,192,187,181,176,170,164,158,152,146,139,133, \
                               127,121,115,108,102,96,90,84,78,73,67,62,56,51,46,42,37,33,29,25,21,18, \
                               15,12,10,7,5,4,2,1,1,0,0,0,1,1,2,4,5,7,10,12,15,18,21,25,29,33,37,42,46, \
                               51,56,62,67,73,78,84,90,96,102,108,115,121
                              };


/**
*** Select the DAC you want to use by uncomment _ONLY_ONE_ line below!
**/
//#define  __DAC_R2R
//#define  __DAC_MAX548A
#define  __DAC_MAX5353
//#define  __DAC_MAX518
//#define  __DAC_MAX518_DUAL


#ifdef __DAC_R2R
#warning "DAC_R2R.C built for explicit R-2R DAC connected to RB9(MSb)-RB2(LSb)"
void  configDAC() {
  CONFIG_RB2_AS_DIG_OUTPUT();
  CONFIG_RB3_AS_DIG_OUTPUT();
  CONFIG_RB4_AS_DIG_OUTPUT();
  CONFIG_RB5_AS_DIG_OUTPUT();
  CONFIG_RB6_AS_DIG_OUTPUT();
  CONFIG_RB7_AS_DIG_OUTPUT();
  CONFIG_RB8_AS_DIG_OUTPUT();
  CONFIG_RB9_AS_DIG_OUTPUT();
}

void  writeDAC(uint16_t u16_x, uint16_t u16_y) {
  uint16_t    u16_temp;

  u16_temp = LATB & 0xFC03;              // read PORTB removing our eight bits
  u16_temp |= ((u16_x & 0xFF00) >> 6);   // put our eight bits into PORTB value
  LATB = u16_temp;
}
#endif

#ifdef __DAC_MAX548A
#warning "DAC_R2R.C built for SPI-based dual 8-bit MAX548A DAC connected to RP14(SDO) and RP13(SCLK)"
#define CONFIG_MAX548A_ENABLE()    CONFIG_RA2_AS_DIG_OUTPUT()
#define MAX548A_ENABLE()            _LATA2 = 0
#define MAX548A_DISABLE()           _LATA2 = 1

void configDAC(void) {
  //spi clock = 40MHz/1*4 = 40MHz/4 = 10MHz
  SPI1CON1 = SEC_PRESCAL_1_1     |    //1:1 secondary prescale
             PRI_PRESCAL_4_1     |    //4:1 primary prescale
             CLK_POL_ACTIVE_HIGH |    //clock active high (CKP = 0)
             SPI_CKE_ON          |    //out changes inactive to active (CKE=0)
             SPI_MODE8_ON        |    //8-bit mode
             MASTER_ENABLE_ON;        //master mode

  //configure pins. Only need SDO, SCLK since MAX548 is output only
  CONFIG_RB14_AS_DIG_OUTPUT();
  CONFIG_SDO1_TO_RP(14);      //use RP14 for SDO
  CONFIG_RB13_AS_DIG_OUTPUT();
  CONFIG_SCK1OUT_TO_RP(13);   //use RP13 for SCLK
  SPI1STATbits.SPIEN = 1;     //enable SPI mode
  CONFIG_MAX548A_ENABLE();      //chip select for MAX548
  MAX548A_DISABLE();            //disable the chip select
}

void writeDAC(uint16_t u16_x, uint16_t u16_y) {

  // only update DAC A
  //MAX548A_ENABLE();         //assert chipselect
  //ioMasterSPI1(0x09);       //command to immediately write DAC A
  //ioMasterSPI1(u16_x>>8);   //write DAC A data
  //MAX548A_DISABLE();        //release CS and update DAC outputs

  MAX548A_ENABLE();         //assert chipselect
  ioMasterSPI1(0x02);       //command to write DAC B input (do not change output)
  ioMasterSPI1(u16_y>>8);   //write DAC B data
  MAX548A_DISABLE();          //release CS and update DAC input
  MAX548A_ENABLE();           //assert chipselect
  ioMasterSPI1(0x09);       //command to write DAC A input and update both DAC outputs
  ioMasterSPI1(u16_x>>8);   //write DAC A data
  MAX548A_DISABLE();          //release CS and update DAC outputs
}
#endif

#ifdef __DAC_MAX5353
#warning "DAC_R2R.C built for SPI-based 12-bit MAX5353 DAC connected to RP14(SDO) and RP13(SCLK)"
#define CONFIG_MAX5353_ENABLE()    CONFIG_RA3_AS_DIG_OUTPUT()
#define MAX5353_CMD_ANDMASK        0x1FFE
#define MAX5353_ENABLE()           _LATA3 = 0
#define MAX5353_DISABLE()          _LATA3 = 1

void configDAC(void) {
  //spi clock = 40MHz/1*4 = 40MHz/4 = 10MHz
  SPI1CON1 = SEC_PRESCAL_1_1     |    //1:1 secondary prescale
             PRI_PRESCAL_4_1     |    //4:1 primary prescale
             CLK_POL_ACTIVE_HIGH |    //clock active high (CKP = 0)
             SPI_CKE_ON          |    //out changes inactive to active (CKE=0)
             SPI_MODE16_ON       |    //16-bit mode
             MASTER_ENABLE_ON;        //master mode

  //configure pins. Only need SDO, SCLK since MAX548 is output only
  CONFIG_RB14_AS_DIG_OUTPUT();
  CONFIG_SDO1_TO_RP(14);      //use RP14 for SDO
  CONFIG_RB13_AS_DIG_OUTPUT();
  CONFIG_SCK1OUT_TO_RP(13);   //use RP13 for SCLK
  SPI1STATbits.SPIEN = 1;     //enable SPI mode
  CONFIG_MAX5353_ENABLE();      //chip select for MAX548
  MAX5353_DISABLE();            //disable the chip select
}

void writeDAC(uint16_t u16_x, uint16_t u16_y) {
  MAX5353_ENABLE();                 //assert chipselect
  //write DAC B data (with command bits and sub-bit cleared)
  ioMasterSPI1((u16_x>>3) & MAX5353_CMD_ANDMASK);
  MAX5353_DISABLE();                //release CS and update DAC input
}
#endif

#ifdef __DAC_MAX518
#warning "DAC_R2R.C built for I2C-based single output 8-bit MAX518 DAC connected to RB9(SDA) and RB8(SCL)"
#define   MAX518_I2C_ADDR       0x58        // see MAX518 datasheet 19-0393 Figure 6
#define   MAX518_WRITE_DACA     0x00        // see MAX518 datasheet 19-0393 Figure 7
#define   MAX518_WRITE_DACB     0x01        // see MAX518 datasheet 19-0393 Figure 7

void configDAC(void) {
  configI2C1(400);            //configure I2C for 400 KHz
}

void writeDAC(uint16_t u16_x, uint16_t u16_y) {
  write2I2C1(MAX518_I2C_ADDR, MAX518_WRITE_DACA, (uint8_t) (u16_x>>8) );
}
#endif

#ifdef __DAC_MAX518_DUAL
#warning "DAC_R2R.C built for I2C-based DUAL output 8-bit MAX518 DAC connected to RB9(SDA) and RB8(SCL)"
#warning "In this example, I2C SCL is beyond specification for the MAX518 DAC."
#define   MAX518_I2C_ADDR       0x58        // see MAX518 datasheet 19-0393 Figure 6
#define   MAX518_WRITE_DACA     0x00        // see MAX518 datasheet 19-0393 Figure 7
#define   MAX518_WRITE_DACB     0x01        // see MAX518 datasheet 19-0393 Figure 7

void configDAC(void) {
  configI2C1(700);            //configure I2C for 400 KHz
}

void writeDAC(uint16_t u16_x, uint16_t u16_y) {
  static  uint8_t   au8_buf[]= {MAX518_WRITE_DACA, 0, MAX518_WRITE_DACB, 0 };

  au8_buf[1] = (uint8_t) (u16_x>>8);
  au8_buf[3] = (uint8_t) (u16_y>>8);
  writeNI2C1(MAX518_I2C_ADDR, &au8_buf[0], 4);

}
#endif



void _ISR _T3Interrupt (void) {
  static uint8_t    u8_idx;
  static uint16_t   u16_idx, u16_old;
  static uint16_t   u16_val;

  writeDAC(u16_val, u16_idx);          // write new DAC value

  // Compute DAC value for next time
  u16_idx+=u16_per;
  u8_idx = (uint8_t) (u16_idx>>9 );
  u16_val = ((uint16_t)au8_sinetbl[u8_idx])<<8;           // get sine fcn value
  u16_val >>= u8_amp;                     // reduce sine amplitude based on input from pot

  if ((u16_idx^u16_old)&0x8000) _LATB12 = !_LATB12;
  u16_old = u16_idx;

  _T3IF = 0;                //clear T3 interrupt flag
}

void configTimer3(void) {
  // configure T3 as 32-bit timer to trigger every 1/64 second
  T3CONbits.TON = 0;
  T3CON = T3_PS_1_1 | T3_SOURCE_INT;
  TMR3 = 0;
  PR3 = usToU16Ticks(50, getTimerPrescale(T3CONbits)) - 1;     // # of ticks for 50us (200KHz) seconds
  _T3IP=7;
  _T3IF=0;
  _T3IE=1;
  T3CONbits.TON = 1;
}


/** \file
 *  Performs a basic config of the ADC and samples two channels manually
 *  and sequentially with 12-bit results.
 *  Conversion results are printed to screen as both HEX values and voltages.
*/
int main (void) {
  uint8_t   u8_uiCount;

  configBasic(HELLO_MSG);
  // configure AN0 and AN1 to for analog input to PIC24 ADC
  CONFIG_AN0_AS_ANALOG();
  CONFIG_AN1_AS_ANALOG();
  configDAC();
  configTimer3();
  CONFIG_RB12_AS_DIG_OUTPUT();
  _LATB12 = 0;

  u8_uiCount=5;
  while (1) {
    configADC1_ManualCH0( ADC_CH0_POS_SAMPLEA_AN0, 31, ADC_12BIT_FLAG );
    DELAY_MS(100);
    u16_per =convertADC1();
    if (u16_per==0) u16_per++;         // u16_per must be >= 1

    configADC1_ManualCH0( ADC_CH0_POS_SAMPLEA_AN1, 31, ADC_12BIT_FLAG );
    DELAY_MS(100);
    u8_amp = convertADC1()>>9;        // 0 <= u8_amp <= 7

    if (!u8_uiCount) {
      printf("timestep=0x%04X    amplitude shift = 0x%02X\n", u16_per, u8_amp );
      u8_uiCount=5;
    } else
      u8_uiCount--;
  } //endof while()
} // endof main()
