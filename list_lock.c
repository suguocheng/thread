#include "list_lock.h"

#include <stdio.h>
#include <stdlib.h>

void listInit(list_lock_t* list) {
    pthread_mutex_init(&list->mutex,NULL);
    pthread_cond_init(&list->cond, NULL);
    list->head->value=0;
    list->head->next=NULL;
}

void producer(list_lock_t* list, DataType value) {
    while(1)
    {
        pthread_mutex_lock(&list->mutex); // 加锁
        LinkList node=(LinkList)malloc(sizeof(LNode));
        LinkList p=list->head;
        while(p->next!=NULL)
        {
            p=p->next;
        }
        node->value=value;
        p->next=node;
        node->next=NULL;
        pthread_mutex_unlock(&list->mutex); // 解锁
        pthread_cond_signal(&list->cond);
    }
}

void consumer(list_lock_t* list) {
    while(1)
    {
        pthread_mutex_lock(&list->mutex); // 加锁
        pthread_cond_wait(&list->cond,&list->mutex); 
        LinkList p=list->head;
        while(p->next->next!=NULL)
        {
            p=p->next;
        }
        LinkList tem=p->next;
        free(tem);
        p->next=NULL;
        pthread_mutex_unlock(&list->mutex); // 解锁
    }
}

int getListSize(list_lock_t* list) {
    int n=0;
    LinkList p=list->head;
    while(p->next!=NULL)
    {
        p=p->next;
        n++;
    }
    return n;
}