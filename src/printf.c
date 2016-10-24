#include <printf.h>


char number[30];

int n;
char* s;

void put_to_string(char c) {
	if (n == 0) return;
	s[0] = c;
	s++;
	n--;
}

int printULL(unsigned long long a, int base, void (*put)(char)) {
	int length = 0;
	if (a == 0) {
		number[length++] = '0';
	}
	while (a > 0) {
		int c = a % base;
		if (c < 10) {
			number[length++] = c + '0';
		} else {
			c -= 10;
			number[length++] = c + 'a';
		}
		a /= base;
	}

	for (int i = length - 1; i >= 0; --i) {
		put(number[i]);
	}

	return length;
}

int printLL(long long a, int base, void (*put)(char)) {
	if (a < 0) {
		put('-');
		a = -a;
	}
	return printULL(a, base, put);
}

int my_printf(const char* format, va_list args, void (*put)(char c)) {
	
	int cnt = 0;	

	while (format[0]) {

		if (format[0] == '%') {
			if (!format[1]) return -1;
			format++;
			if (format[0] == '%') {
				put('%');
				format++;
			} else if (format[0] == 'c') {
				char c = (char)va_arg(args, int);
				put(c);
				format++;
				cnt++;
			} else if (format[0] == 's') {
				const char* s = va_arg(args, const char*);
				while (s[0] != 0) {
					put(s[0]);
					s++;
					cnt++;
				}
				format++;
			} else {
				
				if (format[0] == 'd' || format[0] == 'i') {
					cnt += printLL(va_arg(args, int), 10, put);
					format++;
				} else if (format[0] == 'u') {
					cnt += printULL(va_arg(args, unsigned int), 10, put);
					format++;
				} else if (format[0] == 'o') {
					cnt += printULL(va_arg(args, unsigned int), 8, put);
					format++;
				} else if (format[0] == 'x') {
					cnt += printULL(va_arg(args, unsigned int), 16, put);
					format++;
				} else if (format[0] == 'h') {

					if (!format[1]) return -1;
					format++;

					if (format[0] == 'd' || format[0] == 'i') {
						cnt += printLL(va_arg(args, int), 10, put);
						format++;							
					} else if (format[0] == 'u') {
						cnt += printULL(va_arg(args, unsigned int), 10, put);
						format++;							
					} else if (format[0] == 'o') {
						cnt += printULL(va_arg(args, unsigned int), 8, put);
						format++;			
					} else if (format[0] == 'x') {
						cnt += printULL(va_arg(args, unsigned int), 16, put);
						format++;			
					} else if (format[0] == 'h') {
						format++;

						if (format[0] == 'd' || format[0] == 'i') {
							cnt += printLL(va_arg(args, int), 10, put);
							format++;							
						} else if (format[0] == 'u') {
							cnt += printULL(va_arg(args, unsigned int), 10, put);
							format++;							
						} else if (format[0] == 'o') {
							cnt += printULL(va_arg(args, unsigned int), 8, put);
							format++;			
						} else if (format[0] == 'x') {
							cnt += printULL(va_arg(args, unsigned int), 16, put);
							format++;			
						} else return -1;
						
					} else return -1;

				} else if (format[0] == 'l') {

					if (!format[1]) return -1;
					format++;

					if (format[0] == 'd' || format[0] == 'i') {
						cnt += printLL(va_arg(args, long), 10, put);
						format++;							
					} else if (format[0] == 'u') {
						cnt += printULL(va_arg(args, unsigned long), 10, put);
						format++;							
					} else if (format[0] == 'o') {
						cnt += printULL(va_arg(args, unsigned long), 8, put);
						format++;			
					} else if (format[0] == 'x') {
						cnt += printULL(va_arg(args, unsigned long), 16, put);
						format++;			
					} else if (format[0] == 'l') {
						format++;

						if (format[0] == 'd' || format[0] == 'i') {
							cnt += printLL(va_arg(args, long long), 10, put);
							format++;							
						} else if (format[0] == 'u') {
							cnt += printULL(va_arg(args, unsigned long long), 10, put);
							format++;							
						} else if (format[0] == 'o') {
							cnt += printULL(va_arg(args, unsigned long long), 8, put);
							format++;			
						} else if (format[0] == 'x') {
							cnt += printULL(va_arg(args, unsigned long long), 16, put);
							format++;			
						} else return -1;
						
					} else return -1;

				} else return -1;

			}	

		} else {
			put(format[0]);
			format++;
			cnt++;
		}
	}

	return cnt;
}

int vprintf(const char* format, va_list args) {
	return my_printf(format, args, write_serial_char);
}

int printf(const char* format, ...) {
	va_list args;
	va_start(args, format);
	int res = vprintf(format, args);
	va_end(args);
	return res;	
}

int vsnprintf(char* t, int m, const char* format, va_list args) {
	n = m - 1;
	s = t;
	int res = my_printf(format, args, put_to_string);
	s[0] = 0;
	return res;
}

int snprintf(char* t, int m, const char* format, ...) {
	va_list args;
	va_start(args, format);
	int res = vsnprintf(t, m, format, args);
	va_end(args);
	return res;	
}

char* strcpy(char *dst, const char *s){
	while((*dst++ = *s++));
	return dst;
}