#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *philosopher1(void *arg);
void *philosopher2(void *arg);
void *philosopher3(void *arg);
void *philosopher4(void *arg);
void *philosopher5(void *arg);
int main()
{
    pthread_t tid[5];

    pthread_create(&tid[0],NULL,philosopher1,NULL);
    pthread_create(&tid[1],NULL,philosopher2,NULL);
    pthread_create(&tid[2],NULL,philosopher3,NULL);
    pthread_create(&tid[3],NULL,philosopher4,NULL);
    pthread_create(&tid[4],NULL,philosopher5,NULL);

    pthread_join(tid[0],NULL);
    pthread_join(tid[1],NULL);
    pthread_join(tid[2],NULL);
    pthread_join(tid[3],NULL);
    pthread_join(tid[4],NULL);

    pthread_mutex_destroy(&mutex);
}
void *philosopher1(void *arg)
{
    pthread_mutex_lock(&mutex); // 加锁
    printf("哲学家1拿起筷子1\n");
    printf("哲学家1拿起筷子2\n");
    printf("哲学家1已吃完\n");
    printf("哲学家1放下筷子1\n");
    printf("哲学家1放下筷子2\n");
    pthread_mutex_unlock(&mutex); // 解锁
}
void *philosopher2(void *arg)
{
    pthread_mutex_lock(&mutex); // 加锁
    pthread_mutex_unlock(&mutex); // 解锁
}
void *philosopher3(void *arg)
{
    pthread_mutex_lock(&mutex); // 加锁
    pthread_mutex_unlock(&mutex); // 解锁
}
void *philosopher4(void *arg)
{
    pthread_mutex_lock(&mutex); // 加锁
    pthread_mutex_unlock(&mutex); // 解锁
}
void *philosopher5(void *arg)
{
    pthread_mutex_lock(&mutex); // 加锁
    pthread_mutex_unlock(&mutex); // 解锁
}