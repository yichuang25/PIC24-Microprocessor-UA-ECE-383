	.include "p24Hxxxx.inc"
	.global __reset
	.bss
check_val:		.space 2
ones_count: 	.space 1
first_one: 		.space 1
count:			.space 1
set: 			.space 1


	.text
__reset:
	mov #__SP_init, w15
	mov #__SPLIM_init,W0
	mov W0, SPLIM


;-----------------------C code--------------------------------
;uint16 check_val = 0;
;uint8 ones_count = 0, first_one = 0, count = 0, set = 0;

;void main (void) {
;	for(check_val = 0xFFFF;check_val != 0; check_val = check_val / 2) {
;		if (check_val & 0x0001 == 0x0001) {
;			ones_count++;
;			if(set == 0) {
;				first_one = count;
;				set = 1;
;			}
;		}
;		count++;
;	}
;}
;----------------------code start here----------------------
	
; check_val = 0xF508
	mov #0xF508, w0
	mov wreg, check_val
;ones_count = 0
	clr.b ones_count
;first_one = 0
	clr.b first_one
;count = 0
	clr.b count
;set = 0
clr.b set

top:
	mov check_val w0;
 	bra Z, end_loop;
	and #0x0001, w0;
	bra Z, end_if1;
		inc.b ones_count
		mov.b set, wreg
		bra NZ, end_if2
			mov.b count, wreg
			mov.b wreg, first_one
 			mov.b #0x01, w0
			mov.b wreg, set
		end_if2:
	end_if1:
	inc.b count
	lsr check_val
	bra top

end_loop:

done: goto done

.end
	