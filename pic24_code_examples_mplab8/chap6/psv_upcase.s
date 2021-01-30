 
;
; illustrates string initialization from program memory
; using the PSV window

.include "p24Hxxxx.inc"

.global __reset          ;The label for the first line of code. 


      .bss        ;unitialized data section
;;These start at location 0x0800 because 0-0x07FF reserved for SFRs
s1:   .space  6        ;Allocating space (in bytes) to variable   
s2:   .space 12        ;reserve space

.text                             ;Start of Code section
__reset:                          ; first instruction located at __reset label
       mov #__SP_init, w15       ;Initalize the Stack Pointer
       mov #__SPLIM_init,W0   
       mov W0, SPLIM             ;Initialize the stack limit register
       call init_variables       ;initialize strings
 ;__SP_init set by linker to be after allocated data   
       rcall main              ;call main()
       reset                   ;start over                              
main: 
    mov #s1,W0
    rcall upcase
    mov #s2,W0
    rcall upcase     
done:
    goto    done           ;infinite wait loop

;*p passed in W0
;W1,W2 used as temporaries
upcase:
  mov.b [W0],W1       ;W1=*p
  cp.b W1,#0x00
  bra Z, upcase_exit  ;exit if at end of string
  mov #0x60,W2
  cp.b W1,W2             ;compare *p and 0x60     
  bra LEU, upcase_end_if  ;skip if-body
  mov #0x7B,W2
  cp.b W1,W2          ;compare *p and 0x7B
  bra GEU, upcase_end_if     ;skip if_body
  mov #0x20,W2
  sub.b W1,W2,[W0]    ; *p = *p-0x20
upcase_end_if:
  mov.b [W0],[W0++]   ;dummy move to do p++
  bra upcase          ;loop back to top
upcase_exit:
  return

;; constant data to be moved to data memory
s1_const: .asciz  "Hello"
s2_const: .asciz "UPPER/lower"

init_variables:
;turn on Program Visibility Space, use default PSVPAG
;value of 0
    bset CORCON,#2   ;enable PSV   
;copy source address in program memory to W2
    mov  #psvoffset(s1_const),W2   
    mov  #s1,W3        ;destination address in data memory
    rcall copy_cstring
;copy source address in program memory to W2
    mov  #psvoffset(s2_const),W2   
    mov  #s2,W3        ;destination address in data memory
    rcall copy_cstring
    return
;;copy constant null-terminated string from program memory to data memory
;;W2 points to program memory, W3 to data memory
copy_cstring:
    mov.b [W2],W0
    cp.b W0,#0
    bra Z, copy_cstring_exit  ;exit if null byte
    mov.b [W2++],[W3++]
    bra  copy_cstring
copy_cstring_exit:
    mov.b [W2++],[W3++]    ;copy null byte
    return
   
  
    .end
