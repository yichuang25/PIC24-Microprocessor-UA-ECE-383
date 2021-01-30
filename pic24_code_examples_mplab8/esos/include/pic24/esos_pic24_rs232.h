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
 *  Microchip PIC24 Family specific communications on ESOS
 */


#ifndef _ESOS_PIC24_RS232_H
#define _ESOS_PIC24_RS232_H

/* I N C L U D E S **********************************************************/
#include "esos.h"
#include "esos_comm.h"
#include "esos_pic24.h"

/* D E F I N I T I O N S ****************************************************/

// use the same definitions as the non-ESOS pic24_uart.h/.c library but
// prepend with our __ESOS_ tag
#define __ESOS_UART1_TX_INTERRUPT
#define __ESOS_UART1_RX_INTERRUPT
#define __ESOS_UART1_TX_INTERRUPT_PRIORITY     5
#define __ESOS_UART1_RX_INTERRUPT_PRIORITY     5

/* E X T E R N S ************************************************************/

/* M A C R O S **************************************************************/

/* P U B L I C  P R O T O T Y P E S *****************************************/
void    __esos_hw_signal_start_tx(void);
void    __esos_hw_signal_stop_tx(void);

// Documentation for this file. If the \file tag isn't present,
// this file won't be documented.
/** \file
 *  This file contains routines which configure and
 *  use the UARTs on the PIC. See \ref pic24_serial.h
 *  for higher-level routines, which should typically
 *  be called by the user rather than these routines.
 */

/* ########################################################################### */
/** Chose a default baud rate for the UART, used by
 *  \ref configUART1 to set up the UART.
 */
#define DEFAULT_BAUDRATE  57600

/** Configure the UART's baud rate, based on \ref FCY.
 *  Note that the value computed is truncated, not
 *  rounded, since this is done using integer
 *  arithmetic. That is, BRG = truncate(FCY/16/baud - 1),
 *  giving an actual baud rate of FCY/16/(reg + 1).
 *
 *  NOTE: this code sets BRGH=0 (16 clocks for each bit).
 *  Be careful about using BRGH=1 - this uses only four clock
 *  periods to sample each bit and can be very intolerant of
 *  baud rate %error - you may see framing errors.
 *
 *  \param baudRate Desired baud rate.
 */
static inline void CONFIG_BAUDRATE_UART1(uint32 baudRate) {
  uint32 brg = (FCY/baudRate/16) - 1;
  ASSERT(brg <= 0xFFFF);
  U1MODEbits.BRGH = 0;
  U1BRG = brg;
}


/**@{ \name Constants for the UxMODE.PDSEL bitfield
 *  Use with \ref CONFIG_PDSEL_UART1.
 */
#define UXMODE_PDSEL_8DATA_NOPARITY   0
#define UXMODE_PDSEL_8DATA_EVENPARITY 1
#define UXMODE_PDSEL_8DATA_ODDPARITY  2
#define UXMODE_PDSEL_9DATA_NOPARITY   3
///@}

/** Select the parity and number of data bits for the UART.
 *  Use constants UXMODE_PDSEL_8DATA_NOPARITY and following.
 *  \param u8_pdsel Parity and number of data bits.
 */
inline static void CONFIG_PDSEL_UART1(uint8 u8_pdsel) {
  ASSERT(u8_pdsel <= UXMODE_PDSEL_9DATA_NOPARITY);
  U1MODEbits.PDSEL = u8_pdsel;
}

/** Select the number of stop bits for this UART. Valid values
 *  are 1 or 2.
 *  \param u8_numStopbits Number of stop bits.
 */
inline static void CONFIG_STOPBITS_UART1(uint8 u8_numStopbits) {
  ASSERT(u8_numStopbits <= 2);
  U1MODEbits.STSEL = u8_numStopbits - 1;
}

/** Enable RX, TX for UART. */
static inline void ENABLE_UART1() {
  U1MODEbits.UEN = 0b00;                    // UxTX and UxRX pins are enabled and used; no flow control pins
  U1MODEbits.UARTEN = 1;                    // enable UART RX/TX
  U1STAbits.UTXEN = 1;                      //Enable the transmitter
}

/** Determine if a character is available in the UART's
 *  receive buffer.
 *  \return True (1) if character is available, 0 if not.
 */
#define IS_CHAR_READY_UART1() U1STAbits.URXDA

/** Determine if a the transmit buffer is full.
 *  \return True (1) if the transmit buffer if full,
 *          false (0) if not.
 */
#define IS_TRANSMIT_BUFFER_FULL_UART1() U1STAbits.UTXBF

/** Determines if all characters placed in the UART have been sent.
 *  Returns 1 if the last transmission has completed, or 0 if a transmission
 *  is in progress or queued in the transmit buffer.
 *  \return True (1) if the last transmission has completed, 0 if not.
 */
#define IS_TRANSMIT_COMPLETE_UART1() U1STAbits.TRMT

// communications commands used outside of ESOS tasks (like user_init routine)
// these routines/macros should almost never be used.
void __esos_hw_PutUint8(uint8 u8_c);
void __esos_hw_PutString(uint8* psz_in);
uint8 __esos_hw_GetUint8(void);


void configUART1(uint32 u32_baudRate);
void __esos_hw_InitCommSystem(void);


/** Waits until all characters placed in the UART have been sent. */
inline static void WAIT_UNTIL_TRANSMIT_COMPLETE_UART1() {
  while (!IS_TRANSMIT_COMPLETE_UART1())
    doHeartbeat();
}



#endif // end ESOS_PIC24_RS232_H
