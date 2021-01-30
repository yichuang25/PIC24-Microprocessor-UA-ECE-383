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

/** \file
 * ESOS application program to demonstrate I2C mastering in ESOS.
 * Application recreates code in \ref ds1631_i2c.c in Figure 10.52.
 * (See Figure 10.49 in the text for circuit.)
 *
 * Application also has a flashing LED on RB15.  Flashing LED is generated
 * by an <em>software timer</em> calling a user-provided callback function.
 *
 *  \note Demonstrates child tasks, ESOS software timers, and I2C service
 */


// INCLUDEs go here  (First include the main esos.h file)
//      After that, the user can include what they need
#include    "esos.h"
#ifdef __linux
#include    "esos_pc.h"
#include    "esos_pc_stdio.h"
// INCLUDE these so that printf() and our PC hacks work
#include <stdio.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#else
#include    "esos_pic24.h"
#include    "esos_pic24_rs232.h"
#include    "esos_pic24_i2c.h"
#include    <stdio.h>
#endif

// DEFINEs go here
#define DS1631ADDR 0x90   //DS1631 address with all pins tied low
#define ACCESS_CONFIG 0xAC
#define CONFIG_COMMAND 0x0C //continuous conversion, 12-bit mode
#define START_CONVERT 0x51
#define READ_TEMP 0xAA

#ifndef __linux
#define   CONFIG_LED1()   CONFIG_RB15_AS_DIG_OUTPUT()
#define   LED1            _LATB15
#else
#define   CONFIG_LED1()   printf("called CONFIG_LED1()\n");
uint8_t     LED1 = TRUE;      // LED1 is initially "on"
#endif

// PROTOTYPEs go here

// GLOBALs go here
//  Generally, the user-created semaphores will be defined/allocated here
char psz_CRNL[3]= {0x0D, 0x0A, 0};
char psz_prompt[] = "Temp is  ";
char psz_done[9]= {' ','D','O','N','E','!',0x0D, 0x0A, 0};
int16_t i16_temp;
UINT16 U16_raw;

ESOS_SEMAPHORE(sem_dataReady);
ESOS_SEMAPHORE(sem_dataPrinted);
ESOS_SEMAPHORE(sem_ds1631Ready);

#ifdef __linux
/*
 * Simulate the timer ISR found on a MCU
 *   The PC doesn't have a timer ISR, so this task will periodically
 *   call the timer services callback instead.
 *   USED ONLY FOR DEVELOPMENT AND TESTING ON PC.
 *   Real MCU hardware doesn't need this task
 */
ESOS_USER_TASK( __simulated_isr ) {
  ESOS_TASK_BEGIN();
  while (TRUE) {
    // call the ESOS timer services callback just like a real H/W ISR would
    __esos_tmrSvcsExecute();
    ESOS_TASK_WAIT_TICKS( 1 );

  } // endof while(TRUE)
  ESOS_TASK_END();
} // end child_task
#endif

/************************************************************************
 * User supplied functions
 ************************************************************************
 */

/*
 * An ESOS software timer callback function strobe the heartbeat LED.
 *
 * Toggles LED1 everytime the callback is called.  Exact period is
 * determined by application when this timer callback function is
 * registered with ESOS.  See \ref esos_RegisterTimer
 * Application can change timer period on-the-fly with \ref esos_ChangeTimerPeriod
 *
 * \note Since this heartbeat is performed in an ESOS software
 * timer callabck, a flashing LED indicates that the ESOS system
 * tick ISR is being called properly.  If the LED quits flashing,
 * then the ESOS system tick has ceased working.  This probably indicates
 * some catastrophic failure of the system.  However, the cause could
 * be poorly-behaved user code that is manipulating the hardware registers
 * with the timer or interrupt enables directly.  ESOS provides functions
 * to change state of interrupts and user code should never modify the
 * hardware used by ESOS to implement the system tick.
 * \hideinitializer
 */

// user-created timer callback
ESOS_USER_TIMER( swTimerLED ) {
  LED1 = !LED1;
#ifdef __linux
  if (LED1) {
    printf("\a");
    fflush(stdout);
  }
#endif
} //endof swTimerLED

