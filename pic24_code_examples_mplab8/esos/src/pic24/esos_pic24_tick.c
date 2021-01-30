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

// Documentation for this file. If the \file tag isn't present,
// this file won't be documented.
/**
* \file
* Hardware-specific file (Microchip PIC24 Family) to generate
* the ESOS system tick functions.
*/

//#include    "esos_irq.h"
#include    "esos_pic24.h"

volatile  uint32        esos_tick_count;
volatile  uint8         sub_tick;

// prototype for the ESOS timer service function
extern void __esos_tmrSvcsExecute(void);

/****************************************************/
/*
* \brief Increments the ESOS system tick
*
* \pre  TMR1 is initialized with __esos_hw_InitSystemTick
*
* This ISR provides the mechanism for incrementing ESOS's
* 1.0ms system tick.  Using Timer 1, we can call this ISR
* every 1.0ms and increment esos_tick_count.  Or, we can
* have T1 call this ISR every X ms, and increment the
* value esos_tick_count by X
*
* \note The ESOS system tick will overflow in 2^32 msec
********************************************************/
void _ISRFAST _T1Interrupt (void) {
  esos_tick_count++;            // increment the ESOS system tick by
  _T1IF = 0;                    // clear the timer interrupt bit
  __esos_tmrSvcsExecute();      // let ESOS implement the S/w tmr service
}

/****************************************************/
/*
* \brief Initializes the ESOS system tick.
*
* \pre None assumed
*
* \post Sets up the PIC24 MCU's Timer1 to generate the 1.0ms tick
* required by ESOS.
*
* The (platform-independent) ESOS initialization code will
* call this function to setup and init the hardware (PIC24
* MCU, in this case) to create the required IRQs to generate
* the 1.0ms ESOS system tick.
*
* \note We can either generate an IRQ every 1.0ms or longer period,
* we just need to make sure that ISR that increments the tick
* count is consistent.
********************************************************/
void    __esos_hw_InitSystemTick(void) {

  // FOR NOW, we will init our usual PIC24 development setup here.
  //  THIS REALLY DOESN'T BELONG HERE!!!!!!!
  configClock();        				//config clock

  /* Configure Timer1 to:
  **     operate during IDLE, use a 64x prescaler, and the internal clock
  */
  T1CON = T1_IDLE_CON + T1_PS_1_64 + T1_SOURCE_INT;

  PR1 = MS_TO_TICKS(1, 64);        // 1 ms interrupt interval
  TMR1  = 0;                       // clear T1's count
  _T1IF = 0;                       // clear interrupt flag

  /* set T1's priority to be the highest possible for a PIC24 user IRQ
  ** User IRQs (if used) will be a lower IRQ priority, so the tick will
  ** get serviced in a timely manner
  */
  _T1IP = 7;
  _T1IE = 1;                       // enable the interrupt
  T1CONbits.TON = 1;               // turn on the timer

} // end __esos_hw_InitSystemTick()

/****************************************************/
/*
* \brief Returns the ESOS system tick count.
*
* \pre ESOS system tick is running/working.
*
* \return A 32-bit value of the number of ESOS system ticks
* since the system has booted.
*
********************************************************/
uint32   __esos_hw_GetSystemTickCount(void) {
  return  esos_tick_count;
}  // end __esos_hw_GetSystemTickCount()

