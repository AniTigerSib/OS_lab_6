#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#define LINKED_LIST(T) \
typedef struct Node##T* pNode##T; \
typedef struct Node##T { \
    pNode##T next; \
    pNode##T prev; \
    T* data; \
} Node##T;

#endif