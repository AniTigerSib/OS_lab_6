#ifndef CLIENTS_H
#define CLIENTS_H

#include "../linked_list/linked_list.h"
#include "server.h"

#define pNode pNodeClient
#define Node NodeClient

LINKED_LIST(Client);
typedef struct ClientList* pList;
typedef struct ClientList {
    pNodeClient head;
    pNodeClient tail;
    int size;
} ClientList;

#define F(T) T##Client
pNode F(CreateNode)(pClient client);
void Add(pList list, pClient client);
void Remove(pList list, pClient client);
void ForEverySend(pList list, char* data,  void (*function)(pClient, char*));
void ForEveryClose(pList list);
void F(DestroyNode)(pNode node);

#endif