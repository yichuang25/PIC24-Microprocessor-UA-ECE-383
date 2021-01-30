;
; countOnes subroutine example

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
; u16_k = 0xA501;
       mov #0xA501, W0
       mov WREG, u16_k     ;init u16_k

; u8_j = countOnes(u16_k);
       mov u16_k, WREG     ;W0 used to for u16_v parameter
       rcall countOnes
       mov.b WREG, u8_j
done:                     ;do not return
       bra done           




; u16_v passed in W0
; return value passed back in W0
; W1 used for local u8_cnt, W2 for u8_i
countOnes:
    clr.b W1          ;u8_cnt=0
    clr.b W2          ;u8_i=0
loop_top:
    cp.b W2,#16       ;compare u8_i, 16
    bra GEU,end_loop  ;exit loop if u8_i>=16
    btst.z W0,#0      ;test LSbit for zero
    bra Z, end_if
    inc.b W1,W1       ;u8_cnt++;
end_if:
    lsr W0,#1,W0      ;u16_v = u16_v >> 1;
    inc.b W2,W2       ;u8_i++
    bra loop_top
end_loop:
    mov.b W1,W0       ;W0 = u8_cnt for
    return            ;  return value
  
  
    .end
