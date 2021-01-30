 
;
; Just check out MPLAB

		.include "p24Hxxxx.inc"

       .global __reset          ;The label for the first line of code. 

         .bss            ;unitialized data section
;;These start at location 0x0800 because 0-0x07FF reserved for SFRs
a:       .space 1        ;Allocating space (in bytes) to variable.
b:       .space 1        ;Allocating space (in bytes) to variable.
c:       .space 1        ;Allocating space (in bytes) to variable.
d:		 .space 1
e:		 .space 1
f: 		 .space 1
x:	 	 .space 2

         .text           ;Start of Code section
__reset:                 ; first instruction located at __reset label
        mov #__SP_init, w15       ;Initalize the Stack Pointer
        mov #__SPLIM_init,W0   
        mov W0, SPLIM             ;Initialize the stack limit register
;__SP_init set by linker to be after allocated data      
;User Code starts here.
; C Program equivalent
;  #define avalue 2047
;	uint8 u8_a, u8_b, u8_c, u8_d, u8_e, u8_f;
;	uint16 u16_x=0x0001;
;	u8_a=0xAF;
;	u8_b=0x50;
;	u8_c= u8_a & u8_b
; 	u8_d= u8_a | u8_b
;	u8_e= u8_a ^ u8_b
; 	u8_f=~u8_a
;	u16_x=~u8_d | (u16_x & u8_c);		
mov #0x0001, w0;	w0 = 0x0001
mov wreg, x;		x = 0x0001
mov.b #0xAF, w0;	w0.lsb = 0xAF
mov.b wreg, a;		a = w0.lsb
mov.b #0x50, w0;	w0.lsb = 0x50
mov.b wreg, b;		b = w0.lsb

and.b a, wreg;		w0.lsb = a & b
mov.b wreg, c;		c = w0.lsb (c = 0b )

mov.b b, wreg;		w0.lsb = b
ior.b a, wreg;		w0.lsb = a | b
mov.b wreg, d;		d = w0.lsb

mov.b b, wreg;		w0.lsb = b
xor.b a, wreg;		w0.lsb = a ^ b
mov.b wreg, e;		e = wreg.lsb

com.b a, wreg;		w0.lsb = ~a
mov.b wreg, f;		f = w0.lsb

mov x, wreg;		w0 = x
and.b c, wreg;		w0 = x.lsb & c
mov w0, w1;			w1 = w0
com.b d, wreg;		w0.lsb = ~d
ior.b w0, w1, w0;	w0 = w1.lsb | (~d)
mov wreg, x;		x = w0

done:
    goto     done    ;Place holder for last line of executed code

.end       ;End of program code in this file
