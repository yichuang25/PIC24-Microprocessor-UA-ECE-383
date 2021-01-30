/** This has been modified from the original source
provided by Microchip. It now works with both the PIC24H
and PIC24F families.
Modifications by R. Reese (reese@ece.msstate.edu).
*/



/********************************************************************
* FileName:  main.c
* Dependencies:
* Processor:  PIC24H Family
* Hardware:  Explorer 16
* Compiler:  C30 2.01
* Company:  Microchip Technology, Inc.
*
* Software License Agreement
*
* The software supplied herewith by Microchip Technology Incorporated
* (the “Company”) for its PICmicro® Microcontroller is intended and
* supplied to you, the Company’s customer, for use solely and
* exclusively on Microchip PICmicro Microcontroller products. The
* software is owned by the Company and/or its supplier, and is
* protected under applicable copyright laws. All rights are reserved.
* Any use in violation of the foregoing restrictions may subject the
* user to criminal sanctions under applicable laws, as well as to
* civil liability for the breach of the terms and conditions of this
* license.
*
* THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
* WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
* TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
* IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
* CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
*
*********************************************************************/


/*****

BE SURE THAT YOU HAVE A LINKER FILE FOR THE TARGET PROCESSOR IN THE
CURRENT DIRECTORY WITH THE STARTING PROGRAM MEMORY LOCATION CHANGED
TO 0x400!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
******/

#include "pic24_all.h"





#define COMMAND_NACK     0x00
#define COMMAND_ACK      0x01
#define COMMAND_READ_PM  0x02
#define COMMAND_WRITE_PM 0x03
#define COMMAND_WRITE_CM 0x07
#define COMMAND_RESET    0x08
#define COMMAND_READ_ID  0x09
#define COMMAND_READ_VERSION  0x011
#define COMMAND_POR_RESET  0x13

#define PROGRAM_START 0xC00      //start of application program, do not write below this address

#define VERSION_MAJOR 0x03       //this cannot be zero!
#define VERSION_MINOR 0x00

#if defined(__PIC24FK__)

#define INSTR_PER_ROW 32   //number of instructions per row
#define PM_ROW_SIZE INSTR_PER_ROW * 4   //maximum instructions transferred to RAM for programming
#define PM_ERASE_SIZE INSTR_PER_ROW*4   //can only 

#elif (defined(__PIC24HJ12GP202__) || defined(__PIC24HJ12GP201__) )

//Reserved for processors with less than < 2K SRAM
/*
this is the size of an erase page!
Total byte storage will be PM_ROW_SIZE * 3 because
each instruction is 3 bytes wide.
A total erase page is 1536 bytes = 64*8*3.
Make it half a page, or 64*4*3.
Will have to detect when we cross a half page boundary
and we need to do a fresh erase. If the address
is evenly divisible by 1536, then do an erase.
*/
#define INSTR_PER_ROW 64   //number of instructions per row
#define PM_ROW_SIZE INSTR_PER_ROW * 4     //number of instructions transferred to RAM to be programmed 
#define PM_ERASE_SIZE INSTR_PER_ROW*8     //erases a full page, but RAM can only hold 1/2 page
#elif ( defined(__PIC24E__) || defined(__dsPIC33E__))
#define INSTR_PER_ROW 2   //number of instructions per row
#define PM_ROW_SIZE 512   //page size downloaded, will be 512 instructions

//some processors have an erase size of 512.
#if (defined(__PIC24EP32GP202__) || defined(__PIC24EP32GP203__) || defined(__PIC24EP32GP204__) )
#define PM_ERASE_SIZE 512
#elif (defined(__dsPIC33EP32GP502__) || defined(__dsPIC33EP32GP503__) || defined(__dsPIC33EP32GP504__) )
#define PM_ERASE_SIZE 512
#else
#define PM_ERASE_SIZE 1024 //default erase size.
#endif


#else
//this processor has enough memory for a full page size.
#define INSTR_PER_ROW 64   //number of instructions per row
#define PM_ROW_SIZE INSTR_PER_ROW * 8
#define PM_ERASE_SIZE INSTR_PER_ROW*8
#endif

#define CM_ROW_SIZE 12
#define CONFIG_WORD_SIZE 1

