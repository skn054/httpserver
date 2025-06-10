#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <stdio.h>

/**
 * @brief Reads the entire content of a file into a dynamically allocated buffer.
 *
 * @param filepath The path to the file to be read.
 * @param file_size A pointer to a long where the size of the file will be stored.
 * @return A pointer to the newly allocated buffer containing the file data,
 *         or NULL on failure. The caller is responsible for freeing this buffer.
 */
char* read_file_into_buffer(const char *filepath, long *file_size);

#endif // FILE_HANDLER_H