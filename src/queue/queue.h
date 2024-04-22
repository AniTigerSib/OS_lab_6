#ifndef QUEUE_H
#define QUEUE_H

#include "../linked_list/linked_list.h"

#define pNode pNodechar
#define Node Nodechar

#define QUEUE(T) \
LINKED_LIST(T); \
typedef struct Queue* pQueue; \
typedef struct Queue { \
    pNode head; \
    pNode tail; \
} Queue; \
\
pNode CreateNode(char* data); \
void Enqueue(pQueue queue, char* data); \
char* Dequeue(pQueue queue); \
void Clear(pQueue queue);

#ifndef QUEUE_SOURCE
#undef pNode
#undef Node
#endif

#endif