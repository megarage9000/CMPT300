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

#define ENDSYMBOL '!'

// Two ports
// - My port: Where the remote client will be listening too
// - Their port: Where we are going to listen to

void *get_in_addr(struct sockaddr * sa) {
    if(sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

struct UDPThreadArgs {
    char * machineName;
    char * port;
    char * threadId;
};

MessageList * userMessages, * remoteMessages;

void * getInternetAddress(struct sockaddr * socketAddress) {
    if(socketAddress->sa_family == AF_INET){
        return &(((struct sockaddr_in*)socketAddress)->sin_addr);
    }
    return &(((struct sockaddr_in6*)socketAddress)->sin6_addr);
}

void setupListening(int * sockfd, char * userPort) {
    struct addrinfo hints, *userServerInfo, *p;
    int result;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    if((result = getaddrinfo(NULL, userPort, &hints, &userServerInfo)) != 0) {
        fprintf(stderr, "getaddrinfo listener: %s\n", gai_strerror(result));
        freeaddrinfo(userServerInfo);
        exit(1);
    }

    for(p = userServerInfo; p != NULL; p = p->ai_next) {
        if((*sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("s-talk listener: socket");
            continue;
        }

        if(bind(*sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(*sockfd);
            perror("s-talk listener: bind");
            continue;
        }

        break;
    }

    if(p == NULL) {
        fprintf(stderr, "s-talk listener: unable to create and bind socket\n");
        freeaddrinfo(userServerInfo);
        exit(1);
    }

    freeaddrinfo(userServerInfo);
}

void setupSending(int * sockfd, char * remoteHostName, char * hostPort, struct addrinfo * remoteServer) {
    struct addrinfo hints, *p;
    int result;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if((result = getaddrinfo(remoteHostName, hostPort, &hints, &remoteServer)) != 0) {
        fprintf(stderr, "getaddrinfo sender: %s\n", gai_strerror(result));
        freeaddrinfo(remoteServer);
        exit(1);
    }

    for(p = remoteServer; p != NULL; p = p->ai_next) {
        if((*sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
            perror("s-talk sender: socket\n");
            continue;
        }

        break;
    }


    if(p == NULL) {
        fprintf(stderr, "s-talk sender: unable to create socket\n");
        freeaddrinfo(remoteServer);
        exit(1);
    }
    //freeaddrinfo(remoteServerInfo);
}

// Thread methods
void * readUserInput(void * threadId) {
    char * threadName = (char *)threadId;
    char buf[MAX_MESSAGE_LENGTH];
    while(1) {
        int result = read(STDIN_FILENO, buf, MAX_MESSAGE_LENGTH);
        if(result == 0) {
            fprintf(stderr, "Thread %s ERROR: User input has an error!\n", threadName);
        }
        else if(result == 1) {
            memset(buf, 0, sizeof buf);
            continue;
        }
        else if(result >= MAX_MESSAGE_LENGTH * sizeof(char)){
            fprintf(stderr, "Threas %s ERROR: User input has undefined behaviour!\n", threadName);
        }
        else {
            char * message = (char *)malloc(result);
            int sizeInInt = (result / sizeof(char));
            strncpy(message, buf, sizeInInt);
            message[sizeInInt - 1] = '\0';
            consume(userMessages, message, sizeInInt, threadName);
            free(message);
        }
        memset(buf, 0, sizeof buf);
    }
}

void * printRemoteMessage(void * threadId) {
    char * threadName = (char *)threadId;
    char buf[MAX_MESSAGE_LENGTH + 1];
    while(1) {
        produce(remoteMessages, buf, threadName);
        buf[MAX_MESSAGE_LENGTH] = '\n';
        write(STDOUT_FILENO, buf, sizeof buf);
    }
}

void * sendUserMessages(void * args) {
    struct UDPThreadArgs * newArgs = (struct UDPThreadArgs *)args;
    char * threadName = newArgs->threadId;
    char * remoteHostName = newArgs->machineName;
    char * remoteHostPort = newArgs->port;
    struct addrinfo hints, *remoteServer, *p;
    int sockfd;
    int result;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if((result = getaddrinfo(remoteHostName, remoteHostPort, &hints, &remoteServer)) != 0) {
        fprintf(stderr, "getaddrinfo sender: %s\n", gai_strerror(result));
        freeaddrinfo(remoteServer);
        exit(1);
    }

    for(p = remoteServer; p != NULL; p = p->ai_next) {
        if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
            perror("s-talk sender: socket\n");
            continue;
        }
        break;
    }


    if(p == NULL) {
        fprintf(stderr, "s-talk sender: unable to create socket\n");
        freeaddrinfo(remoteServer);
        exit(1);
    }
    
    char message[MAX_MESSAGE_LENGTH];
    while(1) {
        produce(userMessages, message, threadName);
        if(sendto(sockfd, message, strlen(message), 0, p->ai_addr, p->ai_addrlen) == -1){
            fprintf(stderr, "Thread %s ERROR: Unable to send user message %s\n", threadName, message);
        }
    }
}

void * listenForRemoteMessages(void * args) {
    struct UDPThreadArgs * newArgs = (struct UDPThreadArgs *)args;
    char * threadName = newArgs->threadId;
    char * userPort = newArgs->port;

    struct addrinfo hints, *userServerInfo, *p;
    int sockfd;
    int result;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    if((result = getaddrinfo(NULL, userPort, &hints, &userServerInfo)) != 0) {
        fprintf(stderr, "getaddrinfo listener: %s\n", gai_strerror(result));
        freeaddrinfo(userServerInfo);
        exit(1);
    }

    for(p = userServerInfo; p != NULL; p = p->ai_next) {
        if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("s-talk listener: socket");
            continue;
        }

        if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("s-talk listener: bind");
            continue;
        }

        break;
    }

    if(p == NULL) {
        fprintf(stderr, "s-talk listener: unable to create and bind socket\n");
        freeaddrinfo(userServerInfo);
        exit(1);
    }

    freeaddrinfo(userServerInfo);

    struct sockaddr_storage their_addr;
    socklen_t addr_len;
    char buf[MAX_MESSAGE_LENGTH];

    while(1) {
        addr_len = sizeof their_addr;

        if ((recvfrom(sockfd, buf, MAX_MESSAGE_LENGTH - 1, 0,
            (struct sockaddr *)&their_addr, &addr_len)) == -1) {
                printf("%s: Unable to get message, resulted in -1 bytes\n", threadName);
        }
        else {
            consume(remoteMessages, buf, MAX_MESSAGE_LENGTH, threadName);
        }
    }
}

int main(int argc, char *argv[]) {

    if(argc != 4) {
        printf("Usage: s-talk userPort remoteHostName remoteHostPort");
        return;
    }

    char * threadIds[4] = {
        "Receiving Messages Thread",
        "Sending User Messages Thread",
        "Reading User Input Thread",
        "Displaying Messages Thread"
    };

    userMessages = createMessageListPtr();
    remoteMessages = createMessageListPtr();

    struct UDPThreadArgs listenArgs, sendingArgs;
    
    listenArgs.machineName = "";
    listenArgs.port = argv[1];
    listenArgs.threadId = threadIds[0];
    sendingArgs.machineName = argv[2];
    sendingArgs.port = argv[3];
    sendingArgs.threadId = threadIds[1];

    pthread_t sendingThread, receivingThread, getUserInput, printMessages;

    pthread_create(&sendingThread, NULL, sendUserMessages, (void *)&sendingArgs);
    pthread_create(&receivingThread, NULL, listenForRemoteMessages,(void *)&listenArgs);
    pthread_create(&getUserInput, NULL, readUserInput,(void *)threadIds[2]);
    pthread_create(&printMessages, NULL, printRemoteMessage,(void *)threadIds[3]);

    pthread_join(sendingThread, NULL);
    pthread_join(receivingThread, NULL);
    pthread_join(getUserInput, NULL);
    pthread_join(printMessages, NULL);
    return 0;
}