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
I2C Example: Demonstrates a PIC24 CPU
acting as an I2C slave. The PIC24 slave responds
to both write and read transactions.
*/


#define SLAVE_I2C_ADDR 0x60

int16_t getStringLength(char* psz_1) {
  uint16_t u16_len;
  u16_len = 0;
  while (*psz_1) {
    psz_1++;
    u16_len++;
  }
  u16_len++;  //contains length of string including null
  return u16_len;
}

#define BUFSIZE 64
char sz_1[BUFSIZE];

int main (void) {
  uint16_t u16_len;
  configBasic(HELLO_MSG);
  configI2C1(400);            //configure I2C for 400 KHz
  while (1) {
    inStringEcho(sz_1,BUFSIZE);       //get a string from the console
    if (*sz_1 == 0) continue;         //don't send empty string
    u16_len = getStringLength(sz_1);  //determine string length
    writeNI2C1(SLAVE_I2C_ADDR,(uint8_t *)sz_1,u16_len);   //send the string
    readNI2C1(SLAVE_I2C_ADDR, (uint8_t *) sz_1,u16_len) ;  //read the reversed string
    outString(sz_1);
    outString("\n");
  }
}

