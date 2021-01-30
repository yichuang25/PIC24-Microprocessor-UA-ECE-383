#include "pic24_all.h"


#define RS_HIGH()        _LATB8 = 1
#define RS_LOW()         _LATB8 = 0
#define CONFIG_RS()      CONFIG_RB8_AS_DIG_OUTPUT()
#define ENABLE_OPEN_DRAIN_RB8() _ODCB8 = 1;

#define RW_HIGH()        _LATB9 = 1
#define RW_LOW()         _LATB9 = 0
#define CONFIG_RW()      CONFIG_RB9_AS_DIG_OUTPUT()
#define ENABLE_OPEN_DRAIN_RB9() _ODCB9 = 1;

#define E_HIGH()         _LATB13 = 1
#define E_LOW()          _LATB13 = 0
#define CONFIG_E()       CONFIG_RB13_AS_DIG_OUTPUT()
#define ENABLE_OPEN_DRAIN_RB13() _ODCB13 = 1;


#define LCD4O          _LATB4
#define ENABLE_OPEN_DRAIN_RB4() _ODCB4 = 1;

#define LCD5O          _LATB5
#define ENABLE_OPEN_DRAIN_RB5() _ODCB5 = 1;

#define LCD6O          _LATB6
#define ENABLE_OPEN_DRAIN_RB6() _ODCB6 = 1;

#define LCD7O          _LATB7
#define LCD7I          _RB7
#define ENABLE_OPEN_DRAIN_RB7() _ODCB7 = 1;

#define CONFIG_LCD4_AS_INPUT() CONFIG_RB4_AS_DIG_INPUT()
#define CONFIG_LCD5_AS_INPUT() CONFIG_RB5_AS_DIG_INPUT()
#define CONFIG_LCD6_AS_INPUT() CONFIG_RB6_AS_DIG_INPUT()
#define CONFIG_LCD7_AS_INPUT() CONFIG_RB7_AS_DIG_INPUT()

#define CONFIG_LCD4_AS_OUTPUT() CONFIG_RB4_AS_DIG_OUTPUT()
#define CONFIG_LCD5_AS_OUTPUT() CONFIG_RB5_AS_DIG_OUTPUT()
#define CONFIG_LCD6_AS_OUTPUT() CONFIG_RB6_AS_DIG_OUTPUT()
#define CONFIG_LCD7_AS_OUTPUT() CONFIG_RB7_AS_DIG_OUTPUT()

#define GET_BUSY_FLAG()  LCD7I

/**
Functions above this line must be redefined for
your particular PICmicro-to-LCD interface
*/

//Configure 4-bit data bus for output
void configBusAsOutLCD(void) {
  RW_LOW();                  //RW=0 to stop LCD from driving pins
  CONFIG_LCD4_AS_OUTPUT();   //D4
  ENABLE_OPEN_DRAIN_RB4();

  CONFIG_LCD5_AS_OUTPUT();   //D5
  ENABLE_OPEN_DRAIN_RB5();

  CONFIG_LCD6_AS_OUTPUT();   //D6
  ENABLE_OPEN_DRAIN_RB6();

  CONFIG_LCD7_AS_OUTPUT();   //D7
  ENABLE_OPEN_DRAIN_RB7();
}

//Configure 4-bit data bus for input
void configBusAsInLCD(void) {
  CONFIG_LCD4_AS_INPUT();   //D4
ENABLE_OPEN_DRAIN_RB4();

  CONFIG_LCD5_AS_INPUT();   //D5
ENABLE_OPEN_DRAIN_RB5();

  CONFIG_LCD6_AS_INPUT();   //D6
ENABLE_OPEN_DRAIN_RB6();

  CONFIG_LCD7_AS_INPUT();   //D7
ENABLE_OPEN_DRAIN_RB4();

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
ENABLE_OPEN_DRAIN_RB8();

  CONFIG_RW();     //RW
ENABLE_OPEN_DRAIN_RB9();

  CONFIG_E();      //E
ENABLE_OPEN_DRAIN_RB13();

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
u8_DataFlag is &#39;1&#39; if data byte, &#39;0&#39; if command byte
u8_CheckBusy is &#39;1&#39; if must poll busy bit before write, else simply delay before write
u8_Send8Bits is &#39;1&#39; if must send all 8 bits, else send only upper 4-bits
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
    CLRWDT();                                         //clear the WDT timer
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
    DELAY_MS(10); // don&#39;t use busy, just delay
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
  //configBasic(HELLO_MSG);      // Set up heartbeat, UART, print hello message and diags

  configControlLCD();      //configure the LCD control lines
  initLCD();               //initialize the LCD

  outStringLCD("Shomari Thomas");
  writeLCD(0xC0,0,1,1);  // cursor to 2nd line
  outStringLCD("sdthomas4@crimson.ua.edu");
  while (1) {
   writeLCD(0x18,0,1,1);  // shift left
    DELAY_MS(200);
    doHeartbeat();
  }
}
