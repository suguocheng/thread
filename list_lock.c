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

    }
}

void consumer(list_lock_t* list) {
   perror("This function is not implemented");
}

int getListSize(list_lock_t* list) {
  perror("This function is not implemented");
}