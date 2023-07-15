#pragma once

#include <jemalloc/jemalloc.h>
#include <stdio.h>

/**
 * ksrbuffer structure.
 */
typedef struct {
	char *bytes;
	size_t length;
} ksrbuffer;

/**
 * Create a buffer of given size.
 */
ksrbuffer* ksrbuffer_new(const size_t buffer_size);

/**
 * Create a buffer of given size.
 */
ksrbuffer* ksrbuffer_new_empty(const size_t buffer_size);

/**
 * Create a buffer from given size and content.
 */
ksrbuffer* ksrbuffer_new_from_content(const size_t buffer_size, const char *buffer_content);

/**
 * Create a buffer of default buffer size.
 */
static inline ksrbuffer* ksrbuffer_new_default(void)
{
	return ksrbuffer_new(BUFSIZ);
}

/**
 * Free a ksrbuffer and its content.
 */
static inline void ksrbuffer_free(ksrbuffer *buffer)
{
	free(buffer->bytes);
	free(buffer);
}
