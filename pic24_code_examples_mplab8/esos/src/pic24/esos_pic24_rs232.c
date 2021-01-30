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


/**
 * \addtogroup ESOS_UART_Service
 * @{
 */


// Documentation for this file. If the \file tag isn't present,
// this file won't be documented.
/** \file
 *  To do.
 */

/*** I N C L U D E S *************************************************/
#include "esos_pic24_rs232.h"

/*** G L O B A L S *************************************************/

/*** T H E   C O D E *************************************************/
#define   __ESOS_HW_SIGNAL_START_TX()         _U1TXIE = 1
#define   __ESOS_HW_SIGNAL_STOP_TX()          _U1TXIE = 0

/*********************************************************
 * Public functions intended to be called by other files *
 *********************************************************/
inline void    __esos_hw_signal_start_tx(void) {
  __ESOS_HW_SIGNAL_START_TX();
}

inline void    __esos_hw_signal_stop_tx(void) {
  __ESOS_HW_SIGNAL_STOP_TX();
}



/* ########################################################################### */
void _ISRFAST _U1TXInterrupt (void) {
  if (__st_TxBuffer.u16_Head == __st_TxBuffer.u16_Tail) {
    //empty TX buffer, disable the interrupt, do not clear the flag
    __ESOS_HW_SIGNAL_STOP_TX();
  } else {
    //at least one free spot in the TX buffer!
    __st_TxBuffer.u16_Tail++;      //increment tail pointer
    if (__st_TxBuffer.u16_Tail == ESOS_SERIAL_IN_EP_SIZE)
      __st_TxBuffer.u16_Tail = 0;  //wrap if needed
    _U1TXIF = 0;                  //clear the interrupt flag
    //transfer character from software buffer to transmit buffer
    U1TXREG =  __st_TxBuffer.pau8_Data[__st_TxBuffer.u16_Tail];
  }
}

void _ISRFAST _U1RXInterrupt (void) {
  int8 u8_c;

  _U1RXIF = 0;          //clear the UART RX interrupt bit

  // This fcn is found in pic24_uart.c which we've replaced with
  //   our own ESOS versions.
  u8_c = U1RXREG;       //read character
  __st_RxBuffer.u16_Head++;     //increment head pointer
  if (__st_RxBuffer.u16_Head == ESOS_SERIAL_OUT_EP_SIZE)
    __st_RxBuffer.u16_Head = 0; //wrap if needed

  __st_RxBuffer.pau8_Data[__st_RxBuffer.u16_Head] = u8_c;   //place in buffer
}

/** Configure the UART. Settings chosen:
 *  - TX is on RP11
 *  - RX is on RP10
 *  - Format is 8 data bits, no parity, 1 stop bit
 *  - CTS, RTS, and BCLK not used
 *
 *  \param u32_baudRate The baud rate to use.
 */
void configUART1(uint32 u32_baudRate) {
  /*************************  UART config ********************/
  //Pin mapping macros in pic24_ports.h

  CONFIG_RP10_AS_DIG_PIN();              //RX RP pin must be digital. What about TX pin?
  CONFIG_U1RX_TO_RP(10);                 //U1RX <- RP10
  CONFIG_U1TX_TO_RP(11);                 //U1TX -> RP11

  //UART macros defined in "pic24_uart.h"
  CONFIG_BAUDRATE_UART1(u32_baudRate);   //baud rate
  CONFIG_PDSEL_UART1(UXMODE_PDSEL_8DATA_NOPARITY);        // 8-bit data, no parity
  CONFIG_STOPBITS_UART1(1);            // 1 Stop bit

  // ESOS comm system uses UART1_RX IRQs
  _U1RXIF = 0;              //clear the flag
  _U1RXIP = __ESOS_UART1_RX_INTERRUPT_PRIORITY;  //choose a priority
  _U1RXIE = 1;              //enable the interrupt

  // ESOS comm system uses UART1_RX IRQs
  // do not clear the U1TXIF flag!
  _U1RXIP = __ESOS_UART1_TX_INTERRUPT_PRIORITY;  //choose a priority
  //do not enable the interrupt until we try to write to the UART

  ENABLE_UART1();                        //enable the UART
}

/* ########################################################################### */


/******************************************************************************
 * Function:        void _esos_hw_InitSerialUart( void )
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          ptr to ESOS_COMM_BUFF_DSC structure with initialized ptrs
 *
 * Side Effects:    Turns on USART hardware
 *
 *****************************************************************************/
void __esos_hw_InitCommSystem(void) {
  // use the MSSTATE PIC24 routines to init the RS232 comm subsystem
  // 8N1 @ 56k7 baud (DEFAULT_BAUDRATE) for now
  configUART1(DEFAULT_BAUDRATE) ;

}  // end __esos_hw_InitCommSystem()


/******************************************************************************
 * Function:        uint8 esos_GetCommSystemMaxInDataLen(void)
 *
 * PreCondition:    None.
 *
 * Input:           None
 *
 * Output:          the maximum number of uint8s that the comm system will
 *                  receive in a single buffer transfer from the host -- OR --
 *					in the case of single uint8 xfers (like RS232), the maximum
 *					number of uint8s that can be RX-ed before the buffers
 *					overflow
 *
 * Side Effects:    None
 *
 * Overview:        A way for a run-time determination of the maximum buffer
 *                  size that the user can can expect.  This number is
 *                  actually hard-coded in the USB CDC header file, but this
 *                  method will allow the user code to be more generic, if
 *                  it chooses to be.
 *
 *****************************************************************************/
uint8	esos_GetCommSystemMaxInDataLen(void) {
  return ESOS_SERIAL_OUT_EP_SIZE;
} //end esos_GetCommSystemMaxInDataLen()

/******************************************************************************
 * Function:        uint8 esos_GetCommSystemMaxOutDataLen(void)
 *
 * PreCondition:    None.
 *
 * Input:           None
 *
 * Output:          the maximum number of uint8s that the comm system will
 *                  transfer back to the host in a single buffer  -- OR --
 *					in the case of singe uint8 xfers (like RS232), the maximum
 *					number of uint8s in the output buffer before overflow
 *
 * Side Effects:    None
 *
 * Overview:        A way for a run-time determination of the maximum buffer
 *                  size that the user can can send efficiently.  The USB system
 *                  will send a bigger buffer than getUSBCdcTxMax() size, but
 *                  will do so in several smaller getUSBCdcTxMax()-sized chunks.
 *
 *                  This number is actually hard-coded in the USB CDC header file,
 *                  but this method will allow the user code to be more generic,
 *                  if it chooses to be.
 *
 *****************************************************************************/
uint8 esos_GetCommSystemMaxOutDataLen(void) {
  return  ESOS_SERIAL_IN_EP_SIZE;
} //end esos_GetCommSystemMaxOutDataLen()

/******************************************************************************
 * Function:        uint8 _esos_hw_GetUartVersion(void)
 *
 * PreCondition:    None.
 *
 * Input:           None
 *
 * Output:          Return the version number of the MSU Bulk CDC driver firmware
 *                  currently running.
 *                  The most-significant bit denotes we're running USB
 *                  The most-significant nibble is the major revision number
 *                  The least-significant nibble is the minor revision number
 *
 * Side Effects:    None
 *
 *****************************************************************************/
uint8 _esos_hw_GetSerialUartVersion(void) {
  return  ESOS_COMM_SYS_SERIAL_REV;
} //end _esos_hw_GetUartVersion()

/** @} */
