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
Illustrates run time self programming of the flash memory
*/

#if (defined(__PIC24F__) || defined(__PIC24E__)|| defined(__dsPIC33E__))
#warning For all PIC24F and some PIC24E/dsPIC33E family members, the DATA_FLASH_PAGE is set to the second to last flash page instead of the last page,
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
#if (defined(__PIC24F__) || defined(__PIC24E__)|| defined(__dsPIC33E__))
#define DATA_FLASH_PAGE (((LAST_IMPLEMENTED_PMEM/FLASH_PAGESIZE)*FLASH_PAGESIZE)-FLASH_PAGESIZE)  //2nd to last page of flash
#endif
#if (defined(__PIC24H__)|| defined(__dsPIC33F__))
#define DATA_FLASH_PAGE ((LAST_IMPLEMENTED_PMEM/FLASH_PAGESIZE)*FLASH_PAGESIZE)  //last page of flash
#endif



#define RECORD_SIZE 15      //arbitrarily chosen
#define NUM_RECORDS 8       //arbitrarily chosen
#define FREE_STATUS 0xA5    //arbitrarily chosen, different from USED_STATUS
#define USED_STATUS 0x5A    //arbitrarily chosen, different from FREE_STATUS

typedef struct _REC {   //one record
  uint8_t status;            //indicates if free or used
  char  data[RECORD_SIZE]; //holds the data
} REC;

#define NUM_ROWS (((NUM_RECORDS*sizeof(REC))/FLASH_ROWBYTES) + 1)
#define FLASH_DATA_SIZE (NUM_ROWS*FLASH_ROWBYTES)

typedef union _UFDATA {
  REC  records[NUM_RECORDS];
  char fill[FLASH_DATA_SIZE];  //worst case allocates extra row, but ensures RAM data block is multiple of row size
} UFDATA;

UFDATA fdata __attribute__ ((aligned(4)));

void doFormat(UFDATA* p_ufdata) {
  uint16_t u16_i;
  for (u16_i = 0; u16_i < NUM_RECORDS; u16_i++) {
    p_ufdata->records[u16_i].status = FREE_STATUS;
  }
}

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

void doDelete(uint16_t u16_recnum) {
  fdata.records[u16_recnum].status = FREE_STATUS;
}

void doPrint(UFDATA* p_ufdata) {
  uint16_t u16_i;
  for (u16_i = 0; u16_i < NUM_RECORDS; u16_i++) {
    printf("%d: %s, Data: %s\n",
           u16_i, (p_ufdata->records[u16_i].status == FREE_STATUS ? "Free" : "Used"),
           (p_ufdata->records[u16_i].status == FREE_STATUS ? "n/a" : p_ufdata->records[u16_i].data));
  }
}

void doInsert(UFDATA* p_ufdata, char* sz_1) {
  uint16_t u16_i, u16_j;
  for (u16_i = 0; u16_i < NUM_RECORDS; u16_i++) {
    if (p_ufdata->records[u16_i].status == FREE_STATUS) {
      u16_j = 0;
      while (*sz_1) {  //copy data
        p_ufdata->records[u16_i].data[u16_j] = *sz_1;
        sz_1++;
        u16_j++;
      } //end while
      p_ufdata->records[u16_i].data[u16_j] = *sz_1; //write null
      p_ufdata->records[u16_i].status = USED_STATUS; //mark as used
      break; //exit if
    }//end if
  }//end for
}//end function

uint8_t printMenu() {
  printf("1 Format data\n");
  printf("2 Enter one record\n");
  printf("3 Delete a record\n");
  printf("4 Commit data\n");
  printf("5 Read and print all records\n");
  printf(" Enter number (1-5): ");
  return inCharEcho();
}

void doMenu() {
  uint8_t u8_c;
  char data[RECORD_SIZE];
  uint16_t u16_recnum;
  u8_c = printMenu();
  printf("\n");
  switch(u8_c) {
    case '1':
      doFormat(&fdata);
      break;
    case '2':
      printf("Enter string+\\n (14 chars max): ");
      inStringEcho(data,RECORD_SIZE-1);
      doInsert(&fdata, data);
      break;
    case '3':
      printf("Enter record number+\\n (0 to 15, decimal): ");
      inStringEcho(data,RECORD_SIZE-1);
      sscanf(data,"%d", (int *) &u16_recnum);
      doDelete(u16_recnum);
      break;
    case '4':
      doCommit(&fdata);
      break;
    case '5':
      doRead(&fdata);
      doPrint(&fdata);
      break;
  }
}

int main (void) {
  configBasic(HELLO_MSG);
  while(1) {
    doMenu(1);
  } //end while
}//end main