#if defined(__PIC24FK__)
//This family can only erase up to four rows instead of 8 rows, and the programming word has changed
#define PM_ROW_ERASE   0x405A //erases 4 rows
#define PM_ROW_WRITE   0x4004 //write 1 row
#elif (defined(__PIC24H__) || defined(__PIC24F__)  || defined(__dsPIC33F__))
//for PIC24F, PIC24H
#define PM_ROW_ERASE   0x4042 //erase entire page (8 rows)
#define PM_ROW_WRITE   0x4001 //write 1 row
#elif (defined(__PIC24E__) || defined(__dsPIC33E__))
#define PM_ROW_ERASE   0x4003 //erase entire page 
#define PM_ROW_WRITE   0x4001 //write double word
#else
#error This family is not supported by bootloader.
#endif

#define CONFIG_WORD_WRITE 0x4000



#define CONFIG_LED1()   CONFIG_RB0_AS_DIG_OUTPUT()
/// LED1 state macro.
#define LED1  _LATB0

typedef short          Word16;
typedef unsigned short UWord16;
typedef long           Word32;
typedef unsigned long  UWord32;



typedef union tuReg32 {
  UWord32 Val32;

  struct {
    UWord16 LW;
    UWord16 HW;
  } Word;

  char Val[4];
} uReg32;


void ResetDeviceasPOR(void);

UWord32 ReadLatch(UWord16 addrhi, UWord16 addrlo);
void PutChar(char);
void GetChar(char *);
void WriteBuffer(char *, int);
void ReadPM(char *, uReg32);
void WritePM(char *, uReg32);
void WriteMem(UWord16 val);
void LoadAddr(UWord16 nvmadru, UWord16 nvmadr);
void WriteLatch(UWord16 addrhi,UWord16 addrlo, UWord16 wordhi, UWord16 wordlo);
void ResetDevice(void);
void Erase(UWord16 addrhi, UWord16 addrlo, UWord16 val );
#if (defined(__PIC24E__) || defined(__dsPIC33E__))
void LoadTwoWords(UWord16 addrhi, UWord16 addrlo, UWord16 wordhi, UWord16 wordlo, UWord16 word2hi, UWord16 word2lo);
void WriteMem2(UWord16 addrhi, UWord16 addrlo, UWord16 val);
#endif

/* this needs to be persistent so as to not step on persisent variables
in user's program
*/

_PERSISTENT char Buffer[PM_ROW_SIZE*3 + 1];    //the *3 is because each instruction is 3 bytes


int main(void)

