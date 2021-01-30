 
;
; Just check out MPLAB

		.include "p24Hxxxx.inc"

       .global __reset          ;The label for the first line of code. 

         .bss            ;unitialized data section
;;These start at location 0x0800 because 0-0x07FF reserved for SFRs
xx:      .space 2        ;Allocating space (in bytes) to variable.
yy:		 .space 2
i:       .space 1        ;Allocating space (in bytes) to variable.
j:       .space 1        ;Allocating space (in bytes) to variable.
k:       .space 1
l:       .space 1
m:       .space 1

         .text           ;Start of Code section
__reset:                 ; first instruction located at __reset label
        mov #__SP_init, w15       ;Initalize the Stack Pointer
        mov #__SPLIM_init,W0   
        mov W0, SPLIM             ;Initialize the stack limit register
;__SP_init set by linker to be after allocated data      
;User Code starts here.
; C Program equivalent
;  #define avalue 2047
;	uint16 xx=0xDEAD, yy=0xBEEF;
;	uint8 i, j, k, l, m;
;	i = Y1Y0 82; j = Y3Y2 68; k = Y5Y4 90;
;	l = i + k
; 	m = j – l
;	xx=xx-yy-m;
;	11906882
		.equ avalue, 6882
mov #0xDEAD, w0;	w0 = 0xDEAD
mov wreg, xx;		xx = w0
mov #0xBEEF, w0;	w0 = 0xBEEF
mov wreg, yy;		yy = w0

;11906882
mov.b #0x52, w0;	w0 = 0x52
mov.b wreg, i;		i = 0x52
mov.b #0x44, w0;	w0 = 0x44
mov.b wreg, j;		j = 0x44
mov.b #0x5A, w0;	w0 = 0x5A
mov.b wreg, k;		k = 0x5A

add.b i, wreg;		w0 = k + i
mov.b wreg, l;		l = k + i (l = 90 + 82 = 172)
sub.b j, wreg;		w0 = j - (k+i)
mov.b wreg, m;		m = j-l (m = 68 - 172 = 0b 1001 1000 = 0x98 = 152)

mov.b m, wreg;		w0.LSB = m
ze w0, w1;			w1 = m
mov yy, wreg;		w0 = yy
sub xx, wreg; 		w0 = xx - yy = (57005 - 48879 = 8126)
sub w0, w1, w0;		w0 = xx - yy - m
mov wreg, xx;		xx = w0(xx = 8126 - 152 = 7974)

done:
    goto     done    ;Place holder for last line of executed code

.end       ;End of program code in this file
