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
#include "dataXfer.h"

/** \file
 *  Demonstrates an echo program which includes usage of the
 *  \ref dataXfer "data transfer protocol" .
 */

/// Indexes of all the variables to be transferred.
enum { U16_NUMCHARS_NDX, C_NDX };

/// Number of characters to print on each line.
#define CHARS_PER_LINE 10

int main(void) {
  char c;
  uint16_t u16_numChars = 0;

  // Initialize
  configBasic(HELLO_MSG);
  initDataXfer();

  // All variables received by the PIC must be specified.
  // Params:  Index             Variable      PC can change  Format  Description
  SPECIFY_VAR(U16_NUMCHARS_NDX, u16_numChars, TRUE,          "%hu",  "Number of characters received");
  SPECIFY_VAR(C_NDX,            c,            FALSE,         "%c",   "Character entered");

  while (1) {
    // For debug support, send the index and char received.
    sendVar(U16_NUMCHARS_NDX);
    sendVar(C_NDX);

    // Echo a character
    c = inCharXfer();
    outChar(c);

    // Add a CR every \ref CHARS_PER_LINE
    if ((++u16_numChars % CHARS_PER_LINE) == 0)
      outChar('\n');
  }
}
