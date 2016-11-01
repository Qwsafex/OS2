#include <stdarg.h>
#include <utils.h>
#include <ints.h>
#include <printf.h>

void halt(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    disable_ints();
    __asm__("hlt");
}


uint64_t min(uint64_t x, uint64_t y) {
    return x < y ? x : y;
}

uint64_t max(uint64_t x, uint64_t y) {
    return x > y ? x : y;
}