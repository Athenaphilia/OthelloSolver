#ifndef UTILS_H
#define UTILS_H
#include <stdbool.h>
#include <stdint.h>

/**
 * Prints bits of a 64-bit number
 * @param n 64-bit number
 */
void print_bits(uint64_t n);

/**
 * Checks if the first two chars of a string equals a flag
 * @param string string to check
 * @param flag flag to check
 * @return if flag is in the first two of the string
 */
bool test_flag(char *string, char flag[2]);

#endif