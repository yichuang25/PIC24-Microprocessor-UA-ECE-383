 
;
; countOnes subroutine example

.include "p24Hxxxx.inc"

.global __reset          ;The label for the first line of code. 


      .bss           ;uninitialized data section
;;These start at location 0x0800 because 0-0x07FF reserved for SFRs
k:   .space 2        ;Allocating space (in bytes) to variable    
j:   .space 2        ;Allocating space (in bytes) to variable.

.text                             ;Start of Code section
__reset:                          ; first instruction located at __reset label
       mov #__SP_init, w15       ;Initalize the Stack Pointer
       mov #__SPLIM_init,W0   
       mov W0, SPLIM             ;Initialize the stack limit register
 ;__SP_init set by linker to be after allocated data  
       rcall main              ;call main()
       reset                   ;start over                              

main:                         
       mov #13, W0
       mov WREG,j     ;j=13      
 ;subroutine call
       push W0        ;push j on stack as parameter
       rcall fib
       mov WREG,k
       sub W15,#2,W15 ;clean stack of passed parameters
 
done:
    goto    done           ;infinite wait loop


; stack frame used for n,f1
; return value passed back in W0
fib:
;lnk #2 implements the push, mov, add shown below
	;push W14          ;push FP
    ;mov  W15,W14      ;(FP=SP) new frame pointer
    ;add W15,#2,W15    ;SP=SP+2, local var space
    lnk #2
    mov [W14-8],W0    ;access n from FP
    cp W0,#0          ;n==0?
    bra Z,fib_exit    ;exit with W0=0     
    cp W0,#1          ;n==1?
    bra Z,fib_exit    ;exit with W0=1
    ;set up for next call
    dec W0,W0         ;n=n-1
    push W0           ;push n-1 parameter
    rcall fib         ;fib(n-1)
    sub W15,#2,W15    ;clean n-1 parameter from stack
    mov W0,[W14]      ;save returned f1 in local 
    ;set up for next call
    mov [W14-8],W0    ;access n from FP
    dec2 W0,W0        ;n=n-2
    push W0           ;save n-2 parameter
    rcall fib         ;fib(n-2)
    sub W15,#2,W15    ;clean n-2 parameter from stack
    add W0,[W14],W0   ;W0=fib(n-2)+f1
fib_exit:
;unlk implements the mov, pop shown below
    ;mov W14,W15       ;SP=FP (free local storage)
    ;pop W14           ;restore FP
    ulnk
    return            ;return f1  
  
    .end
