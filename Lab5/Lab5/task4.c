#include "pic24_all.h"

uint16 x,y;
uint8 count;

void main(void) {
	x = 1;
	y = 3;

	for(count = 3; count>0; count--) {
		if((x-y) == 0) {
			y++;	
		}
		if (x < y) {
			x = x + 2;
		}
	}

}