#include "filehandler.h"
#include <stdlib.h>
#include <stdio.h>

char* read_file_into_buffer(const char *filepath, long *file_size) {
    FILE *ptr = NULL;
    long f_size = 0;
    char *buffer = NULL;
    size_t bytes_read = 0;

    printf("file path is : %s",filepath);

    ptr = fopen(filepath, "rb");
    if (ptr == NULL) {
        perror("Error opening file");
        return NULL;
    }

    // Determine file size
    if (fseek(ptr, 0, SEEK_END) != 0) {
        perror("Error seeking to end of file");
        fclose(ptr);
        return NULL;
    }
    f_size = ftell(ptr);
    if (f_size == -1) {
        perror("Error getting file size with ftell");
        fclose(ptr);
        return NULL;
    }

    // Reset file pointer to the beginning
    if (fseek(ptr, 0, SEEK_SET) != 0) {
        perror("Error seeking to beginning of file");
        fclose(ptr);
        return NULL;
    }

    buffer = malloc(f_size);
    if (buffer == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for file buffer (%ld bytes).\n", f_size);
        fclose(ptr);
        return NULL;
    }

    // Copy the content of the file into the buffer
    bytes_read = fread(buffer, 1, f_size, ptr);
    if (bytes_read < (size_t)f_size) {
        if (feof(ptr)) {
            fprintf(stderr, "Error reading file: Unexpected end of file for '%s'. Expected %ld, got %zu.\n", filepath, f_size, bytes_read);
        } else if (ferror(ptr)) {
            perror("Error reading file");
        } else {
            fprintf(stderr, "Error reading file: Unknown error. Expected %ld, got %zu for '%s'.\n", f_size, bytes_read, filepath);
        }
        free(buffer);
        fclose(ptr);
        return NULL;
    }

    fclose(ptr);

    *file_size = f_size;
    return buffer;
}