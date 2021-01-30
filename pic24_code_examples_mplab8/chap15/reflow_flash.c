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
#include <stdio.h>
#include "reflow_oven.h"

/** \file
Utilities for storing/reading reflow oven power versus temperature calibration
data to/from flash memory.
*/

UFDATA fdata;

const PROFILE profiles[NUM_PROFILES] = {
  {100,100,150,100,230,90,20,90,60, 183},
  {150,100,200,100,250,90,30,90,60, 217}
};

uint8_t u8_currentProfile = 0;


void doCommit(UFDATA* p_ufdata) {
  union32 u_memaddr;
  u_memaddr.u32 = DATA_FLASH_PAGE;
  doWritePageFlash(u_memaddr, (uint8_t *) p_ufdata, FLASH_DATA_SIZE);
}

void doRead(UFDATA* p_ufdata) {
  union32 u_memaddr;
  u_memaddr.u32 = DATA_FLASH_PAGE;
  doReadPageFlash(u_memaddr, (uint8_t *) p_ufdata, FLASH_DATA_SIZE);
}

char *getProfileDesc(uint8_t u8_p) {
  if (u8_p == LEADTIN) return("Lead(40%)/Tin (60%) mix");
  else return("Lead-free");
}


void printProfile (uint8_t u8_p) {
  float f_ramp;

  if (u8_p == LEADTIN)
    printf("%s profile, Wet temp: %d C \n",
           getProfileDesc(u8_p),
           profiles[u8_p].i16_wetTemp);
  else printf("%s profile, Wet temp: %d C \n",
                getProfileDesc(u8_p),
                profiles[u8_p].i16_wetTemp);
  f_ramp = (float) (profiles[u8_p].i16_preheatTemp - 25.0)/(float)profiles[u8_p].u16_preheatTime;
  printf("Preheat Temp: %d C, Time: %d s, Ramp: %6.2f (C/s)\n",
         profiles[u8_p].i16_preheatTemp,
         profiles[u8_p].u16_preheatTime,
         (double) f_ramp);
  f_ramp = (profiles[u8_p].i16_soakTemp - profiles[u8_p].i16_preheatTemp)/(float)profiles[u8_p].u16_soakTime;
  printf("Soak Temp: %d C, Time: %d s, Ramp: %6.2f (C/s)\n",
         profiles[u8_p].i16_soakTemp,
         profiles[u8_p].u16_soakTime,
         (double) f_ramp);
  f_ramp = (float)(profiles[u8_p].i16_reflowTemp - profiles[u8_p].i16_soakTemp)/(float)profiles[u8_p].u16_reflowTime;
  printf("Reflow Temp: %d C, Time: %d s, Ramp: %6.2f (C/s), Hold time: %d\n",
         profiles[u8_p].i16_reflowTemp,
         profiles[u8_p].u16_reflowTime,
         (double) f_ramp,
         profiles[u8_p].u16_reflowHoldTime);

  f_ramp = (float)(profiles[u8_p].i16_reflowTemp - profiles[u8_p].i16_coolTemp)/(float)profiles[u8_p].u16_coolTime;
  printf("Cool1 Temp: %d C, Time: %d s, Ramp: %6.2f (C/s)\n",
         profiles[u8_p].i16_coolTemp,
         profiles[u8_p].u16_coolTime,
         (double) f_ramp);


}

#define SETTLE_TIME 15   //seconds

static inline void DELAY_SECONDS(uint8_t u8_s) {
  while (u8_s) {
    DELAY_MS(1000);
    u8_s--;
  }
}

void doTempCal(void) {
  uint8_t u8_c, u8_i;
  uint16_t tempC;

  u8_currPowerSetting = 0;
  printf("Warning: Oven must be at room temperature and door closed to do calibration.\n");
  printf("Warning: This takes 30 minutes! Do you still want to continue? (y/n): ");
  u8_c = inCharEcho();
  if (u8_c != 'y') return;
  tempC = getCelsiusI16Temp();
  if ((tempC < 20) || (tempC > 40)) {
    printf("\nTemperature is %u; it is not between 20C and 40C, something is wrong, aborting...\n",
           tempC);
    return;
  }
  fdata.caldata.temp[0] = tempC;
  printf("\nRoom temperature is: %uC\n", tempC);
  printf("Beginning calibration...hit any key to exit.\n");
  for (u8_i = 1; u8_i <= 100; u8_i++) {
    u8_currPowerSetting++;
    DELAY_SECONDS(SETTLE_TIME);
    tempC = getCelsiusI16Temp();
    fdata.caldata.temp[u8_i] = tempC;
    printf("Power: %u, Temp: %u \n",u8_i, tempC);
    if (isCharReady()) break;
    if (tempC > MAX_TEMP) break;
  }
  u8_currPowerSetting = 0;
  if (isCharReady()) u8_c = inCharEcho();
  while (u8_i <= 100) {
    fdata.caldata.temp[u8_i] = tempC;
    u8_i++;
  }
  doCommit(&fdata);
  printf("Calibration data written.\n");
}

void printTempCal(void) {
  uint8_t u8_i;
  printf("\nPower Setting   Temp (C)\n");
  for (u8_i=0; u8_i <= 100; u8_i++) {
    printf("      %d           %u\n",
           u8_i, fdata.caldata.temp[u8_i]);
  }
}

uint8_t printProfileMenu(void) {
  printf("\n1 Print Tin/Lead mix profile\n");
  printf("2 Print Lead-free profile\n");
  printf("3 Print temperature calibration data\n");
  printf("4 Perform temperature calibration and commit to flash\n");
  printf("5 Toggle Profile\n");
  printf("6 Exit\n");
  printf(" Enter number (1-6): ");
  return inCharEcho();
}


void doProfileMenu(void) {
  uint8_t u8_c;

  do {
    u8_c = printProfileMenu();
    printf("\n");
    switch (u8_c) {
      case '1':
        printProfile(0);
        break;
      case '2':
        printProfile(1);
        break;
      case '3':
        printTempCal();
        break;
      case '4':
        doTempCal();
        break;
      case '5':
        u8_currentProfile =!u8_currentProfile;
        printf("Current profile: %s\n", getProfileDesc(u8_currentProfile));
        break;
    } //end switch
  } while (u8_c != '6');
}


