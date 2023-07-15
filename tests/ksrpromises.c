#include <assert.h>
#include <ksr/promises.h>
#include <ksr/string.h>
#include <unistd.h>
#include <signal.h>

void promise_execution_ok(ksrpromise *promise, void *data)
{
	// checking that user data has been properly transfered.
	assert(data == NULL);

	sleep(1);
	// try to resolve the promise.
	ksrpromise_resolve(promise, "OK");
}

void promise_execution_sigint(ksrpromise *promise, void *data)
{
	// raise a signal to interrupt await.
	raise(SIGINT);
}

void promise_execution_error(ksrpromise *promise, void *data)
{
	// try to read the error details from user data ; the content will be checked in the catch callback.
	const char* error = data;
	sleep(1);
	// try to reject the promise with an error.
	ksrpromise_reject(promise, 124, error);
}

void on_promise_ok(void *result, void *userdata)
{
	// checking that result and user data have been properly transfered.
	assert(str_equals("OK", result));
	assert(userdata == NULL);
}

void on_promise_failed(int error_code, const char *error_message, void *userdata)
{
	// checking that error details and user data have been properly transfered.
	assert(error_code == 124);
	assert(str_equals("hello", error_message));
	assert(str_equals("blabla", userdata));
}

void on_promise_finished(void *userdata)
{
	// checking that user data have been properly transfered.
	assert(str_equals("test_finally", userdata));
}

int main(void)
{
	// try to create a promise.
	ksrpromise *promise = ksrpromise_new(promise_execution_ok, NULL);
	// checking that then is properly launched.
	ksrpromise_then(promise, on_promise_ok, NULL);
	// checking that finally is properly launched.
	ksrpromise_finally(promise, on_promise_finished, "test_finally");
	// checking that await is working properly on a successful promise.
	assert(str_equals("OK", ksrpromise_await(promise)));

	// try to create a promise which fails.
	ksrpromise *promise_fail = ksrpromise_new(promise_execution_error, "hello");
	// checking that catch is properly launched.
	ksrpromise_catch(promise_fail, on_promise_failed, "blabla");
	// checking that finally is properly launched.
	ksrpromise_finally(promise, on_promise_finished, "test_finally");
	// checking that await is working properly on a promise which fails.
	assert(ksrpromise_await(promise_fail) == NULL);

	// try to create a promise that will be interrupted.
	ksrpromise *interrupted_promise = ksrpromise_new(promise_execution_sigint, NULL);
	// checking that interruption is handled properly in an await.
	assert(NULL == ksrpromise_await(interrupted_promise));

	return 0;
}
