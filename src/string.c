#include <ksr/string.h>
#include <ctype.h>

char* to_lowercase(const char* src)
{
	// duplicate the string.
	char *lowercased = strdup(src);
	char *ret = lowercased; // store the first pointer to return it at the end of the conversion.
	
	for (;*lowercased; lowercased++)
		// for each character, convert it to lowercase.
		*lowercased = tolower(*lowercased);

	return ret; // returning the first string pointer.
}
