#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

void *philosopher1(void *arg);
void *philosopher2(void *arg);
void *philosopher3(void *arg);
void *philosopher4(void *arg);
void *philosopher5(void *arg);
//方法一
// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// int main()
// {
//     pthread_t tid[5];

//     pthread_create(&tid[0],NULL,philosopher1,NULL);
//     pthread_create(&tid[1],NULL,philosopher2,NULL);
//     pthread_create(&tid[2],NULL,philosopher3,NULL);
//     pthread_create(&tid[3],NULL,philosopher4,NULL);
//     pthread_create(&tid[4],NULL,philosopher5,NULL);

//     pthread_join(tid[0],NULL);
//     pthread_join(tid[1],NULL);
//     pthread_join(tid[2],NULL);
//     pthread_join(tid[3],NULL);
//     pthread_join(tid[4],NULL);

//     pthread_mutex_destroy(&mutex);
// }
// void *philosopher1(void *arg)
// {
//     pthread_mutex_lock(&mutex); // 加锁
//     printf("哲学家1拿起筷子1\n");
//     printf("哲学家1拿起筷子2\n");
//     printf("哲学家1已吃完\n");
//     printf("哲学家1放下筷子1\n");
//     printf("哲学家1放下筷子2\n");
//     pthread_mutex_unlock(&mutex); // 解锁
// }
// void *philosopher2(void *arg)
// {
//     pthread_mutex_lock(&mutex); // 加锁
//     printf("哲学家2拿起筷子2\n");
//     printf("哲学家2拿起筷子3\n");
//     printf("哲学家2已吃完\n");
//     printf("哲学家2放下筷子2\n");
//     printf("哲学家2放下筷子3\n");
//     pthread_mutex_unlock(&mutex); // 解锁
// }
// void *philosopher3(void *arg)
// {
//     pthread_mutex_lock(&mutex); // 加锁
//     printf("哲学家3拿起筷子3\n");
//     printf("哲学家3拿起筷子4\n");
//     printf("哲学家3已吃完\n");
//     printf("哲学家3放下筷子3\n");
//     printf("哲学家3放下筷子4\n");
//     pthread_mutex_unlock(&mutex); // 解锁
// }
// void *philosopher4(void *arg)
// {
//     pthread_mutex_lock(&mutex); // 加锁
//     printf("哲学家4拿起筷子4\n");
//     printf("哲学家4拿起筷子5\n");
//     printf("哲学家4已吃完\n");
//     printf("哲学家4放下筷子4\n");
//     printf("哲学家4放下筷子5\n");
//     pthread_mutex_unlock(&mutex); // 解锁
// }
// void *philosopher5(void *arg)
// {
//     pthread_mutex_lock(&mutex); // 加锁
//     printf("哲学家5拿起筷子5\n");
//     printf("哲学家5拿起筷子1\n");
//     printf("哲学家5已吃完\n");
//     printf("哲学家5放下筷子5\n");
//     printf("哲学家5放下筷子1\n");
//     pthread_mutex_unlock(&mutex); // 解锁
// }

//方法二(可能发生死锁)
sem_t sem[6];
int main()
{
    pthread_t tid[5];
    for(int i=0;i<5;i++)
    {
        sem_init(&sem[i],0,1);
    }

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

    for(int i=0;i<5;i++)
    {
        sem_destroy(&sem[i]);
    }
}
void *philosopher1(void *arg)
{
    sem_wait(&sem[0]);
    printf("哲学家1拿起筷子1\n");
    sem_wait(&sem[1]);
    printf("哲学家1拿起筷子2\n");
    printf("哲学家1已吃完\n");
    sem_post(&sem[0]);
    printf("哲学家1放下筷子1\n");
    sem_post(&sem[1]);
    printf("哲学家1放下筷子2\n");
}
void *philosopher2(void *arg)
{
    sem_wait(&sem[1]);
    printf("哲学家2拿起筷子2\n");
    sem_wait(&sem[2]);
    printf("哲学家2拿起筷子3\n");
    printf("哲学家2已吃完\n");
    sem_post(&sem[1]);
    printf("哲学家2放下筷子2\n");
    sem_post(&sem[2]);
    printf("哲学家2放下筷子3\n");
}
void *philosopher3(void *arg)
{
    sem_wait(&sem[2]);
    printf("哲学家3拿起筷子3\n");
    sem_wait(&sem[3]);
    printf("哲学家3拿起筷子4\n");
    printf("哲学家3已吃完\n");
    sem_post(&sem[2]);
    printf("哲学家3放下筷子3\n");
    sem_post(&sem[3]);
    printf("哲学家3放下筷子4\n");
}
void *philosopher4(void *arg)
{
    sem_wait(&sem[3]);
    printf("哲学家4拿起筷子4\n");
    sem_wait(&sem[4]);
    printf("哲学家4拿起筷子5\n");
    printf("哲学家4已吃完\n");
    sem_post(&sem[3]);
    printf("哲学家4放下筷子4\n");
    sem_post(&sem[4]);
    printf("哲学家4放下筷子5\n");
}
void *philosopher5(void *arg)
{
    sem_wait(&sem[4]);
    printf("哲学家5拿起筷子5\n");
    sem_wait(&sem[5]);
    printf("哲学家5拿起筷子1\n");
    printf("哲学家5已吃完\n");
    sem_post(&sem[4]);
    printf("哲学家5放下筷子5\n");
    sem_post(&sem[5]);
    printf("哲学家5放下筷子1\n");
}