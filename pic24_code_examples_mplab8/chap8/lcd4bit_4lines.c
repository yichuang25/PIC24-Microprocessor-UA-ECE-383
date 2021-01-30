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

#include "pic24_all.h"


/** \file
Demonstrates a 4-line 20x4 Character LCD using the industry
standard parallel interface in 4-bit mode.
Assumes a 5V LCD; digital-only IO pins are assumed
to be used for the four-bit data bus because they are
5V input tolerant.

Tested with Hantronix HDM20416L-M.
*/

#define RS_HIGH()        _LATB9 = 1
#define RS_LOW()         _LATB9 = 0
#define CONFIG_RS()      CONFIG_RB9_AS_DIG_OUTPUT()

#define RW_HIGH()        _LATB13 = 1
#define RW_LOW()         _LATB13 = 0
#define CONFIG_RW()      CONFIG_RB13_AS_DIG_OUTPUT()

#define E_HIGH()         _LATB14 = 1
#define E_LOW()          _LATB14 = 0
#define CONFIG_E()       CONFIG_RB14_AS_DIG_OUTPUT()

#define LCD4O          _LATB5
#define LCD5O          _LATB6
#define LCD6O          _LATB7
#define LCD7O          _LATB8
#define LCD7I          _RB8

#define CONFIG_LCD4_AS_INPUT() CONFIG_RB5_AS_DIG_INPUT()
#define CONFIG_LCD5_AS_INPUT() CONFIG_RB6_AS_DIG_INPUT()
#define CONFIG_LCD6_AS_INPUT() CONFIG_RB7_AS_DIG_INPUT()
#define CONFIG_LCD7_AS_INPUT() CONFIG_RB8_AS_DIG_INPUT()

#define CONFIG_LCD4_AS_OUTPUT() CONFIG_RB5_AS_DIG_OUTPUT()
#define CONFIG_LCD5_AS_OUTPUT() CONFIG_RB6_AS_DIG_OUTPUT()
#define CONFIG_LCD6_AS_OUTPUT() CONFIG_RB7_AS_DIG_OUTPUT()
#define CONFIG_LCD7_AS_OUTPUT() CONFIG_RB8_AS_DIG_OUTPUT()

#define GET_BUSY_FLAG()  LCD7I



/**
 Functions above this line must be redefined for
 your particular PICmicro-to-LCD interface
*/

//Configure 4-bit data bus for output
void configBusAsOutLCD(void) {
  RW_LOW();                  //RW=0 to stop LCD from driving pins
  CONFIG_LCD4_AS_OUTPUT();   //D4
  CONFIG_LCD5_AS_OUTPUT();   //D5
  CONFIG_LCD6_AS_OUTPUT();   //D6
  CONFIG_LCD7_AS_OUTPUT();   //D7
}

//Configure 4-bit data bus for input
void configBusAsInLCD(void) {
  CONFIG_LCD4_AS_INPUT();   //D4
  CONFIG_LCD5_AS_INPUT();   //D5
  CONFIG_LCD6_AS_INPUT();   //D6
  CONFIG_LCD7_AS_INPUT();   //D7
  RW_HIGH();                   // R/W = 1, for read
}

//Output lower 4-bits of u8_c to LCD data lines
void outputToBusLCD(uint8_t u8_c) {
  LCD4O = u8_c & 0x01;          //D4
  LCD5O = (u8_c >> 1)& 0x01;    //D5
  LCD6O = (u8_c >> 2)& 0x01;    //D6
  LCD7O = (u8_c >> 3)& 0x01;    //D7
}

//Configure the control lines for the LCD
void configControlLCD(void) {
  CONFIG_RS();     //RS
  CONFIG_RW();     //RW
  CONFIG_E();      //E
  RW_LOW();
  E_LOW();
  RS_LOW();
}

//Pulse the E clock, 1 us delay around edges for
//setup/hold times
void pulseE(void) {
  DELAY_US(1);
  E_HIGH();
  DELAY_US(1);
  E_LOW();
  DELAY_US(1);
}

/* Write a byte (u8_Cmd) to the LCD.
u8_DataFlag is '1' if data byte, '0' if command byte
u8_CheckBusy is '1' if must poll busy bit before write, else simply delay before write
u8_Send8Bits is '1' if must send all 8 bits, else send only upper 4-bits
*/
void writeLCD(uint8_t u8_Cmd, uint8_t u8_DataFlag,
              uint8_t u8_CheckBusy, uint8_t u8_Send8Bits) {

  uint8_t u8_BusyFlag;
  uint8_t u8_wdtState;
  if (u8_CheckBusy) {
    RS_LOW();            //RS = 0 to check busy
    // check busy
    configBusAsInLCD();  //set data pins all inputs
    u8_wdtState = _SWDTEN;  //save WDT enable state
    CLRWDT();  			   //clear the WDT timer
    _SWDTEN = 1;            //enable WDT to escape infinite wait
    do {
      E_HIGH();
      DELAY_US(1);  // read upper 4 bits
      u8_BusyFlag = GET_BUSY_FLAG();
      E_LOW();
      DELAY_US(1);
      pulseE();              //pulse again for lower 4-bits
    } while (u8_BusyFlag);
    _SWDTEN = u8_wdtState;   //restore WDT enable state
  } else {
    DELAY_MS(10); // don't use busy, just delay
  }
  configBusAsOutLCD();
  if (u8_DataFlag) RS_HIGH();   // RS=1, data byte
  else    RS_LOW();             // RS=0, command byte
  outputToBusLCD(u8_Cmd >> 4);  // send upper 4 bits
  pulseE();
  if (u8_Send8Bits) {
    outputToBusLCD(u8_Cmd);     // send lower 4 bits
    pulseE();
  }
}

//These definitions are for a Hantronix 20x4 LCD
#define GOTO_LINE1() writeLCD(0x80,0,1,1)
#define GOTO_LINE2() writeLCD(0xC0,0,1,1)
#define GOTO_LINE3() writeLCD(0x94,0,1,1)
#define GOTO_LINE4() writeLCD(0xD4,0,1,1)

// Initialize the LCD, modify to suit your application and LCD
void initLCD() {
  DELAY_MS(50);          //wait for device to settle
  writeLCD(0x20,0,0,0); // 4 bit interface
  writeLCD(0x28,0,0,1); // 2 line display, 5x7 font
  writeLCD(0x28,0,0,1); // repeat
  writeLCD(0x06,0,0,1); // enable display
  writeLCD(0x0C,0,0,1); // turn display on; cursor, blink is off
  writeLCD(0x01,0,0,1); // clear display, move cursor to home
  DELAY_MS(3);
}

//Output a string to the LCD
void outStringLCD(char *psz_s) {
  while (*psz_s) {
    writeLCD(*psz_s, 1, 1,1);
    psz_s++;
  }
}


int main (void) {
  configBasic(HELLO_MSG);      // Set up heartbeat, UART, print hello message and diags

  configControlLCD();      //configure the LCD control lines
  initLCD();               //initialize the LCD

  GOTO_LINE1();          // cursor to 1st line
  outStringLCD("Line 1");
  GOTO_LINE2();          // cursor to 2nd line
  outStringLCD("Line 2");
  GOTO_LINE3();          // cursor to 3rd line
  outStringLCD("Line 3");
  GOTO_LINE4();          // cursor to 4th line
  outStringLCD("Line 4");
  while (1) {
    doHeartbeat();
  }
}
