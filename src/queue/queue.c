#define QUEUE_SOURCE
#include "queue.h"

#include <stdlib.h>
#include <string.h>

QUEUE(char)

pNode CreateNode(char* data) {
    pNode node = (pNode)malloc(sizeof(Node));
    node->data = (char*)calloc(strlen(data) + 1, sizeof(char));
    strcpy(node->data, data);
    node->next = NULL;
    node->prev = NULL;
    return node;
}

void Enqueue(pQueue queue, char* data) {
    pNode node = CreateNode(data);
    if (queue->head == NULL) {
        queue->head = node;
        queue->tail = node;
    } else {
        queue->tail->next = node;
        node->prev = queue->tail;
        queue->tail = node;
    }
}

char* Dequeue(pQueue queue) {
    pNode node = queue->head;
    if (queue->head == NULL) {
        return NULL;
    } else if (queue->head == queue->tail) {
        queue->head = NULL;
        queue->tail = NULL;
    } else {
        queue->head = queue->head->next;
        queue->head->prev = NULL;
    }
    char* to_return = node->data;
    free(node);
    return to_return;
}

void Clear(pQueue queue) {
    pNode node = queue->head;
    while (node!= NULL) {
        pNode next = node->next;
        free(node->data);
        free(node);
        node = next;
    }
}

int IsEmpty(pQueue queue) {
    return queue->head == NULL;
}