/*
Header file that includes all pic24*.h files
*/


#ifndef  _PIC24_ALL_H_
#define _PIC24_ALL_H_


// Include processor-specific header file
#if defined(__PIC24H__)
#include "p24Hxxxx.h"
#elif defined(__PIC24F__)
#include "p24Fxxxx.h"
#elif defined(__dsPIC33F__)
#include "p33Fxxxx.h"
#elif defined(__PIC24FK__)
#include "p24Fxxxx.h"      //this is a variant of the PIC24F family
#elif defined(__PIC24E__)
#include "p24Exxxx.h"      //this is a variant of the PIC24F family
#elif defined(__dsPIC33E__)
#include "p33Exxxx.h"      //this is a variant of the dsPIC family
#else
#error Unknown processor.
#endif

// For convenience, choose the fastest
// possible clock depending on which
// processor we're using. If simulation mode is
// selected, then use the simulation clock.
#ifndef CLOCK_CONFIG
#if defined(SIM)
#define CLOCK_CONFIG SIM_CLOCK
#elif defined(EXPLORER16_100P) && defined(__PIC24H__)
#define CLOCK_CONFIG PRIPLL_8MHzCrystal_40MHzFCY
#elif defined(EXPLORER16_100P) && defined(__PIC24F__)
#define CLOCK_CONFIG PRIPLL_8MHzCrystal_16MHzFCY
#elif defined(__PIC24H__) || defined(__DOXYGEN__)
#define CLOCK_CONFIG FRCPLL_FCY40MHz
#elif defined(__PIC24F__)  || defined(__PIC24FK__)
#define CLOCK_CONFIG FRCPLL_FCY16MHz
#elif defined(__dsPIC33F__)
#define CLOCK_CONFIG FRCPLL_FCY40MHz
#elif defined(__dsPIC33E__)
#define CLOCK_CONFIG FRCPLL_FCY60MHz
#elif defined(__PIC24E__)
#define CLOCK_CONFIG FRCPLL_FCY60MHz
//#define CLOCK_CONFIG PRIPLL_8MHzCrystal_40MHzFCY
#else
#error Unknown processor
#endif
#endif


typedef unsigned char       uint8_t;   //8 bits
typedef unsigned short      uint16_t;  //16 bits
typedef unsigned long       uint32_t;  //32 bits
typedef unsigned long long  uint64;  //64 bits


typedef signed char         int8_t;    //8 bits
typedef signed short        int16_t;   //16 bits
typedef signed long         int32_t;   //32 bits
typedef signed long long    int64;   //64 bits

#define BITS2WORD(sfrBitfield)  ( *((uint16_t*) &sfrBitfield) )
#define BITS2BYTEL(sfrBitfield) ( ((uint8_t*)  &sfrBitfield)[0] )
#define BITS2BYTEH(sfrBitfield) ( ((uint8_t*)  &sfrBitfield)[1] )

#define ASSERT(test)

#include  "pic24_clockfreq.h"

#if defined(__PIC24E__) || defined(__dsPIC33F__)
#define RPMAP_U1TX 		1
#define RPMAP_U2TX  	3
#else

#define RPMAP_U1TX 		3
#define RPMAP_U2TX  	5
#endif

#if defined(_U1RXR)
#define CONFIG_U1RX_TO_RP(pin) _U1RXR = pin
#else
#define CONFIG_U1RX_TO_RP(pin)
#endif
#if defined(_U1RXR)
#define CONFIG_U1TX_TO_RP(pin) _RP##pin##R = RPMAP_U1TX
#else
#define CONFIG_U1TX_TO_RP(pin)
#endif

#if defined(_U2RXR)
#define CONFIG_U2RX_TO_RP(pin) _U2RXR = pin
#else
#define CONFIG_U2RX_TO_RP(pin)
#endif
#if defined(_U2RXR)
#define CONFIG_U2TX_TO_RP(pin) _RP##pin##R = RPMAP_U2TX
#else
#define CONFIG_U2TX_TO_RP(pin)
#endif

/* To configure the UART selection

a. Set DEFAULT_UART to either 1, 2, 3, or 4.
b. Set the DEFAULT_BAUDRATE macro to a value. If you want to specify your
own value for the BRG register instead of having it calculated from FCY, then
edit the appropriate CONFIG_UARTx_BAUDRATE function to your needs.
c. If you are using a processor with reconfigurable pins, then you may need to
edit the CONFIG_DEFAULT_UART() function under the appropriate UART section to
map the UART pins to your needed RPx pins.

*/

