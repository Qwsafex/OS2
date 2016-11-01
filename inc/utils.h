#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdint.h>

#define NOTHING ((void *) -1)

void halt(const char* message, ...);

uint64_t min(uint64_t x, uint64_t y);
uint64_t max(uint64_t x, uint64_t y);

#endif /* UTILS_H */