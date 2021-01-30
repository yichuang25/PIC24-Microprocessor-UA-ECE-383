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
 * ESOS application program to where two tasks increment a counter local to
 * each task 10 times.  Between each counter increment, the tasks wait for
 * a random length of time.  After the 10th iteration, the tasks "rendez-vous"
 * before continuing.
 * \note Application demonstrates rendez-vous synchronization using ESOS semaphores
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
#endif

// DEFINEs go here
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
char psz_T1[] = "Task 1: ";
char psz_T2[] = "Task 2: ";
char psz_rv[] = "rendez-vous!";

ESOS_SEMAPHORE( sem_T1CanRun );
ESOS_SEMAPHORE( sem_T2CanRun );

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
 * return an uint32_t that can be used for a reasonable delay
 * should not be too short (~255 ticks) and not too long (~4096 ticks)
 */
inline uint32_t getRandomDelay() {
  return ((esos_GetRandomUint32() & 0x0FFF)|0x100);
}

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

/* == task1 ==
 * Print out a counter value local to this task, then delay
 * for a random period of time.  (Delays range from approx.
 * 0.25 - 4.1 seconds)
 */
ESOS_USER_TASK(task1) {
  static  uint8_t           u8_cnt=6;

  ESOS_TASK_BEGIN();
  while (u8_cnt) {
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING( psz_T1 );
    ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING( u8_cnt );
    ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
    u8_cnt--;
    ESOS_TASK_YIELD();
  } // endof while()
  ESOS_SIGNAL_SEMAPHORE(sem_T2CanRun, 1);
  ESOS_TASK_WAIT_SEMAPHORE(sem_T1CanRun, 1);
  ESOS_TASK_WAIT_ON_SEND_STRING( psz_T1 );
  ESOS_TASK_WAIT_ON_SEND_STRING( psz_rv );
  ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
  ESOS_TASK_END();
} // end task1()

/* == task2 ==
 * Wait for a signal from task1, then
 * Print out a counter value local to this task
 */
ESOS_USER_TASK(task2) {
  static  uint8_t           u8_cnt=4;

  ESOS_TASK_BEGIN();
  while (u8_cnt) {
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING( psz_T2 );
    ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING( u8_cnt );
    ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
    u8_cnt--;
    ESOS_TASK_YIELD();
  } // endof while()
  ESOS_SIGNAL_SEMAPHORE(sem_T1CanRun, 1);
  ESOS_TASK_WAIT_SEMAPHORE(sem_T2CanRun, 1);
  ESOS_TASK_WAIT_ON_SEND_STRING( psz_T2 );
  ESOS_TASK_WAIT_ON_SEND_STRING( psz_rv );
  ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
  ESOS_TASK_END();
} // end task2()


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

  // initialize the semaphore (initially blocking)
  ESOS_INIT_SEMAPHORE( sem_T1CanRun, 0 );
  ESOS_INIT_SEMAPHORE( sem_T2CanRun, 0 );

  // user_init() should register at least one user task
  esos_RegisterTask(task1);
  esos_RegisterTask(task2);

  // register our callback function with ESOS to create a software timer
  //esos_RegisterTimer( swTimerLED, 250 );

} // end user_init()
