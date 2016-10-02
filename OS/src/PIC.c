#include <PIC.h>
#include <ints.h>

int master_mask = 0;
int slave_mask = 0;

void init_PIC(){
	uint8_t command = (1<<4) + 1;

	// telling PIC that we want to initialize it
	out8(MASTER_COMMAND_PORT, command); 
	out8(SLAVE_COMMAND_PORT, command);

	// mapping inputs to interrupts
	out8(MASTER_DATA_PORT, 32);
	out8(SLAVE_DATA_PORT, 40);

	// configuring cascade
	out8(MASTER_DATA_PORT, 1<<2); // mask of slaves
	out8(SLAVE_DATA_PORT, 2); // numper of input in master

	// some magic
	out8(MASTER_DATA_PORT, 1);
	out8(SLAVE_DATA_PORT, 1);
	mask_all();
}

void apply_mask(){
	out8(MASTER_DATA_PORT, master_mask);
	out8(SLAVE_DATA_PORT, slave_mask);
}

void mask(int i){
	if (i < 8)
		master_mask |= 1<<i;
	else
		slave_mask |= 1<<(i-8);
	apply_mask();
}
void unmask(int i){
	if (i < 8)
		master_mask &= 255 - (1<<i);
	else
		slave_mask &= 255 - (1<<(i-8));
	apply_mask();
}
void mask_all(){
	master_mask = 255 - 4;
	slave_mask = 255;
	apply_mask();
}
void unmask_all(){
	master_mask = 0;
	slave_mask = 0;
	apply_mask();
}