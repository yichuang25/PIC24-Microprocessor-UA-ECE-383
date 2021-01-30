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
Capstone project that digitizes audio, saves to EEPROM,
and plays it back. Uses the internal ADC, 24LC515 serial EEPROM,
8-bit SPI DAC (MAXIM 548A), Timer3.
*/

extern void ADPCMEncoderInit(void);
extern uint8_t ADPCMEncoder( int16_t i16_sample);
extern int16_t ADPCMDecoder(uint8_t u8_code);
extern void ADPCMDecoderInit(void);


#define EEPROM 0xA0   //LC515 address assuming both address pins tied low.
#define BLKSIZE 64

//Assumes WDT is configured for longer than EEPROM write time
void waitForWriteCompletion(uint8_t u8_i2cAddr) {
  uint8_t u8_ack, u8_savedSWDTEN;
  u8_savedSWDTEN = _SWDTEN;
  _SWDTEN = 1; //enable WDT so that do not get stuck in infinite loop!
  u8_i2cAddr = I2C_WADDR(u8_i2cAddr);  //write operation, R/W# = 0;
  do {
    startI2C1();
    u8_ack = putNoAckCheckI2C1(u8_i2cAddr);
    stopI2C1();
  } while (u8_ack == I2C_NAK);
  _SWDTEN = u8_savedSWDTEN;  //restore WDT to original state
}

//this version just expects a block of 64 data bytes
void memWriteLC515(uint8_t u8_i2cAddr,  uint16_t u16_MemAddr, uint8_t *pu8_buf) {
  uint8_t u8_AddrLo, u8_AddrHi;
  uint16_t u16_i;
  u8_AddrLo = u16_MemAddr & 0x00FF;
  u8_AddrHi = (u16_MemAddr >> 8);
  if (u16_MemAddr & 0x8000) {
    // if MSB set , set block select bit
    u8_i2cAddr = u8_i2cAddr | 0x08;
  }
  waitForWriteCompletion(u8_i2cAddr);
  //write the data to the EEPROM
  startI2C1();
  putI2C1(I2C_WADDR(u8_i2cAddr));
  putI2C1(u8_AddrHi);
  putI2C1(u8_AddrLo);
  for (u16_i=0; u16_i < BLKSIZE; u16_i++) {
    putI2C1(*pu8_buf);
    pu8_buf++;
  }
  stopI2C1();
}

void memReadLC515(uint8_t u8_i2cAddr,  uint16_t u16_MemAddr, uint8_t *pu8_buf) {

  uint8_t u8_AddrLo, u8_AddrHi;

  u8_AddrLo = u16_MemAddr & 0x00FF;
  u8_AddrHi = (u16_MemAddr >> 8);

  if (u16_MemAddr & 0x8000) {
    // if MSB set , set block select bit
    u8_i2cAddr = u8_i2cAddr | 0x08;
  }
  waitForWriteCompletion(u8_i2cAddr);
  //set address counter
  write2I2C1(u8_i2cAddr,u8_AddrHi, u8_AddrLo);
  //read data
  readNI2C1(u8_i2cAddr,pu8_buf, BLKSIZE);
}


#define CONFIG_SLAVE_ENABLE() CONFIG_RB3_AS_DIG_OUTPUT()
#define SLAVE_ENABLE()        _LATB3 = 0  //low true assertion
#define SLAVE_DISABLE()       _LATB3 = 1


void configSPI1(void) {
  //spi clock = 40MHz/1*4 = 40MHz/4 = 10MHz
  SPI1CON1 = SEC_PRESCAL_1_1 |     //1:1 secondary prescale
             PRI_PRESCAL_4_1 |     //4:1 primary prescale
             CLK_POL_ACTIVE_HIGH | //clock active high (CKP = 0)
             SPI_CKE_ON          | //out changes active to inactive (CKE=1)
             SPI_MODE8_ON        | //8-bit mode
             MASTER_ENABLE_ON;     //master mode
  //configure pins. Only need SDO, SCLK since POT is output only
  CONFIG_SDO1_TO_RP(6);      //use RP6 for SDO
  CONFIG_SCK1OUT_TO_RP(7);   //use RP7 for SCLK

  SPI1STATbits.SPIEN = 1;  //enable SPI mode
}
void configDAC() {
  CONFIG_SLAVE_ENABLE();       //chip select for DAC
  SLAVE_DISABLE();             //disable the chip select
}