#ifndef DEFAULT_UART
#if defined(EXPLORER16_100P)
#define DEFAULT_UART 2
#else
#define DEFAULT_UART 1
#endif
#endif



// Baud rate manually set
#ifndef DEFAULT_BAUDRATE
#define DEFAULT_BAUDRATE  57600
//#define DEFAULT_BAUDRATE  115200
//#define DEFAULT_BAUDRATE  230400
#endif



#define CONFIG_UART1_8DATA_NOPARITY()  U1MODEbits.PDSEL = 00
#define CONFIG_UART1_ONE_STOPBIT()     U1MODEbits.STSEL = 0

// reg = truncate(FCY/16/baud - 1), actualBaud = FCY/16/(reg + 1)
//for BRGH=0 (16 clocks for each bit)
//Be careful about using BRGH=1 - this uses only four clock
//periods to sample each bit and can be very intolerant of
//baud rate %error  - you may see framing errors

#if DEFAULT_BAUDRATE==230400
static inline void CONFIG_UART1_BAUDRATE(uint32_t x) {
  //for high baud rates, don't compute.
  U1BRG = 10; // 230400 baud assuming FCY = 40 MHZ, change to match your FCY
  U1MODEbits.BRGH = 0;
}
#else
static inline void CONFIG_UART1_BAUDRATE(uint32_t x) {
  uint32_t brg = FCY/x;
  //simple rounding
  if ((brg&0x0F) >= 8) brg = brg/16;
  else brg = brg/16-1;
  U1BRG = brg;
  U1MODEbits.BRGH = 0;

}
#endif


/*
Enable RX, TX for UART.
*/
// _UEN = 0b00;
// UxCTS, UxRTS and BCLKx pins are controlled by
// 4. Enable the UART module by setting the UARTEN (UxMODE<15>) bit.
//_UARTEN = 1;     // port latches
// 5. by setting the UTXEN (UxSTA<10>) bit,
//    which will also set the UxTXIF bit. The UxTXIF bit should be
//    cleared in the software routine that services the UART
//    transmit interrupt. The operation of the UxTXIF bit is
//    controlled by the UTXISEL<1:0> control bits.
// _UTXEN = 1;

static inline void ENABLE_UART1() {
  U1MODEbits.UEN = 0b00;                      // UxTX and UxRX pins are enabled and used; no flow control pins
  U1MODEbits.UARTEN = 1;                    // enable UART RX/TX
  U1STAbits.UTXEN = 1;                       //Enable the transmitter
}

static inline void DISABLE_UART1() {
  U1MODEbits.UARTEN = 0;                    // enable UART RX/TX
  U1STAbits.UTXEN= 0;                       //Enable the transmitter
}

#if defined(_U2RXIF)

#define CONFIG_UART2_8DATA_NOPARITY()  U2MODEbits.PDSEL = 00
#define CONFIG_UART2_ONE_STOPBIT()     U2MODEbits.STSEL = 0

#if DEFAULT_BAUDRATE==230400
static inline void CONFIG_UART2_BAUDRATE(uint32_t x) {
  //for high baud rates, don't compute.
  U2BRG = 10; // 230400 baud assuming FCY = 40 MHZ, change to match your FCY
  U2MODEbits.BRGH = 0;
}
#else
static inline void CONFIG_UART2_BAUDRATE(uint32_t x) {
  uint32_t brg = FCY/x;
  //simple rounding
  if ((brg&0x0F) >= 8) brg = brg/16;
  else brg = brg/16-1;
  U2BRG = brg;
  U2MODEbits.BRGH = 0;

}
#endif

static inline void ENABLE_UART2() {
  U2MODEbits.UEN = 0b00;                      // UxTX and UxRX pins are enabled and used; no flow control pins
  U2MODEbits.UARTEN = 1;                    // enable UART RX/TX
  U2STAbits.UTXEN = 1;                       //Enable the transmitter
}

static inline void DISABLE_UART2() {
  U2MODEbits.UARTEN = 0;                    // enable UART RX/TX
  U2STAbits.UTXEN= 0;                       //Enable the transmitter
}

