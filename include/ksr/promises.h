#pragma once

#include <jemalloc/jemalloc.h>
#include <pthread.h>
#include <ksr/arrays.h>

// sleep delay of promise await function, in µs.
#define KSRPROMISES_AWAIT_SLEEP_DELAY 50*1000

/**
 * KSR promise.
 */
typedef struct {
	pthread_t thread;
	ksrarray *then_callbacks; // array of then callbacks.
	ksrarray *catch_callbacks; // array of catch callbacks.
	ksrarray *finally_callbacks; // array of finally callbacks.

	bool successful; // determine if the promise has been fulfilled or not.
	void *result; // the promised result.

	int error_code;
	const char *error_message;
} ksrpromise;

// type of promise execution function.
typedef void (*ksrpromise_execution_f)(ksrpromise *promise, void *userdata);
// type of then callback.
typedef void (*ksrpromise_then_callback_f)(void *result, void *userdata);
// type of catch callback.
typedef void (*ksrpromise_catch_callback_f)(int code, const char *message, void *userdata);
// type of finally callback.
typedef void (*ksrpromise_finally_callback_f)(void *userdata);

/**
 * Create a new promise.
 * @param exec - promise execution function.
 * @param data - custom user data.
 * @return - the new promise.
 */
ksrpromise* ksrpromise_new(ksrpromise_execution_f exec, void *userdata);

/**
 * Resolve the given promise.
 * @param promise - the promise to resolve.
 * @param result - the promised result.
 */
void ksrpromise_resolve(ksrpromise *promise, void *result);

/**
 * Reject the given promise.
 * @param promise - the promise to resolve.
 * @param code - the error code.
 * @param message - the error message.
 */
void ksrpromise_reject(ksrpromise *promise, int code, const char *message);

/**
 * Await a given promise.
 * @param promise - the promise to await.
 * @return - the promised result.
 */
void* ksrpromise_await(ksrpromise *promise);

/**
 * Set a new callback to call on promise resolution.
 * @param promise - the promise for which to set a resolution callback.
 * @param callback - the callback to set.
 * @param userdata - custom data to passed to the callback.
 */
void ksrpromise_then(ksrpromise *promise, ksrpromise_then_callback_f, void *userdata);

/**
 * Set a new callback to call on promise rejection.
 * @param promise - the promise for which to set a rejection callback.
 * @param callback - a callback to set.
 * @param userdata - custom data to passed to the callback.
 */
void ksrpromise_catch(ksrpromise *promise, ksrpromise_catch_callback_f, void *userdata);

/**
 * Set a new callback to call on promise resolution or rejection.
 * @param promise - the promise for which to set a finally callback.
 * @param callback - a callback to set.
 * @param userdata - custom data to passed to the callback.
 */
void ksrpromise_finally(ksrpromise *promise, ksrpromise_finally_callback_f, void *userdata);

/**
 * Determine if the given promise is successful or not.
 * @param - the promise to check.
 */
static inline bool ksrpromise_is_successful(const ksrpromise *promise)
{
	return promise->successful;
}
/**
 * Determine if the given promise is failed or not.
 * @param - the promise to check.
 */
static inline bool ksrpromise_is_failed(const ksrpromise *promise)
{
	return !!promise->error_code;
}
/**
 * Determine if the given promise is running or not.
 * @param - the promise to check.
 */
static inline bool ksrpromise_is_running(const ksrpromise *promise)
{
	return !ksrpromise_is_successful(promise) && !ksrpromise_is_failed(promise);
}

/**
 * Free the promise.
 * /!\ The promise is freed automatically on rejection or resolution.
 * @param promise - the promise to free.
 */
static inline void ksrpromise_free(ksrpromise *promise)
{
	// free arrays of callbacks.
	ksrarray_free(promise->then_callbacks);
	ksrarray_free(promise->catch_callbacks);
	
	// free promise data.
	free(promise);
}
