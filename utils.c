#include "util.h"
#include <limits.h>

void print_bits(uint64_t n) {
    unsigned long i;
    i = 1UL << (sizeof(n) * CHAR_BIT - 1);
    while (i > 0) {
        if (n & i)
            printf("1");
        else
            printf("0");
        i >>= 1;
    }
}