#ifndef __INTS_H__
#define __INTS_H__

#include <desc.h>
#include <memory.h>
#include <serial.h>

static inline void disable_ints(void)
{ __asm__ volatile ("cli" : : : "cc"); }

static inline void enable_ints(void)
{ __asm__ volatile ("sti" : : : "cc"); }

struct int_desc
{
   uint16_t offset_0_15; 
   uint16_t CS;       // a code segment selector in GDT or LDT
   uint8_t ist;       // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
   uint8_t type_attr; // type and attributes
   uint16_t offset_16_31; 
   uint32_t offset_32_63;
   uint32_t zero;     // reserved
} __attribute__((packed));

#define INT_COUNT 48

struct int_desc IDT [INT_COUNT];

extern uint64_t addrs[];

void init_int_desc(struct int_desc *IDT, uint64_t offset, uint16_t CS, uint8_t ist, uint8_t type, uint8_t DPL){
	IDT->offset_0_15 = offset & 0xffffull;
	IDT->offset_16_31 = (offset & 0xffff0000ull) >> 16;
	IDT->offset_32_63 = offset >> 32;
	IDT->CS = CS;
	IDT->ist = ist;
	IDT->type_attr = (1<<7) + (DPL<<6) + type;
}

void init_ints(){
	disable_ints();

	struct desc_table_ptr ptr = {sizeof(IDT) - 1, (uint64_t)IDT};
	write_idtr(&ptr);

	write_serial_string("One\n");

	for (int i = 0; i < INT_COUNT; i++) {
//		if(i!=8 && i!=13)
			init_int_desc(IDT + i, addrs[i], KERNEL_CS, 0, 14, 0);
	}

	write_serial_string("Two\n");


	enable_ints();

	write_serial_string("Three\n");

}

void int_handler(int n){
	write_serial_string("Interrupt handler\n");
	write_serial_char((n / 10) + '0');
	write_serial_char((n % 10) + '0');
	write_serial_char('\n');

	while(1);
}


#endif /*__INTS_H__*/

