 
;
; countOnes subroutine example

.include "p24Hxxxx.inc"

.global __reset          ;The label for the first line of code. 


      .bss        ;unitialized data section
;;These start at location 0x0800 because 0-0x07FF reserved for SFRs
s:   .space  6        ;Allocating space (in bytes) to variable   

.text                             ;Start of Code section
__reset:                          ; first instruction located at __reset label
       mov #__SP_init, W15       ;Initalize the Stack Pointer
       mov #__SPLIM_init,W0   
       mov W0, SPLIM             ;Initialize the stack limit register
                    
;;uppercase string at s:

  mov #s,W0         ;W0=&s[0]
top_loop:
  mov.b [W0],W1     ;W1=*p       
  cp.b W1,#0x00
  bra Z, end_loop   ;exit if at end of string
  mov #0x60,W2
  cp.b W1,W2        ;compare *p and 0x60     
  bra LEU, end_if   ;skip if-body
  mov #0x7B,W2
  cp.b W1,W2        ;compare *p and 0x7B
  bra GEU, end_if   ;skip if_body
  mov #0x20,W2
  sub.b W1,W2,[W0]  ;*p = *p-0x20
end_if:
  inc W0,W0         ;do p=p+1*sizeof(char)=p+1
  bra top_loop      ;loop back to top
end_loop:
    
done:
    goto    done           ;infinite wait loop

;*p passed in W0
;W1,W2 used as temporaries

  return


  
    .end
