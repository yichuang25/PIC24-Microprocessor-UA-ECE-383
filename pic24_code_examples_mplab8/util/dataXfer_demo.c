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
 *  Tests the PIC data transfor protocol.
 */

/// Indexes of all the variables to be transferred.
enum { I16_VAL_NDX, U16_INDEX_NDX, C_NDX, C1_NDX };

int main(void) {
  int16_t i16_val = -1;
  char c;
  uint16_t u16_index;

  configBasic(HELLO_MSG);
  initDataXfer();

  // All variables received by the PIC must be specified.
  SPECIFY_VAR(I16_VAL_NDX, i16_val, TRUE, "%hi", "Current sum");
  SPECIFY_VAR(U16_INDEX_NDX, u16_index, FALSE, "%hu", "Index of received data");
  SPECIFY_VAR(C_NDX, c, FALSE, "%c", "Character entered");
  SPECIFY_VAR(C1_NDX, c, FALSE, "%hu", "Value of character entered");

  while (1) {
    sendVar(I16_VAL_NDX); // Send i16_val to the PC
    outString("\nCurrent sum is ");
    outUint16(i16_val);
    outString(". Enter digit to sum (0-9): ");

    // Receive one character or variable.
    u16_index = receiveVar(&c);

    // For debug support, send the index and char received.
    sendVar(U16_INDEX_NDX);
    sendVar(C_NDX);
    sendVar(C1_NDX);

    // If it was a character, add it to the sum.
    if (u16_index == CHAR_RECEIVED_INDEX) {
      outChar(c);
      outString("  (0x");
      outUint8Decimal(c);
      outString(") ");
      if ( (c >= '0') && (c <= '9') )
        i16_val += c - '0';  // A plain char received, so turn it into
      //  a number
      // Otherwise, a new total was sent.
    } else {
      ASSERT(u16_index == I16_VAL_NDX);
      outString("\nNew total received!\n");
    }
  }
}
