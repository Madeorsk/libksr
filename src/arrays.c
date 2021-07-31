#include <ksr/arrays.h>

ksrarray* ksrarray_new(const size_t start_size)
{
	// allocate array.
	ksrarray *array = malloc(sizeof(ksrarray));

	// initialize array fields.
	array->_allocated_size = start_size;
	array->length = 0;
	// allocate array from start size.
	array->data = malloc(sizeof(void*) * array->_allocated_size);

	return array; // return initialized array.
}

void ksrarray_push(ksrarray *array, void *element)
{
	if (array->length == array->_allocated_size)
	{ // cannot add a new element without allocating more memory.
		array->_allocated_size *= 2; // allocate two times bigger space.
		array->data = realloc(array->data, sizeof(void*) * array->_allocated_size); // reallocate array.
	}

	// push the element at the end of the array.
	array->data[array->length++] = element;
}

bool ksrarray_remove(ksrarray *array, const size_t index)
{
	if (index >= array->length)
		// the index is not associated with a value, cannot delete it.
		return false;

	array->length--; // length is decreasing because we are removing a value.
	for (size_t i = index; i < array->length; i++)
		// moving each value left.
		array->data[i] = array->data[i+1];

	return true;
}
