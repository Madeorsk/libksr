#include "ksr/arrays.h"
#include <ksr/promises.h>
#include <signal.h>
#include <unistd.h>

// global variable indicating if the program has been interrupted or not.
bool interrupted = false;

/**
 * Function launched on signal reception.
 * @param signal - the received signal.
 */
void onsigint(/*int signal*/)
{
	//if (signal == SIGINT)
	// the program is now interrupted.
	interrupted = true;
}

/**
 * Promise thread structure.
 */
typedef struct {
	ksrpromise *promise;
	ksrpromise_execution_f exec;
	void *data;
} promise_thread_data;

/**
 * Promise then user callback.
 */
typedef struct {
	ksrpromise_then_callback_f callback;
	void *userdata;
} promise_then_user_callback;
/**
 * Promise catch user callback.
 */
typedef struct {
	ksrpromise_catch_callback_f callback;
	void *userdata;
} promise_catch_user_callback;

/**
 * The main promise thread function.
 * @param data - thread data.
 */
void* promise_thread(void *data)
{
	// reading thread data.
	promise_thread_data *thread_data = data;

	// executing the promise.
	thread_data->exec(thread_data->promise, thread_data->data);

	return NULL; // this thread has no result.
}

ksrpromise* ksrpromise_new(ksrpromise_execution_f exec, void *data)
{
	// allocate the promise.
	ksrpromise *promise = malloc(sizeof(ksrpromise));
	promise->then_callbacks = ksrarray_new_tiny();
	promise->catch_callbacks = ksrarray_new_tiny();
	promise->successful = false;
	promise->result = NULL;
	promise->error_code = 0;
	promise->error_message = NULL;

	// allocate thread data.
	promise_thread_data *thread_data = malloc(sizeof(promise_thread_data));
	// assign thread data.
	thread_data->promise = promise;
	thread_data->exec = exec;
	thread_data->data = data;

	// initialize and run a new thread.
	pthread_create(&promise->thread, NULL, promise_thread, thread_data);

	return promise; // returning the created promise.
}

/**
 * Run then callbacks for the given promise.
 * @param promise - the promise for which to run then callbacks.
 */
void ksrpromise_run_then_callbacks(ksrpromise *promise)
{
	promise_then_user_callback *user_callback; // initialize user callback pointer.
	for (size_t i = 0; i < promise->then_callbacks->length; i++)
	{ // for each then callback, execute it.
		user_callback = promise->then_callbacks->data[i]; // getting callback.
		(user_callback->callback)(promise->result, user_callback->userdata); // executing callback with promised result.
	}
}

void ksrpromise_resolve(ksrpromise *promise, void *result)
{
	// save success and result.
	promise->successful = true;
	promise->result = result;

	// running then callbacks.
	ksrpromise_run_then_callbacks(promise);
}

/**
 * Run reject callbacks for the given promise.
 * @param promise - the promise for which to run reject callbacks.
 */
void ksrpromise_run_catch_callbacks(ksrpromise *promise)
{
	promise_catch_user_callback *user_callback; // initialize user callback pointer.
	for (size_t i = 0; i < promise->catch_callbacks->length; i++)
	{ // for each catch callback, execute it.
		user_callback = promise->catch_callbacks->data[i]; // getting callback.
		(user_callback->callback)(promise->error_code, promise->error_message, user_callback->userdata); // executing callback with the promise error.
	}
}

void ksrpromise_reject(ksrpromise *promise, int code, const char *message)
{
	// save error details.
	promise->error_code = code;
	promise->error_message = message;

	// running catch callbacks.
	ksrpromise_run_catch_callbacks(promise);
}

void* ksrpromise_await(ksrpromise *promise)
{
	// register signal reception callback.
	signal(SIGINT, onsigint);

	while (!interrupted)
	{ // infinite loop, waiting for promise end or interruption.
		if (!ksrpromise_is_running(promise))
			// promise is not running anymore, returning result if there is one.
			return promise->result;
		
		usleep(KSRPROMISES_AWAIT_SLEEP_DELAY); // sleeping a bit...
	}

	return NULL; // the promise have been interrupted before the result was found.
}

void ksrpromise_then(ksrpromise *promise, ksrpromise_then_callback_f callback, void *userdata)
{
	// allocate user callback.
	promise_then_user_callback *user_callback = malloc(sizeof(promise_then_user_callback));
	user_callback->callback = callback;
	user_callback->userdata = userdata;

	// we use a pointer of the given function pointer as a workaround for generic arrays (see https://stackoverflow.com/a/16682718/7496951).
	ksrarray_push(promise->then_callbacks, user_callback);
}

void ksrpromise_catch(ksrpromise *promise, ksrpromise_catch_callback_f callback, void *userdata)
{
	// allocate user callback.
	promise_catch_user_callback *user_callback = malloc(sizeof(promise_catch_user_callback));
	user_callback->callback = callback;
	user_callback->userdata = userdata;

	// we use a pointer of the given function pointer as a workaround for generic arrays (see https://stackoverflow.com/a/16682718/7496951).
	ksrarray_push(promise->catch_callbacks, user_callback);
}
