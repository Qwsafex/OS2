static void qemu_gdb_hang(void)
{
#ifdef DEBUG
	static volatile int wait = 1;

	while (wait);
#endif
}

#include <PIC.h>
#include <memmap.h>
#include <PIT.h>
#include <desc.h>
#include <ints.h>
#include <serial.h>

void init(){
	init_serial();
	write_serial_string("Hello!\n");

	init_ints();
	init_PIC();
	init_PIT();
	unmask(0);

	read_memmap();
}

void main(void)
{
	qemu_gdb_hang();

	init();
	print_memmap();

	while (1);
}
