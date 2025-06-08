/** building a http server for deeper understanding of the topic. 
 *  Using beej guide to netwrok programming book as reference.
 */

 #include<string.h>
 #include<stdlib.h>
 #include<stdio.h>
 #include<sys/socket.h>
 #include<netinet/in.h>
 #include<netdb.h>
 #include<unistd.h>
 #include<errno.h>
 #include<sys/types.h>
 #include<fcntl.h>

 #define MYPORT "8080" // the port users will be connecting to
 #define BACKLOG 10 // how many pending connections queue will hold

 /***
  * 
    * 
    * 
    struct addrinfo {
    int ai_flags; // AI_PASSIVE, AI_CANONNAME, etc.
    int ai_family; // AF_INET, AF_INET6, AF_UNSPEC
    int ai_socktype; // SOCK_STREAM, SOCK_DGRAM
    int ai_protocol; // use 0 for "any"
    size_t ai_addrlen; // size of ai_addr in bytes
    struct sockaddr *ai_addr; // struct sockaddr_in or _in6
    char *ai_canonname; // full canonical hostname
    struct addrinfo *ai_next; // linked list, next node
};

    struct sockaddr_in {
    short int sin_family; // Address family, AF_INET
    unsigned short int sin_port; // Port number
    struct in_addr sin_addr; // Internet address
    unsigned char sin_zero[8]; // Same size as struct sockaddr
    };
  */
 void main(){

        int status,socket_id;
        struct addrinfo hints;
        struct addrinfo *servinfo; // will point to the results
        struct sockaddr_storage their_addr;
        socklen_t addr_size;
        memset(&hints,0,sizeof(hints)); // make sure the struct is empty
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        char *message = "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/plain\r\n"
                        "Content-Length: 12\r\n" // Length of "Hello World!"
                        "\r\n"                   // Blank line separating headers from body
                        "Hello World!";
        int len_message = strlen(message);

        if((status = getaddrinfo(NULL,MYPORT,&hints,&servinfo)!=0)){
            fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
            exit(1);
        }
        
        socket_id = socket(servinfo->ai_family, servinfo->ai_socktype,servinfo->ai_protocol);
        if(socket_id == -1){
             perror("socket error"); 
             freeaddrinfo(servinfo); 
             exit(1);
        }

        // int bind_id = (bind(socket_id,servinfo->ai_addr, servinfo->ai_addrlen))
        if(bind(socket_id,servinfo->ai_addr, servinfo->ai_addrlen)== -1){
             perror("bind error");
             freeaddrinfo(servinfo);
             close(socket_id); // Close the socket
             exit(1);
        }

        if(listen(socket_id,BACKLOG)==-1){
            perror("listen error");
            freeaddrinfo(servinfo);
            close(socket_id);
            exit(1);
        }

        while(1){
            addr_size = sizeof(their_addr);
            int new_fd = accept(socket_id,(struct sockaddr *)&their_addr,&addr_size);
            if(new_fd == -1){
                perror("accept error");
                // No need to close new_fd as it's invalid
                // Consider if you should continue listening or exit the whole server
                // For now, let's assume we might want to exit if accept fails,
                // but in a real server you might loop.
                freeaddrinfo(servinfo);
                close(socket_id);
                exit(1); // Or handle differently
            }

            /** receive some data */


            /** send somr data */

            int bytes_sent = send(new_fd,message,len_message,0);
            if(bytes_sent == -1){
                perror("send error");
                // Clean up client and server sockets appropriately
                close(new_fd);
            }
            if(bytes_sent < len_message){
                /** more data needs to be sent */
            }

        

            close(new_fd);

        }
        
        close(socket_id);
        freeaddrinfo(servinfo);
  
 }