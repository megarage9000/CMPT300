#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "listMonitor.h"

// Two ports
// - My port: Where the remote client will be listening too
// - Their port: Where we are going to listen to

MessageList * userMessages, remoteMessages;

void * getInternetAddress(struct sockaddr * socketAddress) {
    if(socketAddress->sa_family == AF_INET){
        return &(((struct sockaddr_in*)socketAddress)->sin_addr);
    }
    return &(((struct sockaddr_in6*)socketAddress)->sin6_addr);
}

void setupListening(int * sockfd, char * userPort) {

}

void setupSending(int * sockfd, char * remoteHostName, char * hostPort) {
    struct addrinfo hints, *remoteServerInfo, *p;
    int result;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if((result = getaddrinfo(remoteHostName, hostPort, &hints, &remoteServerInfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(result));
        freeaddrinfo(remoteServerInfo);
        exit(1);
    }

    for(p = remoteServerInfo; p != NULL; p = p->ai_next) {
        if((*sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
            perror("s-talk: socket\n");
            continue;
        }

        break;
    }

    if(p == NULL) {
        fprintf(stderr, "s-talk: unable to create socket\n");
        freeaddrinfo(remoteServerInfo);
        exit(1);
    }

    freeaddrinfo(remoteServerInfo);
}

int main(int argc, char *argv[]) {
    int listeningSocketfd, sendingSocketfd;
    if(argc != 4) {
        printf("usage: ./s-talk userport remoteHostName remoteport\n");
        return;
    }
    printf("Using hostname %s at port %s\n", argv[2], argv[3]);
    setupSending(&sendingSocketfd, argv[2], argv[3]);
    close(sendingSocketfd);
    //setupListening(listeningSocketfd, argv[1]);
}