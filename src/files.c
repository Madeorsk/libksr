#include "ksr/buffers.h"
#include <ksr/files.h>

#include <string.h>
#include <wordexp.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/stat.h>
#include <errno.h>

char* path_exp_simple(const char* path)
{
	// expanding the path.
	wordexp_t exp;
	wordexp(path, &exp, 0);
	// copying the first expanded path.
	char *expanded_path = strdup(exp.we_wordv[0]);
	wordfree(&exp); // freeing the path expansion.

	return expanded_path; // returning the first expanded path.
}

int mkpath(const char *path, const mode_t mode)
{
	// getting the current directory path to create.
	char *fullpath = strdup(path);
	char *dirpath = dirname(fullpath);

	int res; // result of mkpath / mkdir.
	DIR *dir = opendir(dirpath); // trying to open the directory, to see if it exists.
	if (dir) // the parent directory exists, closing it.
		closedir(dir);
	else
	{ // the parent directory does not exist, creating it recursively.
		res = mkpath(dirpath, mode);
		if (res != 0) goto err; // returning the error instantly if there is one.
	}

	// creating the current directory.
	res = mkdir(dirpath, mode);
	if (errno == EEXIST)
		// the directory exists, everything have been created correctly.
		res = 0;

	err:
	free(fullpath);
	return res; // if an error happened while creating, returning it.
}

ksrbuffer* read_file(FILE *file)
{
	if (file)
	{ // the file stream is open.
		// searching the end of the file.
		fseek(file, 0, SEEK_END);
		long filesize = ftell(file); // getting file size.
		// getting back to the beginning of the file.
		fseek(file, 0, SEEK_SET);

		// allocating a buffer corresponding to the file size, plus the end of string char.
		ksrbuffer *buffer = ksrbuffer_new(filesize + 1);

		// reading the file into the buffer.
		fread(buffer->bytes, 1, buffer->length, file);
		buffer->bytes[buffer->length - 1] = 0; // 0-terminated string.

		return buffer; // returning allocated buffer.
	}
	else // the file is not readable, just returning NULL as we cannot read it.
		return NULL;
}
ksrbuffer* read_file_from_path(const char *file_path)
{
	// expanding the file path.
	char *fullpath = path_exp_simple(file_path);

	// opening file.
	FILE *file = fopen(fullpath, "r");
	
	free(fullpath); // freeing used full path.

	if (file)
	{ // the file is readable.
		// reading file stream.
		ksrbuffer *buffer = read_file(file);
		fclose(file); // closing the read file.

		return buffer; // returning allocated buffer.
	}
	else // the file is not readable, just returning NULL as we cannot read it.
		return NULL;
}

bool write_file(const char *file_path, const ksrbuffer *buffer)
{
	// expanding the file path.
	char *fullpath = path_exp_simple(file_path);

	// creating directories if it is necessary.
	mkpath(fullpath, 0755);
	// opening file.
	FILE *file = fopen(fullpath, "w");

	free(fullpath); // free full path to file.

	if (file)
	{ // the file is writable.
		// write buffer bytes to the file.
		fwrite(buffer->bytes, 1, buffer->length, file);
		fclose(file); // closing file stream.
		return true; // returning true because the file have been written.
	}
	else
		return false; // returning false because the file could not be written.
}
