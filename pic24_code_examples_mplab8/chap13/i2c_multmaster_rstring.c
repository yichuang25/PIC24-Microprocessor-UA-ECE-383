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

#define CPU1 1
#define CPU2 2

#if (CPU_ID == CPU1)
#define SLAVE_I2C_ADDR 0x60
#define MY_ADDR 0x68
#else
#define SLAVE_I2C_ADDR 0x68
#define MY_ADDR 0x60
#endif

typedef enum  {
  STATE_WAIT_FOR_ADDR,
  STATE_WAIT_FOR_WRITE_DATA,
} STATE;

volatile STATE e_mystate = STATE_WAIT_FOR_ADDR;
#define BUFSIZE 64
char sz_1[BUFSIZE];
char sz_2[BUFSIZE];
volatile uint8_t u8_gotString = 0;
volatile uint16_t u16_index;

void _ISRFAST _SI2C1Interrupt(void) {
  uint8_t u8_c;
  _SI2C1IF = 0;
  switch (e_mystate) {
    case STATE_WAIT_FOR_ADDR:
      u8_c = I2C1RCV; //clear RBF bit for address
      u16_index = 0;
      e_mystate = STATE_WAIT_FOR_WRITE_DATA;
      break;
    case STATE_WAIT_FOR_WRITE_DATA:
      //character arrived, place in buffer
      sz_1[u16_index++] = I2C1RCV;  //read the byte
      if (sz_1[u16_index-1] == 0) {
        u8_gotString = 1; //set the semaphore
        e_mystate = STATE_WAIT_FOR_ADDR; //wait for next transaction
      }
      break;
    default:
      e_mystate = STATE_WAIT_FOR_ADDR;
  }
}

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

void reverseString(char *psz_s1, char *psz_s2) {
  char *psz_s1end;
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


int main (void) {
  uint16_t u16_len;
  configBasic(HELLO_MSG);
  configI2C1(400);            //configure I2C for 400 KHz
  I2C1ADD = MY_ADDR>>1;   //initialize the address register
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

  _SI2C1IE = 1;
  while (1) {
    u8_gotString = 0;  //clear semaphore
#if (CPU_ID == CPU1)
    inStringEcho(sz_2,BUFSIZE);       //get a string from the console
    if (*sz_2 == 0) continue;         //don't send empty string
    u16_len = getStringLength(sz_2);  //determine string length
    writeNI2C1(SLAVE_I2C_ADDR,(uint8_t *)sz_2,u16_len);   //send the string
    //now wait for the reversed string to come back
    while (!u8_gotString) doHeartbeat();
    outString(sz_1);
    outString("\n");
#else
    while (!u8_gotString) doHeartbeat();  //wait from string from CPU1
    reverseString(sz_1,sz_2); //reverse it
    u16_len=getStringLength(sz_2);
    writeNI2C1(SLAVE_I2C_ADDR,(uint8_t *)sz_2,u16_len);   //send the string back
#endif

  }
}

