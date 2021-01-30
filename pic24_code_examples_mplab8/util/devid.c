#include "p24Hxxxx.h"
#include <stdio.h>
#include "pic24_uart.h"
#include "pic24_delay.h"


/** Notes
 *  - The end of the p24HJ12GP201.h include file in
 *    C:\Program Files\Microchip\MPLAB C30\support\h
 *    contains lots of useful stuff:
 *    - It defines bit names for all uniquely-named bits
 *      in the chip.
 *      For example, from line 1828 of v3.01 of the file:
 *        #define _C SRbits.C
 *   - Useful macros are defined starting on line 2741 of the
 *     file: Nop(), ClrWdt(), and so on.
 *   - Macros for delcaring ISRs at line 2798:
 *     _ISR and _ISRFAST. Using the correct name for the
 *     interrupt (see comments near that line) cause it to
 *     be placed at the appropriate place in the interrupt
 *     table.
 *   - Config bit setting begin at line 2843. Instead of
 *     ORing bits, AND them. To quote the
 *     comments there:
 *     ** Only one invocation of (each config register) should appear in a project,
 *     ** at the top of a C source file (outside of any function).
 */


// Define if using the simulator
//#define SIM

#include "pic24_clockfreq.h"
#include "pic24_configbits.h"
#include "pic24_delay.h"
#include "pic24_uart.h"
#include "pic24_ports.h"

typedef short          Word16;
typedef unsigned short UWord16;
typedef long           Word32;
typedef unsigned long  UWord32;
typedef union tuReg32 {
  UWord32 Val32;

  struct {
    UWord16 LW;
    UWord16 HW;
  } Word;

  char Val[4];
} uReg32;

extern UWord32 ReadLatch(UWord16, UWord16);
/*
IO configuration Macros go here
*/

//_PCFG2 = 1;// Use RB0/AN2 for digital I/O
//_TRISB0 = 0;// Define RB0 as a digital output
//port Macros defined in pic24_ports.h
#define CONFIG_LED1   CONFIG_RB13_AS_DIG_OUTPUT

#define LED1  _LATB13
//Prints out the Device ID, ProcessID of the processor
int main (void) {
  unsigned char c;
  configClock();   //defined in pic24_clockfreq.c

  /*************************  UART config ********************/
  //UART PIN config macros defined in "pic24_uart.h"
  CONFIG_UART_TX_TO_RP15;
  CONFIG_UART_RX_TO_RP14;
  //CONFIG_UART_TX_TO_RP0;
  //CONFIG_UART_RX_TO_RP1;
  CONFIG_UART_BAUDRATE(38400);
  // 2. Set the number of data bits, number of Stop bits and
  //    parity selection by writing to the PDSEL<1:0>
  //    (UxMODE<2:1>) and STSEL (UxMODE<0>) bits.
  CONFIG_UART_8DATA_NOPARITY;     // 8-bit data, no parity
  CONFIG_UART_ONE_STOPBIT;  // 1 Stop bit

  ENABLE_UART;

  /*************************** GPIO config ***************************/
  CONFIG_LED1;
  puts("Hello from Devid!\n\r");
  {
    uReg32 SourceAddr;
    uReg32 Temp;
    UWord16 devid;
    UWord16 processid;

    SourceAddr.Val32 = 0xFF0000;

    Temp.Val32 = ReadLatch(SourceAddr.Word.HW, SourceAddr.Word.LW);
    devid = ((Temp.Val[1] << 8) &0xFF00)|(Temp.Val[0]);
    printf("Device ID: %x\n\r",devid);

    SourceAddr.Val32 = 0xFF0002;

    Temp.Val32 = ReadLatch(SourceAddr.Word.HW, SourceAddr.Word.LW);

    processid = (Temp.Val[1]>>4)&0x0F;
    printf("Process ID: %x\n\r",processid);
  }
  /************************* Echo code ******************************/
  // Echo character + 1
  // Toggle LED with each character received.
  while (1);

  return 0;
}
