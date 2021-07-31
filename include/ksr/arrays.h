#pragma once

#include <jemalloc/jemalloc.h>
#include <stddef.h>

/*
 *  KSRARRAY DEFAULT SIZES.
 */

#define KSRARRAY_TINY_SIZE 2
#define KSRARRAY_SMALL_SIZE 8
#define KSRARRAY_BASIC_SIZE 16
#define KSRARRAY_BIG_SIZE 64
#define KSRARRAY_HUGE_SIZE 128

/**
 * ksrarray structure.
 */
typedef struct {
	void **data; // array data.
	size_t length; // currently used size.

	size_t _allocated_size; // currently allocated size.
} ksrarray;

/**
 * Create a ksrarray with the given start size.
 * @param start_size - the allocated size at start.
 */
ksrarray* ksrarray_new(const size_t start_size);
/**
 * Create a tiny sized ksrarray.
 */
static inline ksrarray* ksrarray_new_tiny()
{ return ksrarray_new(KSRARRAY_TINY_SIZE); }
/**
 * Create a small sized ksrarray.
 */
static inline ksrarray* ksrarray_new_small()
{ return ksrarray_new(KSRARRAY_SMALL_SIZE); }
/**
 * Create a normal sized ksrarray.
 */
static inline ksrarray* ksrarray_new_basic()
{ return ksrarray_new(KSRARRAY_BASIC_SIZE); }
/**
 * Create a big sized ksrarray.
 */
static inline ksrarray* ksrarray_new_big()
{ return ksrarray_new(KSRARRAY_BIG_SIZE); }
/**
 * Create a huge sized ksrarray.
 */
static inline ksrarray* ksrarray_new_huge()
{ return ksrarray_new(KSRARRAY_HUGE_SIZE); }

/**
 * Push a new element at the end of a ksrarray.
 * @param array - the ksrarray in which to push.
 * @param element - the element to push.
 */
void ksrarray_push(ksrarray *array, void *element);

/**
 * Remove an element from a ksrarray.
 * @param array - the ksrarray in which to remove.
 * @param index - the index of the element to remove.
 */
bool ksrarray_remove(ksrarray *array, const size_t index);

/**
 * Free a ksrarray.
 * /!\ This will NOT free the elements, just the container and its internal data.
 */
static inline void ksrarray_free(ksrarray *array)
{
	free(array->data);
	free(array);
}

/**
 * Get the iterator variable name, used in ksrarray_foreach.
 */
#define _ksrarray_get_iter_name(a, b) b ## _iter
/**
 * For each element of the given ksrarray, do...
 */
#define ksrarray_foreach(ksrarray, element) \
        for(size_t _ksrarray_get_iter_name(ksrarray, element) = 0; \
        	(element = ksrarray.data[_ksrarray_get_iter_name(ksrarray, element)]) || true, \
        		_ksrarray_get_iter_name(ksrarray, element) < ksrarray.length; \
        	_ksrarray_get_iter_name(ksrarray, element)++)
