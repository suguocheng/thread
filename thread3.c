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
pthread_cond_t cond_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_full = PTHREAD_COND_INITIALIZER;

SPSCQueue *SPSCQueueInit(int capacity);
void SPSCQueuePush(SPSCQueue *queue, int s);
void *SPSCQueuePop(SPSCQueue *queue);
void SPSCQueueDestory(SPSCQueue *queue);
void *producter(void *arg);
void *consumer(void *arg);
int main()
{
    srand(time(NULL));

    SPSCQueue *queue=SPSCQueueInit(MAX_BUF);
    
    pthread_t tid1,tid2;
    pthread_create(&tid1,NULL,producter,queue);
    pthread_create(&tid2,NULL,consumer,queue);

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_empty);
    pthread_cond_destroy(&cond_full);
    SPSCQueueDestory(queue);
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
void SPSCQueuePush(SPSCQueue *queue, int s)
{
    queue->value[queue->rear]=s;
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
        pthread_mutex_lock(&mutex); // 加锁
        while(((SPSCQueue *)arg)->buf_count==MAX_BUF)
        {
            pthread_cond_wait(&cond_full,&mutex); 
        }
        SPSCQueuePush((SPSCQueue *)arg,rand()%10);
        ((SPSCQueue *)arg)->buf_count++;
        printf("product:%d\n",((SPSCQueue *)arg)->value[((SPSCQueue *)arg)->rear-1]);
        pthread_mutex_unlock(&mutex); // 解锁
        pthread_cond_signal(&cond_empty);
        sleep(rand()%5);
    }
}
void *consumer(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex); // 加锁
        while(((SPSCQueue *)arg)->buf_count==0)
        {
            pthread_cond_wait(&cond_empty,&mutex); 
        }
        printf("consume:%d\n",((SPSCQueue *)arg)->value[((SPSCQueue *)arg)->front]);
        SPSCQueuePop((SPSCQueue *)arg);
        ((SPSCQueue *)arg)->buf_count--;
        pthread_mutex_unlock(&mutex); // 解锁
        pthread_cond_signal(&cond_full);
        sleep(rand()%5);
    }
}