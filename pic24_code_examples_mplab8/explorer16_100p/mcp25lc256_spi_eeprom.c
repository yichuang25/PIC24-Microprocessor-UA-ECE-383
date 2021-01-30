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
SPI Example: Demonstrates SPI functions by reading/writing
from that Microchip 25LC256 serial EEPROM that is on the
Explorer-16 board.

*/


#define CONFIG_SLAVE_ENABLE() CONFIG_RD12_AS_DIG_OUTPUT()
#define SLAVE_ENABLE()        _LATD12 = 0  //low true assertion
#define SLAVE_DISABLE()       _LATD12 = 1

#define EEPROM_RDSR  0x05   //read status register command
#define EEPROM_READ  0x03   //read command
#define EEPROM_WRITE 0x02   //write command
#define EEPROM_WENABLE 0x06   //write command

#define BLKSIZE 64

void configSPI2(void) {
  //spi clock = 40MHz/1*4*4 = 40MHz/16 = 2.5 MHz
  SPI2CON1 = SEC_PRESCAL_4_1 |     //4:1 secondary prescale
             PRI_PRESCAL_4_1 |     //4:1 primary prescale
             CLK_POL_ACTIVE_HIGH | //clock active high (CKP = 0)
             SPI_CKE_ON          | //out changes active to inactive (CKE=1)
             SPI_MODE8_ON        | //8-bit mode
             MASTER_ENABLE_ON;     //master mode
  SPI2STATbits.SPIEN = 1;  //enable SPI mode
  CONFIG_SLAVE_ENABLE();       //chip select for MCP41xxx
  SLAVE_DISABLE();             //disable the chip select
}

//Assumes WDT is configured for longer than EEPROM write time
void waitForWriteCompletion() {
  uint8_t u8_spidata,u8_savedSWDTEN;
  u8_savedSWDTEN = _SWDTEN;
  _SWDTEN = 1; //enable WDT so that do not get stuck in infinite loop!
  do {
    SLAVE_ENABLE();
    u8_spidata = ioMasterSPI2(EEPROM_RDSR); //send read status command
    u8_spidata = ioMasterSPI2(0);   //get status back
    SLAVE_DISABLE();
  } while (u8_spidata & 0x01);  //LSB of status is write-in-progress flag
  _SWDTEN = u8_savedSWDTEN;  //restore WDT to original state
}

//Write enable must be done before attempting a write.
void writeEnable() {
  SLAVE_ENABLE();
  ioMasterSPI2(EEPROM_WENABLE);
  SLAVE_DISABLE();
}

void memWrite25LC256(uint16_t u16_MemAddr, uint8_t *pu8_buf) {
  uint8_t u8_AddrLo, u8_AddrHi;
  uint8_t u8_i;

  u8_AddrLo = u16_MemAddr & 0x00FF;
  u8_AddrHi = (u16_MemAddr >> 8);

  waitForWriteCompletion();
  writeEnable(); //enable the write
  SLAVE_ENABLE();
  ioMasterSPI2(EEPROM_WRITE);
  ioMasterSPI2(u8_AddrHi);
  ioMasterSPI2(u8_AddrLo);
  for (u8_i=0; u8_i< BLKSIZE; u8_i++) {
    ioMasterSPI2(pu8_buf[u8_i]);
  }
  SLAVE_DISABLE();
}

void memRead25LC256(uint16_t u16_MemAddr, uint8_t *pu8_buf) {
  uint8_t u8_AddrLo, u8_AddrHi;
  uint8_t u8_i;

  waitForWriteCompletion();
  u8_AddrLo = u16_MemAddr & 0x00FF;
  u8_AddrHi = (u16_MemAddr >> 8);
  SLAVE_ENABLE();
  ioMasterSPI2(EEPROM_READ);
  ioMasterSPI2(u8_AddrHi);
  ioMasterSPI2(u8_AddrLo);
  for (u8_i=0; u8_i<BLKSIZE ; u8_i++) {
    pu8_buf[u8_i] = ioMasterSPI2(0) ;  //get one byte
  }
  SLAVE_DISABLE();
}


int main (void) {
  uint8_t au8_buf[BLKSIZE];  //holds data for EEPROM I/O
  uint16_t u16_MemAddr;
  uint8_t u8_Mode;

  configBasic(HELLO_MSG);
  configSPI2();             //configure SPI2 Module
  outString("\nEnter 'w' for write mode, anything else reads: ");
  u8_Mode = inCharEcho();
  outString("\n");
  u16_MemAddr = 0;     //start at location 0 in memory
  while (1) {
    uint8_t u8_i;
    if (u8_Mode == 'w') {
      outString("Enter 64 chars.\n");
      //first two buffer locations reserved for starting address
      for (u8_i = 0; u8_i< BLKSIZE; u8_i++) {
        au8_buf[u8_i] = inCharEcho();
      }
      outString("\nDoing Write\n");
      // write same string twice to check Write Busy polling
      memWrite25LC256(u16_MemAddr, au8_buf); // do write
      u16_MemAddr = u16_MemAddr + BLKSIZE;
      memWrite25LC256(u16_MemAddr,au8_buf); // do write
      u16_MemAddr = u16_MemAddr + BLKSIZE;
    } else {
      memRead25LC256(u16_MemAddr,au8_buf); // do read
      for (u8_i = 0; u8_i< BLKSIZE; u8_i++) outChar(au8_buf[u8_i]);
      outString("\nAny key continues read...\n");
      inChar();
      u16_MemAddr = u16_MemAddr + BLKSIZE;
    }
  }
}
