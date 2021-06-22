#define _XOPEN_SOURCE 600 // From https://stackoverflow.com/questions/7571870/netdb-h-not-linking-properly
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MYPORT "3490"
#define BACKLOG 10


int showIps(int argc, char *argv[]);
void connect_func(int argc, char *argv[]);
void bind_func(int argc, char *argv[]);
void listen_func(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    // return showIps(argc, argv);

    // - Getting socket
    int s;
    struct addrinfo hints, *res;

    getaddrinfo("www.example.com", "http", &hints, &res);

    s = socket(res->ai_family, res->ai_socktype, res->ai_protocol); // returns a socket descriptor that we can use for our program! returns -1 if it doesn't work


}

// Sample program to show how to get ips
int showIps(int argc, char *argv[]){
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    if(argc != 2) {
        fprintf(stderr, "usage: showip hostname\n");
        return 1;
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;    // AF_IFNET or AF_INET6 to force version
    hints.ai_socktype = SOCK_STREAM;
    
    if((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    printf("IP addresses for %s: \n\n", argv[1]);

    for (p = res; p != NULL; p = p->ai_next){
        void *addr;
        char *ipver;

        if (p->ai_family == AF_INET) {
            struct sockaddr_in * ipv4 = (struct  sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else {
            struct sockaddr_in6 * ipv6 = (struct  sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";    
        }

        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf("%s: %s\n", ipver, ipstr);
    }

    freeaddrinfo(res);

    return 0;
}

void connect_func(int argc, char *argv[]) {
    struct addrinfo hints, * res;
    int sockfd;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    getaddrinfo("www.example.com", "3490", &hints, &res);

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    // Connect!
    connect(sockfd, res->ai_addr, res->ai_protocol);
}

void bind_func(int argc, char *argv[]) {
    // - Using bind to a specific port
    struct addrinfo hints, *res;
    int sockfd;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, "3490", &hints, &res);

    // make socket
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    // bind to the port we passed in "3940"
    bind(sockfd, res->ai_addr, res->ai_addrlen);

    // Reusing a used port
    // int yes = 1;

    // if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
    //     perror("setsockopt");
    //     exit(1);
    // }
}

void listen_func(int argc, char *argv[]) {
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints, *res;
    int sockfd, new_fd;

    // Attempt to do error checking!

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;    // Use any IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;    // fill in IP

    getaddrinfo(NULL, MYPORT, &hints, &res);

    // Make a socket, bind port
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    bind(sockfd, res->ai_addr, res->ai_addrlen);
    listen(sockfd, BACKLOG);

    // accept incoming connection
    addr_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr * )&their_addr, &addr_size);
}

void sendAndRecv(int argc, char *argv[]) {
    
}