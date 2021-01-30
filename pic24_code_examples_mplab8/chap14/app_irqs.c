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
 * ESOS application program to measure the pulse width of a button push
 * using the 32-bit T2/T3 timer.  Recreates the program
 * \ref timer32bit_switch_pulse_measure.c with the same
 * functionality (see Figures 12.4-12.5 in the text)
 * \note Application demonstrates user interrupts under ESOS
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
#include    "pic24_timer.h"
#endif

// DEFINEs go here
#ifndef __linux
#define   CONFIG_LED1()   CONFIG_RB15_AS_DIG_OUTPUT()
#define   LED1            _LATB15
#else
#define   CONFIG_LED1()   printf("called CONFIG_LED1()\n");
uint8_t     LED1 = TRUE;      // LED1 is initially "on"
#endif


#define WAITING_FOR_FALLING_EDGE        ESOS_USER_FLAG_0
#define CAPTURED_FLAG                   ESOS_USER_FLAG_1

// PROTOTYPEs go here

// GLOBALs go here
//  Generally, the user-created semaphores will be defined/allocated here
char psz_CRNL[3]= {0x0D, 0x0A, 0};
char psz_prompt[] = "Press button...";
char psz_r1[] = "Pulse width = ";
char psz_r2[] = "us\n";

ESOS_SEMAPHORE( sem_CapturedData );

volatile UINT32 U32_lastCapture; // UINT32 declared in all_generic.h
volatile UINT32 U32_thisCapture;
volatile int32_t u32_delta;

/// Switch1 configuration, use RB13
inline void CONFIG_SW1()  {
  CONFIG_RB13_AS_DIG_INPUT();   //use RB13 for switch input
  ENABLE_RB13_PULLUP();         //enable the pullup
  CONFIG_INT1_TO_RP(13);   //map INT1 to RP13
}

//Timer2/3 used as single 32-bit timer, control word of Timer2 controls timer,
//interrupt status of Timer3 used for the combined timer
void  configTimer23(void) {
  T2CON = T2_OFF | T2_IDLE_CON | T2_GATE_OFF
          | T2_32BIT_MODE_ON
          | T2_SOURCE_INT
          | T2_PS_1_1 ;
  PR2 = 0xFFFF;                    //maximum period
  PR3 = 0xFFFF;                    //maximum period
  TMR3HLD = 0;                     //write MSW first
  TMR2  = 0;                       //then LSW
  ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED(ESOS_IRQ_PIC24_T3);
  T2CONbits.TON = 1;               //turn on the timer
}

//Interrupt Service Routine for INT1
//void _ISRFAST _INT1Interrupt (void) {
ESOS_USER_INTERRUPT( ESOS_IRQ_PIC24_INT1 ) {
  ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED(ESOS_IRQ_PIC24_INT1);    //clear the interrupt bit
  if (esos_IsUserFlagSet(WAITING_FOR_FALLING_EDGE)) {
    if (esos_IsUserFlagClear(CAPTURED_FLAG)) {
      U32_lastCapture.u16LoWord = TMR2;
      U32_lastCapture.u16HiWord = TMR3HLD;
      _INT1EP = 0;  //configure for rising edge
      esos_ClearUserFlag(WAITING_FOR_FALLING_EDGE);
    } //endif CAPTURED_FLAG
  } else {
    U32_thisCapture.u16LoWord = TMR2;
    U32_thisCapture.u16HiWord = TMR3HLD;
    u32_delta = U32_thisCapture.u32 - U32_lastCapture.u32;
    esos_SetUserFlag(CAPTURED_FLAG);
    _INT1EP = 1;     //configure for falling edge
    esos_SetUserFlag(WAITING_FOR_FALLING_EDGE);
  } //endif-else
} //end INT1 ISR


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
  static  uint32_t          u32_pulseWidth;

  ESOS_TASK_BEGIN();
  while (TRUE) {
    ESOS_TASK_WAIT_ON_SEND_STRING(psz_prompt);
    ESOS_TASK_WAIT_UNTIL(esos_IsUserFlagSet(CAPTURED_FLAG));
    u32_pulseWidth = ticksToUs(u32_delta, getTimerPrescale(T2CONbits));
    esos_ClearUserFlag(CAPTURED_FLAG);
    ESOS_TASK_WAIT_ON_SEND_STRING(psz_r1);
    ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(u32_pulseWidth);
    ESOS_TASK_WAIT_ON_SEND_STRING(psz_r2);
  } // endof while
  ESOS_TASK_END();
} // end task1()

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
  CONFIG_SW1();
  configTimer23();

  // initialize the semaphore (initially blocking)
  //ESOS_INIT_SEMAPHORE( sem_T1CanRun, 0 );
  //ESOS_INIT_SEMAPHORE( sem_T2CanRun, 0 );

  // user_init() should register at least one user task
  esos_RegisterTask(task1);
  //esos_RegisterTask(task2);

  // register our callback function with ESOS to create a software timer
  esos_RegisterTimer( swTimerLED, 250 );

  // Configure INT1
  esos_SetUserFlag(WAITING_FOR_FALLING_EDGE);
  _INT1EP = 1;     //negative edge triggerred
  ESOS_REGISTER_PIC24_USER_INTERRUPT( ESOS_IRQ_PIC24_INT1, ESOS_USER_IRQ_LEVEL1, _INT1Interrupt );
  ESOS_ENABLE_PIC24_USER_INTERRUPT(ESOS_IRQ_PIC24_INT1);

} // end user_init()

