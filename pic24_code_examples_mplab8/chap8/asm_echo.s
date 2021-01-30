/** \file
   Demonstrates how to call C functions from assembly lanaguage.
   Implements a UART1 character echo, and calls the
   \ref configBasic, \ref inChar, and \ref outChar functions.
*/
/** \cond nodoxygen */
.include "p24Hxxxx.inc"         ; Include processor-specific headers

.global _main                   ; Make _main visible outside
								; this file so C startup code
                                ; can call it

         .section psv psv		; Place following statements in PSV space
HELLO_MSG: .asciz "asm_echo.s ready!\n" ; Define a null-terminated ASCII string


;..............................................................................
;Code Section in Program Memory
;..............................................................................

.text                             ;Start of Code section

_main:                            ; _main called after C startup code runs
	mov #HELLO_MSG,W0			  ; Equivalent to
	call _configBasic             ;  configBasic(HELLO_MSG) in C

while1:                           ; while (1) {
	call _inChar                  ;   W0 = inChar();
	inc W0, W0                    ;   W0 = W0 + 1;
	call _outChar                 ;   outChar(W0);
    goto    while1                ;}

.end       ;End of program code in this file

/** \endcond */
