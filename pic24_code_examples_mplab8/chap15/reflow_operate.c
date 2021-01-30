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
Contains the functions for executing a reflow profile
*/

#define MIN_WETTING_TIME 60  //seconds

//stat variables
uint16_t u16_peakTime;
int16_t i16_maxTemp;
uint16_t u16_startWetting;
uint16_t u16_endWetting;
uint8_t coolingFlag;
uint16_t u16_startReflow;
uint16_t u16_endReflow;

void resetStats(void) {
  i16_maxTemp = 0;
  coolingFlag  = 0;
  u16_startWetting = 0;
  u16_endWetting = 0;
}

void printStats (void) {
  uint16_t u16_wettingTime;
  uint16_t u16_reflowTime;
  if (u16_endWetting == 0) u16_wettingTime = u16_tenthSeconds - u16_startWetting;
  else u16_wettingTime = u16_endWetting - u16_startWetting;
  if (u16_endReflow == 0) u16_reflowTime = u16_tenthSeconds - u16_startReflow;
  else u16_reflowTime = u16_endReflow - u16_startReflow;

  printf ("\nMax temp: %u, Temp Overshoot: %d, Wetting time: %d (s), Reflow Time: %d (s), Elapsed time: %u (s)\n",
          i16_maxTemp, i16_maxTemp - profiles[u8_currentProfile].i16_reflowTemp,
          u16_wettingTime/10, u16_reflowTime/10, u16_tenthSeconds/10 );
}

int16_t updateStats(void) {
  int16_t i16_tempC;
  i16_tempC = getCelsiusI16Temp();
  if (i16_tempC > i16_maxTemp) i16_maxTemp  = i16_tempC;
  if (i16_tempC > profiles[u8_currentProfile].i16_wetTemp &&
      u16_startWetting == 0) u16_startWetting = u16_tenthSeconds;
  if (coolingFlag && (i16_tempC < profiles[u8_currentProfile].i16_wetTemp)
      && u16_endWetting == 0)
    u16_endWetting = u16_tenthSeconds;
  if (i16_tempC > profiles[u8_currentProfile].i16_reflowTemp &&
      u16_startReflow == 0) u16_startReflow = u16_tenthSeconds;
  if (coolingFlag && (i16_tempC < profiles[u8_currentProfile].i16_reflowTemp)
      && u16_endReflow == 0)
    u16_endReflow = u16_tenthSeconds;
  return(i16_tempC);
}

uint16_t tempToPower(int16_t i16_t) {
  uint16_t u16_i;

  for (u16_i=0; u16_i <= 100; u16_i++) {
    if (fdata.caldata.temp[u16_i] >= i16_t) return(u16_i);
  }
  reportError("Reflow application error: Unable to find power setting for target temp");
  return(0);
}

void doRampUp(int16_t i16_targetTemp, uint16_t u16_targetTime) {
  int16_t i16_tempC, i16_lastTemp;
  uint8_t  u8_endPower;
  uint16_t u16_endTime;

  u16_endTime =  u16_tenthSeconds + u16_targetTime*10;  //end time does not change
  i16_tempC = updateStats();  //get current temp;
  u8_endPower = tempToPower(i16_targetTemp);
  setPower(100);  //need to heat this up
  do {
    i16_tempC = updateStats(); //get current temp;
    printf("Current temp/time: %u %u Power: %u\n",i16_tempC,u16_tenthSeconds, getPower());
    DELAY_MS(1000);
    if (u16_tenthSeconds > u16_endTime) return;
  } while ((i16_targetTemp - i16_tempC) > 10 );
  setPower(30); // near target temp
  i16_lastTemp = i16_tempC;
  do {
    DELAY_MS(1000);
    i16_tempC = updateStats();
    printf("Current temp/time: %u %u Power: %u\n",i16_tempC,u16_tenthSeconds, getPower());
    if ((i16_tempC <  i16_lastTemp) && (i16_tempC < i16_targetTemp ) ) {
      //temp is decreasing
      setPower(u8_endPower);
    } else  setPower(30);
    i16_lastTemp = i16_tempC;
  } while (u16_tenthSeconds < u16_endTime);
}

/* Oven cools down so slowly just turn off power and wait
 for target time */
