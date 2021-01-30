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
 *  Inputs a string, outputs the reverse. This file is used
 *  in three MPLAB projects:
 *   reverse_string.mcp - polled RX, TX I/O
 *   uartrx_fifo.mcp - interrupt RX, polled TX I/O
 *   uartrxtx_fifo.mcp - interrupt RX, interrupt TX I/O
 * Interrupt RX inChar1() is selected by defining  UART1_RX_INTERRUPT macro
 * Interrupt TX outChar1() is selected by defining UART1_TX_INTERRUPT macro
 * These macros are defined in their respective MPLAB projects.
*/

void reverseString(char *psz_s1, char *psz_s2) {
  char *psz_s1end;
  if (!(*psz_s1)) {
    *psz_s2 = 0;  //psz_s1 is empty, return.
    return;
  }
  psz_s1end = psz_s1;
  //find end of first string
  while (*psz_s1end) psz_s1end++;
  psz_s1end--;      //backup one to first non-zero byte
  //now copy to S2 in reverse order
  while (psz_s1end != psz_s1) {
    *psz_s2 = *psz_s1end;
    psz_s1end--;
    psz_s2++;
  }
  //copy last byte
  *psz_s2 = *psz_s1end;
  psz_s2++;
  //mark end of string
  *psz_s2 = 0;
}


#define BUFSIZE 63
char  sz_1[BUFSIZE+1];
char  sz_2[BUFSIZE+1];
int main (void) {
  configBasic(HELLO_MSG);
  while (1) {
    inString(sz_1,BUFSIZE);   //get a string from the console
    reverseString(sz_1, sz_2);
    outString(sz_2);          //output the reversed string
    outString("\n");
  }
}
