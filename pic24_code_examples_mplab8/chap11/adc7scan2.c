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

#ifdef _DMA0IF
#error "This processor selection has the DMA module; this code example is incompatible with a PIC24 CPU that has DMA."

#endif

#define CONFIG_LED2()       CONFIG_RB0_AS_DIG_OUTPUT()
#define LED2                _LATB0

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

volatile  uint16_t    au16_buffer[16];
volatile  uint8_t     u8_waiting;

/***
 *** HERE IS THE CODE!
 ***
 ***/

void _ISR _ADC1Interrupt (void) {
  uint8_t       u8_i;
  uint16_t*     au16_adcHWBuff = (uint16_t*) &ADC1BUF0;

  /* If ADC is writing in the 2nd half of the buffer, so determine which
   * half of the buffer is valid (and caused this interrupt), then fetch
   * that half into our local array of ADC results.
   */
  if (AD1CON2bits.BUFS) {
    for ( u8_i=0; u8_i<8; u8_i++) {
      au16_buffer[u8_i] = au16_adcHWBuff[u8_i];
    } //end for()
  } else {
    for ( u8_i=8; u8_i<16; u8_i++) {
      au16_buffer[u8_i] = au16_adcHWBuff[u8_i];
    } //end for()
  } // end if-else

  AD1CON1bits.DONE = 0;               /* do we really need to clear this? */
  u8_waiting = 0;             // signal to main() that data is ready
  _AD1IF = 0;   //clear the interrupt flag

  // toggle an LED so we can measure how often ADC IRQs are coming in
  LED2 = !LED2;
}

/** \file
 *  Performs a basic config of the ADC and samples seven channels sequentially
 *  with automatic channel scanning.  ADC values are 12-bit results.
 *  Samples are obtained continuously.  Uses ADC completion interrupts to get
 *  values from ADCxBUFn registers.  Main routine fetches the "latest" values
 *  from memory.
 *
 *  Conversion results are printed to screen to match adc2pots1.c project
 *  (HEX values and voltages are printed.)
 *  This is only for PIC24 CPUs without DMA.
*/

int main (void) {
  uint8_t   u8_i;
  uint16_t  u16_pot;
  float   f_pot;

  configBasic(HELLO_MSG);

  // make RA0/AN0/VREF+ a digital input to kill the pullup and
  // set the TRISA bit, then make it ANALOG so the ADC will work
  CONFIG_AN0_AS_ANALOG();
  CONFIG_AN1_AS_ANALOG();
  CONFIG_AN4_AS_ANALOG();
  CONFIG_AN5_AS_ANALOG();
  CONFIG_AN10_AS_ANALOG();
  CONFIG_AN11_AS_ANALOG();
  CONFIG_AN12_AS_ANALOG();

  CONFIG_LED2();

  configADC1_AutoHalfScanIrqCH0(ADC_SCAN_AN0 | ADC_SCAN_AN1 | ADC_SCAN_AN4 |    \
                                ADC_SCAN_AN5 | ADC_SCAN_AN10 | ADC_SCAN_AN11 | ADC_SCAN_AN12,       \
                                31, ADC_12BIT_FLAG);

  // wait for first conversion cycle to finish before proceeding
  while ( !AD1CON1bits.DONE) {};

  while (1) {
    while(u8_waiting) {};      // wait for valid data from ISR
    u8_waiting = 1;
    for ( u8_i=0; u8_i<16; u8_i++) {
      u16_pot = au16_buffer[u8_i];
      f_pot = 3.3 / ADC_NSTEPS * u16_pot;
      printf("r");
      if (u8_i < 10)
        outChar( '0'+u8_i );
      else
        outChar( 'A'-10+u8_i );
#ifdef SMALLRAM
      {
        uint16_t ui16_potmv;
        ui16_potmv = f_pot*1000;
        printf(":0x%04X=%d mV  ",  u16_pot, ui16_potmv );
      }
#else
      printf(":0x%04X=%1.3fV  ",  u16_pot, (double) f_pot );
#endif
      if ((u8_i % 4) == 3) printf("\n");
    } //end for()
    printf("\n");

    doHeartbeat();
    DELAY_MS(1500);

  } //endof while()
} // endof main()

