#ifndef SERVER_H
#define SERVER_H

#include <pthread.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

typedef struct Client* pClient;
typedef struct Client {
    int socket_fd;
    pthread_t* p_reciever_thread;
} Client;

typedef struct Server* pServer;
typedef struct Server {
    int server_fd;
    struct sockaddr_in address;
    socklen_t addrlen;
} Server;

void *Listner(void *arg);
void *MessageReciever(void *arg);
void *MessageSender(void *arg);

#endif