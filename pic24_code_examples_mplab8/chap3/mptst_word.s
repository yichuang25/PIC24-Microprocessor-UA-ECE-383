 
;
; Just check out MPLAB

		.include "p24Hxxxx.inc"

       .global __reset          ;The label for the first line of code. 

         .bss            ;unitialized data section
;;These start at location 0x0800 because 0-0x07FF reserved for SFRs
i:       .space 2        ;Allocating space (in bytes) to variable.
j:       .space 2        ;Allocating space (in bytes) to variable.
k:       .space 2        ;Allocating space (in bytes) to variable.


;..............................................................................
;Code Section in Program Memory
;..............................................................................

         .text           ;Start of Code section
__reset:                 ; first instruction located at __reset label
        mov #__SP_init, w15       ;Initalize the Stack Pointer
        mov #__SPLIM_init,W0   
        mov W0, SPLIM             ;Initialize the stack limit register
;__SP_init set by linker to be after allocated data      

;User Code starts here.
; C Program equivalent
;  #define avalue 2047
;  uint16_t i,j,k;
;
;     i = avalue;   /* myvalue = 2047 (0x7FF) */
;     i = i + 1;   /* i++, i = 2048 (0x800)  */
;     j = i;       /* j is 2048 (0x0800) */
;     j = j - 1;   /* j--, j is 2047   */
;     k = j + i;    /* k = 4095 (0x0FFF) */
;
		.equ avalue, 2047

;i = avalue;   /* myvalue = 2047 */
	mov #avalue, w0      ; w0 = 2047 (w0 is wreg)
    mov wreg,i           ; i = 2047

; i = i + 1;
    inc   i              ; i = i + 1

; j = i
    mov   i,wreg           ; w0 = i
    mov   wreg,j           ; j = w0

; j = j - 1;  /* j--, j is 100 */
    dec   j              ; j= j - 1

; k = j + i
    mov   i,wreg           ; w0 = i
    add   j,wreg           ; w0 = w0+j  
    mov   wreg,k           ; k = 0

done:
    goto     done    ;Place holder for last line of executed code

.end       ;End of program code in this file
