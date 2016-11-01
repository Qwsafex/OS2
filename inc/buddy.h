#ifndef __BUDDY_ALLOCATOR_H__
#define __BUDDY_ALLOCATOR_H__

#include <memmap.h>
#include <stdint.h>

#define NOT_FOR_CACHE 0

struct buddy_desc
{
    uint8_t level;
    uint8_t state;
    uint16_t cache_size;
    uint64_t prev;
    uint64_t next;
};

void buddy_init(struct memmap_entry memmap_table[], uint32_t memmap_size);
void *buddy_alloc(int level, uint16_t cache_size);
void buddy_free(void *logical);
struct buddy_desc *buddy_get_desc(void *logical);

#endif /* __BUDDY_ALLOCATOR_H__ */