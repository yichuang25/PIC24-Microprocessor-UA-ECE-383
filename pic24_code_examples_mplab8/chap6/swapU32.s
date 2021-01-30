 
;
; countOnes subroutine example

.include "p24Hxxxx.inc"

.global __reset          ;The label for the first line of code. 


     .bss           ;unitialized data section

a:    .space   4*4  ; four element u32 array
     
.text                             ;Start of Code section
__reset:                          ; first instruction located at __reset label
       mov #__SP_init, w15       ;Initalize the Stack Pointer
       mov #__SPLIM_init,W0   
       mov W0, SPLIM             ;Initialize the stack limit register
       rcall init_a
       goto main
 ;__SP_init set by linker to be after allocated data                                 

main:        
       mov #a,W0
       mov #1,W1
       mov #3,W2
       rcall swapU32
                     

done:
    goto    done           ;infinite wait loop


; u32_baseptr passed in W0
; u8_k passed in W1
; u8_i passed in W2
; W3 used for &bptr[k]
; W4 used for &bptr[j]
; W5,W6 used for u32_tmp
swapU32:
    sl W1,#2,W1  ;k=k<<2=k*4
    sl W2,#2,W2  ;j=j<<2=j*4
    add W0,W1,W3 ;W3 = &bptr[k]
    add W0,W2,W4 ;W4 = &bptr[j]
 
    mov [W3++],W5  ;W5 = bptr[k].LSW
    mov [W3--],W6  ;W6 = bptr[k].MSW

    mov [W4++],[W3++] ;bptr[k].LSW= bptr[j].LSW
    mov [W4--],[W3--] ;bptr[k].MSW= bptr[j].MSW

    add W0,W2,W4   ;W4 = &bptr[j]   
  
    mov W5,[W4++]  ;bptr[j].LSW= u32_tmp.LSW;
    mov W6,[W4--]  ;bptr[j].MSW= u32_tmp.MSW;
   
    return
  
;initialize A array
init_a:

    mov #a,W2
    mov #0xA1F9,W0
    mov #0x0982,W1
    mov.d W0,[W2++]
    mov #0x3204,W0
    mov #0x88B2,W1
    mov.d W0,[W2++]
    mov #0x6B10,W0
    mov #0xE3D9,W1
    mov.d W0,[W2++]
    mov #0xFB29,W0
    mov #0xC385,W1
    mov.d W0,[W2++]
    return

    .end
