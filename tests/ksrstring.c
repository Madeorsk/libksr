#include "ksr/string.h"
#include <assert.h>
#include <ksr/buffers.h>

int main(/*int argc, char *argv[]*/)
{
	assert(str_equals("hello", to_lowercase("heLLo")));
	assert(str_equals("hello", to_lowercase("HELLO")));
	assert(str_equals("hello", to_lowercase("Hello")));
	assert(str_equals("hello", to_lowercase("hellO")));

	assert(!str_equals("heLLo", to_lowercase("heLLo")));

	return 0;
}