{

  uReg32 SourceAddr;
  uReg32 Delay;

  T2CON = 0x0000;
  T3CON = 0x0000;
  U1MODE = 0x0000;

  configClock();          //clock configuration


  RCONbits.SWDTEN=0;            /* Disable Watch Dog Timer*/

#if (defined(__PIC24E__) || defined(__dsPIC33E__))
  SourceAddr.Val32 = 0x1000;
#else
  SourceAddr.Val32 = 0xc00;
#endif

  Delay.Val32 = ReadLatch(SourceAddr.Word.HW, SourceAddr.Word.LW);

  if (_SWR) ResetDevice();

  if (Delay.Val[0] == 0 || (_POR==0 && _EXTR==0)) {
    ResetDevice();
  }


  //must be power-on or MCLR reset in order to enter the bootloader.
  //This is so if application executes a software reset, then we do not enter bootloader.

  T2CONbits.T32 = 1; /* to increment every instruction cycle */
  IFS0bits.T3IF = 0; /* Clear the Timer3 Interrupt Flag */
  IEC0bits.T3IE = 0; /* Disable Timer3 Interrup Service Routine */


  if ((Delay.Val32 & 0x000000FF) != 0xFF) {
    /* Convert seconds into timer count value */
    Delay.Val32 = ((UWord32)(FCY)) * ((UWord32)(Delay.Val[0]));

    PR3 = Delay.Word.HW;
    PR2 = Delay.Word.LW;

    /* Enable Timer */
    T2CONbits.TON=1;
  }

  CONFIG_DEFAULT_UART();

  while (1) {
    char Command;

    GetChar(&Command);

    switch (Command) {
      case COMMAND_READ_PM: {  /*tested*/
        uReg32 SourceAddr;

        GetChar(&(SourceAddr.Val[0]));
        GetChar(&(SourceAddr.Val[1]));
        GetChar(&(SourceAddr.Val[2]));
        SourceAddr.Val[3]=0;

        ReadPM(Buffer, SourceAddr);

        WriteBuffer(Buffer, PM_ROW_SIZE*3);

        break;
      }

      case COMMAND_WRITE_PM: {  /* tested */
        uReg32 SourceAddr;
        int    Size;

        GetChar(&(SourceAddr.Val[0]));
        GetChar(&(SourceAddr.Val[1]));
        GetChar(&(SourceAddr.Val[2]));
        SourceAddr.Val[3]=0;

        for (Size = 0; Size < PM_ROW_SIZE*3; Size++) {
          GetChar(&(Buffer[Size]));
        }

        /*
          */

        if ((SourceAddr.Val32 % (PM_ERASE_SIZE*2)) == 0) {
          //on page boundary, do erase.
          if (SourceAddr.Val32 >= PROGRAM_START) Erase(SourceAddr.Word.HW,SourceAddr.Word.LW,PM_ROW_ERASE);
        }

        if (SourceAddr.Val32 >= PROGRAM_START) WritePM(Buffer, SourceAddr);  /*program page */

        PutChar(COMMAND_ACK);    /*Send Acknowledgement */

        break;
      }

      case COMMAND_READ_ID: {
        uReg32 SourceAddr;
        uReg32 Temp;

        SourceAddr.Val32 = 0xFF0000;

        Temp.Val32 = ReadLatch(SourceAddr.Word.HW, SourceAddr.Word.LW);

        WriteBuffer(&(Temp.Val[0]), 4);

        SourceAddr.Val32 = 0xFF0002;

        Temp.Val32 = ReadLatch(SourceAddr.Word.HW, SourceAddr.Word.LW);

        WriteBuffer(&(Temp.Val[0]), 4);

        break;
      }

      case COMMAND_READ_VERSION: {
        PutChar(VERSION_MAJOR);
        PutChar(VERSION_MINOR);
        PutChar(COMMAND_ACK);    /*Send Acknowledgement */
        break;
      }

      case COMMAND_POR_RESET: {
        ResetDeviceasPOR();
        break;
      }

      case COMMAND_WRITE_CM: {
        int    Size;

        for (Size = 0; Size < CM_ROW_SIZE*3;) {
          GetChar(&(Buffer[Size++]));
          GetChar(&(Buffer[Size++]));
          GetChar(&(Buffer[Size++]));

          PutChar(COMMAND_ACK);    /*Send Acknowledgement */
        }


        break;
      }

      case COMMAND_RESET: {

#if (defined (__PIC24H__) || defined(__dsPIC33F__) || defined(__PIC24FK__))
        //only do this for PIC24/dsPIC33F/PIC24FK family members, PIC24F configuration
        //bits are in normal program memory
        uReg32 SourceAddr;
        int    Size;
        uReg32 Temp;


        for (Size = 0, SourceAddr.Val32 = 0xF80000; Size < CM_ROW_SIZE*3;
             Size +=3, SourceAddr.Val32 += 2) {
          if (Buffer[Size] == 0) {
            Temp.Val[0]=Buffer[Size+1];
            Temp.Val[1]=Buffer[Size+2];

            WriteLatch( SourceAddr.Word.HW,
                        SourceAddr.Word.LW,
                        Temp.Word.HW,
                        Temp.Word.LW);

            WriteMem(CONFIG_WORD_WRITE);
          }
        }
#endif
        //we have programmed the device, make it think that this was a POR reset
        //so that persistent variables are cleared.
        ResetDeviceasPOR();

        break;
      }




      case COMMAND_NACK: {
        ResetDevice();

        break;
      }


      default:
        PutChar(COMMAND_NACK);
        break;
    }

  }

}

/******************************************************************************/


void GetChar(char * ptrChar) {
  char c;
  while (1) {
    /* if timer expired, signal to application to jump to user code */
    if (IFS0bits.T3IF == 1) {
      * ptrChar = COMMAND_NACK;
      break;
    }
    /* check for receive errors */
    if (DEFAULT_UART_FERR == 1) {
      c = DEFAULT_UART_RXREG;     //clear framing error
      __asm__ volatile ("reset");  //we are hosed...
    }

    /* must clear the overrun error to keep uart receiving */
    if (DEFAULT_UART_OERR == 1) {
      DEFAULT_UART_OERR = 0;
      __asm__ volatile ("reset");  //we are hosed...
    }

    /* get the data */
    if (DEFAULT_UART_URXDA == 1) {
      T2CONbits.TON=0; /* Disable timer countdown */
      * ptrChar = DEFAULT_UART_RXREG;
      break;
    }
  }
}


