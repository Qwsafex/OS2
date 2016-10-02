static void qemu_gdb_hang(void)
{
#ifdef DEBUG
	static volatile int wait = 1;

	while (wait);
#endif
}

#include <desc.h>
#include <ints.h>
#include <serial.h>

void main(void)
{
	qemu_gdb_hang();

//	init_serial();
	write_serial_string("Hello, Ilya");

	init_ints();
	write_serial_string("Four");


	while (1){
//		write_serial_char('#');
	}
}
