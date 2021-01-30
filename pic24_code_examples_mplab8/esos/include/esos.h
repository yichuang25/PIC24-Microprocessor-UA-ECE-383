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
/** \file
 * Embedded Systems Operating System (ESOS)
 *
 */

#ifndef ESOS_H
#define ESOS_H

#ifndef BUILT_ON_ESOS
#define BUILT_ON_ESOS
#endif


// Include all the files we need
//#include "user_config.h"      // get the user's configuration requests
#include "all_generic.h"
#include "esos_task.h"          // defines ESOS tasks and semaphores

// PUT THESE HERE FOR NOW.  They belong somewhere else
// in the long-run.
//
// PC versions do NOT have IRQs (yet) so do NOT define the variables
#ifndef __linux
#define	ESOS_USE_IRQS
#endif
#define ESOS_USE_SERIAL_PORT

/**
* Define the constants and structures required for our little
* embedded systems operating system
*/
#if defined(ESOS_USE_BULK_CDC_USB) || defined(ESOS_USE_SERIAL_PORT)
#include    "esos_comm.h"
#endif      // USE_USB or USE_SERIAL

#ifdef  ESOS_USE_IRQS
/*
*  user wants IRQ support, so prototype the required IRQ functions
*/
#include    "esos_irq.h"
#endif      //ESOS_USE_IRQS

#ifdef  ESOS_RUNS_ON_REAL_OS
// computers running a host OS (windoze/Linux/etc) expect
//      their applications to return and return a value
//      so define a return type for main and create a return statement
//      also, multitasking OS-es will want some CPU time for other
//      applications, so sleep some (10ms = 10000us) should be enough
typedef             int                 main_t;
#define             OS_END              return(1)
#define             OS_ITERATE          usleep(10000)
#else
// hardware w/o a hosting OS will never return. So main_t
//      must be void return type and hang-up when done!
//      (Of course, real hardware apps should never be done.)
typedef             int	                main_t;
#define             OS_END              while(1)
#define             OS_ITERATE
#endif

/*
 *  Create the ESOS structures we need
 */
struct stTimer {
  void    (*pfn)(void);
  uint32  u32_period;
  uint32  u32_cntDown;
};

// Define masks for the user to use for their flags
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_0          BIT0
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_1          BIT1
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_2          BIT2
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_3          BIT3
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_4          BIT4
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_5          BIT5
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_6          BIT6
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_7          BIT7
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_8          BIT8
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_9          BIT9
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_A          BIT10
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_B          BIT11
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_C          BIT12
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_D          BIT13
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_E          BIT14
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_F          BIT15


/**
 * Declaration of an user-defined timer callback (for ESOS timer services)
 *
 * This macro is used to declare a user-timer. All timers using ESOS timer
 * services  must be declared with this macro.
 *
 * \param timername The name of the software timer to create.
 * \note You are really creating a C function implementing the user timer callback.
 *   Since ESOS timer serivces calls this callback function at the appropriate time,
 *  this function cannot be passed arguments and cannot return values
 * \hideinitializer
 */
#define   ESOS_USER_TIMER(timername)    void timername(void)

/**
 * Handle to a software timer in the ESOS timer service.
 *
 * \sa ESOS_USER_TIMER
 * \sa esos_RegisterTimer
 * \sa esos_UnregisterTimer
 * \sa esos_GetTimerHandle
 * \sa esos_ChangeTimerPeriod
 *
 * \hideinitializer
 */
typedef   uint8                   ESOS_TMR_HANDLE;

/*
 * Now define the public user function prototypes that are
 *   always available regardless of the user_config settings
 */
/**
 * User-provided function to initialize user's hardware configuration
 * register user tasks.
 *
 * \note All ESOS applications <em>MUST</em> provide this function.
 * \note This function  <em>MUST</em>  register at least one ESOS task.
 *
 * \note user_init() is a centralized initialization routine where
 *                  the user can setup their application.   It is called
 *                  automagically by ES_OS during the operating system
 *                  initialization.
 *
 * \note User should set up any state machines and init
 *                  all application variables.  They can also turn on
 *                  any needed peripherals here.
 *
 * \note User <em>shall not</em> mess with the interrupt hardware
 *                  directly!!!  The ESOS must be aware of the interrupts
 *                  and provides esos_XXXXXXX functions for the user to use.
 *                  Using these ESOS-provided functions, the user may
 *                  (and probably should) initialize, register, and enable
 *                  interrupts in this routine.
 *
 * \note Furthermore, the user should register AT LEAST one
 *                  user application task here via \ref esos_RegisterTask
 *                  or the ES_OS scheduler will have nothing to schedule
 *                  to run when this function returns.
 * \hideinitializer
 */

