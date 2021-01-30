 
;
; test some multiply combinations

.include "p24Hxxxx.inc"

.global __reset          ;The label for the first line of code. 


         .data            ;unitialized data section

         .bss
;;These start at location 0x0800 because 0-0x07FF reserved for SFRs
u8_buf:   .space 2        ;Allocating space (in bytes) to variable.

.text                          ;Start of Code section
__reset:                       ; first instruction located at __reset label
       mov #__SP_init, w15     ;Initalize the Stack Pointer
       mov #__SPLIM_init,W0   
       mov W0, SPLIM           ;Initialize the stack limit register
       rcall main              ;call main()
       reset                   ;start over
 ;__SP_init set by linker to be after allocated data                                 
            
main:                    
      mov #0x9A,W0
      mov #u8_buf,W1
      rcall byte2aschex
      
done:                     ;do not return
       bra done           

;;Convert lower 4-bits of W0 to ASCII Hex
nyb2aschex:
      mov.b #0x30,W2   ;offset for '0' to '9' digit
      cp.b W0,#10      ; W0  >= 10?
      bra LTU, nyb_1   ; branch if W0 < 10
      mov.b #0x37,W2   ;offset for 'A' to 'F' digit
nyb_1:
      add.b W0,W2,W0   
      return

;;W0 is byte to be converted
;;W1 points to temp buffer that can hold 2 bytes
byte2aschex:
      ;do most significant digit
      push  W0          ;save 
      lsr   W0,#4,W0    ;do upper 4-bits
      and.b W0,#0x0F,W0 ; mask off upper bits
      rcall nyb2aschex
      mov.b W0,[W1++]   ;save return value
      pop W0            ;restore
      ;do least significant digit
      and.b W0,#0x0F,W0 ; mask off upper bits
      rcall nyb2aschex
      mov.b W0,[W1]     ;save return value
      return
 
  
    .end
