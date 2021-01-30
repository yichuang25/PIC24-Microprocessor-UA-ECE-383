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
 Utilities for debugging reflow
*/

void debugPower(void) {
  uint8_t u8_c;
  u8_c = 0;
  outString("\n");
  do {
    printf("Power: %d\n",u8_currPowerSetting);
    u8_c = inChar();
    if (u8_c == '-') decrementPower();
    if (u8_c == '+') incrementPower();
  } while ((u8_c == '-') || (u8_c == '+'));
}


void debugZeroCross(void) {
  uint8_t u8_x;
  u8_x = ZEROCROSS;
  do {
    while ((u8_x == ZEROCROSS) && !isCharReady()) doHeartbeat();
    u8_x = !u8_x;
    DELAY_MS(2);
    if (u8_x) outChar('0');
    else outChar('1');
  } while (!isCharReady());
  inChar();
}

void debugThermocouple(void) {
  float  f_tempC,f_tempF;
  do {
    f_tempC = getCelsiusFloatTemp();
    f_tempF = f_tempC*9/5 + 32;
    printf("Temp is: %4.4f (C), %4.4f (F)\n", (double) f_tempC, (double) f_tempF);
    DELAY_MS(400);
  } while (!isCharReady());
  inChar();
}

void doDebugMenu(void) {
  uint8_t u8_c;
  printf("Debug menu:\n");
  printf("   't' - read thermocouple, hit any key to exit\n");
  printf("   'z' - read zerocross, hit any key to exit \n");
  printf("   'p' - control power setting, '+' increments,'-' decrements, other exits.\n");
  printf("   'x' - exit to main menu\n");
  printf("Enter character: ");
  u8_c = inCharEcho();
  if (u8_c == 't') debugThermocouple();
  else if (u8_c == 'z') debugZeroCross();
  else if (u8_c == 'p') debugPower();
  printf("\n");
}


