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
 *  \brief This file contains macros, prototypes, and definitions for
 *  Microchip PIC24 family specific interrupts on ESOS
 */

/************************************************************************
 * esos_pic24_irq.h
 ************************************************************************
 * User-supplied include file which defines the IRQ which are supported
 *
 * NOTE:  the file must be consistent with esos_pic24_irq.c which uses
 *          many of these constant to manipulate IRQ registers
 */

#ifndef     ESOS_PIC24_IRQ_H
#define ESOS_PIC24_IRQ_H

#include "esos.h"
#include "esos_irq.h"
#include "esos_pic24.h"


/**
 * Declaration of an ESOS interrupt.
 *
 * This macro is used to declare an ESOS user inerrupt. All ESOS user
 * interrupt tasks must be declared with this macro.
 *
 * \param desc Descriptor of the hardware interrupt
 *
 * \note ESOS user interrupts have no arguments passed in and cannot return values.
 * \note Hardware interrupt descriptors are found in the file <b>esos_<i>hw</i>_irq.h</b>
 * where <i>hw</i> is the name of the chip for the ESOS port has been written.
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 *
 * \hideinitializer
 */
#define ESOS_USER_INTERRUPT(desc)    __xESOS_USER_ISR(desc)
#define __xESOS_USER_ISR(attrib, ivt, ifsr, ifsb, ipcr, ipcb)      void _ISRFAST attrib (void)



/*
 * Define the ESOS user IRQ levels here
 *  ESOS-based IRQs will run at IRQ priority levels
 *  7 and 5.
 *
 * NOTE: Any user IRQ with its IRQ priority level at 0 signifies
 *  that the IRQ is not registered with ESOS.
 */
/**
 * Highest priority user interrupt level on PIC24 Family
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \hideinitializer
 */
#define	ESOS_USER_IRQ_LEVEL1				6
/**
 * Second-highest priority user interrupt level on PIC24 Family
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \hideinitializer
 */
#define	ESOS_USER_IRQ_LEVEL2				4
/**
 * Third-highest priority user interrupt level on PIC24 Family
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \hideinitializer
 */
#define	ESOS_USER_IRQ_LEVEL3				3
/**
 * Lowest priority user interrupt level on PIC24 Family
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \hideinitializer
 */
#define	ESOS_USER_IRQ_LEVEL4				2

#define	__ESOS_USER_IRQ_UNREGISTERED		0
// this #define is the IPL that will disabled all user IRQs at once
#define __ESOS_DISABLE_USER_IRQS_LEVEL		ESOS_USER_IRQ_LEVEL2
// this #define is the IPL that will enabled all user IRQs at once
#define __ESOS_ENABLE_USER_IRQS_LEVEL		__ESOS_USER_IRQ_UNREGISTERED