#endif

#if defined(_U3RXIF)

#define CONFIG_UART3_8DATA_NOPARITY()  U3MODEbits.PDSEL = 00
#define CONFIG_UART3_ONE_STOPBIT()     U3MODEbits.STSEL = 0

#if DEFAULT_BAUDRATE==230400
static inline void CONFIG_UART3_BAUDRATE(uint32_t x) {
  //for high baud rates, don't compute.
  U3BRG = 10; // 230400 baud assuming FCY = 40 MHZ, change to match your FCY
  U3MODEbits.BRGH = 0;
}
#else
static inline void CONFIG_UART3_BAUDRATE(uint32_t x) {
  uint32_t brg = FCY/x;
  //simple rounding
  if ((brg&0x0F) >= 8) brg = brg/16;
  else brg = brg/16-1;
  U3BRG = brg;
  U3MODEbits.BRGH = 0;

}
#endif

static inline void ENABLE_UART3() {
  U3MODEbits.UEN = 0b00;                      // UxTX and UxRX pins are enabled and used; no flow control pins
  U3MODEbits.UARTEN = 1;                    // enable UART RX/TX
  U3STAbits.UTXEN = 1;                       //Enable the transmitter
}

static inline void DISABLE_UART3() {
  U3MODEbits.UARTEN = 0;                    // enable UART RX/TX
  U3STAbits.UTXEN= 0;                       //Enable the transmitter
}

#endif

#if defined(_U4RXIF)

#define CONFIG_UART4_8DATA_NOPARITY()  U4MODEbits.PDSEL = 00
#define CONFIG_UART4_ONE_STOPBIT()     U4MODEbits.STSEL = 0
#if DEFAULT_BAUDRATE==230400
static inline void CONFIG_UART4_BAUDRATE(uint32_t x) {
  //for high baud rates, don't compute.
  U4BRG = 10; // 230400 baud assuming FCY = 40 MHZ, change to match your FCY
  U4MODEbits.BRGH = 0;
}
#else
static inline void CONFIG_UART4_BAUDRATE(uint32_t x) {
  uint32_t brg = FCY/x;
  //simple rounding
  if ((brg&0x0F) >= 8) brg = brg/16;
  else brg = brg/16-1;
  U4BRG = brg;
  U4MODEbits.BRGH = 0;

}
#endif

static inline void ENABLE_UART4() {
  U4MODEbits.UEN = 0b00;                      // UxTX and UxRX pins are enabled and used; no flow control pins
  U4MODEbits.UARTEN = 1;                    // enable UART RX/TX
  U4STAbits.UTXEN = 1;                       //Enable the transmitter
}

static inline void DISABLE_UART4() {
  U4MODEbits.UARTEN = 0;                    // enable UART RX/TX
  U4STAbits.UTXEN= 0;                       //Enable the transmitter
}

#endif

#if DEFAULT_UART==1

#define DEFAULT_UART_FERR U1STAbits.FERR
#define DEFAULT_UART_OERR U1STAbits.OERR
#define DEFAULT_UART_RXREG U1RXREG
#define DEFAULT_UART_URXDA U1STAbits.URXDA
#define DEFAULT_UART_UTXBF U1STAbits.UTXBF
#define DEFAULT_UART_TXREG U1TXREG

static inline void CONFIG_DEFAULT_UART(void) {
  DISABLE_UART1();
#if defined(STARTER_BOARD_28P) && defined(_U1RXR)
  CONFIG_U1RX_TO_RP(9);
  CONFIG_U1TX_TO_RP(8);
#elif (defined(__PIC24E__) || defined(__dsPIC33E__))
  CONFIG_U1RX_TO_RP(42);
  CONFIG_U1TX_TO_RP(43);
#warning  UART1 RX pin configured to RP42(RB10), UART1 TX pin configured to RP43(RB11)
#elif defined(_U1RXR)
  CONFIG_U1RX_TO_RP(10);
  CONFIG_U1TX_TO_RP(11);
#warning  UART1 RX pin configured to RP10(RB10), UART1 TX pin configured to RP11(RB11)
#endif
#if defined(__PIC24FK__)
#if ( defined(__PIC24F16KA102__) || defined(__PIC24F08KA102__))
  AD1PCFG=0xFFFF; //disable analog pin functionality since  UART hard-mapped.
#else
#error UART not configured! Edit pic24_all.h
#endif
#endif
  CONFIG_UART1_BAUDRATE(DEFAULT_BAUDRATE);
  // 2. Set the number of data bits, number of Stop bits and
  //    parity selection by writing to the PDSEL<1:0>
  //    (UxMODE<2:1>) and STSEL (UxMODE<0>) bits.
  CONFIG_UART1_8DATA_NOPARITY();     // 8-bit data, no parity
  CONFIG_UART1_ONE_STOPBIT();  // 1 Stop bit
  ENABLE_UART1();
}

