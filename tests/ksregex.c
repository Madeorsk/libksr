#include <assert.h>
#include <ksr/regex.h>
#include <ksr/string.h>

int main(void)
{
	// try a simple regex and check that it matches properly.
	ksregex *reg1 = ksregex_new_nogroup("^[0-9]+$", 0);
	assert(ksregex_matches(reg1, "12") == true);
	assert(reg1->matches == NULL);

	// try a more complex regex with matches groups.
	ksregex *reg2 = ksregex_new("^([0-9])([a-z])$", 3, REG_ICASE);
	assert(ksregex_matches(reg2, "1a"));
	assert(!ksregex_matches(reg2, "H8"));
	assert(!ksregex_matches(reg2, "4MM"));
	assert(!ksregex_matches(reg2, "4_"));
	assert(ksregex_matches(reg2, "3F"));

	// check that groups have been extracted properly.
	assert(str_equals("3F", reg2->matches[0]));
	assert(str_equals("3", reg2->matches[1]));
	assert(str_equals("F", reg2->matches[2]));

	// check that groups are properly extracted even in more complex cases.
	ksregex *reg3 = ksregex_new("^([0-9]+)([a-z]?)$", 3, 0);
	assert(ksregex_matches(reg3, "123545"));
	assert(str_equals("123545", reg3->matches[0]));
	assert(str_equals("123545", reg3->matches[1]));
	assert(str_equals("", reg3->matches[2]));

	// check that matches are empty when matching fail.
	assert(!ksregex_matches(reg3, "123bA"));
	assert(reg3->matches == NULL);

	// try to create an invalid regex.
	assert(ksregex_new_nogroup("^(123$", 0) == NULL);

	// try to free instantiated regex.
	ksregex_free(reg1);
	ksregex_free(reg2);
	ksregex_free(reg3);

	return 0;
}