/*
*  IRQ masks for the PIC24HJ32GP202
*/
#if defined(__PIC24HJ32GP202__)  || defined(__DOXYGEN__)
//         MNEMONIC            C30 ISR ATTRIB, IVT addr, IFS register, IFS bit, IPC register, IPC bit
/**
 * AD1 Conversion Complete Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_AD1	_ADC1Interrupt, 0x002E, 0, 13, 3, 4            // AD1 Conversion complete
/**
 * SP1 Event Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_SPI1	_SPI1Interrupt, 0x0028, 0, 10, 2, 8            // SPI1 event
/**
 * SP1 Exception Event Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_SPI1E	_SPI1ErrInterrupt, 0x0026, 0, 9, 2, 4          // SPI1 (exception) fault event
/**
 * Timer 3 expiration Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_T3     _T3Interrupt, 0x0024, 0, 8, 2, 0               // Timer 3
/**
 * Timer 2 expiration Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_T2	_T2Interrupt, 0x0022, 0, 7, 1, 12              // Timer 2
/**
 * Output Compare Channel 2 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_OC2	_OC2Interrupt, 0x0020, 0, 6, 1, 8              // Output compare channel 2
/**
 * Input Capture Channel 2 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_IC2	_IC2Interrupt, 0x001E, 0, 5, 1, 4              // Input capture channel 2
/**
 * Output Compare Channel 1 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_OC1	_OC1Interrupt, 0x0018, 0, 2, 0, 8              // Output compare channel 1

/**
 * Input Capture Channel 1 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_IC1	_IC1Interrupt, 0x0016, 0, 1, 0, 4              // Input capture channel 1
/**
 * External IRQ 0 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_INT0	_INT0Interrupt, 0x0014, 0, 0, 0, 0             // external interrupt 0
/**
 * External IRQ 2 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_INT2	_INT2Interrupt, 0x004E, 1, 13, 7, 4            // external interrupt 2

/**
 * Input Capture Channel 8 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_IC8	_IC8Interrupt, 0x0042, 1, 7, 5, 12             // Input capture channel 8
/**
 * Input Capture Channel 7 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_IC7	_IC7Interrupt, 0x0040, 1, 6, 5, 8              // Input capture channel 7
/**
 * External IRQ 1 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_INT1	_INT1Interrupt, 0x003C, 1, 4, 5, 0             // external interrupt 1
/**
 * Input Change Notification Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_CN	_CNInterrupt, 0x003A, 1, 3, 4, 12              // input change notification
/**
 * I2C1 Master Event Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_MI2C1	_MI2C1Interrupt, 0x0036, 1, 1, 4, 4            // I2C1 Master event
/**
 * I2C1 Slave Event Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_SI2C1	_SI2C1Interrupt, 0x0034, 1, 0, 4, 0            // I2C1 slave event

// if the user is __NOT__  using the built-in ESOS comm system but __IS__ using user IRQs, then
//    we should define the UART IRQs for their use
//  #ifndef _ESOS_PIC24_RS232_H
#if !defined(_ESOS_PIC24_RS232_H)  || defined(__DOXYGEN__)
/**
 * UART1 TX Event Interrupt (User interrupt descriptor)
 * \note Only defined if ESOS is built <em>without</em> the communications subsystem.
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_U1TX	_U1TXInterrupt, 0x002C, 0, 12, 3, 0            // UART1 TX event
/**
 * UART1 RX Event Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \note Only defined if ESOS is built <em>without</em> the communications subsystem.
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_U1RX	_U1RXInterrupt, 0x002A, 0, 11, 2, 12           // UART1 RX event

/**
 * UART1 Error Event Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \note Only defined if ESOS is built <em>without</em> the communications subsystem.
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_U1E	_U1ErrInterrupt, 0x0096, 4, 1, 16, 4           // UART1 Error event

#endif      // end of UART1 constants

#endif      // __PIC24HJ32GP202__

/*
 * MACROs COMMON TO ALL PIC24 PROCESSORS
 */

#define __GET_IRQ_ATTRIB(Q)     __xGET_IRQ_ATTRIB(Q)
#define __GET_IVTQ(Q)           __xGET_IVT(Q)
#define __GET_IFS_NUM(Q)        __xGET_IFS_NUM(Q)
#define __GET_IFS_BITNUM(Q)     __xGET_IFS_BITNUM(Q)
#define __GET_IEC_NUM(Q)        __xGET_IFS_NUM(Q)
#define __GET_IEC_BITNUM(Q)     __xGET_IFS_BITNUM(Q)
#define __GET_IPC_NUM(Q)        __xGET_IPC_NUM(Q)
#define __GET_IPC_BITNUM(Q)     __xGET_IPC_BITNUM(Q)

