 
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
      mov #'9',W0
      mov.b WREG,u8_buf    ;u8_buf[0]='9'=0x39
      mov #'A',W0
      mov.b WREG,u8_buf+1   ;u8_buf[1]='A'=0x41
      mov #u8_buf,W0   
      rcall aschex2byte
      ;; upon return, W0=0x9A
      
done:                     ;do not return
       bra done           

;;Convert ASCII-hex value in W0 to binary
aschex2nyb:
      bclr.b W0,#5     ;lowercase W0
      mov.b  #0x30,W2   ;offset for '0' to '9' digit
      mov.b  #0x3A,W3
      cp.b   W0,W3       ;W0  >= 0x3A?
      bra    LTU, asc_1   ;branch if W0 < 0x3A
      mov.b  #0x37,W2   ;offset for 'A' to 'F' digit
asc_1:
      sub.b W0,W2,W0   
      return

;;W0 points to buffer that holds 2 bytes to be converted
aschex2byte:
      mov   W0,W1         ;save pointer
      mov.b [W1++], W0  ;get first digit
      rcall aschex2nyb
      sl    W0,#4, W4      ;shift to upper 4-bits      
      mov.b [W1],W0     ;get 2nd digit
      rcall aschex2nyb
      ior.b W4,W0,W0    ;combine
      ze    W0,W0       ;zero extend
      return 
  
    .end
