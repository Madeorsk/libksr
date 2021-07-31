#include <ksr/buffers.h>
#include <string.h>

ksrbuffer* ksrbuffer_new(const size_t buffer_size)
{
	// allocate buffer.
	ksrbuffer *buffer = malloc(sizeof(ksrbuffer));

	buffer->length = buffer_size;
	// allocate buffer bytes.
	buffer->bytes = malloc(buffer->length);

	return buffer; // return allocated buffer.
}

ksrbuffer* ksrbuffer_new_empty(const size_t buffer_size)
{
	// create buffer.
	ksrbuffer *buffer = ksrbuffer_new(buffer_size);

	// cleanup allocated memory.
	memset(buffer->bytes, 0, buffer->length);

	return buffer; // return created buffer.
}

ksrbuffer* ksrbuffer_new_from_content(const size_t buffer_size, const char *buffer_content)
{
	// create buffer.
	ksrbuffer *buffer = ksrbuffer_new(buffer_size);

	// cleanup allocated memory.
	memcpy(buffer->bytes, buffer_content, buffer->length);

	return buffer; // return created buffer.
}
