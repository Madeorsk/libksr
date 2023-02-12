#include <jemalloc/jemalloc.h>
#include <assert.h>
#include <ksr/errors.h>
#include <stdio.h>
#include <string.h>

ksrerror err_throwsomething(int var)
{
	if (var == 1)
		// we consider that if the value of var is one, there is an error.
		ksrerror_throw(1, "something.");
	
	return ksrerror_none; // no error happened.
}

ksrerror err_propagation(void)
{
	// calling throwsomething and propagating error if there is one.
	ksrerror_propagate(err_throwsomething(1));

	return ksrerror_none; // no error happened.
}

ksresult_of_type(char*) res_returnok(void)
{
	// creating a new result and returning it.
	return ksresult_new("testing something.");
}

ksresult_of_type(char*) res_returnerr(int var)
{
	if (var != 10)
		// we consider that if the value of var is ten, there is an error.
		ksresult_throw(ksrerror_new(6, "test error."));

	// creating a new result and returning it.
	return ksresult_new("another test.");
}

ksresult_of_type(int*) res_propagate_err(void)
{
	int *returnval = malloc(sizeof(int));
	(*returnval) = 5;

	char *test = ksresult_get(test, char*, res_returnerr(*returnval));
	printf("propagation failed? %s\n", test);

	return ksresult_new(returnval);
}

int main(void)
{
	/* *  Testing simple errors  * */


	// testing that ksrerror_none is not an error.
	assert(ksrerror_check(ksrerror_none));

	// testing that an error is correctly thrown.
	assert(ksrerror_check(err_throwsomething(0)));
	assert(!ksrerror_check(err_throwsomething(1)));
	ksrerror_ignore(err_throwsomething(1));

	// testing that propagation works.
	assert(!ksrerror_check(err_propagation()));
	assert(err_propagation().code == 1);
	assert(strcmp(err_propagation().message, "something.") == 0);

	// testing panic when no error happen.
	ksrerror_panic(err_throwsomething(0));


	/* *  Testing ksresults  * */


	// test1: no error, valid return value.
	ksresult res1 = res_returnok();
	assert(ksresult_check(res1)); // checking that there is no error.
	assert(strcmp(ksresult_get_ignore(char*, res1), "testing something.") == 0);


	// test2: error.
	ksresult res2 = res_returnerr(2);
	assert(!ksresult_check(res2)); // checking that there is an error.
	// checking that the error has been correctly passed.
	assert(ksresult_error(res2).code == 6);
	assert(strcmp(ksresult_error(res2).message, "test error.") == 0);
	ksrerror_free(res2.error); // free the thrown error.


	// test3: error propagation.
	ksresult res3 = res_propagate_err();
	assert(!ksresult_check(res3)); // checking that there is an error.
	// checking that the error has been correctly passed.
	assert(ksresult_error(res3).code == 6);
	assert(strcmp(ksresult_error(res3).message, "test error.") == 0);
	ksrerror_free(res3.error); // free the thrown error.


	return 0;
}
