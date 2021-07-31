#include <ksr/files.h>
#include <assert.h>

int main(/*int argc, char *argv[]*/)
{
	// delete test directory if it exists.
	system("rm -Rf ksrtesttesttmp");

	// create a new buffer and write it in a file.
	ksrbuffer *buffer = ksrbuffer_new_from_content(12, "Hello World!");
	write_file("ksrtesttesttmp/tmp/test.tmp", buffer);

	// try to read the previously written file, and checking that the buffer has been read properly.
	ksrbuffer *read_buffer = read_file_from_path("test.tmp");
	assert(read_buffer->length == 13);
	assert(read_buffer->bytes[0] == 'H');
	assert(read_buffer->bytes[3] == 'l');
	assert(read_buffer->bytes[7] == 'o');
	assert(read_buffer->bytes[11] == '!');
	assert(read_buffer->bytes[12] == 0);

	// delete test directory.
	system("rm -Rf ksrtesttesttmp");

	// try to write a file where it is not writable.
	assert(write_file("/dev/test/test.tmp", buffer) == false);
	// try to read a non-existing file.
	assert(read_file_from_path("/dev/test.tmp") == NULL);
	// try to read a NULL file.
	assert(read_file(NULL) == NULL);

	return 0;
}
