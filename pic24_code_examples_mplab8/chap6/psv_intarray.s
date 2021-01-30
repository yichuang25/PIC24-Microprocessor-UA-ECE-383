 
;
;
; illustrates array initialization from program memory
; using the PSV window

.include "p24Hxxxx.inc"

.global __reset          ;The label for the first line of code. 


      .bss        ;unitialized data section
;;These start at location 0x0800 because 0-0x07FF reserved for SFRs

i8_a:   .space 4*1      ;Allocating space (in bytes) to variable   
i16_b:  .space 4*2      ;Allocating space (in bytes) to variable   
ui32_c: .space 4*4      ;Allocating space (in bytes) to variable   

.text                             ;Start of Code section
__reset:                          ; first instruction located at __reset label
       mov #__SP_init, w15       ;Initalize the Stack Pointer
       mov #__SPLIM_init,W0   
       mov W0, SPLIM             ;Initialize the stack limit register
       call init_variables       ;initialize global variables
 ;__SP_init set by linker to be after allocated data   
       rcall main              ;call main()
       reset                   ;start over                              
main: 
    ;empty body...
done:
    goto    done           ;infinite wait loop



;; constant data to be moved to data memory
i8_a_const:  .byte -9, 10, 58, -125
i16_b_const:  .word 200, -3200, -2, 450
ui32_c_const:  .long 160235, 250345, 65536, 489300

init_variables:
;turn on Program Visibility Space, use default PSVPAG
;value of 0
    bset CORCON,#2   ;enable PSV 
;copy i8_a_const to i8_a
    mov  #psvoffset(i8_a_const),W2   
    mov  #i8_a,W3        ;destination address in data memory
    mov  #((4*1)-1),W4    ;(number of bytes)-1 to copy 
    call byte_copy  
;copy i16_b_const to i16_b
    mov  #psvoffset(i16_b_const),W2   
    mov  #i16_b,W3        ;destination address in data memory
    mov  #((4*2)-1),W4    ;(number of bytes)-1 to copy
    call byte_copy
;copy i32_c_const to i32_c
    mov  #psvoffset(ui32_c_const),W2   
    mov  #ui32_c,W3        ;destination address in data memory
    mov  #((4*4)-1),W4    ;(number of bytes)-1 to copy1 
    call byte_copy
    return

byte_copy:
    repeat W4
    mov.b [W2++],[W3++]
    return
 .end
