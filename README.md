# Basic C HTTP Server

A simple HTTP/1.1 server written in C from scratch. The primary goal of this project is to gain a deeper understanding of network programming, socket APIs, and the HTTP protocol.

This server is being developed incrementally.

## Current Status (Initial Setup)

*   Sets up a TCP listening socket on a predefined port.
*   Accepts incoming client connections one at a time.
*   Sends a hardcoded, minimal HTTP/1.1 "200 OK" response with a "Hello World!" body to any request.
*   Closes the client connection after sending the response.
*   Currently handles one connection and then (depending on implementation) either exits or loops to accept the next.

## Phase 1 Planned Features

The following features are planned for the initial, single-threaded version of the server:

*   **[ ] GET Request Support:** Handle HTTP GET requests.
*   **[ ] Static File Serving:** Serve HTML, CSS, JS, and image files from a `public/` directory.
*   **[ ] Basic Routing:**
    *   **[ ]** Route `/` to `public/index.html` automatically.
*   **[ ] Error Handling:**
    *   **[ ]** Custom 404 Not Found page (`public/404.html`).
    *   **[ ]** Custom 500 Internal Server Error page (`public/500.html`).
*   **[ ] Request Logging:**
    *   **[ ]** Basic console output for requests (e.g., `METHOD /path HTTP/version - STATUS`).
    *   **[ ]** (Optional) Add color to console logs.
    *   **[ ]** (Optional) Log to a file.
*   **[ ] Security Features:**
    *   **[ ]** Basic directory traversal protection.
*   **[ ] Configurable Port:**
    *   **[ ]** Allow port specification via command-line argument.
*   **[ ] Graceful Shutdown:**
    *   **[ ]** Handle `SIGINT` (Ctrl+C) and `SIGTERM` signals for clean shutdown.

*(Checkboxes `[ ]` can be marked `[x]` as features are completed.)*

## Prerequisites

*   A C compiler (e.g., GCC, Clang)
*   Standard C libraries (typically included with the compiler)


## Building

To compile the server (assuming the source file is `httpserver.c`):

```bash
gcc httpserver.c -o httpserver 

## Running 

./httpserver
By default, the server will attempt to listen on port 8080

##Testing

Once the server is running, you can test it using:
Web Browser:
Open your browser and navigate to http://localhost:PORT (e.g., http://localhost:8080).