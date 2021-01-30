 
;
; Emulate the number sequencing computer in PIC24 assembly

	.include "p24Hxxxx.inc"
    .global __reset          ;The label for the first line of code. 

         .bss        ;unitialized data section

loc:     .space 1    ;byte variable
dout:     .space 1    ;byte variable

         .text           ;Start of Code section
__reset:                 ; first instruction located at __reset label
    mov #__SP_init, W15  ;Initalize the Stack Pointer


   ;bclr   loc, #0   ;uncomment for loc<0>=0
   bset    loc, #0   ;uncomment for loc<0>=1
loop_top:
   btsc.b  loc,#0    ;skip next if loc<0> is 0
   goto    loc_lsb_is_1
   ;loc<0> is 0 if reach here
   mov.b   #3,W0     
   mov.b   WREG,dout  ;dout = 3    
   mov.b   #2,W0     
   mov.b   WREG,dout  ;dout = 2    
   mov.b   #4,W0     
   mov.b   WREG,dout  ;dout = 4    
loc_lsb_is_1:
   mov.b   #8,W0     
   mov.b   WREG,dout  ;dout = 8    
   mov.b   #5,W0     
   mov.b   WREG,dout  ;dout = 5    
   mov.b   #6,W0     
   mov.b   WREG,dout  ;dout = 6    
   mov.b   #1,W0     
   mov.b   WREG,dout  ;dout = 1
   goto    loop_top  ;loop forever

.end       ;End of program code in this file
