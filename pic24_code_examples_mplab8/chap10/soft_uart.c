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
 *  Demonstrates a software driven UART TX/RX using PIO pins
*/

/// Soft UART Config
#define CONFIG_STX() CONFIG_RB2_AS_DIG_OUTPUT()
#define STX _LATB2     //STX state
#define CONFIG_SRX() CONFIG_RB3_AS_DIG_INPUT()
#define SRX _RB3     //SRX state
#define DEFAULT_SOFT_BAUDRATE 19200
uint16_t u16_softBaudRate = DEFAULT_SOFT_BAUDRATE;

void doBitDelay (uint16_t u16_baudRate) {
  if (u16_baudRate == 9600) {
    DELAY_US(106);
  } else if (u16_baudRate == 19200) DELAY_US(52);
}

void doBitHalfDelay (uint16_t u16_baudRate) {
  if (u16_baudRate == 9600) {
    DELAY_US(53);
  } else if (u16_baudRate == 19200) DELAY_US(26);
}

void outCharSoft(uint8_t u8_c) {
  uint8_t u8_i;
  STX = 0;
  doBitDelay(u16_softBaudRate);
  for (u8_i=0; u8_i<8; u8_i++) {
    if (u8_c & 0x01)
      STX = 1;
    else STX = 0;
    doBitDelay(u16_softBaudRate);
    u8_c = u8_c >> 1;
  }
  STX = 1;
  doBitDelay(u16_softBaudRate);
}

uint8_t inCharSoft(void) {
  uint8_t u8_i, u8_c;

  u8_c = 0x00;
  while (SRX) doHeartbeat();
  doBitHalfDelay(u16_softBaudRate);
  for (u8_i=0; u8_i<8; u8_i++) {
    doBitDelay(u16_softBaudRate);
    if (SRX) u8_c = u8_c | 0x80;
    if (u8_i != 7) u8_c = u8_c >> 1;
  }
  doBitDelay(u16_softBaudRate);
  return(u8_c);
}



int main (void) {
  uint8_t u8_c;

  configClock();
  configHeartbeat();
  /** GPIO config ***************************/
  CONFIG_STX();      //software TX
  STX = 1;           //should be high
  while (1) {
    u8_c = inCharSoft();      //get character
    u8_c++;               //increment the character
    outCharSoft(u8_c);    //echo the character
    doHeartbeat();
  }
  // End program
  return 0;
}
