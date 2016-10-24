#ifndef __MEMMAP_H__
#define __MEMMAP_H__

#include <stdint.h>

#define MEMMAP_TYPE_KERNEL    0
#define MEMMAP_TYPE_AVAILABLE 1
#define MEMMAP_TYPE_RESERVED  2
#define MAX_MEMMAP_TABLE_SIZE 64

struct memmap_entry
{
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
} __attribute__((packed));

extern uint32_t memmap_table_size;
extern struct memmap_entry memmap_table[];
extern uint64_t mem_size;

void read_memmap();
void print_memmap();

#endif