#define __xGET_IRQ_ATTRIB(attrib, ivt, ifsr, ifsb, ipcr, ipcb)            attrib
#define __xGET_IVT(attrib, ivt, ifsr, ifsb, ipcr, ipcb)               ivt
#define __xGET_IFS_NUM(attrib, ivt, ifsr, ifsb, ipcr, ipcb)           ifsr
#define __xGET_IFS_BITNUM(attrib, ivt, ifsr, ifsb, ipcr, ipcb)        ifsb
#define __xGET_IEC_NUM(attrib, ivt, ifsr, ifsb, ipcr, ipcb)           ifsr
#define __xGET_IEC_BITNUM(attrib, ivt, ifsr, ifsb, ipcr, ipcb)        ifsb
#define __xGET_IPC_NUM(attrib, ivt, ifsr, ifsb, ipcr, ipcb)           ipcr
#define __xGET_IPC_BITNUM(attrib, ivt, ifsr, ifsb, ipcr, ipcb)        ipcb


// this macro gets the IRQ's IPL number from its corresponding IPC register
#define __GET_IPL_FROM_IPCX(Q)   		((*(&IPC0+(__xGET_IPC_NUM(Q))) >> __xGET_IPC_BITNUM(Q))&0x7)

// this macro tests the IRQ's IPL number from its corresponding IPC register
// against a value
#define __IS_IPL_FROM_IPCX(Q, val)		((__GET_IPL_FROM_IPCX(Q))==(val))

// this macro sets the IRQ's IPL number in its corresponding IPC register
#define __PUT_IPL_INTO_IPCX(ipc,ipcb,ipl)                    					\
    do {                                                  					\
		BIT_CLEAR_MASK( *(&IPC0+(ipc)), 0x07<<ipcb) + (ipl<<ipcb);	\
		*(&IPC0+ipc) += (ipl<<ipcb); \
      }while(0)

/**
* Unregisters the user interrupt denoted by the interrupt descriptor
* \param desc   Descriptor of the hardware interrupt
* \note Hardware interrupt descriptors are found in the file <b>esos_<i>hw</i>_irq.h</b>
* where <i>hw</i> is the name of the chip for the ESOS port has been written.
* \hideinitializer
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 */
#define  ESOS_UNREGISTER_PIC24_USER_INTERRUPT(desc)    __xUNREGISTER_PIC24_USER_INTERRUPT(desc)
#define  __xUNREGISTER_PIC24_USER_INTERRUPT(attrib, ivt, ifsr, ifsb, ipcr, ipcb)   \
  do{                                               \
  __xDISABLE_PIC24_USER_INTERRUPT(attrib, ivt, ifsr, ifsb, ipcr, ipcb);               \
  __PUT_IPL_INTO_IPCX(ipcr,ipcb, __ESOS_USER_IRQ_UNREGISTERED);      \
  }while(0)


/**
* Registers the user interrupt denoted by the interrupt descriptor
* \param desc   Descriptor of the hardware interrupt
* \param ipl     Interrupt Priority Level (IPL) for the interrupt being registered
* \param p2f        Pointer to function to serve as the ISR. Function must have <em>void</em>
* arguments and return <em>void</em>
* \note Hardware interrupt descriptors are found in the file <b>esos_<i>hw</i>_irq.h</b>
* where <i>hw</i> is the name of the chip for the ESOS port has been written.
* \hideinitializer
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 */
#define ESOS_REGISTER_PIC24_USER_INTERRUPT(desc, ipl, p2f)    __xREGISTER_PIC24_USER_INTERRUPT(desc, ipl, p2f)
#define __xREGISTER_PIC24_USER_INTERRUPT(attrib, ivt, ifsr, ifsb, ipcr, ipcb, ipl, p2f)    \
   do {                                               \
      __xDISABLE_PIC24_USER_INTERRUPT(attrib, ivt, ifsr, ifsb, ipcr, ipcb);   \
      __PUT_IPL_INTO_IPCX(ipcr, ipcb, ipl);   \
   }while(0)

/**
* Disables all registered user interrupts.
* \note This function does <em>NOT</em> disable all interrupts.  Interrupts used by
* ESOS will continue to function.
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
* \hideinitializer
*/
#define  ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS()     SET_CPU_IPL(__ESOS_DISABLE_USER_IRQS_LEVEL)

