#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

typedef struct Client* pClient;
typedef struct Client {
    int socket_fd;
} Client;

void *MessageReciever(void *arg);
void *MessageSender(void *arg);

#endif