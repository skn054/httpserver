#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "httphandler.h"

/** @return 0 on success, -1 on failure. */
int parse_http_request(const char *request_buffer, HttpRequest *req){

        char *request_copy = strdup(request_buffer);
         if(!request_copy){
             perror("strdup failed");
            return -1;
        }

         char *line = strtok(request_copy,"\r\n");
        if(!line){
            fprintf(stderr, "Invalid HTTP request: No request line.\n");
            free(request_copy);
            return -1;
        }
        char *method = strtok(line," ");
        char *path   = strtok(NULL," ");

        if(!method || !path){
            fprintf(stderr, "Invalid HTTP request: Malformed request line.\n");
            free(request_copy);
            return -1;
        }

        req->method = strdup(method);
        req->path = strdup(path);

        if (strcmp(req->path, "/") == 0) {
        strcpy(req->path, "/index.html");
    }

        if (!req->method || !req->path) {
        perror("strdup failed for request fields");
        free(req->method); // free what might have been allocated
        free(req->path);
        free(request_copy);
        return -1;
    }

    free(request_copy);
    return 0;

        

}

void free_http_request(HttpRequest *req){

    if(req){
        free(req->method);
        free(req->path);
    }
}

void send_404_response(int client_fd){
    const char *not_found_response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
    send(client_fd, not_found_response, strlen(not_found_response), 0);
}

void send_200_ok_response(int client_fd, const char *content_type, const char *content, long content_length){
     char response_headers[512];
     int header_len = snprintf(response_headers, sizeof(response_headers),
                        "HTTP/1.1 200 OK\r\n"
                         "Content-Type: %s\r\n"
                         "Content-Length: %ld\r\n"
                         "\r\n", // Blank line
                         content_type,content_length);

      send(client_fd,response_headers,header_len,0);
      send(client_fd,content,content_length,0);
                    
}

