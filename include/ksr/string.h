#pragma once

#include <string.h>
#include <stdbool.h>

/**
 * Check if two strings are equal.
 * @param a - the first string to check.
 * @param b - the second string to check.
 */
static inline bool str_equals(const char *a, const char *b)
{ return 0 == strcmp(a, b); }

/**
 * Convert a string to lowercase.
 */
char* to_lowercase(const char* str);