void writeDAC (uint8_t dacval) {
  SLAVE_ENABLE();                 //assert Chipselect line to DAC
  ioMasterSPI1(0b00001001);      //control byte that enables DAC A
  ioMasterSPI1(dacval);          //write DAC value
  SLAVE_DISABLE();
}

#define ISR_PERIOD     125                // in microseconds

void  configTimer3(void) {
  //ensure that Timer2,3 configured as separate timers.
  T2CONbits.T32 = 0;     // 32-bit mode off
  //T3CON set like this for documentation purposes.
  //could be replaced by T3CON = 0x0020
  T3CON = T3_OFF |T3_IDLE_CON | T3_GATE_OFF
          | T3_SOURCE_INT
          | T3_PS_1_1 ;
  PR3 = usToU16Ticks (ISR_PERIOD, getTimerPrescale(T3CONbits)) - 1;
  TMR3  = 0;                       //clear timer3 value
  _T3IF = 0;                       //clear interrupt flag
  _T3IP = 1;                       //choose a priority
  _T3IE = 1;                       //enable the interrupt
  T3CONbits.TON = 1;               //turn on the timer
}


uint8_t au8_bufferA[BLKSIZE];
uint8_t au8_bufferB[BLKSIZE];
//some one-bit flags
typedef struct tagFLAGBITS {
unsigned u1_activeBuffer:
  1;
unsigned u1_writeFlag:
  1;
unsigned u1_recordFlag:
  1;
unsigned u1_playbackFlag:
  1;
unsigned u1_readFlag:
  1;
unsigned u1_passThruFlag:
  1;
unsigned u1_compressionFlag:
  1;
} FLAGBITS;
volatile FLAGBITS flags;

uint8_t u8_sampleCount;
uint8_t u8_bufferCount;
uint16_t u16_adcVal;
uint8_t u8_dacVal;
int16_t i16_adcval;

void isrRecord() {
  uint8_t u8_tmp;
  if (flags.u1_compressionFlag) {
    //convert to signed value
    i16_adcval = u16_adcVal;
    i16_adcval -= 2048;
    u8_tmp = ADPCMEncoder( i16_adcval);
    if (u8_sampleCount & 0x01)  {
      u8_dacVal = (u8_tmp << 4)| u8_dacVal;
      u8_sampleCount++;
      if (flags.u1_activeBuffer) au8_bufferB[u8_bufferCount++] = u8_dacVal;
      else au8_bufferA[u8_bufferCount++] = u8_dacVal;
    } else {
      u8_dacVal  = u8_tmp;
      u8_sampleCount++;
    }
  } else {
    u8_dacVal = (u16_adcVal>>4) & 0x00FF;
    if (flags.u1_activeBuffer) au8_bufferB[u8_bufferCount++] = u8_dacVal;
    else au8_bufferA[u8_bufferCount++] = u8_dacVal;
  }
  if (u8_bufferCount ==  BLKSIZE) {
    flags.u1_activeBuffer = !flags.u1_activeBuffer;
    flags.u1_writeFlag = 1;
    u8_bufferCount = 0;
    u8_sampleCount = 0;
  }
}

void isrPlayback() {
  uint8_t u8_tmp;
  if (flags.u1_activeBuffer) u8_tmp = au8_bufferB[u8_bufferCount];
  else u8_tmp = au8_bufferA[u8_bufferCount];
  if (flags.u1_compressionFlag) {
    if (u8_sampleCount & 0x01) {
      i16_adcval = ADPCMDecoder(u8_tmp >> 4);
      u8_bufferCount++;
    } else {
      i16_adcval = ADPCMDecoder(u8_tmp & 0x0F);
    }
    i16_adcval += 2048;
    u16_adcVal = i16_adcval;
    u8_dacVal = (u16_adcVal>>4) & 0x00FF;
    writeDAC(u8_dacVal);
    u8_sampleCount++;
  } else {
    writeDAC(u8_tmp);
    u8_bufferCount++;
  }
  if (u8_bufferCount ==  BLKSIZE) {
    flags.u1_activeBuffer = !flags.u1_activeBuffer;
    flags.u1_readFlag = 1;
    u8_bufferCount = 0;
    u8_sampleCount = 0;
  }
}

