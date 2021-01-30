 
;
; test some multiply combinations

.include "p24Hxxxx.inc"

.global __reset          ;The label for the first line of code. 


         .data            ;unitialized data section

         .bss
;;These start at location 0x0800 because 0-0x07FF reserved for SFRs
u16_k:   .space 2        ;Allocating space (in bytes) to variable.
u8_j:    .space 1        ;Allocating space (in bytes) to variable.

.text                          ;Start of Code section
__reset:                       ; first instruction located at __reset label
       mov #__SP_init, w15     ;Initalize the Stack Pointer
       mov #__SPLIM_init,W0   
       mov W0, SPLIM           ;Initialize the stack limit register
       rcall main              ;call main()
       reset                   ;start over
 ;__SP_init set by linker to be after allocated data                                 
            
main:                    
      mov #0xFFFF,W0  ; 65535 unsigned, -1 signed
      mov #0x1,W1     ; 1 unsigned, +1 signed

      mul.uu W0,W0,W2  ;65535 * 65535 = 4294836225 = 0xFFFE0001 = W3:W2
      mul.ss W0,W0,W4  ;-1 * -1 = +1 = 0x00000001 = W5:W4 
      mul.uu W0,W1,W6  ;65535 * 1 = 65535 = 0x0000FFFF = W7:W6 
      mul.ss W0,W1,W8  ;-1 * 1 = -1 = 0xFFFFFFFF = W9:W8
      mul.uu W0,#1,W10 ;65535 * 1 = 65535 = 0x0000FFFF = W11:10
      mul.su W0,#1,W12 ;-1 * 1 = -1 = 0xFFFFFFFF = W13:W12

      
done:                     ;do not return
       bra done           





  
    .end
