 
;
; Just check out MPLAB

.include "p24Hxxxx.inc"

.global __reset          ;The label for the first line of code. 


         .bss            ;unitialized data section
;;These start at location 0x0800 because 0-0x07FF reserved for SFRs
i:       .space 1        ;Allocating space (in bytes) to variable.
j:       .space 1        ;Allocating space (in bytes) to variable.
k:       .space 1        ;Allocating space (in bytes) to variable.


;..............................................................................
;Code Section in Program Memory
;..............................................................................

.text                             ;Start of Code section
__reset:                          ; first instruction located at __reset label
        mov #__SP_init, W15       ;Initalize the Stack Pointer
        mov #__SPLIM_init,W0   
        mov W0, SPLIM             ;Initialize the stack limit register
 ;__SP_init set by linker to be after allocated data                                 
                              
;User Code starts here.
; C Program equivalent
;  #define avalue 100
;  uint8_t i,j,k;
;
;     i = avalue;   /* myvalue = 100 (0x64) */
;     i = i + 1;   /* i++, i = 101 (0x65) */
;     j = i;       /* j is 101  (0x65) */
;     j = j - 1;   /* j--, j is 100 (0x64) */
;     k = j + i;    /* k = 201 (0xC9) */
;
		.equ avalue, 100

;i = avalue;   /* avalue = 100 */
	mov.b #avalue, W0       ; W0 = 100
    mov.b WREG,i            ; i = 100

; i = i + 1;
    inc.b   i               ; i = i + 1

; j = i
    mov.b   i,WREG          ; W0 = i
    mov.b   WREG,j          ; j = W0

; j = j - 1;  /* j--, j is 100 */
    dec.b   j              ; j= j - 1

; k = j + i
    mov.b   i,WREG         ; W0 = i
    add.b   j,WREG         ; W0 = W0+j  (WREG is W0)
    mov.b   WREG,k         ; k = W0

done:
    goto    done              ;Place holder for last line of executed code

.end       ;End of program code in this file
