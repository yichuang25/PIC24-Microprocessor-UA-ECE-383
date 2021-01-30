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
A simple program that flashes the power LED, does not use I/O macros.
RB15 is configured as an open drain output, and drives an LED at
the junction between the LED and a pullup resistor to VDD. When
RB15 is driven low, the LED is off. When RB15 is driven high,
the RB15 output floats because of the open drain, and so the
LED is turned on by the pullup resistor to VDD. This allows
the power LED to function as a 'blinky' LED in addition to serving
as a power indicator.
*/

//a naive software delay function
void a_delay(void) {
  uint16_t u16_i,u16_k;
  // change count values to alter delay
  for (u16_k=1800; --u16_k;) {
    for (u16_i = 1200 ; --u16_i ;);
  }
}


int main(void) {
  configClock();    //clock configuration
  /********** GPIO config **********/
#ifdef _ODB15          //PIC24F CPU header files define this instead of ODCB15
  _ODB15 = 1;          //enable open drain
#else
  _ODCB15 = 1;          //enable open drain
#endif
  _TRISB15 = 0;         //Config RB15 as output
  _LATB15 = 0;          //RB15 initially low
  while (1) {           //infinite while loop
    a_delay();          //call delay function
    _LATB15 = !_LATB15;  //Toggle LED attached to RB15
  } // end while (1)
}
