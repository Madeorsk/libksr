#include <ksr/regex.h>
#include <regex.h>
#include <string.h>

ksregex* ksregex_new(const char *expression, unsigned nmatches, int flags)
{
	ksregex *regex = malloc(sizeof(ksregex));
	regex->nmatches = nmatches;
	regex->matches = NULL;

	if (regcomp(&regex->expr, expression, // compile expression in regex.
			flags | REG_EXTENDED | (nmatches == 0 ? REG_NOSUB : 0)) != 0)
		return NULL; // an error happened, returning NULL.

	return regex; // return created regex.
}

void ksregex_save_string_matches(ksregex *regex, regmatch_t *raw_matches, const char *source)
{
	// allocate string matches array.
	regex->matches = malloc(sizeof(char *) * regex->nmatches);

	// save full string match.
	regex->matches[0] = strdup(source);

	for(unsigned i = 1; i < regex->nmatches; i++)
	{ // for each group match, converting it to an independent string.
		regoff_t current_match_length = raw_matches[i].rm_eo - raw_matches[i].rm_so;
		regex->matches[i] = malloc(current_match_length + 1); // allocate the current group match string.
		// copying the string part from source to the current match string.
		strncpy(regex->matches[i], &source[raw_matches[i].rm_so], current_match_length);
		regex->matches[i][current_match_length] = 0; // set end of string.
	}
}

bool ksregex_matches(ksregex *regex, const char *tested)
{
	// if there was matches, free them.
	_ksregex_free_matches(regex);

	// allocate raw matches array.
	regmatch_t *raw_matches = malloc(sizeof(regmatch_t) * regex->nmatches);

	if (regexec(&regex->expr, tested, regex->nmatches, raw_matches, 0) != 0)
	{ // an error happened, freeing results then returning NULL.
		free(raw_matches); // free raw groups matches.
		return false;
	}

	if (regex->nmatches > 0)
	{ // there are matches, getting them.
		ksregex_save_string_matches(regex, raw_matches, tested);
	}

	free(raw_matches); // free raw groups matches.

	return true; // no error, return true.
}
