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
#include "stdio.h"

// uncomment the next line to setup this project for a 12-bit ADC
#define USE_12BIT_ADC

#ifdef  USE_12BIT_ADC
#define   ADC_LEN           12
#define   ADC_NSTEPS        4096
#define   ADC_12BIT_FLAG    1
#else
#define   ADC_LEN           10
#define   ADC_NSTEPS        1024
#define   ADC_12BIT_FLAG    0
#endif

/** \file
 *  Performs a basic config of the ADC and samples two channels manually
 *  and sequentially with 12-bit results.
 *  Conversion results are printed to screen as both HEX values and voltages.
*/

int main (void) {
  uint16_t  u16_pot1, u16_pot2;
  float   f_pot1, f_pot2;

  configBasic(HELLO_MSG);

  // make RA0/AN0/VREF+ a digital input to kill the pullup and
  // set the TRISA bit, then make it ANALOG so the ADC will work
  CONFIG_AN0_AS_ANALOG();
  CONFIG_AN1_AS_ANALOG();

  while (1) {
    configADC1_ManualCH0( ADC_CH0_POS_SAMPLEA_AN0, 31, ADC_12BIT_FLAG );
    u16_pot1 = convertADC1();
    configADC1_ManualCH0( ADC_CH0_POS_SAMPLEA_AN1, 31, ADC_12BIT_FLAG );
    u16_pot2 = convertADC1();

    f_pot1 = 3.30 / ADC_NSTEPS * u16_pot1;
    f_pot2 = 3.30 / ADC_NSTEPS * u16_pot2;
#ifdef SMALLRAM
    {
      //use integers to avoid floating point printf which does not fit in this data space
      uint16_t ui16_pot1mv,ui16_pot2mv;
      ui16_pot1mv = f_pot1*1000;
      ui16_pot2mv = f_pot2*1000;
      printf("AN0 is 0x%0X or %d mV. |  AN1 is 0x%0X or %d mV.\n",    \
             u16_pot1, ui16_pot1mv, u16_pot2, ui16_pot2mv );
    }

#else
    printf("AN0 is 0x%0X or %1.4fV. |  AN1 is 0x%0X or %1.4fV.\n",    \
           u16_pot1, (double) f_pot1, u16_pot2, (double) f_pot2 );
#endif
    DELAY_MS(1500);

  } //endof while()
} // endof main()

