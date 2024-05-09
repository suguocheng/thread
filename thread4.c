#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define MAX_BUF 100

struct MPMCQueue {
    int *value;
    int buf_count;
    int front;
    int rear;
} typedef MPMCQueue;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_full = PTHREAD_COND_INITIALIZER;

MPMCQueue *MPMCQueueInit(int capacity);
void MPMCQueuePush(MPMCQueue *queue, int s);
void *MPMCQueuePop(MPMCQueue *queue);
void MPMCQueueDestory(MPMCQueue *);
void *producter1(void *arg);
void *producter2(void *arg);
void *producter3(void *arg);
void *consumer1(void *arg);
void *consumer2(void *arg);
void *consumer3(void *arg);
int main()
{
    srand(time(NULL));

    MPMCQueue *queue=MPMCQueueInit(MAX_BUF);

    pthread_t tid_pro1,tid_pro2,tid_pro3,tid_con1,tid_con2,tid_con3;
    pthread_create(&tid_pro1,NULL,producter1,queue);
    pthread_create(&tid_pro1,NULL,producter2,queue);
    pthread_create(&tid_pro1,NULL,producter3,queue);
    pthread_create(&tid_con1,NULL,consumer1,queue);
    pthread_create(&tid_con2,NULL,consumer2,queue);
    pthread_create(&tid_con3,NULL,consumer3,queue);

    pthread_join(tid_pro1,NULL);
    pthread_join(tid_pro2,NULL);
    pthread_join(tid_pro3,NULL);
    pthread_join(tid_con1,NULL);
    pthread_join(tid_con2,NULL);
    pthread_join(tid_con3,NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_empty);
    pthread_cond_destroy(&cond_full);
    MPMCQueueDestory(queue);
}
MPMCQueue *MPMCQueueInit(int capacity)
{
    MPMCQueue*queue=(MPMCQueue *)malloc(sizeof(MPMCQueue));
    queue->value=(int*)malloc(sizeof(int)*capacity);
    queue->buf_count=0;
    queue->front=0;
    queue->rear=0;
    return queue;
}
void MPMCQueuePush(MPMCQueue *queue, int s)
{
    queue->value[queue->rear]=s;
    queue->rear=(queue->rear+1)%MAX_BUF;
}
void *MPMCQueuePop(MPMCQueue *queue)
{
    queue->front=(queue->front+1)%MAX_BUF;
}
void MPMCQueueDestory(MPMCQueue *queue)
{
    free(queue->value);
    free(queue);
}
void *producter1(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex); // 加锁
        while(((MPMCQueue *)arg)->buf_count==MAX_BUF)
        {
            pthread_cond_wait(&cond_full,&mutex); 
        }
        MPMCQueuePush((MPMCQueue *)arg,rand()%10);
        ((MPMCQueue *)arg)->buf_count++;
        printf("product:%d\n",((MPMCQueue *)arg)->value[((MPMCQueue *)arg)->rear-1]);
        pthread_mutex_unlock(&mutex); // 解锁
        pthread_cond_signal(&cond_empty);
        sleep(rand()%5);
    }
}
void *producter2(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex); // 加锁
        while(((MPMCQueue *)arg)->buf_count==MAX_BUF)
        {
            pthread_cond_wait(&cond_full,&mutex); 
        }
        MPMCQueuePush((MPMCQueue *)arg,rand()%10);
        ((MPMCQueue *)arg)->buf_count++;
        printf("product:%d\n",((MPMCQueue *)arg)->value[((MPMCQueue *)arg)->rear-1]);
        pthread_mutex_unlock(&mutex); // 解锁
        pthread_cond_signal(&cond_empty);
        sleep(rand()%5);
    }
}
void *producter3(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex); // 加锁
        while(((MPMCQueue *)arg)->buf_count==MAX_BUF)
        {
            pthread_cond_wait(&cond_full,&mutex); 
        }
        MPMCQueuePush((MPMCQueue *)arg,rand()%10);
        ((MPMCQueue *)arg)->buf_count++;
        printf("product:%d\n",((MPMCQueue *)arg)->value[((MPMCQueue *)arg)->rear-1]);
        pthread_mutex_unlock(&mutex); // 解锁
        pthread_cond_signal(&cond_empty);
        sleep(rand()%5);
    }
}
void *consumer1(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex); // 加锁
        while(((MPMCQueue *)arg)->buf_count==0)
        {
            pthread_cond_wait(&cond_empty,&mutex); 
        }
        printf("consume:%d\n",((MPMCQueue *)arg)->value[((MPMCQueue *)arg)->front]);
        MPMCQueuePop((MPMCQueue *)arg);
        ((MPMCQueue *)arg)->buf_count--;
        pthread_mutex_unlock(&mutex); // 解锁
        pthread_cond_signal(&cond_full);
        sleep(rand()%5);
    }
}
void *consumer2(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex); // 加锁
        while(((MPMCQueue *)arg)->buf_count==0)
        {
            pthread_cond_wait(&cond_empty,&mutex); 
        }
        printf("consume:%d\n",((MPMCQueue *)arg)->value[((MPMCQueue *)arg)->front]);
        MPMCQueuePop((MPMCQueue *)arg);
        ((MPMCQueue *)arg)->buf_count--;
        pthread_mutex_unlock(&mutex); // 解锁
        pthread_cond_signal(&cond_full);
        sleep(rand()%5);
    }
}
void *consumer3(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex); // 加锁
        while(((MPMCQueue *)arg)->buf_count==0)
        {
            pthread_cond_wait(&cond_empty,&mutex); 
        }
        printf("consume:%d\n",((MPMCQueue *)arg)->value[((MPMCQueue *)arg)->front]);
        MPMCQueuePop((MPMCQueue *)arg);
        ((MPMCQueue *)arg)->buf_count--;
        pthread_mutex_unlock(&mutex); // 解锁
        pthread_cond_signal(&cond_full);
        sleep(rand()%5);
    }
}