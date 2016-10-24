#include <ints.h>
#include <PIC.h>

void init_int_desc(struct int_desc *IDT, uint64_t offset, uint16_t CS, uint8_t ist, uint8_t type, uint8_t DPL){
	IDT->offset_0_15 = offset & 0xffffull;
	IDT->offset_16_31 = (offset & 0xffff0000ull) >> 16;
	IDT->offset_32_63 = offset >> 32;
	IDT->CS = CS;
	IDT->ist = ist;
	IDT->type_attr = (1<<7) + (DPL<<6) + type;
}

void init_ints(){
	struct desc_table_ptr ptr = {sizeof(IDT) - 1, (uint64_t)IDT};
	write_idtr(&ptr);

	for (int i = 0; i < INT_COUNT; i++) {
		init_int_desc(IDT + i, addrs[i], KERNEL_CS, 0, 14, 0);
	}
}

int timer_cnt = 0;

void int_handler(int n){
	if (n == 32){
		timer_cnt++;
		if (timer_cnt == 8){
			//write_serial_string("Timer interrupt.\n");
			timer_cnt = 0;
		}
	}
	else if(n == 0){
		write_serial_string("Division by zero!\n");
	}
	else{
		write_serial_string("Interrupt handler\n");
		write_serial_char((n / 10) + '0');
		write_serial_char((n % 10) + '0');
		write_serial_char('\n');
	}

	if (n >= 32) {
		out8(MASTER_COMMAND_PORT, (1 << 5));
		if (n >= 40) 
			out8(SLAVE_COMMAND_PORT, (1 << 5));
	} 
}