// /** building a http server for deeper understanding of the topic. 
//  *  Using beej guide to netwrok programming book as reference.
//  */

//  #include<string.h>
//  #include<stdlib.h>
//  #include<stdio.h>
//  #include<sys/socket.h>
//  #include<netinet/in.h>
//  #include<netdb.h>
//  #include<unistd.h>
//  #include<errno.h>
//  #include<sys/types.h>
//  #include<fcntl.h>

//  #define MYPORT "8080" // the port users will be connecting to
//  #define BACKLOG 10 // how many pending connections queue will hold
//  #define BUFF_SIZE 4096

//  /***
//   * 
//     * 
//     * 
//     struct addrinfo {
//     int ai_flags; // AI_PASSIVE, AI_CANONNAME, etc.
//     int ai_family; // AF_INET, AF_INET6, AF_UNSPEC
//     int ai_socktype; // SOCK_STREAM, SOCK_DGRAM
//     int ai_protocol; // use 0 for "any"
//     size_t ai_addrlen; // size of ai_addr in bytes
//     struct sockaddr *ai_addr; // struct sockaddr_in or _in6
//     char *ai_canonname; // full canonical hostname
//     struct addrinfo *ai_next; // linked list, next node
// };

//     struct sockaddr_in {
//     short int sin_family; // Address family, AF_INET
//     unsigned short int sin_port; // Port number
//     struct in_addr sin_addr; // Internet address
//     unsigned char sin_zero[8]; // Same size as struct sockaddr
//     };
//   */

//  char* read_data_into_buffer(char *filepath,long *file_size){

//     FILE *ptr = NULL;
//     long f_size = 0;
//     size_t bytes_read = 0;

//     ptr = fopen(filepath,"rb");
//     if(ptr == NULL){
//         perror("Error opening file");
//         return NULL;
//     }
//     /** determine file size */
//     if(fseek(ptr,0,SEEK_END)!=0){
//         perror("Error seeking to end of file");
//         fclose(file_ptr);
//         return NULL;
//     }
//     f_size = ftell(ptr);
//     if(f_size == -1){
//         perror("Error getting file size with ftell");
//         fclose(file_ptr);
//         return NULL;
//     }
//     /** reset ptr to start  */
//     if(fseek(ptr,0,SEEK_SET)!=0){
//          perror("Error seeking to beginning of file");
//         fclose(file_ptr);
//         return NULL;
//     }

//     char *buffer = malloc(f_size);
//     if(buffer == NULL){
//         fprintf(stderr, "Error: Could not allocate memory for file buffer (%ld bytes).\n", file_size);
//         fclose(file_ptr);
//         return NULL;
//     }

//     /** copy content of file into buffer */
//     bytes_read = fread(buffer,1,f_size,ptr);
//     if(bytes_read < (size_t)f_size){
//         if(feof(ptr)){
//              fprintf(stderr, "Error reading file: Unexpected end of file for '%s'. Expected %ld, got %zu.\n", filepath, f_size, bytes_read);
//         }else if(ferror(ptr)){
//              perror("Error reading file");
//         }else{
//             fprintf(stderr, "Error reading file: Unknown error. Expected %ld, got %zu for '%s'.\n", f_size, bytes_read, filepath);
//         }

//         free(buffer);
//         close(ptr);
//         return NULL;
//     }

//     if(fclose(ptr) == EOF){
//         perror("Error closing file");
//     }
    
//     *file_size = f_size;

//     return buffer;

//  }
//  void main(){

//         int status,socket_id;
//         struct addrinfo hints;
//         struct addrinfo *servinfo; // will point to the results
//         struct sockaddr_storage their_addr;
//         socklen_t addr_size;
//         memset(&hints,0,sizeof(hints)); // make sure the struct is empty
//         hints.ai_family = AF_UNSPEC;
//         hints.ai_socktype = SOCK_STREAM;
//         hints.ai_flags = AI_PASSIVE;

//         char *message = "HTTP/1.1 200 OK\r\n"
//                         "Content-Type: text/plain\r\n"
//                         "Content-Length: 12\r\n" // Length of "Hello World!"
//                         "\r\n"                   // Blank line separating headers from body
//                         "Hello World!";
//         int len_message = strlen(message);

