#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "ioport.h"

#define PORT 0x3f8

void init_serial(){	
	out8(PORT+3, 0x80); // enable DLAB
	out8(PORT+0, 0x03); // baud divisor to 3 (low byte)
	out8(PORT+1, 0x00); // (high byte)

	out8(PORT+3, 0x03); // 8-bit frame, no parity check, one stop bit
						// and disable DLAB to allow writing to serial
}

int serial_ready(){
	return (in8(PORT+5) & (1<<5));
}

void write_serial_char(char c){
	while(!serial_ready());

	out8(PORT+0, c);
}

void write_serial_string(const char *s){
	while(*s){
		write_serial_char(*s);
		s++;
	}
}

#endif /* __SERIAL_H__ */