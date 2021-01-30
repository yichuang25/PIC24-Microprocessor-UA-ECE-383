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
DMA Example: Uses DMA in ping-pong mode to
capture data from the UART, write it to the
24LC515 EEPROM.
*/

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

#define DMA_TRANSFER_SIZE  BLKSIZE
//DMA buffers
uint8_t au8_bufferA[DMA_TRANSFER_SIZE] __attribute__((space(dma)));
uint8_t au8_bufferB[DMA_TRANSFER_SIZE] __attribute__((space(dma)));
//some one-bit flags
typedef struct tagFLAGBITS {
unsigned u1_activeBuffer:
  1;
unsigned u1_writeFlag:
  1;
} FLAGBITS;
volatile FLAGBITS flags;

void configDMA0() {
  DMA0PAD = (unsigned int) &U1RXREG;      //peripheral address to read
  DMA0REQ = DMA_IRQ_U1RX;  //source from UART1 RX
  _U1RXIF = 0;             //clear the UART RX IF flag
//set up ping pong buffer registers
  DMA0STA = __builtin_dmaoffset(au8_bufferA);
  DMA0STB = __builtin_dmaoffset(au8_bufferB);
//setup transfer size
  DMA0CNT =   DMA_TRANSFER_SIZE -1;
  DMA0CON =             //configure and enable the module Module
    (DMA_MODULE_ON |
     DMA_SIZE_BYTE |
     DMA_DIR_READ_PERIPHERAL |
     DMA_INTERRUPT_FULL |
     DMA_NULLW_OFF |
     DMA_AMODE_REGISTER_POSTINC |
     DMA_MODE_CONTINUOUS_PING_PONG);
//enable the UART1RX Error interrupt
  _U1EIF = 0;
  _U1EIP = 1;
  _U1EIE = 1;
//enable DMA channel 0 interrupt
  _DMA0IF = 0;
  _DMA0IP = 2;
  _DMA0IE = 1;
}

//UART error interrupt, need this with DMA since DMA does not check for errors.
void _ISRFAST _U1ErrInterrupt(void) {
  _U1EIF = 0;      //clear the UART1 Error flag
  checkRxErrorUART1();
}

//interrupted when a ping-pong buffer is full
void _ISRFAST _DMA0Interrupt(void) {
  _DMA0IF = 0;
  flags.u1_activeBuffer = !flags.u1_activeBuffer;
  flags.u1_writeFlag = 1;
}

int main (void) {
  uint8_t au8_eepromBuf[BLKSIZE];  //holds read data from EEPROM
  uint16_t u16_MemAddr;
  uint8_t u8_mode, u8_pause;
  configBasic(HELLO_MSG);
  configI2C1(400);            //configure I2C for 400 KHz
  outString("\nEnter 'w' for write mode, anything else reads: ");
  u8_mode = inCharEcho();
  outString("\n");
  u16_MemAddr = 0;     //start at location 0 in memory
  u8_pause = 0;
  while (1) {
    uint8_t u8_i;
    if (u8_mode == 'w') {
      configDMA0();
      while (1) {
        if (flags.u1_writeFlag) reportError("DMA overflow!\n");
        while (!flags.u1_writeFlag) doHeartbeat();
        flags.u1_writeFlag = 0;
        if (flags.u1_activeBuffer)
          memWriteLC515(EEPROM,u16_MemAddr,au8_bufferA);
        else
          memWriteLC515(EEPROM,u16_MemAddr,au8_bufferB);
        u16_MemAddr = u16_MemAddr +64;
        outString("*");
      }//end while(1), press reset to escape
    } else {
      while (1) {
        if (u8_pause) {
          inChar();  //get a character
          u8_pause = 0;
        }
        memReadLC515(EEPROM,u16_MemAddr,au8_eepromBuf); // do read
        for (u8_i = 0; u8_i< 64; u8_i++) outChar(au8_eepromBuf[u8_i]);
        if (isCharReady()) {
          inChar();
          u8_pause = 1;
        }
        u16_MemAddr = u16_MemAddr + 64;
      }//end while(1) read mode, press reset to escape
    }//end else
  }//end while(1) main
}//end main
