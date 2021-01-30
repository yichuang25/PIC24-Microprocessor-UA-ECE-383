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
 *  SPI example: PIC24 uC in Master mode to Microchip MCP41xxx Digital Potentiometer
*/

#define CONFIG_SLAVE_ENABLE() CONFIG_RB2_AS_DIG_OUTPUT()
#define SLAVE_ENABLE()        _LATB2 = 0  //low true assertion
#define SLAVE_DISABLE()       _LATB2 = 1


void configSPI1(void) {
  //spi clock = 40MHz/1*4 = 40MHz/4 = 10MHz
  SPI1CON1 = SEC_PRESCAL_1_1 |     //1:1 secondary prescale
             PRI_PRESCAL_4_1 |     //4:1 primary prescale
             CLK_POL_ACTIVE_HIGH | //clock active high (CKP = 0)
             SPI_CKE_ON          | //out changes active to inactive (CKE=1)
             SPI_MODE8_ON        | //8-bit mode
             MASTER_ENABLE_ON;     //master mode
  SPI1STATbits.SPIEN = 1;  //enable SPI mode
  //configure pins. Only need SDO, SCLK since POT is output only
#if (defined(__dsPIC33E__) || defined(__PIC24E__))
  //nothing to do here. On this family, the SPI1 port uses dedicated
  //pins for higher speed. The SPI2 port can be used with remappable pins.
  //you may need to add code to disable analog functionality if the SPI ports
  //are on analog-capable pins.
#else
  //all other families (PIC24H/PIC24F/dsPIC33F)
  CONFIG_SDO1_TO_RP(6);      //use RP6 for SDO
  CONFIG_RP6_AS_DIG_PIN();   //ensure that analog is disabled
  CONFIG_SCK1OUT_TO_RP(7);   //use RP7 for SCLK
  CONFIG_RP7_AS_DIG_PIN();   //ensure that analog is disabled
#endif
  CONFIG_SLAVE_ENABLE();       //chip select for MCP41xxx
  SLAVE_DISABLE();             //disable the chip select
}

void setPotWiper(uint8_t u8_i) {
  SLAVE_ENABLE();         //assert MCP41xxx chipselect
  ioMasterSPI1(0x11);   //command byte to select wiper register
  ioMasterSPI1(u8_i);
  SLAVE_DISABLE();         //negate MCP41xxx chipselect
}

#define BUFSIZE 15
char  sz_1[BUFSIZE+1];

int main (void) {
  uint16_t u16_pv;
  configBasic(HELLO_MSG);
  configSPI1();
  while (1) {
    outString("Input decimal value (0-255): ");
    inString(sz_1,BUFSIZE);
    sscanf(sz_1,"%d",  (int *) &u16_pv);
    printf("\nSending %d to pot.\n",u16_pv);
    setPotWiper(u16_pv & 0x00FF);
  }
}
