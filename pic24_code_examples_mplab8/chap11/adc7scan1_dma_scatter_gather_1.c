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


#ifndef _DMA0IF
#error "This processor selection does not have a DMA module; this code example is incompatible with a PIC24 CPU that does not have DMA."

#endif

/** \file
 *  Performs a basic config of the ADC and samples seven channels sequentially
 *  with automatic channel scanning.  ADC values are 10-bit results.
 *  Samples are obtained continuously, and DMA is used to write a memory buffer.
 *  The DMA is done using Scatter/Gather mode, with a block size determined by CONVERSIONS_PER_INPUT.
 *  For block size = 1 and scatter/gather mode, this
 *  means that AN0 result is written to DMA buffer[0], AN1 result to the DMA buffer[1],
 *  AN15 to DMA buffer[15], etc.
 *  Uses DMA completion interrupts to get values from the DMA memory buffer.
 *  The DMA mode for Scatter/Gather mode should be DMA_AMODE_PERIPHERAL_INDIRECT (peripheral indirect)
 *  Main routine fetches the "latest" values  from memory.
 *  This code assumes a maximum of 16 ANx inputs.
 *  For block size > 1, the main code averages the values for a given channel.
 *  Compare the main averaging code with 'adc7scan1_dma2.c' to see the difference in
 *  how results are stored in the buffer for 'ordered' mode versus 'scatter/gather'.
 *
 *  Conversion results are printed to screen to match adc2pots1.c project
 *  (HEX values and voltages are printed.)
 *  This is only for PIC24 CPUs with DMA.
*/

#define CONFIG_LED2()       CONFIG_RB5_AS_DIG_OUTPUT()
#define LED2                _LATB5


// uncomment the next line to setup this project for a 12-bit ADC
//#define USE_12BIT_ADC

#ifdef  USE_12BIT_ADC
#define   ADC_LEN           12
#define   ADC_NSTEPS        4096
#define   ADC_12BIT_FLAG    1
#else
#define   ADC_LEN           10
#define   ADC_NSTEPS        1024
#define   ADC_12BIT_FLAG    0
#endif



/***
 *** HERE IS THE CODE!
 ***
 ***/

// set this to one of the values of
// 1, 2, 4, 8, 16, 32, 64, 128
#define CONVERSIONS_PER_INPUT  4
#define MAX_CHANNELS   16
//DMA transfer size is in words.
#define MAX_TRANSFER (CONVERSIONS_PER_INPUT*MAX_CHANNELS)   //make power of two for alignment to work

//DMA buffers, alignment is based on number of bytes
uint16_t au16_bufferA[MAX_TRANSFER] __attribute__((space(dma),aligned(MAX_TRANSFER*2)));