#endif  //end DEFAULT_UART==1

#if DEFAULT_UART==2

#define DEFAULT_UART_FERR U2STAbits.FERR
#define DEFAULT_UART_OERR U2STAbits.OERR
#define DEFAULT_UART_RXREG U2RXREG
#define DEFAULT_UART_URXDA U2STAbits.URXDA
#define DEFAULT_UART_UTXBF U2STAbits.UTXBF
#define DEFAULT_UART_TXREG U2TXREG


static inline void CONFIG_DEFAULT_UART(void) {
  DISABLE_UART2();
#ifdef _U2RXR
#error UART2 RX,TX pins must be configured to RPxx pins!!! Edit pic24_all.h to chose your RP pins!
#endif
  CONFIG_UART2_BAUDRATE(DEFAULT_BAUDRATE);
  // 2. Set the number of data bits, number of Stop bits and
  //    parity selection by writing to the PDSEL<1:0>
  //    (UxMODE<2:1>) and STSEL (UxMODE<0>) bits.
  CONFIG_UART2_8DATA_NOPARITY();     // 8-bit data, no parity
  CONFIG_UART2_ONE_STOPBIT();  // 1 Stop bit
  ENABLE_UART2();
}

#endif  //end DEFAULT_UART==2


#if DEFAULT_UART==3

#define DEFAULT_UART_FERR U3STAbits.FERR
#define DEFAULT_UART_OERR U3STAbits.OERR
#define DEFAULT_UART_RXREG U3RXREG
#define DEFAULT_UART_URXDA U3STAbits.URXDA
#define DEFAULT_UART_UTXBF U3STAbits.UTXBF
#define DEFAULT_UART_TXREG U3TXREG


static inline void CONFIG_DEFAULT_UART(void) {
  DISABLE_UART3();
#ifdef _U3RXR
#error UART3 RX,TX pins must be configured to RPxx pins!!! Edit pic24_all.h to chose your RP pins!
#endif
  CONFIG_UART3_BAUDRATE(DEFAULT_BAUDRATE);
  // 2. Set the number of data bits, number of Stop bits and
  //    parity selection by writing to the PDSEL<1:0>
  //    (UxMODE<2:1>) and STSEL (UxMODE<0>) bits.
  CONFIG_UART3_8DATA_NOPARITY();     // 8-bit data, no parity
  CONFIG_UART3_ONE_STOPBIT();  // 1 Stop bit
  ENABLE_UART3();
}

#endif  //end DEFAULT_UART==3


#if DEFAULT_UART==4

#define DEFAULT_UART_FERR U4STAbits.FERR
#define DEFAULT_UART_OERR U4STAbits.OERR
#define DEFAULT_UART_RXREG U4RXREG
#define DEFAULT_UART_URXDA U4STAbits.URXDA
#define DEFAULT_UART_UTXBF U4STAbits.UTXBF
#define DEFAULT_UART_TXREG U4TXREG


static inline void CONFIG_DEFAULT_UART(void) {
  DISABLE_UART4();
#ifdef _U4RXR
#error UART4 RX,TX pins must be configured to RPxx pins!!! Edit pic24_all.h to chose your RP pins!
#endif
  CONFIG_UART4_BAUDRATE(DEFAULT_BAUDRATE);
  // 2. Set the number of data bits, number of Stop bits and
  //    parity selection by writing to the PDSEL<1:0>
  //    (UxMODE<2:1>) and STSEL (UxMODE<0>) bits.
  CONFIG_UART4_8DATA_NOPARITY();     // 8-bit data, no parity
  CONFIG_UART4_ONE_STOPBIT();  // 1 Stop bit
  ENABLE_UART4();
}

#endif  //end DEFAULT_UART==4





#endif




