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

/** \file
 *  Perform a divide by 0. Because there is not handler
 *  defined for the math error trap, the
 *  \ref _DefaultInterrupt will catch it and report an error.
 *  Code in trap_test_handled.c gives an example of
 *  an ISR that handles this trap.
 */

#include "pic24_all.h"

int main (void) {
  // If u8_zero is not declared as volatile, then the
  // compiler will optimize out
  // the 1/u8_zero code, NOT producing a divide by zero!
  volatile uint8_t u8_zero;

  configBasic(HELLO_MSG);
  /** start **/
  while (1) {
    outString("Hit a key to start divide by zero test...");
    inChar();
    outString("OK. Now dividing by zero.\n"
              "The math errror trap is not handled, so\n"
              "the _DefaultInterrupt handler should be\n"
              "called, causing the chip to reset.\n\n");
    u8_zero = 0;
    u8_zero = 1/u8_zero;
    doHeartbeat();
  }// end while (1)
}
