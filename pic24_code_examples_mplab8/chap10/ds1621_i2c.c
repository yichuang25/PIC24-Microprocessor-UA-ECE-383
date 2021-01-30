
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
I2C Example: Demonstrates I2C functions by reading/writing
from a Maxim DS1621 Digital Thermometer.

*/

#define DS1621ADDR 0x90   //DS1621 address with all pins tied low
#define ACCESS_CONFIG 0xAC
#define START_CONVERT 0xEE
#define READ_TEMP 0xAA

void writeConfigDS1621(uint8_t u8_i) {
  write2I2C1(DS1621ADDR, ACCESS_CONFIG, u8_i);
}

void startConversionDS1621() {
  write1I2C1(DS1621ADDR, START_CONVERT);
}

int16_t readTempDS161() {
  uint8_t u8_lo, u8_hi;
  int16_t i16_temp;
  write1I2C1(DS1621ADDR, READ_TEMP);
  read2I2C1 (DS1621ADDR, &u8_hi, &u8_lo);
  i16_temp = u8_hi;
  return ((i16_temp<<8)|u8_lo);
}

int main (void) {
  int16_t i16_temp;
  float  f_tempC,f_tempF;
  configBasic(HELLO_MSG);
  configI2C1(400);            //configure I2C for 400 KHz
  writeConfigDS1621(0x00);    //continuous conversion
  startConversionDS1621();    //start conversions
  while (1) {
    DELAY_MS(1500);
    i16_temp = readTempDS161();
    f_tempC = i16_temp;  //convert to floating point
    f_tempC = f_tempC/256;  //divide by precision
    f_tempF = f_tempC*9/5 + 32;
#ifdef SMALLRAM
    {
      //use integers to avoid floating point printf which does not fit in this data space
      int16_t i16_tempC,i16_tempF;
      i16_tempC = f_tempC;
      i16_tempF = f_tempF;
      printf("Temp is: 0x%0X, %d (C), %d (F)\n",  i16_temp, i16_tempC, i16_tempF);
    }

#else
    printf("Temp is: 0x%0X, %4.4f (C), %4.4f (F)\n", i16_temp, (double) f_tempC, (double) f_tempF);
#endif
  }

}
