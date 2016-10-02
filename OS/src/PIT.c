#include <PIT.h>
#include <ioport.h>


void init_PIT(){
	// mode 2 (periodical), setting both counter bytes
	uint8_t command = (1<<4) + (1<<5) + (1<<2); 

	out8(TIMER_COMMAND_PORT, command);
	out8(TIMER_DATA_PORT, 0xff); // low byte
	out8(TIMER_DATA_PORT, 0xff); // high byte
}