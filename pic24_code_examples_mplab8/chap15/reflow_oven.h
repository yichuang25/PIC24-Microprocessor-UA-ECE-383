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
#ifndef _PIC24_REFLOW_H_
#define _PIC24_REFLOW_H_

typedef struct _PSET {
  uint8_t u8_dc;
  uint8_t u8_period;
} PSET;

#if defined(__PIC24HJ64GP502__) || defined(__PIC24FJ64GA002__)
#define LAST_IMPLEMENTED_PMEM 0x00ABFF
#elif defined(__PIC24HJ32GP202__) || defined(__PIC24FJ32GA002__) || defined(__dsPIC33FJ32GP202__)
#define LAST_IMPLEMENTED_PMEM 0x0057FF
#elif defined(__dsPIC33FJ128GP802__)
#define LAST_IMPLEMENTED_PMEM 0x0157FF
#else
#error "Define LAST_IMPLEMENTED_PMEM for your processor!
#endif



//calculate starting address of last page of program memory
#define DATA_FLASH_PAGE (LAST_IMPLEMENTED_PMEM/FLASH_PAGESIZE)*FLASH_PAGESIZE  //each instruction occupies two program memory addresses

#define FREE_STATUS 0x69A5    //arbitrarily chosen, different from USED_STATUS
#define USED_STATUS 0x965A    //arbitrarily chosen, different from FREE_STATUS

#define NUM_PROFILES  2   //number of profiles
#define LEADFREE 1
#define LEADTIN 0


/*
profile is hardcoded since generic lead-free versus
Tin-solder mix works ok
*/


typedef struct _PROFILE {
  int16_t i16_preheatTemp;   //target temperature
  uint16_t u16_preheatTime ;  //ramp
  int16_t i16_soakTemp;      //target temperature
  uint16_t u16_soakTime;
  int16_t i16_reflowTemp;    //time to maintain reflow in seconds
  uint16_t u16_reflowTime;
  uint16_t u16_reflowHoldTime;
  int16_t i16_coolTemp;
  uint16_t u16_coolTime;
  int16_t i16_wetTemp;
} PROFILE;

/* first profile is Tin/Lead mix, 2nd is lead free */

extern const PROFILE profiles[NUM_PROFILES];

typedef struct _CALDATA {
  uint16_t temp[100];    //temp data from 1 to 100
} CALDATA;


#define NUM_ROWS (((1*sizeof(CALDATA))/FLASH_ROWBYTES) + 1)
#define FLASH_DATA_SIZE (NUM_ROWS*FLASH_ROWBYTES)

typedef union _UFDATA {
  CALDATA  caldata;
  char fill[FLASH_DATA_SIZE];  //worst case allocates extra row, but ensures RAM data block is multiple of row size
} UFDATA;

extern UFDATA fdata;


#define MAX_POWER_SETTING 100
#define MAX_TEMP 270  //do not exceed this temperature


#define CONFIG_SLAVE_ENABLE() CONFIG_RB8_AS_DIG_OUTPUT()
#define SLAVE_ENABLE()        _LATB8 = 0  //high true assertion
#define SLAVE_DISABLE()       _LATB8 = 1

#define ZEROCROSS _RB6

extern uint8_t u8_currPowerSetting;
extern uint8_t u8_currentProfile;
extern uint16_t readMAX6675(void);
extern void decrementPower(void);
extern void incrementPower(void);
extern void setPower(uint8_t newPower);
extern void doDebugMenu(void);
extern void doProfileMenu(void);
extern char *getProfileDesc(uint8_t u8_p);
extern int16_t getCelsiusI16Temp(void);
extern float getCelsiusFloatTemp(void);
extern void doReflow(void);
extern uint16_t volatile u16_tenthSeconds;
extern uint8_t getPower(void);
extern void doRead(UFDATA* p_ufdata);


#endif


