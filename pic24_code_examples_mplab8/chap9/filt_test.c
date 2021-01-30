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
Low-pass RC Filter test.
RB2 generates a pulse train that is read by RB3, the
external RC filter is assumed to be on the RB3 input.

*/

#define CONFIG_TOUT() CONFIG_RB2_AS_DIG_OUTPUT()
#define TOUT  _LATB2     //output state

#define TIN          _RB3     //test in
#define CONFIG_TIN()  CONFIG_RB3_AS_DIG_INPUT();

#define TPW  1     // in ms, pulsewidth of TOUT

int main (void) {
  uint8_t u8_oldvalueTIN;
  configBasic(HELLO_MSG);
  TOUT = 1;  // TOUT drives TIN
  CONFIG_TIN();
  CONFIG_TOUT();
  DELAY_MS(10);   //wait for output to stablize because of filter
  u8_oldvalueTIN = TIN;
  while (1) {
    TOUT = !TOUT;
    DELAY_MS(TPW);
    if (u8_oldvalueTIN != TIN) {
      u8_oldvalueTIN = TIN;
      outString("*");
    }
  }
}
