#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "ioport.h"

#define PORT 0x3f8

void init_serial();

int serial_ready();

void write_serial_char(char c);

void write_serial_string(const char *s);

#endif /* __SERIAL_H__ */