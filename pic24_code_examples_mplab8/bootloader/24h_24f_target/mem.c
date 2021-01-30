#include "pic24_all.h"

typedef short          Word16;
typedef unsigned short UWord16;
typedef long           Word32;
typedef unsigned long  UWord32;

//write these in C so that can use the
//__PIC24H__, __PIC24F__ defines.

void WriteMem(UWord16 val) {
  asm("mov	W0,NVMCON");
  __builtin_write_NVM();

  asm("1:	btsc NVMCON,#15");    //	;Wait for write end
  asm("	bra 1b");

}

#if (defined(__PIC24E__) || defined(__dsPIC33E__))
void WriteMem2(UWord16 addrhi, UWord16 addrlo, UWord16 val) {
  asm("mov     w0,NVMADRU");           //; Init Pointer to page to be erased
  asm("mov     w1,NVMADR");           //; Init Pointer to offset to be erased
  asm("mov	W2,NVMCON");
  //__builtin_write_NVM();
  asm("disi #06");
  asm("mov #0x55,W0");
  asm("mov W0, NVMKEY");
  asm("mov #0xAA,W0");
  asm("mov W0,NVMKEY");
  asm("bset NVMCON,#15");
  asm("nop");
  asm("nop");

  asm("1:	btsc NVMCON,#15");    //	;Wait for write end
  asm("	bra 1b");

}
#endif


//_LoadAddr:	;W0=NVMADRU,W1=NVMADR - no return values
void LoadAddr(UWord16 nvmadru, UWord16 nvmadr) {
  asm("mov	W0,TBLPAG");
  asm("mov	W1,W1");
}

//_WriteLatch: ;W0=TBLPAG,W1=Wn,W2=WordHi,W3=WordLo - no return values
void WriteLatch(UWord16 addrhi,UWord16 addrlo, UWord16 wordhi, UWord16 wordlo) {
  asm("	mov	W0,TBLPAG");
  asm("	tblwtl W3,[W1]");
  asm("	tblwth W2,[W1]");
}

#if (defined(__PIC24E__) || defined(__dsPIC33E__))
//_LoadTwoWords: ;W0=TBLPAG,W1=Wn,W2=WordHi,W3=WordLo W4=Word2Hi,W5=Word2Lo
//W0,W1 not really used
void LoadTwoWords(UWord16 addrhi, UWord16 addrlo, UWord16 wordhi, UWord16 wordlo, UWord16 word2hi, UWord16 word2lo) {
  asm("	mov	#0xFA,W0");
  asm(" mov W0, TBLPAG");
  asm("	mov	#0,W1");
  asm("	tblwtl W3,[W1]");
  asm("	tblwth W2,[W1++]");
  asm("	tblwtl W5,[W1]");
  asm("	tblwth W4,[W1++]");
}
#endif

//_ReadLatch: ;W0=TBLPAG,W1=Wn - data in W1:W0
UWord32 ReadLatch(UWord16 addrhi, UWord16 addrlo) {

  asm("	mov	W0,TBLPAG");
  asm("	tblrdl [W1],W0");
  asm("	tblrdh [W1],W1");

}
#if (defined(__PIC24E__) || defined(__dsPIC33E__))
void ResetDevice(void) {

//  asm(" goto 0xc02");
  asm(" goto 0x1300");
}

void ResetDeviceasPOR(void) {
  _POR = 1;
//  asm(" goto 0xc02");
  asm(" goto 0x1300");
}

#else
void ResetDevice(void) {

//  asm(" goto 0xc02");
  asm(" goto 0xe00");
}

void ResetDeviceasPOR(void) {
  _POR = 1;
//  asm(" goto 0xc02");
  asm(" goto 0xe00");
}
#endif


#if (defined(__PIC24E__) || defined(__dsPIC33E__))
void Erase(UWord16 addrhi, UWord16 addrlo, UWord16 val ) {


  asm("mov	W2,NVMCON");

  asm("mov     w0,NVMADRU");           //; Init Pointer to page to be erased
  asm("mov     w1,NVMADR");           //; Init Pointer to offset to be erased

  //__builtin_write_NVM();

  asm("disi #06");
  asm("mov #0x55,W0");
  asm("mov W0, NVMKEY");
  asm("mov #0xAA,W0");
  asm("mov W0,NVMKEY");
  asm("bset NVMCON,#15");
  asm("nop");
  asm("nop");

  asm("1:	btsc NVMCON,#15");    //	;Wait for write end
  asm("	bra 1b");


}
#else
//_Erase:
void Erase(UWord16 addrhi, UWord16 addrlo, UWord16 val ) {

  asm("push    TBLPAG");
  asm("mov	W2,NVMCON");

  asm("mov     w0,TBLPAG");           //; Init Pointer to page to be erased
  asm("tblwtl  w1,[w1]");		        //; Dummy write to select the row

  __builtin_write_NVM();


  asm("1:	btsc NVMCON,#15");    //	;Wait for write end
  asm("	bra 1b");

  asm("pop     TBLPAG");
}
#endif




