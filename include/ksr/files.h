#pragma once

#include <ksr/buffers.h>
#include <stdbool.h>
#include <sys/types.h>

/**
 * Expand the path in parameter.
 * If the path expands to multiple paths, return the first one.
 */
char* path_exp_simple(const char* path);

/**
 * Make all directories to the file in argument.
 * @param path - will create the directories for this path.
 * @param mode - permissions of created directories.
 */
int mkpath(const char *path, const mode_t mode);

/**
 * Read the file.
 * @param file - the opened file stream.
 * @return - the buffer of the read file.
 */
ksrbuffer* read_file(FILE *file);
/**
 * Read the file from its path.
 * @param file_path - path of the file to read.
 * @return - the buffer of the read file.
 */
ksrbuffer* read_file_from_path(const char *file_path);

/**
 * Write a buffer into a file.
 * @param file_path - path of the file to read.
 * @return - true if the writing is successful, false otherwise.
 */
bool write_file(const char *file_path, const ksrbuffer *buffer);
