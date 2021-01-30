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


#define LED1                                      _LATB15
#define CONFIG_LED1()                 CONFIG_RB15_AS_DIG_OUTPUT()

#define LED2                                      _LATB1
#define CONFIG_LED2() CONFIG_RB2_AS_DIG_OUTPUT()


#define SW1                                                      _RB14
inline void CONFIG_SW1()
                {
                CONFIG_RB14_AS_DIG_INPUT();
                ENABLE_RB14_PULLUP();
                DELAY_US(1);
                }
#define SW1_PRESSED()    (SW1 == 0)
#define SW1_RELEASED()   (SW1 == 1)	

//semaphore variable
volatile uint8_t u8_pressAndRelease = 0;    //initially cleared

typedef enum  {
  STATE_RESET = 0,
  STATE_WAIT_FOR_PRESS,
  STATE_WAIT_FOR_RELEASE
} STATE;

volatile STATE e_mystate = STATE_WAIT_FOR_PRESS;

//Interrupt Service Routine for Timer3
void _ISRFAST _T3Interrupt (void) {
  if (!u8_pressAndRelease) {
    //semaphore is cleared, watch for another press & release
    switch (e_mystate) {
      case STATE_WAIT_FOR_PRESS:
        if (SW1_PRESSED()) {
          e_mystate = STATE_WAIT_FOR_RELEASE;
			LED2 = 1;
		  DELAY_US(5);
        }
        break;
      case STATE_WAIT_FOR_RELEASE:
        if (SW1_RELEASED()) {
          //have received a complete Press & Release.
          //Set the semaphore
          u8_pressAndRelease = 1;
          e_mystate = STATE_WAIT_FOR_PRESS;
			LED2 = 0;
			DELAY_US(5);
        }
        break;
      default:
        e_mystate = STATE_WAIT_FOR_PRESS;
    }
  }

  _T3IF = 0;                 //clear the timer interrupt bit
}

#define ISR_PERIOD     15                // in ms
void  configTimer3(void) {
  //ensure that Timer2,3 configured as separate timers.
  T2CONbits.T32 = 0;     // 32-bit mode off
  //T3CON set like this for documentation purposes.
  //could be replaced by T3CON = 0x0020
  T3CON = T3_OFF |T3_IDLE_CON | T3_GATE_OFF
          | T3_SOURCE_INT
          | T3_PS_1_64 ;  //results in T3CON= 0x0020
  PR3 = msToU16Ticks (ISR_PERIOD, getTimerPrescale(T3CONbits)) - 1;
  TMR3  = 0;                       //clear timer3 value
  _T3IF = 0;                       //clear interrupt flag
  _T3IP = 1;                       //choose a priority
  _T3IE = 1;                       //enable the interrupt
  T3CONbits.TON = 1;               //turn on the timer
}

#define SW2                                                      _RB12
inline void CONFIG_SW2()
                {
                CONFIG_RB12_AS_DIG_INPUT();
                ENABLE_RB12_PULLUP();
                DELAY_US(1);
                }
#define SW2_PRESSED() (SW2 == 0)
#define SW2_RELEASED() (SW2 == 1)

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

int count = 0;
char str[3];
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
ENABLE_OPEN_DRAIN_RB7();

  RW_HIGH();                   // R/W = 1, for read
}

//Output lower 4-bits of u8_c to LCD data lines
void outputToBusLCD(uint8_t u8_c) {
  LCD4O = u8_c & 0x01;          //D4
  LCD5O = (u8_c >> 1)& 0x01;    //D5
  LCD6O = (u8_c >> 2)& 0x01;    //D6
  LCD7O = (u8_c >> 3)& 0x01;    //D,7
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

/* Write a byte (u8_Cmd) to the LCD.-
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
	configBasic(HELLO_MSG);      // Set up heartbeat, UART, print hello message and diags
  	configControlLCD();      //configure the LCD control lines
  	configBusAsOutLCD();
  	initLCD();               //initialize the LCD
  	CONFIG_SW2();
  	CONFIG_SW1();
  	configTimer3();
  	CONFIG_LED1();
  	CONFIG_LED2();
  	count = 0;
  	writeLCD(0x01,0,0,1);
  	sprintf(str, "%.3d",count);
  	outStringLCD(str);
	while (1){
		if (u8_pressAndRelease) {
			//	LED1 = 0;
			count++;
			u8_pressAndRelease = 0;	
		}
		if (SW2_PRESSED()) {
			count = 0;
			
		}					
		DELAY_MS(15);
		if (count == 60) {
			LED1 = 1;
			DELAY_MS(1000);
			count = 0;
			LED1 = 0;
		}
		writeLCD(0x01,0,0,1);
 		sprintf(str, "%.3d",count);
  		outStringLCD(str);
  	}
}
