#include "lock.h"

#include <stdio.h>

void amountInit(lock_t* Account)
{
    Account->amount=0;
}
void Income(lock_t* Account, int amount)
{
    pthread_mutex_lock(&Account->mutex); // 加锁
    Account->amount+=amount;
    pthread_mutex_unlock(&Account->mutex); // 解锁
}
void Expend(lock_t* Account, int amount)
{
    pthread_mutex_lock(&Account->mutex); // 加锁
    Account->amount-=amount;
    pthread_mutex_unlock(&Account->mutex); // 解锁
}