//         char buffer[4096];
//         char method[16];
//         char path[256];

//         if((status = getaddrinfo(NULL,MYPORT,&hints,&servinfo)!=0)){
//             fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
//             exit(1);
//         }
        
//         socket_id = socket(servinfo->ai_family, servinfo->ai_socktype,servinfo->ai_protocol);
//         if(socket_id == -1){
//              perror("socket error"); 
//              freeaddrinfo(servinfo); 
//              exit(1);
//         }

//         // int bind_id = (bind(socket_id,servinfo->ai_addr, servinfo->ai_addrlen))
//         if(bind(socket_id,servinfo->ai_addr, servinfo->ai_addrlen)== -1){
//              perror("bind error");
//              freeaddrinfo(servinfo);
//              close(socket_id); // Close the socket
//              exit(1);
//         }

//         if(listen(socket_id,BACKLOG)==-1){
//             perror("listen error");
//             freeaddrinfo(servinfo);
//             close(socket_id);
//             exit(1);
//         }

//         while(1){
//             addr_size = sizeof(their_addr);
//             int new_fd = accept(socket_id,(struct sockaddr *)&their_addr,&addr_size);
//             if(new_fd == -1){
//                 perror("accept error");
//                 // No need to close new_fd as it's invalid
//                 // Consider if you should continue listening or exit the whole server
//                 // For now, let's assume we might want to exit if accept fails,
//                 // but in a real server you might loop.
//                 freeaddrinfo(servinfo);
//                 close(socket_id);
//                 exit(1); // Or handle differently
//             }

//             /** receive some data */
//             int bytes_read = recv(socket_id,buffer,BUFF_SIZE-1,0);
//             if(bytes_read == -1){
//                 perror("cannot read data error");
//                 freeaddrinfo(servinfo);
//                 close(socket_id);
//                 exit(1); // Or handle differently
//             }
//             /** The very first line is the most critical for basic routing. It looks like: GET / HTTP/1.1\r\n or GET /somepage.html HTTP/1.1\r\n. */
//             buffer[BUFF_SIZE ] = '\0';
            
//             // printf("%s", buffer);
//             /** strtok modifies the string, create a copy of string for parsing. */
//             char *request_copy = strdup(buffer);
//             if(!request_copy){
//                 perror("failed creating copy of header request");
//                 freeaddrinfo(servinfo);
//                 close(socket_id);
//                 exit(1);
//             }
//             char *line = strtok(request_copy,"\r\n");
//             if(!line){
//                 free(request_copy);
//                 freeaddrinfo(servinfo);
//                 close(socket_id);
//                 exit(1);

//             }
//             char *method = strtok(line," ");
//             char *path   = strtod(line," ");
            
//             if(!method || !path){
//                 free(request_copy);
//                 freeaddrinfo(servinfo);
//                 close(socket_id);
//                 exit(1);
//             }

//             if(strcmp(method,"GET")==0 && strcmp(path,"/")==0){
//                     const *filepath =  "public/index.html";
//                     long file_len = 0;

//                     char *file_data = read_data_into_buffer(filepath,&file_len);
//                     if(!file_data){
//                         const char *not_found_response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
//                         send(client_fd, not_found_response, strlen(not_found_response), 0);
//                     }else{
//                         char response_headers[512];
//                         int header_len = snprintf(response_headers, sizeof(response_headers),
//                          "HTTP/1.1 200 OK\r\n"
//                          "Content-Type: text/html\r\n"
//                          "Content-Length: %ld\r\n"
//                          "\r\n", // Blank line
//                          file_len);

//                     send(socket_id,response_headers,header_len,0);
//                     send(socket_id,file_data,file_len,0);
//                     free(file_data);
//                     }
                    
//             }

            



//             /** parser the request and find out hhtp method and path */

//             /** send somr data */

//             int bytes_sent = send(new_fd,message,len_message,0);
//             if(bytes_sent == -1){
//                 perror("send error");
//                 // Clean up client and server sockets appropriately
//                 close(new_fd);
//             }
//             if(bytes_sent < len_message){
//                 /** more data needs to be sent */
//             }

        
//             free(request_copy);
//             close(new_fd);

//         }
        
//         close(socket_id);
//         freeaddrinfo(servinfo);
  
//  }