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

void reverseString(volatile char *psz_s1, volatile char *psz_s2) {
  volatile char *psz_s1end;
  if (!(*psz_s1)) {
    *psz_s2 = 0;  //psz_s1 is empty, return.
    return;
  }
  psz_s1end = psz_s1;
  //find end of first string
  while (*psz_s1end) psz_s1end++;
  psz_s1end--;      //backup one to first non-zero byte
  //now copy to S2 in reverse order
  while (psz_s1end != psz_s1) {
    *psz_s2 = *psz_s1end;
    psz_s1end--;
    psz_s2++;
  }
  //copy last byte
  *psz_s2 = *psz_s1end;
  psz_s2++;
  //mark end of string
  *psz_s2 = 0;
}



#define SLAVE_I2C_ADDR 0x60

typedef enum  {
  STATE_WAIT_FOR_ADDR,
  STATE_WAIT_FOR_WRITE_DATA,
  STATE_SEND_READ_DATA,
  STATE_SEND_READ_LAST
} STATE;

volatile STATE e_mystate = STATE_WAIT_FOR_ADDR;
#define BUFSIZE 64
volatile char  sz_1[BUFSIZE+1];
volatile char  sz_2[BUFSIZE+1];
volatile uint16_t u16_index;


void _ISRFAST _SI2C1Interrupt(void) {
  uint8_t u8_c;
  _SI2C1IF = 0;
  switch (e_mystate) {
    case STATE_WAIT_FOR_ADDR:
      u8_c = I2C1RCV; //clear RBF bit for address
      u16_index = 0;
      //check the R/W bit and see if read or write transaction
      if (I2C1STATbits.R_W) {
        I2C1TRN = sz_2[u16_index++];  //get first data byte
        I2C1CONbits.SCLREL = 1;     //release clock line so MASTER can drive it
        e_mystate = STATE_SEND_READ_DATA; //read transaction
      } else e_mystate = STATE_WAIT_FOR_WRITE_DATA;
      break;
    case STATE_WAIT_FOR_WRITE_DATA:
      //character arrived, place in buffer
      sz_1[u16_index++] = I2C1RCV;  //read the byte
      if (sz_1[u16_index-1] == 0) {
        //have a complete string, reverse it.
        reverseString(sz_1,sz_2);
        e_mystate = STATE_WAIT_FOR_ADDR; //wait for next transaction
      }
      break;
    case STATE_SEND_READ_DATA:
      //just keep placing reversed characters in buffer as MASTER reads our I2C port
      I2C1TRN = sz_2[u16_index++];
      I2C1CONbits.SCLREL = 1;     //release clock line so MASTER can drive it
      if (sz_2[u16_index-1] == 0) e_mystate = STATE_SEND_READ_LAST;
      //this is the last character, after byte is shifted out, release the clock line again
      break;
    case STATE_SEND_READ_LAST:  //this is interrupt for last character finished shifting out
      e_mystate = STATE_WAIT_FOR_ADDR;
      break;
    default:
      e_mystate = STATE_WAIT_FOR_ADDR;
  }

}

int main (void) {
  configBasic(HELLO_MSG);
  configI2C1(400);            //configure I2C for 400 KHz
  I2C1ADD = SLAVE_I2C_ADDR>>1;   //initialize the address register
  _SI2C1IF = 0;
#ifdef _SI2C1IP
  _SI2C1IP = 1;
#else
#ifdef _SI2C1IP0
  _SI2C1IP0 = 1;    //header files can be inconsistent in how these are defined for PIC24H versus PIC24F
  _SI2C1IP1 = 0;
  _SI2C1IP2 = 0;
#endif
#endif
  _SI2C1IE = 1;                //enable ISR
  while (1) doHeartbeat();    //ISR does all work
}