/**
* Enables all registered user interrupts.
* \note Does not affect interrupts used by ESOS
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
* \hideinitializer
*/
#define  ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS()      SET_CPU_IPL(__ESOS_ENABLE_USER_IRQS_LEVEL)

/**
* Queries the enabled state of user interrupt denoted by the interrupt descriptor
* \param desc   Descriptor of the hardware interrupt
* \retval TRUE    If the user interrupt is currently enabled
* \retval FALSE    If the user interrupt is currently disabled
* \note Hardware interrupt descriptors are found in the file <b>esos_<i>hw</i>_irq.h</b>
* where <i>hw</i> is the name of the chip for the ESOS port has been written.
* \hideinitializer
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 */
#define  ESOS_IS_PIC24_USER_INTERRUPT_ENABLED(desc)     __xIS_PIC24_USER_INTERRUPT_ENABLED(desc)
#define  __xIS_PIC24_USER_INTERRUPT_ENABLED(attrib, ivt, ifsr, ifsb, ipcr, ipcb)    IS_BIT_SET(*(&IEC0+ifsr),ifsb)

/**
* Queries the "flag" state of user interrupt denoted by the interrupt descriptor
* \param desc   Descriptor of the hardware interrupt
* \retval TRUE    If the user interrupt is needs to be serviced
* \retval FALSE    Otherwise
* \note Hardware interrupt descriptors are found in the file <b>esos_<i>hw</i>_irq.h</b>
* where <i>hw</i> is the name of the chip for the ESOS port has been written.
* \hideinitializer
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
*/
#define  ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING(desc)    __xDOES_PIC24_USER_INTERRUPT_NEED_SERVICING(desc)
#define  __xDOES_PIC24_USER_INTERRUPT_NEED_SERVICING(attrib, ivt, ifsr, ifsb, ipcr, ipcb)     IS_BIT_SET(*(&IFS0+ifsr), ifsb)

/**
* Tells ESOS to mark the user interrupt denoted by the interrupt descriptor as being serviced
* \param desc   Descriptor of the hardware interrupt
* \note Hardware interrupt descriptors are found in the file <b>esos_<i>hw</i>_irq.h</b>
* where <i>hw</i> is the name of the chip for the ESOS port has been written.
* \hideinitializer
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
*/
#define  ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED(desc)   __xMARK_PIC24_USER_INTERRUPT_SERVICED(desc)
#define  __xMARK_PIC24_USER_INTERRUPT_SERVICED(attrib, ivt, ifsr, ifsb, ipcr, ipcb)            BIT_CLEAR(*(&IFS0+ifsr),ifsb)

/**
* Enables the user interrupt denoted by the interrupt descriptor
* \param desc   Descriptor of the hardware interrupt
* \note Hardware interrupt descriptors are found in the file <b>esos_<i>hw</i>_irq.h</b>
* where <i>hw</i> is the name of the chip for the ESOS port has been written.
* \hideinitializer
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 */
#define   ESOS_ENABLE_PIC24_USER_INTERRUPT(desc)      __xENABLE_PIC24_USER_INTERRUPT(desc)
#define  __xENABLE_PIC24_USER_INTERRUPT(attrib, ivt, ifsr, ifsb, ipcr, ipcb)     BIT_SET(*(&IEC0+ifsr), ifsb)

/**
* Disables the user interrupt denoted by the interrupt descriptor
* \param desc   Descriptor of the hardware interrupt
* \note Hardware interrupt descriptors are found in the file <b>esos_<i>hw</i>_irq.h</b>
* where <i>hw</i> is the name of the chip for the ESOS port has been written.
* \hideinitializer
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 */
#define  ESOS_DISABLE_PIC24_USER_INTERRUPT(desc)       __xDISABLE_PIC24_USER_INTERRUPT(desc)
#define  __xDISABLE_PIC24_USER_INTERRUPT(attrib, ivt, ifsr, ifsb, ipcr, ipcb)       BIT_CLEAR(*(&IEC0+ifsr), ifsb)

#endif          // ESOS_PIC24_IRQ_H