void _ISR _T3Interrupt (void) {

  _T3IF = 0;
  u16_adcVal = ADC1BUF0;
  //start next sample
  SET_SAMP_BIT_ADC1();
  if (flags.u1_recordFlag) isrRecord();
  if (flags.u1_playbackFlag) isrPlayback();
  if (flags.u1_passThruFlag) {
    u8_dacVal = (u16_adcVal>>4) & 0x00FF;
    writeDAC(u8_dacVal);
  }
}

#define VREF 3.3  //assume Vref = 3.3 volts

void doRecord(uint8_t u8_compression) {
  uint16_t u16_MemAddr;
  flags.u1_compressionFlag = u8_compression;
  if (u8_compression) ADPCMEncoderInit();
  flags.u1_activeBuffer = 0;
  u8_sampleCount = 0;
  u8_bufferCount = 0;
  u16_MemAddr = 0;     //start at location 0 in memory
  flags.u1_writeFlag = 0;
  flags.u1_recordFlag = 1;
  do {
    if (flags.u1_writeFlag) reportError("Record overflow");
    while (!flags.u1_writeFlag) doHeartbeat();
    flags.u1_writeFlag = 0;
    if (flags.u1_activeBuffer)
      memWriteLC515(EEPROM,u16_MemAddr,au8_bufferA);
    else
      memWriteLC515(EEPROM,u16_MemAddr,au8_bufferB);
    u16_MemAddr = u16_MemAddr +64;
    outString("*");
  } while (u16_MemAddr != 0);//end record loop
  flags.u1_recordFlag = 0;
  flags.u1_compressionFlag = 0;
  outString("Finished recording\n");
}

void doPlayback(uint8_t u8_compression) {
  uint16_t u16_MemAddr;

  flags.u1_compressionFlag = u8_compression;
  if (u8_compression) ADPCMDecoderInit();
  flags.u1_activeBuffer = 0;
  flags.u1_readFlag = 0;
  u8_sampleCount = 0;
  u8_bufferCount = 0;
  u16_MemAddr = 0;     //start at location 0 in memory
  //playback mode
  memReadLC515(EEPROM,u16_MemAddr,au8_bufferA); //first buffer read
  u16_MemAddr = u16_MemAddr +64;
  flags.u1_playbackFlag = 1;
  while (!isCharReady()) {
    if (flags.u1_activeBuffer)
      memReadLC515(EEPROM,u16_MemAddr,au8_bufferA); // do read
    else
      memReadLC515(EEPROM,u16_MemAddr,au8_bufferB); // do read
    u16_MemAddr = u16_MemAddr +64;
    if (flags.u1_writeFlag) reportError("Playback underflow");
    while (!flags.u1_readFlag) doHeartbeat();
    flags.u1_readFlag = 0;
    outChar('*');
  }//end while
  inChar();
  outString("\nPlayback terminated.\n");
  flags.u1_playbackFlag = 0;
  flags.u1_compressionFlag = 0;
}

void doSampledPassthru(void) {
  flags.u1_passThruFlag = 1;
  while (!isCharReady()) doHeartbeat();
  inChar();
  flags.u1_passThruFlag = 0;
  outString("\nFeedthru terminated.\n");
}

//just examine raw bytes
void doTest(void) {
  _T3IE = 0;
  while (!isCharReady()) {
    doHeartbeat();
    while (!_T3IF);
    _T3IF = 0;
    u16_adcVal = ADC1BUF0;
    //start next sample
    SET_SAMP_BIT_ADC1();
    outUint16(u16_adcVal);
    outString("\n");
  }
  _T3IE = 1;
}

int main (void) {

  uint8_t u8_mode;

  configBasic(HELLO_MSG);
  CONFIG_AN0_AS_ANALOG();
  configADC1_ManualCH0( ADC_CH0_POS_SAMPLEA_AN0, 31, 1 );
  SET_SAMP_BIT_ADC1();   //start first conversion
  configSPI1();
  configDAC();
  configTimer3();
  configI2C1(400);            //configure I2C for 400 KHz
  while (1) {
    outString("\nEnter 'r' (record), 'p' (playback), 'f' (sampled feedthru). 'R', 'P' use compression. Any key terminates: ");
    u8_mode = inCharEcho();
    outString("\n");
    switch (u8_mode) {
      case 'r':
        doRecord(0);
        break;
      case 'R':
        doRecord(1);
        break;
      case 'p':
        doPlayback(0);
        break;
      case 'P':
        doPlayback(1);
        break;
      case 'f':
        doSampledPassthru();
        break;
      case 't':
        doTest();
        break;

    }
  } //end while(1)
}
