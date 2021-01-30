#include "pic24_all.h"

uint16 check_val = 0;
uint8 ones_count = 0, first_one = 0, count = 0, set = 0;

void main (void) {
	for(check_val = 0xF508;check_val != 0; check_val = check_val / 2) {
		if (check_val & 0x0001 == 0x0001) {
			ones_count++;
			if(set == 0) {
				first_one = count;
				set = 1;
			}
		}
		count++;
	}
	
	

}