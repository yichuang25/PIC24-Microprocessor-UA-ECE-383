#include "pic24_all.h"
uint16 u16_a, u16_b, u16_c, u16_d;
uint8 u8_x, u8_y, u8_z;

void main(void) {
	u8_x=0xFF;
	u8_y=0x01;
	u16_a = 0xFFFF;
	u16_b = 0x0001;
	u8_z=u8_x+u8_y;
	u16_d=(uint16) u8_x + (uint16) u8_y;
	u16_c=u16_a+u16_b;
}