void    user_init( void );
ESOS_TASK_HANDLE   esos_RegisterTask( uint8 (*pfn_TaskFcn)(struct stTask *pst_Task) );
uint8   esos_UnregisterTask( uint8 (*pfn_TaskFcn)(struct stTask *pst_Task) ) ;
ESOS_TASK_HANDLE	esos_GetFreeChildTaskStruct();
uint32    esos_GetRandomUint32();

// prototypes for ESOS software timers
ESOS_TMR_HANDLE    esos_RegisterTimer( void (*pfnTmrFcn)(void), uint32 u32_period );
uint8    esos_UnregisterTimer( ESOS_TMR_HANDLE hnd_timer );
ESOS_TMR_HANDLE    esos_GetTimerHandle( void (*pfnTmrFcn)(void) );
uint8    esos_ChangeTimerPeriod( ESOS_TMR_HANDLE hnd_timer, uint32 u32_period );

// The user must provide the HW-specific way of getting a 32bit 1.0ms tick
void    __esos_hw_InitSystemTick(void);
uint32  __esos_hw_GetSystemTickCount(void);

/**
 * Get the current value of the ESOS system tick counter
 * In the current implementation of ESOS, a tick equal 1.0ms.
 * Therefore, the value returned by this function is approximately
 * equal to the number of milliseconds since the since was last
 * reset.
 * \return The \ref uint32 value of current value of the ESOS
 * system tick counter
 * \note This counter value will roll-over every 49.7 days.
 * \hideinitializer
 */
#define   esos_GetSystemTick()          __esos_hw_GetSystemTickCount()
uint16  __esos_hasTickDurationPassed(uint32 u32_startTick, uint32 u32_period);
void    __esos_tmrSvcsExecute(void);

void    __esos_InitCommSystem(void);


/*
 * expose these ESOS system variables to allow macro access
 * intead of fcn access
 */
extern uint8        __esos_u8UserTasksRegistered;
extern uint16       __esos_u16UserFlags, __esos_u16SystemFlags;

/**
 * Get the current number of user task registered with the
 * ESOS scheduler.
 * \return The \ref uint8 number of currently registered user tasks
 * \note This value does not include the number of child tasks
 * (tasks of the type \ref ESOS_CHILD_TASK ), just the tasks
 * of the type \ref ESOS_USER_TASK
 * \hideinitializer
 */
#define esos_GetNumberRegisteredTasks()        (__esos_u8UserTasksRegistered)

/**
 * Returns the system tick value of a future time
 * \param deltaT the number of ticks in the future you'd like the
 * system tick value for
 * \return The \ref uint32 number corresponding to the system tick
 * value of that future time
 * \sa esos_GetSystemTick
 * \hideinitializer
 */
#define esos_GetFutureSystemTick(deltaT)    ((uint32)(deltaT) + __esos_hw_GetSystemTickCount());

/**
 * Sets bits in the global user flags provided by ESOS
 * \param mask An \ref uint16 value composed of the OR-ed user
 * mask flag masks, where each flag in the OR will be set
 * \note User should use the provided bits masks like \ref ESOS_USER_FLAG_0
 *  to create their own readable constants
 * \code
 * #define HEADLIGHTS_ARE_ON         ESOS_USER_FLAG_3
 * #define MY_USER_FRIENDLY_FLAG     ESOS_USER_FLAG_7
 * esos_SetUserFlag( HEADLIGHTS_ARE_ON | MY_USER_FRIENDLY_FLAG);
 * \endcode
 * \sa esos_ClearUserFlag
 * \sa esos_IsUserFlagSet
 * \sa esos_IsUserFlagClear
 * \hideinitializer
 */
#define esos_SetUserFlag(mask)              BIT_SET_MASK(__esos_u16UserFlags, (mask))

/**
 * Clears bits in the global user flags provided by ESOS
 * \param mask An \ref uint16 value composed of the OR-ed user
 * mask flag masks, where each flag in the OR will be cleared
 * \note User should use the provided bits masks like \ref ESOS_USER_FLAG_0
 * and \ref ESOS_USER_FLAG_1 and ... \ref ESOS_USER_FLAG_F
 *  to create their own readable constants
 * \code
 * #define HEADLIGHTS_ARE_ON         ESOS_USER_FLAG_3
 * #define MY_USER_FRIENDLY_FLAG     ESOS_USER_FLAG_7
 * esos_ClearUserFlag( HEADLIGHTS_ARE_ON | MY_USER_FRIENDLY_FLAG);
 * \endcode
 * \sa esos_SetUserFlag
 * \sa esos_IsUserFlagSet
 * \sa esos_IsUserFlagClear
 * \hideinitializer
 */

#define esos_ClearUserFlag(mask)            BIT_CLEAR_MASK(__esos_u16UserFlags, (mask))

