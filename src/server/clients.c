#include "clients.h"

#include <stdlib.h>
#include "server.h"

pNode F(CreateNode)(pClient client) {
    pNode node = (pNode) malloc(sizeof(Node));
    node->data = client;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

void Add(pList list, pClient client) {
    pNode node = F(CreateNode)(client);
    if ((node->next = list->head) != NULL){
        list->head->prev = node;
    } else {
        list->tail = node;
    }
    list->head = node;
    list->size++;
}

void Remove(pList list, pClient client) {
    pNode node = list->head;
    while (node->data != client)
        node = node->next;
    if (node == list->head) {
        list->head = node->next;
        if (list->head != NULL)
            list->head->prev = NULL;
    } else if (node == list->tail) {
        list->tail = node->prev;
        if (list->tail != NULL)
            list->tail->next = NULL;
    } else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    list->size--;
    F(DestroyNode)(node);
}

void ForEverySend(pList list, char* data, void (*function)(pClient, char*)) {
    pNode node = list->head;
    while (node != NULL) {
        function(node->data, data);
        node = node->next;
    }
}

void ForEveryClose(pList list) {
    pNode node = list->head;
    pNode prev = NULL;
    while (node != NULL) {
        prev = node;
        node = prev->next;
        F(DestroyNode)(prev);
        list->head = NULL;
        list->tail = NULL;
    }
}

void F(DestroyNode)(pNode node) {
    close(node->data->socket_fd);
    free(node->data);
    free(node);
}