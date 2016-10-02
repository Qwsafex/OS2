#ifndef __PIC_H__
#define __PIC_H__

#define MASTER_COMMAND_PORT 0x20
#define MASTER_DATA_PORT 0x21
#define SLAVE_COMMAND_PORT 0xa0
#define SLAVE_DATA_PORT 0xa1

void init_PIC();

void mask(int i);
void unmask(int i);
void mask_all();
void unmask_all();

#endif /* __PIC_H__ */