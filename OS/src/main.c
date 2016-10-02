static void qemu_gdb_hang(void)
{
#ifdef DEBUG
	static volatile int wait = 1;

	while (wait);
#endif
}

#include <PIC.h>
#include <PIT.h>
#include <desc.h>
#include <ints.h>
#include <serial.h>

void main(void)
{
	qemu_gdb_hang();

	init_serial();
	write_serial_string("Hello!\n");

	init_ints();
	init_PIC();

	__asm__("int $0");

	unmask(0);

	init_PIT();

	while (1);
}
