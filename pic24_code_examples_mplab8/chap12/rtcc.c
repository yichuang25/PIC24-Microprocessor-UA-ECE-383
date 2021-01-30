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


/** \file
Program uses the Real Time Clock module and an external 32.768 kHZ crystal for timekeeping.
*/

typedef union _unionRTCC {
  struct {  //four 16 bit registers
    uint8_t yr;
    uint8_t null;
    uint8_t date;
    uint8_t month;
    uint8_t hour;
    uint8_t wday;
    uint8_t sec;
    uint8_t min;
  } u8;
  uint16_t regs[4];
} unionRTCC;

unionRTCC u_RTCC;

uint8_t getBCDvalue(char *sz_1) {
  char sz_buff[8];
  uint16_t u16_bin;
  uint8_t  u8_bcd;
  outString(sz_1);
  inStringEcho(sz_buff,7);
  sscanf(sz_buff,"%d", (int *)&u16_bin);
  u8_bcd = u16_bin/10;   //most significant digit
  u8_bcd = u8_bcd << 4;
  u8_bcd = u8_bcd | (u16_bin%10);
  return(u8_bcd);
}

void getDateFromUser(void) {
  u_RTCC.u8.yr = getBCDvalue("Enter year (0-99): ");
  u_RTCC.u8.month = getBCDvalue("Enter month (1-12): ");
  u_RTCC.u8.date = getBCDvalue("Enter day of month (1-31): ");
  u_RTCC.u8.wday = getBCDvalue("Enter week day (0-6): ");
  u_RTCC.u8.hour = getBCDvalue("Enter hour (0-23): ");
  u_RTCC.u8.min = getBCDvalue("Enter min (0-59): ");
  u_RTCC.u8.sec = getBCDvalue("Enter sec(0-59): ");
}

//set date
void setRTCC(void) {
  uint8_t u8_i;
  __builtin_write_RTCWEN();   //enable write to RTCC, sets RTCWEN
  RCFGCALbits.RTCEN = 0;      //disable the RTCC
  RCFGCALbits.RTCPTR = 3;     //set pointer reg to start
  for (u8_i=0; u8_i<4; u8_i++) RTCVAL = u_RTCC.regs[u8_i];
  RCFGCALbits.RTCEN = 1;     //Enable the RTCC
  RCFGCALbits.RTCWREN = 0;   //can clear without unlock
}

void readRTCC(void) {
  uint8_t u8_i;
  RCFGCALbits.RTCPTR = 3;     //set pointer reg to start
  for (u8_i=0; u8_i<4; u8_i++) u_RTCC.regs[u8_i] = RTCVAL;
}

void printRTCC(void) {
  printf ("day(wday)/mon/yr: %2x(%2x)/%2x/%2x, %02x:%02x:%02x \n",
          (uint16_t) u_RTCC.u8.date,(uint16_t) u_RTCC.u8.wday, (uint16_t) u_RTCC.u8.month,
          (uint16_t) u_RTCC.u8.yr, (uint16_t) u_RTCC.u8.hour, (uint16_t) u_RTCC.u8.min, (uint16_t) u_RTCC.u8.sec);
}

int main(void) {
  __builtin_write_OSCCONL(OSCCON | 0x02);    //  OSCCON.SOSCEN=1;
  configBasic(HELLO_MSG);   //say Hello!
  getDateFromUser();    //get initial date/time
  setRTCC();            //set the date/time
  while (1) {
    while (!RCFGCALbits.RTCSYNC) doHeartbeat();
    readRTCC();
    printRTCC();
    DELAY_MS(30);
  }
}
