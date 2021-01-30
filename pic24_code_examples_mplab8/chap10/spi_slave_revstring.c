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
 *  SPI example: PIC24 uC in slave mode talking to PIC24 uC in master mode (spi_master_revstring.c),
 *  which is using this slave PIC24 uC to reverse strings.
*/
void reverseString(volatile char *psz_s1, volatile char *psz_s2);

#define CONFIG_SLAVE_ORDY() CONFIG_RB2_AS_DIG_OUTPUT()
#define SLAVE_ORDY _LATB2

typedef enum  {
  STATE_WAIT_FOR_STRING,
  STATE_SEND_REV_STRING,
  STATE_LAST_REVCHAR_STRING,
} STATE;

volatile STATE e_mystate = STATE_WAIT_FOR_STRING;
#define BUFSIZE 63
volatile char  sz_1[BUFSIZE+1];
volatile char  sz_2[BUFSIZE+1];
volatile uint16_t u16_index;

void _ISR _SPI1Interrupt (void) {
  uint16_t u16_tmp;
  switch (e_mystate) {
    case STATE_WAIT_FOR_STRING:
      //character arrived, place in buffer
      sz_1[u16_index] = SPI1BUF;
      u16_index++;
      if (sz_1[u16_index-1] == 0) {

        //have a complete string, reverse it.
        reverseString(sz_1,sz_2);
        //place first character of reversed string in SPIBUF
        u16_index = 0;
        SPI1BUF = sz_2[u16_index];
        u16_index++;
        SLAVE_ORDY = 1;  //indicate to MASTER that reversed string is ready
        e_mystate = STATE_SEND_REV_STRING;
      }
      break;
    case STATE_SEND_REV_STRING:
      u16_tmp = SPI1BUF; //must read the SPIBUF to prevent overflow, discard it.
      //just keep placing reversed characters in buffer as MASTER reads our SPI port
      SPI1BUF = sz_2[u16_index];
      u16_index++;
      if (sz_2[u16_index-1] == 0) {
        //this is the last character, no more data
        SLAVE_ORDY = 0;
        e_mystate = STATE_LAST_REVCHAR_STRING;
      }
      break;
    case STATE_LAST_REVCHAR_STRING:
      u16_index = 0;
      u16_tmp = SPI1BUF; //must read the SPIBUF to prevent overflow, discard it.
      //null terminator of reversed string just read, wait for next string
      e_mystate = STATE_WAIT_FOR_STRING;
      break;
    default:
      e_mystate = STATE_WAIT_FOR_STRING;
  }
  _SPI1IF = 0;  //clear interrupt flag
}

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

void configSPI1(void) {
  //no need for prescaler since master supplies the clock
  SPI1CON1 =  CLK_POL_ACTIVE_HIGH |
              SPI_CKE_ON          |
              SLAVE_ENABLE_ON     |
              SPI_MODE8_ON        |
              MASTER_ENABLE_OFF;

  //configure pins
#if (defined(__dsPIC33E__) || defined(__PIC24E__))
  //nothing to do here. On this family, the SPI1 port uses dedicated
  //pins for higher speed. The SPI2 port can be used with remappable pins.
  //you may need to add code to disable analog functionality if the SPI ports
  //are on analog-capable pins.
#else
  CONFIG_SDO1_TO_RP(6);      //use RP6 for SDO
  CONFIG_RP6_AS_DIG_PIN();   //Ensure that this is a digital pin
  CONFIG_SCK1IN_TO_RP(7);    //use RP7 for SCLK input
  CONFIG_RP7_AS_DIG_PIN();   //Ensure that this is a digital pin
  CONFIG_SDI1_TO_RP(5);      //use RP5 for SDI
  CONFIG_RP5_AS_DIG_PIN();   //Ensure that this is a digital pin
  CONFIG_SS1IN_TO_RP(3);     //use RP3 for SS#
  CONFIG_RP3_AS_DIG_PIN();   //Ensure that this is a digital pin
#endif
  CONFIG_SLAVE_ORDY();       //handshake to indicate output ready
  SLAVE_ORDY = 0;            //output is not ready
  u16_index = 0;             //no data yet
  _SPI1IF = 0;              //clear the flag
  _SPI1IP = 3;             //choose a priority
  _SPI1IE = 1;              //enable the interrupt
  SPI1STATbits.SPIROV = 0; //clear overflow flag
  SPI1STATbits.SPIEN = 1;  //enable SPI mode
}


int main (void) {
  configClock();        //no UART for slave
  configHeartbeat();
  configSPI1();
  while (1) doHeartbeat();
}
