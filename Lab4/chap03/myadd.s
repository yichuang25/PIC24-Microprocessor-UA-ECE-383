 
;
; Just check out MPLAB

		.include "p24Hxxxx.inc"

       .global __reset          ;The label for the first line of code. 

         .bss            ;unitialized data section
;;These start at location 0x0800 because 0-0x07FF reserved for SFRs
aa:       .space 1        ;Allocating space (in bytes) to variable.
bb:       .space 1        ;Allocating space (in bytes) to variable.
lsp:       .space 2        ;Allocating space (in bytes) to variable.
msp:		.space 2
sum:		.space 2

;..............................................................................
;Code Section in Program Memory
;..............................................................................

         .text           ;Start of Code section
__reset:                 ; first instruction located at __reset label
        mov #__SP_init, w15       ;Initalize the Stack Pointer
        mov #__SPLIM_init,W0   
        mov W0, SPLIM             ;Initialize the stack limit register
;__SP_init set by linker to be after allocated data      

;User Code starts here.
; C Program equivalent
;  #define avalue 2047
;			uint8 aa=100, bb=22;
;			uint16 lsp, msp, sum;
;			lsp = 0xY3Y2Y1Y0; // Four digits of CWID treated as hex 
;			msp = 0xY7Y6Y5Y4; // Four digits of CWID treated as hex 
;			sum = lsp + msp;
;			sum = sum + aa + bb;
;
		.equ avalue, 6882
mov #0x6882 ,w0;		w0 = 0x6882
mov wreg, lsp;		lsp = 0x6882

mov #0x1190 ,w0;		w0 = 0x1190
mov wreg, msp;		msp = 0x1190

mov.b #0x64, w0;		w0.LSB = 100
mov.b wreg aa; 		aa = 100

mov.b #0x16, w0;		w0.LSB = 22
mov.b wreg, bb;		bb = 22

mov msp, wreg;		w0 = msp
add lsp, wreg;		w0 = lsp + msp(w0 = 26754 + 4496 = 31250)
mov wreg, sum;		sum = w0

mov.b aa, wreg;		w0.LSB = aa
ze w0, w1;			w1 = aa
mov.b bb, wreg;		w0.LSB = bb
ze w0, w0;			w0 = bb

add w0, w1, w0; 	w0 = aa + bb(w0 = 100 + 22 = 122)
add sum;			sum = aa + bb + sum (sum = 122 + 31250 = 31372)


done:
    goto     done    ;Place holder for last line of executed code

.end       ;End of program code in this file
