#include <ksr/arrays.h>
#include <assert.h>
#include <string.h>

int main(/*int argc, char *argv[]*/)
{
	// try to create arrays.
	ksrarray *array_tiny = ksrarray_new_tiny();
	ksrarray *array_small = ksrarray_new_small();
	ksrarray *array_basic = ksrarray_new_basic();
	ksrarray *array_big = ksrarray_new_big();
	ksrarray *array_huge = ksrarray_new_huge();

	// try to push things in arrays.
	ksrarray_push(array_tiny, "hello");
	ksrarray_push(array_tiny, "world");

	// check that things have been inserted properly.
	assert(strcmp(array_tiny->data[0], "hello") == 0);
	assert(strcmp(array_tiny->data[1], "world") == 0);
	assert(array_tiny->_allocated_size == KSRARRAY_TINY_SIZE);
	assert(array_tiny->length == 2);

	// try to remove something.
	ksrarray_remove(array_tiny, 0);
	// try to remove something that does not exists.
	ksrarray_remove(array_tiny, 3);

	// check that the removal produces a good result.
	assert(strcmp(array_tiny->data[0], "world") == 0);
	assert(array_tiny->_allocated_size == KSRARRAY_TINY_SIZE);
	assert(array_tiny->length == 1);

	// insert more things.
	ksrarray_push(array_tiny, "hello");
	ksrarray_push(array_tiny, "you");
	ksrarray_push(array_tiny, "!");

	// check that the allocated size has increased.
	assert(array_tiny->length == 4);
	assert(array_tiny->_allocated_size == 2*KSRARRAY_TINY_SIZE);

	// check that foreach is working.
	char *element;
	ksrarray_foreach((*array_tiny), element)
	{
		assert(element);
	}

	// free arrays.
	ksrarray_free(array_tiny);
	ksrarray_free(array_small);
	ksrarray_free(array_basic);
	ksrarray_free(array_big);
	ksrarray_free(array_huge);

	return 0;
}
