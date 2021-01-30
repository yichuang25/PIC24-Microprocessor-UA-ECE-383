 
;
; test some divide combinations

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
       mov #0xFF00, W0
       mov #0x2000, W1
       rcall sat_add
       mov #0xFF00,W0
       mov #0x0034,W1
       rcall sat_add

    
done:                     ;do not return
       bra done           

;Do saturating word addition W0= sat_add(W0+W1)
sat_add:
       add W0,W1,W0
       bra NC, sat_add_1
       setm W0
sat_add_1:
       return

  
    .end
