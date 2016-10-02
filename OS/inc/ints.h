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

void init_int_desc(struct int_desc *IDT, uint64_t offset, uint16_t CS, uint8_t ist, uint8_t type, uint8_t DPL);

void init_ints();

void int_handler(int n);


#endif /*__INTS_H__*/

