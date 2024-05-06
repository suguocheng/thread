#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include<unistd.h>

#define MAX_BUF 100

struct SPSCQueue {
    int *value;
    int buf_count;
    int front;
    int rear;
} typedef SPSCQueue;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

SPSCQueue *SPSCQueueInit(int capacity);
void SPSCQueuePush(SPSCQueue *queue, void *s);
void *SPSCQueuePop(SPSCQueue *queue);
void SPSCQueueDestory(SPSCQueue *queue);
void *producter(void *arg);
void *consumer(void *arg);
int main(void)
{
    srand(time(NULL));
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond, NULL);
    pthread_t tid1,tid2;
    pthread_create(&tid1,NULL,producter,NULL);
    pthread_create(&tid2,NULL,consumer,NULL);
    pthread_join(&tid1,NULL);
    pthread_join(&tid2,NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}
SPSCQueue *SPSCQueueInit(int capacity)
{
    SPSCQueue*queue=(SPSCQueue*)malloc(sizeof(SPSCQueue));
    queue->value=(int*)malloc(sizeof(int)*capacity);
    queue->buf_count=0;
    queue->front=0;
    queue->rear=0;
    return queue;
}
void SPSCQueuePush(SPSCQueue *queue, void *s)
{
    queue->value[queue->rear]=(int*)s;
    queue->rear=(queue->rear+1)%MAX_BUF;
}
void *SPSCQueuePop(SPSCQueue *queue)
{
    queue->front=(queue->front+1)%MAX_BUF;
}
void SPSCQueueDestory(SPSCQueue *queue)
{
    free(queue->value);
    free(queue);
}
void *producter(void *arg)
{
    while(1)
    {
        SPSCQueuePush();
    }
}
void *consumer(void *arg)
{
    while(1)
    {

    }
}