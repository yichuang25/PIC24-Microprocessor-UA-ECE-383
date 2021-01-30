 
;
; countOnes subroutine example

.include "p24Hxxxx.inc"

.global __reset          ;The label for the first line of code. 


      .bss        ;unitialized data section
;;These start at location 0x0800 because 0-0x07FF reserved for SFRs
k:   .space 2        ;Allocating space (in bytes) to variable    
j:   .space 2        ;Allocating space (in bytes) to variable.

.text                             ;Start of Code section
__reset:                          ; first instruction located at __reset label
       mov #__SP_init, W15       ;Initalize the Stack Pointer
       mov #__SPLIM_init,W0   
       mov W0, SPLIM             ;Initialize the stack limit register
 ;__SP_init set by linker to be after allocated data   
       rcall main              ;call main()
       reset                   ;start over                              

main:                              
       mov #13, W0
       mov WREG,j     ;j=13
 ;subroutine call
       mov j,WREG     ;W0 used to for n parameter
       rcall fib
       mov WREG,k
 
done:
    goto    done           ;infinite wait loop


; n passed in W0
; return value passed back in W0
; W1 used for f1 local
fib:
    cp W0,#0          ;n==0?
    bra Z,fib_exit    ;if W0 is 0, return it
    cp W0,#1          ;n==1?
    bra Z,fib_exit    ;if W0 is 1, return it
    push W0           ;save n (W0)
    dec W0,W0         ;n=n-1
    rcall fib         ;fib(n-1)
    mov W0,W1         ;save f1 in W1
    pop W0            ;get old n
    dec2 W0,W0        ;n=n-2
    push W1           ;save f1
    rcall fib         ;fib(n-2)
    pop W1            ;restore f1 to W1
    add W0,W1,W0      ;W0=fib(n-2)+f1
fib_exit:
    return            ;return f1  
  
    .end
