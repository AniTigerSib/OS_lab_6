#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "server.h"

#include <semaphore.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>

#include "../queue/queue.h"
#include "clients.h"

QUEUE(char);

Queue g_messages;
ClientList g_clients;

sem_t g_messages_to_send;
sem_t g_messages_lock;

sem_t g_clients_lock;

int main() {
    // Main part

    Server server;
    server.addrlen = sizeof(server.address);
    int opt = 1;

    // Threads part

    pthread_t hThreadSender;
    pthread_t hThreadListner;
    // int hThreads[2];

    sem_init(&g_messages_to_send, 0, 0);
    sem_init(&g_messages_lock, 0, 1);
    sem_init(&g_clients_lock, 0, 1);

    // Initializing of server

    if ((server.server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        return(EXIT_FAILURE);
    }
    if (setsockopt(server.server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        return(EXIT_FAILURE);
    }
    server.address.sin_family = AF_INET;
    server.address.sin_addr.s_addr = INADDR_ANY;
    server.address.sin_port = htons(PORT);
    if (bind(server.server_fd, (struct sockaddr *)&server.address, server.addrlen) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server.server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    // Initializing of threads

    pthread_create(&hThreadListner, NULL, Listner, &server);
    pthread_create(&hThreadSender, NULL, MessageSender, NULL);

    pthread_exit(0);

    close(server.server_fd);
    return 0;
}

void ClearBuffer(char* buffer) {
    memset(buffer, 0, BUFFER_SIZE);
}

void *Listner(void *arg) {
    pServer server = (pServer)arg;
    while (1) {
        int new_socket;
        if ((new_socket = accept(server->server_fd, (struct sockaddr *)&(server->address), (socklen_t*)&(server->addrlen))) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        fprintf(stdout, "Connected\n");
        pClient client = (pClient)malloc(sizeof(Client));
        client->socket_fd = new_socket;
        Add(&g_clients, client);
        client->p_reciever_thread = (pthread_t*)malloc(sizeof(pthread_t));
        pthread_create(client->p_reciever_thread, NULL, MessageReciever, client);
    }
    return NULL;
}

void *MessageReciever(void *arg) {
    pClient client = (pClient)arg;
    char buffer[BUFFER_SIZE] = {0};
    ssize_t valread;
    while (1) {
        valread = read(client->socket_fd, buffer, BUFFER_SIZE - 1);
        if (valread != 0) {
            if (strcmp(buffer, "close") == 0) {
                send(client->socket_fd, buffer, strlen(buffer), 0);
                fprintf(stdout, "Disconnected\n");
                Remove(&g_clients, client);
                break;
            }
            if (sem_wait(&g_messages_lock) != -1) {
                Enqueue(&g_messages, buffer);
                sem_post(&g_messages_lock);
                sem_post(&g_messages_to_send);
            }
            ClearBuffer(buffer);
        }
    }
    return NULL;
}

void Send(pClient client, char* buffer) {
    send(client->socket_fd, buffer, strlen(buffer), 0);
}

void *MessageSender(void *arg) {
    char buffer[BUFFER_SIZE] = {0};
    while (1) {
        if (sem_wait(&g_messages_to_send)!= -1 && sem_wait(&g_messages_lock) != -1) {
            strcpy(buffer, Dequeue(&g_messages));
            sem_post(&g_messages_lock);
        }
        if (sem_wait(&g_clients_lock)!= -1) {
            ForEverySend(&g_clients, buffer, Send);
            sem_post(&g_clients_lock);
        }
        ClearBuffer(buffer);
    }
    return NULL;
}