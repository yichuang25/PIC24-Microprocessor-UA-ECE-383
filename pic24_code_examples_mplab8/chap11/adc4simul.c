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

// setup an output to help us measure ADC IRQ responses
#define CONFIG_LED2()       CONFIG_RB5_AS_DIG_OUTPUT()
#define LED2                _LATB5

#define   ADC_LEN           10
#define   ADC_NSTEPS        1024
#define   ADC_12BIT_FLAG    0

uint16_t              au16_buffer[8];
volatile  uint16_t    au16_sum[8];
volatile  uint8_t     u8_gotData;

/***
 *** HERE IS THE CODE!
 ***
 ***/
void _ISR _ADC1Interrupt (void) {
  static uint8_t      u8_adcCount=64;
  uint8_t             u8_i;
  uint16_t*           au16_adcHWBuff = (uint16_t*) &ADC1BUF0;

  /* If ADC is writing in the 2nd half of the buffer, so determine which
   * half of the buffer is valid (and caused this interrupt), then fetch
   * that half into our local array of ADC results.
   */
  if (AD1CON2 & ADC_ALT_BUF_STATUS_2) {
    for ( u8_i=0; u8_i<8; u8_i++) {
      au16_buffer[u8_i] += au16_adcHWBuff[u8_i];
    } //end for()
  } else {
    for ( u8_i=8; u8_i<16; u8_i++) {
      au16_buffer[u8_i-8] += au16_adcHWBuff[u8_i];
    } //end for()
  } // end if-else

  _AD1IF = 0;                   //clear the interrupt flag
  // we got the data, so start the sampling process again
  SET_SAMP_BIT_ADC1();

  u8_adcCount--;
  if (u8_adcCount==0) {
    u8_adcCount = 64;
    u8_gotData = 1;
    for ( u8_i=0; u8_i<8; u8_i++) {
      au16_sum[u8_i] = au16_buffer[u8_i];
      au16_buffer[u8_i] = 0;
    } //end for()
  };

  // toggle an LED so we can measure how often ADC IRQs are coming in
  LED2 = !LED2;
}

/** \file
 *  Simultaneously samples four channels and uses a timer used to trigger conversions.
 *  Ping-pong buffering is used.
 *  Conversion results are printed to screen.
 *  (HEX values and voltages are printed.)
 *  This is only for PIC24 CPUs without DMA.
*/

int main (void) {
  uint8_t   u8_i;
  uint16_t  u16_pot;
  uint32_t  u32_ticks;
  float   f_pot;

  configBasic(HELLO_MSG);

  // make RA0/AN0/VREF+ a digital input to kill the pullup and
  // set the TRISA bit, then make it ANALOG so the ADC will work
  CONFIG_AN0_AS_ANALOG();
  CONFIG_AN1_AS_ANALOG();
  CONFIG_AN2_AS_ANALOG();
  CONFIG_AN12_AS_ANALOG();

  CONFIG_LED2();

  u8_gotData = 0;

  // configure T2/T3 as 32-bit timer to trigger every 1/64 second
  T3CONbits.TON = 0;
  T2CONbits.TON = 0;
  T2CON = T2_32BIT_MODE_ON | T2_PS_1_1 | T2_SOURCE_INT;
  TMR3 = 0;
  TMR2 = 0;
  u32_ticks = usToU32Ticks(15625, getTimerPrescale(T2CONbits)) - 1;     // # of ticks for 1/64 seconds
  PR3 = u32_ticks>>16;
  PR2 = u32_ticks & 0xFFFF;
  T2CONbits.TON = 1;

  configADC1_Simul4ChanIrq(12, ADC_CH123_POS_SAMPLEA_AN0AN1AN2, ADC_CONV_CLK_10Tcy );
  SET_SAMP_BIT_ADC1();

  while (1) {
    while (!u8_gotData) {
      doHeartbeat();
    }
    u8_gotData = 0;
    for ( u8_i=0; u8_i<4; u8_i++) {
      u16_pot = au16_sum[u8_i];
      f_pot = (3.3 / 1023 / 64 ) * u16_pot;
      printf("r");
      outChar( '0'+u8_i );
#ifdef SMALLRAM
      {
        uint16_t ui16_potmv;
        ui16_potmv = f_pot*1000;
        printf(":0x%04X=%d mV  ",  u16_pot, ui16_potmv );
      }
#else
      printf(":0x%04X=%1.3fV  ",  u16_pot, (double) f_pot );
#endif
    } //end for()
    printf("\n");
  } //endof while()
} // endof main()