//generic DMA/ADC configuration function, enables scanning, uses DMA channel 0
//returns the number of channels that are scanned as specified by the mask.
uint8_t configDMA_ADC(uint16_t   u16_ch0ScanMask, \
                      uint8_t    u8_autoSampleTime, \
                      uint8_t    u8_use12bit,
                      uint8_t    u8_useScatterGather,
                      uint8_t    u8_dmaLocsPerInput) {
  uint8_t     u8_i, u8_nChannels=0;
  uint16_t    u16_mask = 0x0001;
  uint16_t    u16_dmaMode;


  // compute the number of Channels the user wants to scan over
  for (u8_i=0; u8_i<16; u8_i++) {
    if (u16_ch0ScanMask & u16_mask)
      u8_nChannels++;
    u16_mask<<=1;
  } //end for

  if (u8_autoSampleTime > 31) u8_autoSampleTime=31;

  AD1CON1bits.ADON = 0;   // turn off ADC (changing setting while ADON is not allowed)
  /** Configure the internal ADC **/
  AD1CON1 = ADC_CLK_AUTO | ADC_AUTO_SAMPLING_ON;
#ifdef _AD12B
  if (u8_use12bit)
    AD1CON1bits.AD12B = 1;
  else
    AD1CON1bits.AD12B = 0;
#endif
  if (u8_useScatterGather) {
    AD1CON1bits.ADDMABM = 0;
    u16_dmaMode = DMA_AMODE_PERIPHERAL_INDIRECT;
  } else {
    //order mode
    AD1CON1bits.ADDMABM = 1;
    u16_dmaMode = DMA_AMODE_REGISTER_POSTINC;
  }
  //AD1CON3 = ADC_CONV_CLK_INTERNAL_RC | (u8_autoSampleTime<<8);
  //at FCY = 40 MHz, Tcy = 25 ns, and use ADC clock = 10* Tcy = 10 * 25 ns = 250 ns
  //use clock based on Tcy so that we can accurately measure ADC clock period
  AD1CON3 = ADC_CONV_CLK_SYSTEM | (u8_autoSampleTime<<8) |ADC_CONV_CLK_10Tcy;
  //Note: PIC24H family reference manual (16.13.2) says that for 'ordered' mode, the
  //SMPI bits should be cleared. However, when scanning, this seems to be incorrect as the
  //settings that work are the same ones used for 'scatter/gather' mode.
  AD1CON2 = ADC_VREF_AVDD_AVSS | ADC_CONVERT_CH0 | ADC_SCAN_ON | ((u8_nChannels-1)<<2);

#if (defined(__PIC24H__)|| defined(__dsPIC33F__))
  AD1CHS0 = ADC_CH0_NEG_SAMPLEA_VREFN;
#else
  AD1CHS = ADC_CH0_NEG_SAMPLEA_VREFN;
#endif
  AD1CSSL = u16_ch0ScanMask;

  switch (u8_dmaLocsPerInput) {
    case 1   :
      AD1CON4 = ADC_1_WORD_PER_INPUT;
      break;
    case 2   :
      AD1CON4 = ADC_2_WORD_PER_INPUT;
      break;
    case 4   :
      AD1CON4 = ADC_4_WORD_PER_INPUT;
      break;
    case 8   :
      AD1CON4 = ADC_8_WORD_PER_INPUT;
      break;
    case 16  :
      AD1CON4 = ADC_16_WORD_PER_INPUT;
      break;
    case 32  :
      AD1CON4 = ADC_32_WORD_PER_INPUT;
      break;
    case 64  :
      AD1CON4 = ADC_64_WORD_PER_INPUT;
      break;
    case 128 :
      AD1CON4 = ADC_128_WORD_PER_INPUT;
      break;
    default:
      AD1CON4 = ADC_1_WORD_PER_INPUT;
      break;
  }

//configure the DMA channel 0 interrupt
  DMA0PAD = (unsigned int) &ADC1BUF0;
  DMA0REQ = DMA_IRQ_ADC1;
  DMA0STA = __builtin_dmaoffset(au16_bufferA);
  DMA0CNT = (u8_nChannels * u8_dmaLocsPerInput)-1;
  DMA0CON =   //configure and enable the module Module
    (DMA_MODULE_ON |
     DMA_SIZE_WORD |
     DMA_DIR_READ_PERIPHERAL |
     DMA_INTERRUPT_FULL |
     DMA_NULLW_OFF |
     u16_dmaMode |
     DMA_MODE_CONTINUOUS);

  _DMA0IF = 0;
  _DMA0IP = 6;
  _DMA0IE = 1;

  AD1CON1bits.ADON = 1;   // turn on the ADC
  return(u8_nChannels);
}

volatile  uint16_t    au16_buffer[MAX_TRANSFER];
volatile  uint8_t     u8_waiting;


void _ISRFAST _DMA0Interrupt(void) {
  uint8_t       u8_i;
  uint16_t*     au16_adcHWBuff = (uint16_t*) &au16_bufferA;
  _DMA0IF = 0;
  if (u8_waiting ) {
    for ( u8_i=0; u8_i<MAX_TRANSFER; u8_i++) {
      au16_buffer[u8_i] = au16_adcHWBuff[u8_i];
    } //end for()
    u8_waiting = 0;  // signal main() that data is ready
  }
  // toggle a port pin so that we can measure how often DMA IRQs are coming in
  LED2 = !LED2;
}



int main (void) {
  uint8_t   u8_i, u8_j, u8_k;
  uint16_t  u16_sum;
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

  configDMA_ADC( ADC_SCAN_AN0 | ADC_SCAN_AN1 | ADC_SCAN_AN4 |    \
                 ADC_SCAN_AN5 | ADC_SCAN_AN10 | ADC_SCAN_AN11 | ADC_SCAN_AN12,
                 31, ADC_12BIT_FLAG, 1, CONVERSIONS_PER_INPUT);

  u8_waiting = 1;
  while (1) {
    while (u8_waiting) {}; // wait for valid data in ISR
    //data is updated in array by DMA ISR when u8_waiting flag is cleared
    //iterate over channels, and average results for each channel
    //data in array will not be updated again by DMA ISR until u8_waiting flag is set.
    u8_k = 0;  //buffer index
    for ( u8_i=0; u8_i<16; u8_i++) {  //each channel
      for (u8_j=0; u8_j<CONVERSIONS_PER_INPUT; u8_j++) {  //each result per channel
        if (u8_j == 0) u16_sum = au16_buffer[u8_k];
        else u16_sum += au16_buffer[u8_k];
        u8_k++;
      }
      u16_pot = u16_sum/CONVERSIONS_PER_INPUT; //take the average
      f_pot = 3.3 / ADC_NSTEPS * u16_pot;
      printf("r");
      if (u8_i < 10) outChar( '0'+u8_i );
      else outChar( 'A'-10+u8_i );
      printf(":0x%04X=%1.3fV  ",  u16_pot, (double) f_pot );
      if ((u8_i % 4) == 3) printf("\n");
    } //end for()
    printf("\n");
    u8_waiting = 1;
    doHeartbeat();
    DELAY_MS(1500);
  } //endof while()
} // endof main()

