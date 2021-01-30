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
I2C Example: Demonstrates I2C functions by reading/writing
from a Microchip 24LC515 serial EEPROM.

*/

#define EEPROM 0xA0   //LC515 address assuming both address pins tied low.
#define BLKSIZE (64)

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

void memWriteLC515(uint8_t u8_i2cAddr,  uint16_t u16_MemAddr, uint8_t *pu8_buf) {
  uint8_t u8_AddrLo, u8_AddrHi;

  u8_AddrLo = u16_MemAddr & 0x00FF;
  u8_AddrHi = (u16_MemAddr >> 8);
  pu8_buf[0] = u8_AddrHi;   //place address into buffer
  pu8_buf[1] = u8_AddrLo;

  if (u16_MemAddr & 0x8000) {
    // if MSB set , set block select bit
    u8_i2cAddr = u8_i2cAddr | 0x08;
  }
  waitForWriteCompletion(u8_i2cAddr);
  writeNI2C1(u8_i2cAddr,pu8_buf,BLKSIZE+2);
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


int main (void) {
  uint8_t au8_buf[BLKSIZE+2];  //2 extra bytes for address
  uint16_t u16_MemAddr;
  uint8_t u8_Mode;

  configBasic(HELLO_MSG);
  configI2C1(400);            //configure I2C for 400 KHz
  outString("\nEnter 'w' for write mode, anything else reads: ");
  u8_Mode = inCharEcho();
  outString("\n");
  u16_MemAddr = 0;     //start at location 0 in memory
  while (1) {
    uint8_t u8_i;
    if (u8_Mode == 'w') {
      outString("Enter chars.\n");
      //first two buffer locations reserved for starting address
      for (u8_i = 2; u8_i< BLKSIZE+2; u8_i++) {
        au8_buf[u8_i] = inCharEcho();
      }
      outString("\nDoing Write\n");
      // write same string twice to check Write Busy polling
      memWriteLC515(EEPROM,u16_MemAddr, au8_buf); // do write
      u16_MemAddr = u16_MemAddr + BLKSIZE;
      memWriteLC515(EEPROM,u16_MemAddr,au8_buf); // do write
      u16_MemAddr = u16_MemAddr + BLKSIZE;
    } else {
      memReadLC515(EEPROM,u16_MemAddr,au8_buf); // do read
      for (u8_i = 0; u8_i< BLKSIZE; u8_i++) outChar(au8_buf[u8_i]);
      outString("\nAny key continues read...\n");
      inChar();
      u16_MemAddr = u16_MemAddr + BLKSIZE;
    }
  }
}
