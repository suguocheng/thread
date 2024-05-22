#include "list_lock.h"

#include <stdio.h>
#include <stdlib.h>

void listInit(list_lock_t* list) {
    pthread_mutex_init(&list->mutex,NULL);
    pthread_cond_init(&list->cond, NULL);
    list->head=NULL;
}

void producer(list_lock_t* list, DataType value) {
    while(1)
    {
        pthread_mutex_lock(&list->mutex); // 加锁
        LinkList node=(LinkList)malloc(sizeof(LNode));
        node->value=value;
        node->next=list->head;
        list->head=node;
        pthread_mutex_unlock(&list->mutex); // 解锁
        pthread_cond_signal(&list->cond);
    }
}

void consumer(list_lock_t* list) {
    while(1)
    {
        pthread_mutex_lock(&list->mutex); // 加锁
        while(list->head==NULL)
        {
            pthread_cond_wait(&list->cond,&list->mutex);  
        }
        LinkList tem=list->head;
        list->head=list->head->next;
        free(tem);
        pthread_mutex_unlock(&list->mutex); // 解锁
    }
}

int getListSize(list_lock_t* list) {
    pthread_mutex_lock(&list->mutex); // 加锁
    int n=0;
    LinkList p=list->head;
    while(p->next!=NULL)
    {
        n++;
        p=p->next;
    }
    pthread_mutex_unlock(&list->mutex); // 解锁
    return n;
}