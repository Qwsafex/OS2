#include "memmap.h"
#include "printf.h"

uint32_t memmap_table_size = 0;
struct memmap_entry memmap_table[MAX_MEMMAP_TABLE_SIZE];
uint64_t mem_size;

extern char text_phys_begin[];
extern char bss_phys_end[];
extern uint64_t multiboot_info;

void read_memmap() {
    struct memmap_entry kernel_entry;
    kernel_entry.base_addr = (uint64_t) text_phys_begin; 
    kernel_entry.length = (uint64_t) bss_phys_end - kernel_entry.base_addr; 
    kernel_entry.type = 0;
    memmap_table[memmap_table_size++] = kernel_entry;

    uint32_t memmap_size = *((uint32_t*) ((char*)multiboot_info + 44));
    char *memmap_addr = *((char**) ((char*)multiboot_info + 48));
    char *memmap_end = memmap_addr + memmap_size;
    while (memmap_addr < memmap_end) {
        uint32_t size = *((uint32_t*) memmap_addr);
        memmap_addr += sizeof(size);
        struct memmap_entry cur_entry;
        cur_entry = *((struct memmap_entry*) memmap_addr);
        memmap_addr += size;

        uint64_t kernel_start = kernel_entry.base_addr;
        uint64_t kernel_end = kernel_start + kernel_entry.length;
        uint64_t cur_start = cur_entry.base_addr;
        uint64_t cur_end = cur_start + cur_entry.length;

        // intersection with kernel
        if(cur_start < kernel_end && kernel_start < cur_end){
            // there is space after
            if(cur_end > kernel_end){
                struct memmap_entry after_entry;
                after_entry.base_addr = kernel_end;
                after_entry.length = cur_end - kernel_end;
                after_entry.type = cur_entry.type;
                memmap_table[memmap_table_size++] = after_entry;
            }

            // no space before
            if (kernel_start <= cur_start){
                continue;
            }
            cur_entry.length = kernel_start - cur_start;
        }

        memmap_table[memmap_table_size++] = cur_entry;
    }

    for (uint32_t i = 0; i < memmap_table_size; i++) {
        if (mem_size < memmap_table[i].base_addr + memmap_table[i].length){
            mem_size = memmap_table[i].base_addr + memmap_table[i].length;
        }
    }
}

void print_memmap() {
    for (uint32_t i = 0; i < memmap_table_size; i++) {
        char type [10];
        switch(memmap_table[i].type){
            case MEMMAP_TYPE_KERNEL: strcpy(type, "kernel"); break;
            case MEMMAP_TYPE_AVAILABLE: strcpy(type, "available"); break;
            case MEMMAP_TYPE_RESERVED: strcpy(type, "reserved"); break;
        }
        printf("memory range %d: %llx - %llx; type=%s; length=%llu bytes\n", i, memmap_table[i].base_addr, 
               memmap_table[i].base_addr + memmap_table[i].length - 1, type, memmap_table[i].length);
    }
}
