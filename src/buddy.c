#include <buddy.h>
#include <stdint.h>
#include <memory.h>
#include <utils.h>
#include <printf.h>
#include <limits.h>

#define BUDDY_FREE 0
#define BUDDY_USED 1

#define LEVELS 30

struct buddy_list {
    uint64_t head;
};

static struct buddy_list lists[LEVELS];
static struct buddy_desc *descs;

uint32_t desc_cnt;

static void list_add(struct buddy_list *list, uint64_t num) {
    if (list->head != ULLONG_MAX) {
        descs[list->head].prev = num;
    }

    descs[num].prev = ULLONG_MAX;
    descs[num].next = list->head;
    list->head = num;
}

static void list_remove(uint64_t num)
{
    int level = descs[num].level;
    if (descs[num].prev != ULLONG_MAX) {
        descs[descs[num].prev].next = descs[num].next; 
    }
    if (descs[num].next != ULLONG_MAX) {
        descs[descs[num].next].prev = descs[num].prev;
    }
    if (lists[level].head == num) {
        lists[level].head = descs[num].next;
    }
}

void buddy_init(struct memmap_entry memmap_table[], uint32_t memmap_size)
{
    printf("Initializing buddy...\n");
    if (memmap_size == 0) {
        halt("There is no memory for buddy.");
    }

    desc_cnt = 0; 
    for (uint32_t i = 0; i < memmap_size; i++) {
        if(memmap_table[i].type != MEMMAP_TYPE_AVAILABLE){
            continue;
        }
        desc_cnt = max(desc_cnt, (memmap_table[i].base_addr + memmap_table[i].length) / PAGE_SIZE);
    }
    uint64_t bytes_needed =  ((uint64_t) sizeof(struct buddy_desc)) * desc_cnt;
    printf("Buddy needs %d descriptors or %llu bytes\n", desc_cnt, bytes_needed);

    int mem_entry = -1; 
    for (uint32_t i = 0; i < memmap_size; i++) {
        if(memmap_table[i].type != MEMMAP_TYPE_AVAILABLE){
            continue;
        }
        if (bytes_needed <= memmap_table[i].length &&
            memmap_table[i].base_addr + bytes_needed <= (((uint64_t) 1) << 32)) {
            mem_entry = i;
            break;
        }    
    }
    if (mem_entry == -1) {
        halt("Not enough memory for buddy.");
    }
    printf("Placing buddy descriptors at 0x%lx...0x%lx\n", 
            memmap_table[mem_entry].base_addr,
            memmap_table[mem_entry].base_addr + bytes_needed);
    descs = (struct buddy_desc *) (memmap_table[mem_entry].base_addr);
    memmap_table[mem_entry].base_addr += bytes_needed;

    for (uint32_t i = 0; i < desc_cnt; i++) {
        descs[i].state = BUDDY_USED;
    }
    for (uint32_t i = 0; i < memmap_size; i++) {
        if(memmap_table[i].type != MEMMAP_TYPE_AVAILABLE){
            continue;
        }
        uint64_t pointer = memmap_table[i].base_addr + PAGE_SIZE - 1;
        pointer -= pointer % PAGE_SIZE;
        while (pointer + PAGE_SIZE <= memmap_table[i].base_addr + memmap_table[i].length) {
            int desc_number = pointer / PAGE_SIZE;
            descs[desc_number].level = 0; 
            descs[desc_number].state = BUDDY_FREE;
            pointer += PAGE_SIZE;
        } 
    }
   
    for (int level = 1; level < LEVELS; level++) {
        for (uint32_t i = 0; i < desc_cnt; i += (1 << level)) {
            uint64_t buddy = i + (1 << (level - 1));
            if (buddy < desc_cnt &&
                descs[i].level == level - 1  && descs[buddy].level == level - 1 &&
                descs[i].state == BUDDY_FREE && descs[buddy].state == BUDDY_FREE) {
                descs[i].level = level;
                descs[buddy].state = BUDDY_USED;               
            }
        }
    }
  
    for (int i = 0; i < LEVELS; i++) {
        lists[i].head = ULLONG_MAX;
    }
    for (int i = desc_cnt - 1; i >= 0; i--) {
        if (descs[i].state == BUDDY_FREE) {
            list_add(lists + descs[i].level, i);           
        }
    }
    printf("Buddy initialized.\n\n");
}

static void buddy_make(int level) {
    if (lists[level].head != ULLONG_MAX) {
        return;
    }
    if (level + 1 == LEVELS) {
        return;
    }
    
    buddy_make(level + 1);

    if (lists[level + 1].head == ULLONG_MAX) {
        return;
    }

    int desc_num = lists[level + 1].head;
    list_remove(desc_num);

    descs[desc_num].level = level;
    descs[desc_num + (1 << level)].state = BUDDY_FREE;
    list_add(lists + level, desc_num);
    list_add(lists + level, desc_num + (1 << level)); 
}

void* buddy_alloc(int level, uint16_t cache_size) {
    buddy_make(level);
    if (lists[level].head == ULLONG_MAX) {
        printf("ERROR: buddy failed to alloc.\n");
        return NOTHING;
    }
    uint64_t result = lists[level].head;
    list_remove(result);

    descs[result].state = BUDDY_USED;
    descs[result].cache_size = cache_size;

    return (void *) (((unsigned long) PAGE_SIZE) * result);
}

static void buddy_add(uint64_t desc_num) {
    int level = descs[desc_num].level;
    
    if (level + 1 != LEVELS) {
        uint64_t buddy_desc_num = desc_num ^ (1 << level);
        if (buddy_desc_num < desc_cnt &&
            descs[buddy_desc_num].state == BUDDY_FREE && descs[buddy_desc_num].level == level) {
            list_remove(buddy_desc_num);

            uint64_t mn = min(desc_num, buddy_desc_num);
            uint64_t mx = max(desc_num, buddy_desc_num);
            descs[mn].level = level + 1;
            descs[mx].state = BUDDY_USED;
            buddy_add(mn);
            return;
        }
    }
    list_add(lists + level, desc_num);
}

struct buddy_desc* buddy_get_desc(void *logical) {
    unsigned long phys_addr = ((unsigned long) logical);
    uint64_t desc_number = phys_addr / PAGE_SIZE;

    return descs + desc_number; 
}

void buddy_free(void *logical) {
    struct buddy_desc *desc = buddy_get_desc(logical);
    desc->state = BUDDY_FREE;
    unsigned long phys_addr = ((unsigned long) logical);
    uint64_t desc_number = phys_addr / PAGE_SIZE; 
    buddy_add(desc_number);
}