/**
 * Queries whether the global user flags provided by ESOS are set
 * \param mask An \ref uint16 value composed of the OR-ed user
 * mask flag masks, where each flag in the OR will be checked for being set
 * \retval TRUE if <em>at least</em> one of the flags is set
 * \retval FALSE if none of the flags are set
 * \note User should use the provided bits masks like \ref ESOS_USER_FLAG_0
 * and \ref ESOS_USER_FLAG_1 and ... \ref ESOS_USER_FLAG_F
 *  to create their own readable constants
 * \code
 * #define HEADLIGHTS_ARE_ON         ESOS_USER_FLAG_3
 * #define MY_USER_FRIENDLY_FLAG     ESOS_USER_FLAG_7
 * esos_ClearUserFlag( HEADLIGHTS_ARE_ON | MY_USER_FRIENDLY_FLAG);
 * while(esos_IsUserFlagSet( HEADLIGHTS_ARE_ON));  // falls through
 * \endcode
 * \sa esos_SetUserFlag
 * \sa esos_ClearUserFlag
 * \sa esos_IsUserFlagClear
 * \hideinitializer
 */
#define esos_IsUserFlagSet(mask)            IS_BIT_SET_MASK(__esos_u16UserFlags, (mask))

/**
 * Queries whether the global user flags provided by ESOS are clear
 * \param mask An \ref uint16 value composed of the OR-ed user
 * mask flag masks, where each flag in the OR will be checked for being clear
 * \retval TRUE if <em>at least</em> one of the flags is clear
 * \retval FALSE if none of the flags are clear
 * \note User should use the provided bits masks like \ref ESOS_USER_FLAG_0
 * and \ref ESOS_USER_FLAG_1 and ... \ref ESOS_USER_FLAG_F
 *  to create their own readable constants
 * \code
 * #define HEADLIGHTS_ARE_ON         ESOS_USER_FLAG_3
 * #define MY_USER_FRIENDLY_FLAG     ESOS_USER_FLAG_7
 * esos_ClearUserFlag( HEADLIGHTS_ARE_ON | MY_USER_FRIENDLY_FLAG);
 * while(esos_IsUserFlagClear( HEADLIGHTS_ARE_ON));  // infinite loop
 * \endcode
 * \sa esos_SetUserFlag
 * \sa esos_ClearUserFlag
 * \sa esos_IsUserFlagSet
 * \hideinitializer
 */
#define esos_IsUserFlagClear(mask)          IS_BIT_CLEAR_MASK(__esos_u16UserFlags, (mask))

// define macros for ESOS system flags
//    these flags are NOT to be manipulated directly by the user!
#define __esos_SetSystemFlag(mask)              BIT_SET_MASK(__esos_u16SystemFlags, (mask))
#define __esos_ClearSystemFlag(mask)            BIT_CLEAR_MASK(__esos_u16SystemFlags, (mask))
#define __esos_IsSystemFlagSet(mask)            IS_BIT_SET_MASK(__esos_u16SystemFlags, (mask))
#define __esos_IsSystemFlagClear(mask)          IS_BIT_CLEAR_MASK(__esos_u16SystemFlags, (mask))

// Defines for ESOS timer services
#define   ESOS_TMR_FAILURE    0xFF
#define   MAX_NUM_TMRS        16

/**
 * Get the current number of user software timers registers (running)
 * in the ESOS timer services
 * \return The \ref uint8 number of currently registered user tasks
 * \hideinitializer
 */
#define esos_GetNumberRunningTimers()          (__esos_u8TmrSvcsRegistered)

/**
 * Determines if the software timer  represented by the handle is currently running
 * \param hndl The \ref ESOS_TMR_HANDLE of a software timer
 * \retval TRUE if the timer is currently running
 * \retval FALSE if the timer is not currently running
 * \sa ESOS_USER_TIMER
 * \sa esos_UnregisterTimer
 * \sa esos_GetTimerHandle
 * \sa esos_ChangeTimerPeriod
 * \hideinitializer
 */
#define   esos_IsTimerRunning(hndl)            IS_BIT_SET_MASK(__esos_u16TmrActiveFlags, (BIT0<<(hndl)))
#define   __esos_MarkTimerRunning(hndl)        BIT_SET_MASK(__esos_u16TmrActiveFlags, (BIT0<<(hndl)))
#define   __esos_MarkTimerStopped(hndl)        BIT_CLEAR_MASK(__esos_u16TmrActiveFlags, (BIT0<<(hndl)))


// System flag definitions... only ESOS needs to use these
#define     __ESOS_SYS_FLAG_PACK_TASKS              BIT0
#define     __ESOS_SYS_FLAG_NULL_LAST_TASK          BIT1
#define     __ESOS_SYS_COMM_TX_IS_BUSY				BIT2
#define     __ESOS_SYS_COMM_RX_IS_BUSY				BIT3

// Other useful macros for the user
#define   __abs(x)    (((x) < 0) ? -(x) : (x))

#endif          // ESOS_H
