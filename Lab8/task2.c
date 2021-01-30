#include "pic24_all.h"
#include <stdio.h>
//#include <stdint.h>

/** \file
 *  Demonstrates reading the internal ADC in 12-bit mode and
 *  then sending the upper 8-bits to an external
 *  8-bit SPI DAC (MAXIM 548A)
 */

#define CONFIG_SLAVE_ENABLE() CONFIG_RB3_AS_DIG_OUTPUT()
#define SLAVE_ENABLE()        _LATB3 = 0  //low true assertion
#define SLAVE_DISABLE()       _LATB3 = 1


void configSPI1(void) {
  //spi clock = 40MHz/1*4 = 40MHz/4 = 10MHz
  SPI1CON1 = SEC_PRESCAL_1_1 |     //1:1 secondary prescale
             PRI_PRESCAL_4_1 |     //4:1 primary prescale
             CLK_POL_ACTIVE_HIGH | //clock active high (CKP = 0)
             SPI_CKE_ON          | //out changes active to inactive (CKE=1)
             SPI_MODE8_ON        | //8-bit mode
             MASTER_ENABLE_ON;     //master mode
#if (defined(__dsPIC33E__) || defined(__PIC24E__))
  //nothing to do here. On this family, the SPI1 port uses dedicated
  //pins for higher speed. The SPI2 port can be used with remappable pins.
  //you may need to add code to disable analog functionality if the SPI ports
  //are on analog-capable pins.
#else
  CONFIG_SDO1_TO_RP(11);      //use RP6 for SDO
  CONFIG_SCK1OUT_TO_RP(12);   //use RP7 for SCLK
#endif

  SPI1STATbits.SPIEN = 1;  //enable SPI mode
}
void configDAC() {
  CONFIG_SLAVE_ENABLE();       //chip select for DAC
  SLAVE_DISABLE();             //disable the chip select
}

void writeDAC (uint8_t dacval) {
  SLAVE_ENABLE();                 //assert Chipselect line to DAC
  ioMasterSPI1(0b00001011);      //control byte that enables DAC A
  ioMasterSPI1(dacval);          //write DAC value
  SLAVE_DISABLE();
}

#define VREF 3  //assume Vref = 3 volts



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

/***** LCD Code *****/
#if __PIC24HJ128GP502__
#define LED1 _LATB2 // MicroStick II definitions
#define CONFIG_LED1() CONFIG_RB2_AS_DIG_OUTPUT()
#endif

 
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

typedef enum {
 STATE_RESET = 0,
 STATE_LED_ON,
 STATE_LED_OFF,
 STATE_LED_BLINK
 
} STATE;
STATE e_myState = STATE_RESET;


 
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

#define ISR_PERIOD 50 // in ms
void configTimer2(void) {
// T2CON set like this for documentation purposes.
// could be replaced by T2CON = 0x0020
 T2CON = T2_OFF | T2_IDLE_CON | T2_GATE_OFF
| T2_32BIT_MODE_OFF
| T2_SOURCE_INT
| T2_PS_1_64 ; // Results in T2CON = 0x0020
// Subtract 1 from ticks value assigned to PR2
 // because period is PRx + 1
 PR2 = msToU16Ticks (ISR_PERIOD, getTimerPrescale(T2CONbits)) - 1;
 TMR2 = 0; // Clear timer2 value
 _T2IF = 0; // Clear interrupt flag
 _T2IP = 1; // Choose a priority
 _T2IE = 1; // Enable the interrupt
 T2CONbits.TON = 1; // Turn on the timer
}

#define Vinmin 0
#define Vinmax 3
void _ISRFAST _T2Interrupt() {
  _T2IF = 0;
  uint16_t u16_adcVal;
  uint8_t u8_dacVal;
  float f_adcVal;
  float f_dacVal;
  char str[4];

u16_adcVal = convertADC1();   //get ADC value
    u8_dacVal = (u16_adcVal>>4) & 0x00FF;  //upper 8 bits to DAC value
    writeDAC(u8_dacVal);
    f_adcVal = u16_adcVal;
    f_adcVal = f_adcVal/4096.0 * VREF;  //convert to float 0.0 to VREF
    f_dacVal = u8_dacVal;
    f_dacVal = f_dacVal/256.0 * VREF;
#ifdef SMALLRAM
    {
      uint16_t ui16_adcValmv, ui16_dacValmv;
      ui16_adcValmv = f_adcVal * 1000;
      ui16_dacValmv = f_dacVal * 1000;
      printf("ADC in: %d mV (0x%04x), To DAC: %d mV (0x%02x) \n",
             ui16_adcValmv, u16_adcVal, ui16_dacValmv, u8_dacVal);
    }
#else
    printf("ADC in: %4.3f V (0x%04x), To DAC: %4.3f V (0x%02x) \n",
           (double) f_adcVal, u16_adcVal, (double) f_dacVal, u8_dacVal);
#endif

if(f_adcVal < 1.7) e_myState = STATE_LED_ON;
else if(f_adcVal > 1.7) e_myState = STATE_LED_BLINK;
else e_myState = STATE_LED_OFF;

int vScaled = (int) 255 * (f_adcVal-Vinmin)/(Vinmax-Vinmin);

writeLCD(0x01,0,0,1);
sprintf(str, "%.3d",vScaledl);
outStringLCD(str);


}


int main (void) {


  configBasic(HELLO_MSG);
  CONFIG_AN1_AS_ANALOG();
  configControlLCD();      //configure the LCD control lines
  configBusAsOutLCD();
  initLCD();               //initialize the LCD
  CONFIG_LED1();
  configTimer2();
  e_myState = STATE_LED_OFF;

  // Configure A/D to sample AN0 for 31 Tad periods in 12-bit mode
  // then perform a single conversion.
  configADC1_ManualCH0(ADC_CH0_POS_SAMPLEA_AN1, 31, 1);
  configSPI1();
  configDAC();
  while (1) {
	switch (e_myState) {
		case STATE_LED_ON:
			LED1 = 1;
			break;
		case STATE_LED_OFF:
			LED1 = 0;
			break;
		case STATE_LED_BLINK:
			LED1 = !LED1;
			break;
		
	
	default:
		e_myState = STATE_LED_OFF;
		break;

	}//end Switch
    doHeartbeat();
	DELAY_MS(50); // Delay 100ms
  } //end while(1)

}
