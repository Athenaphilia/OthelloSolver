#include "utils.h"
#include <limits.h>
#include <stdio.h>

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

bool test_flag(char *string, char flag[2]) {
    if (string[0] == flag[0] && string[1] == flag[1]) {
        return true;
    }
    return false;
}

double lerp(double v0, double v1, double t) {
    return (1 - t) * v0 + t * v1;
}