#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

#define ARR_SIZE 8

struct args
{
    int *num;
    int size;
};

void *sort(void* arg);
void bubble_sort(struct args *arg);
int merge(int *num1,int size1,int *num2,int size2,int *narr);
int main()
{
    //创建数组以及初始化
    int *arr=(int *)malloc(sizeof(int)*ARR_SIZE);
    srand(time(NULL));
    for(int i=0;i<ARR_SIZE;i++)
    {
        arr[i]=rand();
    }

    //打印排序前数组
    printf("排序前：");
    for(int i=0;i<ARR_SIZE;i++)
    {
        printf("%d ",arr[i]);
    }
    printf("\n");

    pthread_t *tid1=(pthread_t *)malloc(sizeof(pthread_t));
    pthread_t *tid2=(pthread_t *)malloc(sizeof(pthread_t));
    pthread_t *tid3=(pthread_t *)malloc(sizeof(pthread_t));
    pthread_t *tid4=(pthread_t *)malloc(sizeof(pthread_t));

    //创建各个线程的结构体
    struct args *arg1=(struct args *)malloc(sizeof(struct args));
    arg1->num=arr;
    arg1->size=ARR_SIZE/4;
    struct args *arg2=(struct args *)malloc(sizeof(struct args));
    arg2->num=arr+ARR_SIZE/4;
    arg2->size=ARR_SIZE/4;
    struct args *arg3=(struct args *)malloc(sizeof(struct args));
    arg3->num=arr+ARR_SIZE/4*2;
    arg3->size=ARR_SIZE/4;
    struct args *arg4=(struct args *)malloc(sizeof(struct args));
    arg4->num=arr+ARR_SIZE/4*3;
    arg4->size=ARR_SIZE/4;
    
    // for(int i=0;i<5;i++)
    // {
    //     printf("arg1->num[%d]=%d arg2->num[%d]=%d arg3->num[%d]=%d arg4->num[%d]=%d\n",i,arg1->num[i],i,arg2->num[i],i,arg3->num[i],i,arg4->num[i]);
    // }

    //各线程中排序
    pthread_create(tid1,NULL,sort,arg1);
    pthread_create(tid2,NULL,sort,arg2);
    pthread_create(tid3,NULL,sort,arg3);
    pthread_create(tid4,NULL,sort,arg4);
    pthread_join(*tid1,NULL);
    pthread_join(*tid2,NULL);
    pthread_join(*tid3,NULL);
    pthread_join(*tid4,NULL);

    //合并排序
    int *narr=(int *)malloc(sizeof(int)*ARR_SIZE);
    int *arr1=(int *)malloc(sizeof(int)*ARR_SIZE/2);
    int *arr2=(int *)malloc(sizeof(int)*ARR_SIZE/2);
    merge(arg1->num,arg1->size,arg2->num,arg2->size,arr1);
    merge(arg3->num,arg3->size,arg4->num,arg4->size,arr2);
    merge(arr1,ARR_SIZE/2,arr2,ARR_SIZE/2,narr);

    //打印排序后数组
    // printf("a=%d b=%d count=%d\n",a,b,count);
    printf("排序后：");
    for(int i=0;i<ARR_SIZE;i++)
    {
        printf("%d ",narr[i]);
    }

    free(arr);
    free(arg1);
    free(arg2);
    free(tid1);
    free(tid2);
    free(narr);
    return 0;
}
void *sort(void* arg)
{
    bubble_sort(arg);
    //打印各线程中排序后数组
    printf("排序：");
    for(int i=0;i<((struct args *)arg)->size;i++)
    {
        printf("%d ",((struct args *)arg)->num[i]);
    }
    printf("\n");
}
void bubble_sort(struct args *arg)
{
    int temp;
    for(int i=0;i<arg->size-1;i++)
    {
        int count=1;
        for(int j=0;j<arg->size-i-1;j++)
        {
            if(arg->num[j]>arg->num[j+1])
            {
                temp=arg->num[j];
                arg->num[j]=arg->num[j+1];
                arg->num[j+1]=temp;
                count=0;
            }
        }
        if(count==1)
        {
            break;
        }
    }
}
int merge(int *num1,int size1,int *num2,int size2,int *narr)
{
    int a=0,b=0,count=0;
    while(1)
    {
        if(a==size1)
        {
            narr[count++]=num2[b++];
        }
        else if(b==size2)
        {
            narr[count++]=num1[a++];
        }
        else
        {
            // printf("a=%d b=%d\n",a,b);
            // printf("arg1->num[%d]=%d arg2->num[%d]=%d\n",a,arg1->num[a],b,arg2->num[b]);
            if(num1[a]<=num2[b])
            {
                // printf("arg1->num[a]=%d\n",arg1->num[a]);
                narr[count++]=num1[a++];
            }
            else
            {
                // printf("arg2->num[b]=%d\n",arg2->num[b]);
                narr[count++]=num2[b++];
            }
        }
        if(a==size1&&b==size2)
        {
            break;
        }
    }
}