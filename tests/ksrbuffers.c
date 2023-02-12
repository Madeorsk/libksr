#include <assert.h>
#include <ksr/buffers.h>

int main(void)
{
	// create an empty buffer and check that it is properly instantiated.
	ksrbuffer *empty_buffer = ksrbuffer_new_empty(30);
	assert(empty_buffer->bytes[0] == 0);
	assert(empty_buffer->bytes[10] == 0);
	assert(empty_buffer->bytes[20] == 0);
	assert(empty_buffer->bytes[29] == 0);

	// create a buffer and set some bytes.
	ksrbuffer *buffer = ksrbuffer_new(20);

	buffer->bytes[0] = 12;
	buffer->bytes[5] = 3;
	buffer->bytes[10] = 44;
	buffer->bytes[15] = 54;
	buffer->bytes[19] = 8;

	// create a buffer from existing content, taking raw content from the previous buffer.
	ksrbuffer *copied_buffer = ksrbuffer_new_from_content(buffer->length, buffer->bytes);

	// checking that content has been properly copied.
	assert(buffer->bytes[0] == copied_buffer->bytes[0]);
	assert(buffer->bytes[5] == copied_buffer->bytes[5]);
	assert(buffer->bytes[10] == copied_buffer->bytes[10]);
	assert(buffer->bytes[15] == copied_buffer->bytes[15]);
	assert(buffer->bytes[19] == copied_buffer->bytes[19]);

	assert(copied_buffer->bytes[0] == 12);
	assert(copied_buffer->bytes[5] == 3);
	assert(copied_buffer->bytes[10] == 44);
	assert(copied_buffer->bytes[15] == 54);
	assert(copied_buffer->bytes[19] == 8);

	// try to create a default buffer.
	ksrbuffer *default_buffer = ksrbuffer_new_default();
	assert(default_buffer->length == BUFSIZ);

	// free buffers.
	ksrbuffer_free(empty_buffer);
	ksrbuffer_free(buffer);
	ksrbuffer_free(copied_buffer);
	ksrbuffer_free(default_buffer);

	return 0;
}