#if 0
void doRampDown (int16_t i16_targetTemp, uint16_t u16_targetTime) {
  int16_t i16_tempC;

  setPower(0);
  while (u16_targetTime) {
    i16_tempC = updateStats();
    printf("Current temp/time: %u %u Power: %u\n",i16_tempC,u16_tenthSeconds, getPower());
    DELAY_MS(1000);
    u16_targetTime--;
  }
}
#endif
void doRampDown(void) {
  int16_t i16_tempC;
  uint16_t u16_wetTime;
  uint8_t  u8_openDoorFlag;

  setPower(0);
  u8_openDoorFlag = 0;
  do {
    u16_wetTime = u16_tenthSeconds - u16_startWetting;
    if ((u16_wetTime > MIN_WETTING_TIME) && !u8_openDoorFlag) {
      printf("\n\nWetting time is met, open door to aid cooling\n");
      u8_openDoorFlag = 1;
    }
    i16_tempC = updateStats();
    printf("Current temp/time: %u %u Power: %u\n",i16_tempC,u16_tenthSeconds, getPower());
    DELAY_MS(1000);
  } while (i16_tempC > 100);
}


void doHold (int16_t i16_targetTemp, uint16_t u16_targetTime) {
  uint16_t u16_endTime, u16_maxExitTime;
  int16_t i16_tempC, i16_lastTemp;
  uint8_t u8_endPower;

  //maximum time to wait before reaching target temp
  u16_maxExitTime =  u16_tenthSeconds + u16_targetTime*10*5;
  i16_tempC = updateStats(); //get current temp;
  u8_endPower = tempToPower(i16_targetTemp);
  if (i16_tempC < i16_targetTemp)setPower(100) ;
  while (i16_tempC < i16_targetTemp) {  //we need to reach the target reflow hold temperature
    DELAY_MS(1000);
    i16_tempC = updateStats();
    printf("Current temp/time: %u %u Power: %u\n",i16_tempC,u16_tenthSeconds, getPower());
    if (u16_tenthSeconds > u16_maxExitTime)return;
  }
  i16_lastTemp = i16_tempC;
  u16_endTime =  u16_tenthSeconds + u16_targetTime*10;
  do {
    DELAY_MS(1000);
    i16_tempC = updateStats();
    printf("Current temp/time: %u %u Power: %u\n",i16_tempC, u16_tenthSeconds, getPower());
    if ((i16_tempC <  i16_lastTemp) && (i16_tempC < i16_targetTemp ) ) {
      //temp is decreasing
      setPower(u8_endPower);
    } else  setPower(30);
    i16_lastTemp = i16_tempC;
  } while (u16_tenthSeconds < u16_endTime);
}


void doReflow(void) {

  uint8_t u8_rdyFlag,u8_c;
  uint16_t i16_tempC;

  u8_rdyFlag = 0;
  do {
    printf("\n\nOpen door, place board in oven, close door.\n");
    printf("Turn 'TEMP' knob to 'Toast On/Bake Off'\n");
    printf("Ensure that end of thermocouple is not touching\n");
    printf("the board or metal. \n");
    printf("Enter 'y' to continue or 'n' to exit...\n");
    u8_c = inCharEcho();
    if (u8_c == 'n') return;
  } while (u8_c != 'y');

  u16_tenthSeconds = 0;
  u8_c = ZEROCROSS;
  while (u8_c == ZEROCROSS) {
    if (u16_tenthSeconds > 4) break;
    doHeartbeat();
  }
  if (u16_tenthSeconds > 4) {
    printf("\nExiting, power is not on to oven, cannot detect 60Hz cycling.\n");
    return;
  }
  u16_tenthSeconds = 0;
  resetStats();
//preheat
  i16_tempC = getCelsiusI16Temp();
  printf("\nStart Preheat (temp = %uC)\n",i16_tempC);
  doRampUp(profiles[u8_currentProfile].i16_preheatTemp,
           profiles[u8_currentProfile].u16_preheatTime);
//soak
  i16_tempC = getCelsiusI16Temp();
  printf("\nStart Soak (temp = %uC), Elapsed time: %u s\n",i16_tempC,
         u16_tenthSeconds/10);
  doRampUp(profiles[u8_currentProfile].i16_soakTemp,
           profiles[u8_currentProfile].u16_soakTime);
//reflow
  printf("\nStart Reflow (temp = %uC), Elapsed time: %u s\n",i16_tempC,
         u16_tenthSeconds/10);
  doRampUp(profiles[u8_currentProfile].i16_reflowTemp,
           profiles[u8_currentProfile].u16_reflowTime);
  printf("\nStart Reflow Hold (temp = %uC), Elapsed time: %u s\n",i16_tempC,
         u16_tenthSeconds/10);
  doHold(profiles[u8_currentProfile].i16_reflowTemp,
         profiles[u8_currentProfile].u16_reflowHoldTime);
//initial cooldown

  coolingFlag = 1;
  setPower(0);
  printf("\nStart Cool1 (temp = %uC), Elapsed time: %u s\n",i16_tempC,
         u16_tenthSeconds/10);
  doRampDown();
#if 0

  doRampDown(profiles[u8_currentProfile].i16_coolTemp,
             profiles[u8_currentProfile].u16_coolTime);
//
#endif

  printStats();

}


