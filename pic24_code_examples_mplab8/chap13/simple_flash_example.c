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
A simple example of run time self programming - reads/writes a single 16-bit value to flash.
*/

#ifdef __PIC24F__
#warning For a PIC24F, the DATA_FLASH_PAGE is set to the second to last flash page instead of the last page,
#warning as the last flash page in the 24F family contains the configuration bits.
#endif


#if defined(__PIC24HJ64GP502__) || defined(__PIC24FJ64GA002__)
#define LAST_IMPLEMENTED_PMEM 0x00ABFF
#elif defined(__PIC24HJ32GP202__) || defined(__PIC24FJ32GA002__) || defined(__dsPIC33FJ32GP202__)
#define LAST_IMPLEMENTED_PMEM 0x0057FF
#elif defined(__dsPIC33FJ128GP802__)
#define LAST_IMPLEMENTED_PMEM 0x0157FF
#elif defined(__PIC24EP64GP202__)     //PIC24E test
#define LAST_IMPLEMENTED_PMEM 0x00AFFF
#else
#error "Define LAST_IMPLEMENTED_PMEM for your processor!
#endif


//calculate starting address of a flash page to store data
//some PIC24H/dsPIC33 store configuration bits on last page, so skip that
#if (defined(__PIC24F__) || defined(__PIC24E__)|| defined(__dsPIC33E__))
#define DATA_FLASH_PAGE (((LAST_IMPLEMENTED_PMEM/FLASH_PAGESIZE)*FLASH_PAGESIZE)-FLASH_PAGESIZE)  //2nd to last page of flash
#endif
#if (defined(__PIC24H__)|| defined(__dsPIC33F__))
#define DATA_FLASH_PAGE ((LAST_IMPLEMENTED_PMEM/FLASH_PAGESIZE)*FLASH_PAGESIZE)  //last page of flash
#endif



typedef struct _REC {
  uint16_t u16_val;     //single 16-bit value
} REC;

#define NUM_ROWS (((sizeof(REC))/FLASH_ROWBYTES) + 1)
#define FLASH_DATA_SIZE (NUM_ROWS*FLASH_ROWBYTES)

typedef union _UFDATA {
  REC  dat;
  char fill[FLASH_DATA_SIZE];  //worst case allocates extra row, but ensures RAM data block is multiple of row size
} UFDATA;

UFDATA fdata;

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


uint8_t printMenu() {
  printf("1 Read 16-bit value from flash.\n");
  printf("2 Write 16-bit value to flash. \n");
  printf(" Enter number (1-2): ");
  return inCharEcho();
}

void doMenu() {
  uint8_t u8_c;
  char data[32];
  uint16_t u16_val;
  u8_c = printMenu();
  printf("\n");
  switch(u8_c) {
    case '1':
      doRead(&fdata);
      printf("The 16-bit value read from memory is: %u (%x hex)\n",fdata.dat.u16_val,fdata.dat.u16_val);
      break;
    case '2':
      printf("Enter 16-bit value+\\n (0 to 65535, decimal): ");
      inStringEcho(data,31);
      sscanf(data,"%d", (int *) &u16_val);
      fdata.dat.u16_val = u16_val;
      doCommit(&fdata);  //write the data
      break;
  }
}

int main (void) {
  configBasic(HELLO_MSG);
  while(1) {
    doMenu(1);
  } //end while
}//end main
