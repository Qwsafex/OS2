#ifndef __PRINTF_H__

#define __PRINTF_H__

#include <serial.h>
#include <stdarg.h>
#include <string.h>

int vprintf(const char* format, va_list args);
int printf(const char* format, ...);
int vsnprintf(char* t, int m, const char* format, va_list args);
int snprintf(char* t, int m, const char* format, ...);

#endif