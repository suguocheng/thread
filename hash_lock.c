#include "hash_lock.h"

#include <stdio.h>
#include <stdlib.h>

void hashInit(hash_lock_t* bucket) {
    for(int i=0;i<HASHNUM;i++)
    {
        bucket->table[i].head=NULL;
        pthread_mutex_init(&(bucket->table[i].mutex),NULL);
    }
}

int getValue(hash_lock_t* bucket, int key) {
    int n=HASH(key);
    pthread_mutex_lock(&bucket->table[n].mutex); // 加锁
    Hlist p=bucket->table[n].head;
    while(p)
    {
        if(p->key==key)
        {
            pthread_mutex_unlock(&bucket->table[n].mutex); // 解锁
            return p->value;
        }
        p=p->next;
    }
    pthread_mutex_unlock(&bucket->table[n].mutex); // 解锁
    return -1;
}

void insert(hash_lock_t* bucket, int key, int value) {
    int n=HASH(key);
    pthread_mutex_lock(&bucket->table[n].mutex); // 加锁
    Hlist p=bucket->table[n].head;
    while(p)
    {
        if(p->key==key)
        {
            p->value=value;
            pthread_mutex_unlock(&bucket->table[n].mutex); // 解锁
            return;
        }
        p=p->next;
    }
    Hnode *node=(Hnode *)malloc(sizeof(Hnode));
    node->key=key;
    node->value=value;
    node->next=bucket->table[n].head;
    bucket->table[n].head=node;
    pthread_mutex_unlock(&bucket->table[n].mutex); // 解锁
}

int setKey(hash_lock_t* bucket, int key, int new_key) {
    int n1=HASH(key);
    int n2=HASH(new_key);
    
    pthread_mutex_lock(&bucket->table[n1].mutex); // 加锁
    Hlist prev=NULL;
    Hlist p=bucket->table[n1].head;
    while(p)
    {
        if(p->key==key)
        {
            if(prev)
            {
                prev->next=p->next;
            }
            else
            {
                bucket->table[n1].head=p->next;
            }
            pthread_mutex_unlock(&bucket->table[n1].mutex); // 解锁

            pthread_mutex_lock(&bucket->table[n2].mutex); // 加锁
            p->key=new_key;
            p->next=bucket->table[n2].head->next;
            bucket->table[n2].head=p;
            pthread_mutex_unlock(&bucket->table[n2].mutex); // 解锁
            
            return 0;
        }
        prev=p;
        p=p->next;
    }
    pthread_mutex_unlock(&bucket->table[n1].mutex); // 解锁
    return -1;
}