#pragma once

#include <jemalloc/jemalloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ksr/logging.h>

/**
 * Type of an error code.
 */
typedef int ksrerrorcode_t;

/**
 * Structure of an error.
 */
typedef struct {
	ksrerrorcode_t code;
	char *message;
	void *data;
} ksrerror;

#define KSRERROR_NOERROR_CODE 0

/**
 * An error struct that indicates no error.
 */
#define ksrerror_none ((ksrerror) { KSRERROR_NOERROR_CODE, NULL, NULL })
/**
 * Create a new error structure with given code, message and custom error data.
 */
#define ksrerror_new_d(error_code, error_message, error_data) ((ksrerror) { error_code, strdup(error_message), error_data })
/**
 * Create a new error structure with given code and message.
 */
#define ksrerror_new(error_code, error_message) ksrerror_new_d(error_code, error_message, NULL)

/**
 * Throw a new error structure with given code, message and custom error data.
 */
#define ksrerror_throw_d(error_code, error_message, error_data) return ksrerror_new_d(error_code, error_message, error_data)
/**
 * Throw a new error structure with given code and message.
 */
#define ksrerror_throw(error_code, error_message) return ksrerror_new(error_code, error_message)

/**
 * Free an error and its custom data, with the given free function.
 */
static inline void ksrerror_free_d(ksrerror error, void (*free_data_func)(void*))
{
	if (error.message) free(error.message);
	if (error.data) free_data_func(error.data);
}
/**
 * Free an error without custom data.
 */
static inline void ksrerror_free(ksrerror error)
{
	ksrerror_free_d(error, free);
}

/**
 * Ignore an error and free its custom data.
 */
#define ksrerror_ignore_d(error, free_data_func) ksrerror_free_d(error, free_data_func)
/**
 * Ignore an error without custom data.
 */
#define ksrerror_ignore(error) ksrerror_free(error)

/**
 * Check if the structure indicates an error.
 * Return true if there is no error, false if there is one.
 */
static inline bool ksrerror_check(ksrerror error)
{
	return error.code == KSRERROR_NOERROR_CODE;
}

/**
 * Propagate the error, if there is one.
 */
#define ksrerror_propagate(error) { ksrerror __ksrerror__cache__ = error; if (!ksrerror_check(__ksrerror__cache__)) return __ksrerror__cache__; }

/**
 * Panic! Show error message and exit app.
 */
#define ksrerror_panic(error) { \
	ksrerror __ksrerror__cache__ = error; \
	if (__ksrerror__cache__.code != KSRERROR_NOERROR_CODE) { \
	char *__ksrerror__error_message__ = malloc(5 + 64 + strlen(__ksrerror__cache__.message) + 1); \
	sprintf(__ksrerror__error_message__, "code %d: %s", __ksrerror__cache__.code, __ksrerror__cache__.message); \
	ksrlog_error(__ksrerror__error_message__); exit(__ksrerror__cache__.code); \
} }



/**
 * Structure of a result that can be an error.
 * This structure can be used as a return type for functions that can encounter an error.
 */
typedef struct {
	void *result;
	ksrerror error;
} ksresult;

/**
 * Define a result of a specific type.
 */
#define ksresult_of_type(result_type) ksresult
/**
 * Create a new result.
 */
#define ksresult_new(_result) ((ksresult) { _result, ksrerror_none })
/**
 * Create a result that contains an error.
 */
#define ksresult_new_error(error) ((ksresult) { NULL, (error) })
/**
 * Throw an error as a result.
 */
#define ksresult_throw(error) return ksresult_new_error(error)

/**
 * Try to get a result and put it in the variable called varname.
 * You must specify the type of the result you want to get.
 * If there is an error, it is thrown as a result. You cannot use this function if your function does not return a ksresult.
 */
#define ksresult_get(varname, result_type, _result) NULL; { \
	ksresult __ksresult_cache__ = (_result); \
	if (!ksresult_check(__ksresult_cache__)) return __ksresult_cache__; \
	varname = ((result_type) __ksresult_cache__.result); \
}
/**
 * Try to get a result while ignoring the error if there is one.
 * You must specify the type of the result you want to get.
 * Directly return the result data.
 */
#define ksresult_get_ignore(result_type, _result) ((result_type) (_result).result)
/**
 * Get the error of a result.
 */
#define ksresult_error(_result) (_result).error
/**
 * Check if there is an error in the given result.
 */
#define ksresult_check(_result) ksrerror_check(ksresult_error(_result))