/******************************************************************************/
void ReadPM(char * ptrData, uReg32 SourceAddr) {
  int    Size;
  uReg32 Temp;

  for (Size = 0; Size < PM_ROW_SIZE; Size++) {
    Temp.Val32 = ReadLatch(SourceAddr.Word.HW, SourceAddr.Word.LW);

    ptrData[0] = Temp.Val[2];;
    ptrData[1] = Temp.Val[1];;
    ptrData[2] = Temp.Val[0];;

    ptrData = ptrData + 3;

    SourceAddr.Val32 = SourceAddr.Val32 + 2;
  }
}
/******************************************************************************/

void WriteBuffer(char * ptrData, int Size) {
  int DataCount;

  for (DataCount = 0; DataCount < Size; DataCount++) {
    PutChar(ptrData[DataCount]);
  }
}
/******************************************************************************/

void PutChar(char Char) {
  ///putchar(Char);
  while (DEFAULT_UART_UTXBF);    //transmit buffer is full
  DEFAULT_UART_TXREG = Char;
}



/******************************************************************************/
#if (defined(__PIC24E__) || defined(__dsPIC33E__))
//this does double word programming, works for all PIC24E, PIC33E
void WritePM(char * ptrData, uReg32 SourceAddr) {
  int    Size,Size1;
  uReg32 Temp;
  uReg32 Temp2;


  for (Size = 0,Size1=0; Size < PM_ROW_SIZE; Size = Size+2) {

    Temp.Val[0]=ptrData[Size1+0];
    Temp.Val[1]=ptrData[Size1+1];
    Temp.Val[2]=ptrData[Size1+2];
    Temp.Val[3]=0;
    Size1+=3;

    Temp2.Val[0]=ptrData[Size1+0];
    Temp2.Val[1]=ptrData[Size1+1];
    Temp2.Val[2]=ptrData[Size1+2];
    Temp2.Val[3]=0;
    Size1+=3;

    LoadTwoWords(SourceAddr.Word.HW, SourceAddr.Word.LW, Temp.Word.HW,Temp.Word.LW, Temp2.Word.HW,Temp2.Word.LW);
    //WriteLatch(SourceAddr.Word.HW, SourceAddr.Word.LW,Temp.Word.HW,Temp.Word.LW);
    WriteMem2(SourceAddr.Word.HW, SourceAddr.Word.LW, PM_ROW_WRITE);

    SourceAddr.Val32 = SourceAddr.Val32 + 4;
  }
}

#else
void WritePM(char * ptrData, uReg32 SourceAddr) {
  int    Size,Size1;
  uReg32 Temp;
  uReg32 TempAddr;
  uReg32 TempData;

  for (Size = 0,Size1=0; Size < PM_ROW_SIZE; Size++) {

    Temp.Val[0]=ptrData[Size1+0];
    Temp.Val[1]=ptrData[Size1+1];
    Temp.Val[2]=ptrData[Size1+2];
    Temp.Val[3]=0;
    Size1+=3;

    WriteLatch(SourceAddr.Word.HW, SourceAddr.Word.LW,Temp.Word.HW,Temp.Word.LW);

    /* Device ID errata workaround: Save data at any address that has LSB 0x18 */
    if ((SourceAddr.Val32 & 0x0000001F) == 0x18) {
      TempAddr.Val32 = SourceAddr.Val32;
      TempData.Val32 = Temp.Val32;
    }

    if ((Size !=0) && (((Size + 1) % INSTR_PER_ROW) == 0)) {
      /* Device ID errata workaround: Reload data at address with LSB of 0x18 */
      WriteLatch(TempAddr.Word.HW, TempAddr.Word.LW,TempData.Word.HW,TempData.Word.LW);

      WriteMem(PM_ROW_WRITE);
    }

    SourceAddr.Val32 = SourceAddr.Val32 + 2;
  }


}
#endif


/******************************************************************************/



