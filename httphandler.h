#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include <stdio.h> // For long

// A struct to hold the parsed request details
typedef struct {
    char *method;
    char *path;
} HttpRequest;

/**
 * @brief Parses the raw HTTP request buffer to extract method and path.
 *
 * @param request_buffer The raw string received from the client.
 * @param req A pointer to an HttpRequest struct to be filled.
 * @return 0 on success, -1 on failure.
 */
int parse_http_request(const char *request_buffer, HttpRequest *req);

/**
 * @brief Frees memory allocated within an HttpRequest struct.
 */
void free_http_request(HttpRequest *req);

/**
 * @brief Sends a 404 Not Found response to the client.
 */
void send_404_response(int client_fd);

/**
 * @brief Constructs and sends a 200 OK response with file content.
 *
 * @param client_fd The client's socket file descriptor.
 * @param content_type The MIME type of the content (e.g., "text/html").
 * @param content A buffer containing the file data.
 * @param content_length The size of the content in bytes.
 */
void send_200_ok_response(int client_fd, const char *content_type, const char *content, long content_length);

#endif // HTTP_HANDLER_H