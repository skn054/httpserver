#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>

#include "httphandler.h"
#include "filehandler.h"

#define PORT "8080"
#define BACKLOG 10
#define BUFFER_SIZE 4096
#define PUBLIC_DIR "./public"

void handle_connection(int client_fd) {
    char buffer[BUFFER_SIZE];
    HttpRequest request = {0}; // Initialize to NULLs

    // Receive the HTTP request
    int bytes_read = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_read <= 0) {
        perror("recv error or client disconnected");
        close(client_fd);
        return;
    }
    buffer[bytes_read] = '\0'; // Null-terminate the received data

    // Parse the HTTP request
    if (parse_http_request(buffer, &request) != 0) {
        // We can send a 400 Bad Request here, but for now, just close
        fprintf(stderr, "Failed to parse request.\n");
        close(client_fd);
        return;
    }

    printf("Received Request: %s %s\n", request.method, request.path);

    // Basic routing
    if (strcmp(request.method, "GET") == 0) {
        char filepath[256];
        if (strcmp(request.path, "/") == 0) {
            // Serve index.html for the root path
            snprintf(filepath, sizeof(filepath), "%s%s", PUBLIC_DIR, request.path);
        } else {
            // Serve other files (with basic security to prevent path traversal)
            if (strstr(request.path, "..")) {
                send_404_response(client_fd); // Don't allow access to parent directories
            } else {
                 snprintf(filepath, sizeof(filepath), "public%s", request.path);
            }
        }

        long file_len = 0;
        char *file_data = read_file_into_buffer(filepath, &file_len);

        if (file_data) {
            // For now, we only serve HTML. A real server would check file extension.
            send_200_ok_response(client_fd, "text/html", file_data, file_len);
            free(file_data);
        } else {
            // File not found
            send_404_response(client_fd);
        }
    } else {
        // Method not supported
        send_404_response(client_fd); // Simple response for non-GET requests
    }

    // Cleanup
    free_http_request(&request);
    close(client_fd);
}

int main(void) {
    int status, listen_fd, new_fd;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr;
    socklen_t addr_size;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return 1;
    }

    // Loop through results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((listen_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        int yes = 1;
        if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(listen_fd, p->ai_addr, p->ai_addrlen) == -1) {
            close(listen_fd);
            perror("server: bind");
            continue;
        }
        break; // if we get here, we must have connected successfully
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        return 2;
    }

    if (listen(listen_fd, BACKLOG) == -1) {
        perror("listen");
        return 3;
    }

    printf("Server listening on port %s...\n", PORT);

    while (1) {
        addr_size = sizeof(their_addr);
        new_fd = accept(listen_fd, (struct sockaddr *)&their_addr, &addr_size);
        if (new_fd == -1) {
            perror("accept");
            continue; // Continue listening for new connections
        }
        
        handle_connection(new_fd);
    }

    close(listen_fd);
    return 0;
}