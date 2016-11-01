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
#include <printf.h>
#include <buddy.h>

void init(){
	init_serial();
	write_serial_string("Hello!\n");

	init_ints();
	init_PIC();
	init_PIT();
	unmask(0);

	init_mem();
}

void main(void)
{
	qemu_gdb_hang();

	init();

	// buddy test
    void* page_addr_0 = (void*) buddy_alloc(0, 0);
    printf("%llx\n", page_addr_0);
    void* page_addr_1 = (void*) buddy_alloc(0, 0);
    printf("%llx\n", page_addr_1);
    buddy_free(page_addr_0);
    page_addr_0 = (void*) buddy_alloc(5, 0);
    printf("%llx\n", page_addr_0);
    buddy_free(page_addr_0);
    page_addr_0 = (void*) buddy_alloc(5, 0);
    printf("%llx\n", page_addr_0);
    buddy_free(page_addr_0);
    buddy_free(page_addr_1);
    page_addr_0 = (void*) buddy_alloc(1, 0);
    printf("%llx\n", page_addr_0);

	while (1);
}