/*
 * user task to setup DS1631 for continuous temperature
 * conversion.  Will signal when DS1631 is ready to be
 * used.
 */
ESOS_USER_TASK(start_ds1631) {
  ESOS_TASK_BEGIN();
  ESOS_TASK_WAIT_TICKS(500);
  ESOS_TASK_WAIT_ON_WRITE2I2C1(DS1631ADDR, ACCESS_CONFIG, CONFIG_COMMAND);
  ESOS_TASK_WAIT_ON_WRITE1I2C1(DS1631ADDR, START_CONVERT);
  ESOS_TASK_WAIT_TICKS(500);
  ESOS_SIGNAL_SEMAPHORE(sem_ds1631Ready, 1);
  ESOS_TASK_END();
} //end task start_ds1631

/*
 * user task to read DS1631 every 3/4 second
 * will signal when data has been successfully read
 */
ESOS_USER_TASK(read_ds1631) {
  static uint8_t u8_lo, u8_hi;

  ESOS_TASK_BEGIN();
  ESOS_TASK_WAIT_SEMAPHORE(sem_ds1631Ready, 1);
  while (TRUE) {
    ESOS_TASK_WAIT_ON_WRITE1I2C1(DS1631ADDR, READ_TEMP);
    ESOS_TASK_WAIT_ON_READ2I2C1(DS1631ADDR, u8_hi, u8_lo);
    U16_raw.u8Msb = u8_hi;
    U16_raw.u8Lsb = u8_lo;
    i16_temp = u8_hi;
    i16_temp = ((i16_temp<<8)|u8_lo);
    ESOS_SIGNAL_SEMAPHORE(sem_dataReady, 1);
    ESOS_TASK_WAIT_TICKS(750);
    ESOS_TASK_WAIT_SEMAPHORE(sem_dataPrinted, 1);
  } //end while()
  ESOS_TASK_END();
} //end task read_ds1631

/*
 * User task to display temperature results from DS1631.
 * Used printf and floating point for convenience.  A
 * production application would likely create custom functions
 * to avoid including these huge libraries.
 */
ESOS_USER_TASK(update) {
  float   f_tempC, f_tempF;

  ESOS_TASK_BEGIN();
  while (TRUE) {
    ESOS_TASK_WAIT_SEMAPHORE(sem_dataReady, 1);
    f_tempC = (float) i16_temp;  //convert to floating point
    f_tempC = f_tempC/256;  //divide by precision
    f_tempF = f_tempC*9/5 + 32;
    printf("Temp is: 0x%0X, %4.4f (C), %4.4f (F)\n", i16_temp, (double) f_tempC, (double) f_tempF);
    ESOS_SIGNAL_SEMAPHORE(sem_dataPrinted, 1);
  } // end while(TRUE)
  ESOS_TASK_END();
} // end update()

/****************************************************
 *  user_init()
 ****************************************************
 */
void user_init(void) {

  // Call the hardware-provided routines to print the
  // HELLO_MSG to the screen.  Must use this call because
  // the ESOS communications subsystems is not yet fully
  // initialized, since this call is in user_init()
  //
  // In general, users should call hardware-specific
  // function like this.
  __esos_unsafe_PutString( HELLO_MSG );

#ifdef __linux
  // register our little ESOS task to mimic MCU's TIMER T1 IRQ which kicks off
  // the ESOS S/W timers when they expire
  esos_RegisterTask( __simulated_isr );
#endif

  // configure our hardware to support to support our application
  CONFIG_LED1();
  esos_pic24_configI2C1(400);            //configure I2C for 400 KHz

  ESOS_INIT_SEMAPHORE(sem_ds1631Ready, 0);
  ESOS_INIT_SEMAPHORE(sem_dataReady, 0);
  ESOS_INIT_SEMAPHORE(sem_dataPrinted, 0);

  // user_init() should register at least one user task
  esos_RegisterTask(start_ds1631);
  esos_RegisterTask(read_ds1631);
  esos_RegisterTask(update);

  // register our callback function with ESOS to create a software timer
  esos_RegisterTimer( swTimerLED, 250);

} // end user_init()
