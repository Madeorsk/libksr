#pragma once

#include <jemalloc/jemalloc.h>
#include <regex.h>

/**
 * Regex structure.
 */
typedef struct {
	regex_t expr;
	unsigned nmatches;
	char **matches;
} ksregex;

/**
 * Create a ksregex from an expression.
 * @param expression - the expression to compile.
 * @param nmatches - the number of matches groups to get, 0 if you want none.
 * @param flags - regex flags.
 * @return - the created regex, NULL if an error happen.
 */
ksregex* ksregex_new(const char *expression, unsigned nmatches, int flags);
/**
 * Create a ksregex from an expression with 0 match group.
 * @param expression - the expression to compile.
 * @param flags - regex flags.
 */
static inline ksregex* ksregex_new_nogroup(const char *expression, int flags)
{ return ksregex_new(expression, 0, flags); }

/**
 * Try to match the regex with a tested string.
 * @param regex - the regex to match.
 * @param tested - the string that should match the regex.
 * @return - true if the string matches, false otherwise.
 */
bool ksregex_matches(ksregex *regex, const char *tested);

/**
 * Free ksregex matches.
 */
static inline void _ksregex_free_matches(ksregex *regex)
{
	if (regex->matches)
	{
		// free all matches.
		for (unsigned i = 0; i < regex->nmatches; i++) free(regex->matches[i]);
		// free matches array.
		free(regex->matches);
		regex->matches = NULL;
	}
}

/**
 * Free a ksregex.
 */
static inline void ksregex_free(ksregex *regex)
{
	// free matches groups.
	_ksregex_free_matches(regex);

	free(regex); // free regex data.
}
