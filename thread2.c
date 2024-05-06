#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include<unistd.h>

#define MAX_TASKS 100
#define MAX_MATRIX 4
#define MAX_THREADS 10

typedef struct{
    int ***matrix;
    int **nmatrix;
    int row;
}Task;

Task task_queue[MAX_TASKS];
int task_count=0;
int front=0;
int rear=0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int count1=1;
int count2=1;

void submit_task(Task *arg);
void *calculate(void *arg);
int main()
{
    pthread_mutex_init(&mutex,NULL);
    Task *arg=(Task *)malloc(sizeof(Task));
    printf("请输入矩阵的行列数：\n");
    scanf("%d",&arg->row);

    //为计算前的两个矩阵分配内存
    arg->matrix=(int ***)malloc(sizeof(int**)*2);
    for(int i=0;i<2;i++)
    {
        arg->matrix[i]=(int**)malloc(sizeof(int*)*arg->row);
    }
    for(int i=0;i<2;i++)
    {
        for(int j=0;j<arg->row;j++)
        {
            arg->matrix[i][j]=(int *)malloc(sizeof(int)*arg->row);
        }
    }

    //为计算后的矩阵分配内存
    arg->nmatrix=(int **)malloc(sizeof(int*)*arg->row);
    for(int i=0;i<arg->row;i++)
    {
        arg->nmatrix[i]=(int*)malloc(sizeof(int)*arg->row);
    }

    //创建线程计算矩阵相乘
    pthread_t tid[MAX_THREADS];
    for(int i=0;i<MAX_THREADS;i++)
    {
        pthread_create(&tid[i],NULL,calculate,NULL);
    }

    srand(time(NULL));

    int task_arg;
    while(1)
    {
        printf("请输入想要添加的任务数量(输入0退出)：\n");
        scanf("%d",&task_arg);
        if(task_arg==0)
        {
            break;
        }
        for(int i=0;i<task_arg;i++)
        {
            for(int i=0;i<2;i++)
            {
                for(int j=0;j<arg->row;j++)
                {
                    for(int k=0;k<arg->row;k++)
                    {
                        arg->matrix[i][j][k]=rand()%10;
                    }
                }
            }
            //输出计算前矩阵
            printf("第%d个任务:\n",count1++);
            for(int i=0;i<2;i++)
            {
                printf("第%d个矩阵:\n",i+1);
                for(int j=0;j<arg->row;j++)
                {
                    for(int k=0;k<arg->row;k++)
                    {
                        printf("%4d",arg->matrix[i][j][k]);
                    }
                    printf("\n");
                }
                printf("\n");
            }
            submit_task(arg);
        }
    }


    for(int i=0;i<MAX_THREADS;i++)
    {
        pthread_join(tid[i], NULL);
    }
    
    //释放计算后矩阵内存
    for(int i=0;i<arg->row;i++)
    {
        free(arg->nmatrix[i]);
    }
    free(arg->nmatrix);

    //释放计算前两个矩阵的内存
    for(int i=0;i<2;i++)
    {
        for(int j=0;j<arg->row;j++)
        {
            free(arg->matrix[i][j]);
        }
    }
    for(int i=0;i<2;i++)
    {
        free(arg->matrix[i]);
    }
    free(arg->matrix);

    free(arg);

    return 0;
}
void *calculate(void *arg)
{
    while(1)
    {
        int value=0;
        int len=0;
        Task task;
        pthread_mutex_lock(&mutex); // 加锁
        if(front!=rear)
        {
            // printf("任务队列中的两个矩阵(front=%d)：\n",front);
            // for(int i=0;i<2;i++)
            // {
            //     printf("第%d个矩阵:\n",i+1);
            //     for(int j=0;j<task_queue[0].row;j++)
            //     {
            //         for(int k=0;k<task_queue[0].row;k++)
            //         {
            //             printf("%4d",task_queue[0].matrix[i][j][k]);
            //         }
            //         printf("\n");
            //     }
            //     printf("\n");
            // }

            task=task_queue[front];
            front=(front+1)%MAX_TASKS;
            task_count--;
            
            // printf("线程中计算前的两个矩阵：\n");
            // for(int i=0;i<2;i++)
            // {
            //     printf("第%d个矩阵:\n",i+1);
            //     for(int j=0;j<task.row;j++)
            //     {
            //         for(int k=0;k<task.row;k++)
            //         {
            //             printf("%4d",task.matrix[i][j][k]);
            //         }
            //         printf("\n");
            //     }
            //     printf("\n");
            // }

            for(int i=0;i<task.row;i++)
            {
                for(int j=0;j<task.row;j++)
                {
                    for(int k=0;k<task.row;k++)
                    {
                        //行、列 元素的乘积之和 
                        value+=task.matrix[0][i][k]*task.matrix[1][k][j];
                    }
                    task.nmatrix[i][len++]=value;
                    value=0;
                    //len==a1 指的是二维数组已经到达当行的最后元素
                    if(len==task.row)
                    {
                        len=0;
                    }
                }
            }
            printf("第%d个任务的结果：\n",count2++);
            for(int i=0;i<task.row;i++)
            {
                for(int j=0;j<task.row;j++)
                {
                    printf("%4d",task.nmatrix[i][j]);
                }
                printf("\n");
            }
            printf("\n");
            pthread_mutex_unlock(&mutex); // 解锁
        }
        else
        {
            pthread_mutex_unlock(&mutex); // 解锁
            // 如果没有任务可执行，线程休眠一段时间
            usleep(10000);
        }
    }
    return NULL;
}
void submit_task(Task *arg)
{
    pthread_mutex_lock(&mutex); // 加锁
    if(task_count<MAX_TASKS)
    {
        // 为新任务创建内存空间
        Task *new_task=(Task*)malloc(sizeof(Task));
        new_task->row=arg->row;

        new_task->matrix=(int ***)malloc(sizeof(int**)*2);
        new_task->nmatrix=(int **)malloc(sizeof(int*)*new_task->row);
        for(int i=0;i<2;i++)
        {
            new_task->matrix[i]=(int**)malloc(sizeof(int*)*new_task->row);
            for(int j=0;j<new_task->row;j++)
            {
                new_task->matrix[i][j]=(int *)malloc(sizeof(int)*new_task->row);
                for(int k=0; k<new_task->row;k++)
                {
                    new_task->matrix[i][j][k]=arg->matrix[i][j][k];
                }
            }
        }

        for(int i=0;i<new_task->row;i++) {
            new_task->nmatrix[i]=(int *)malloc(sizeof(int)*new_task->row);
        }

        task_queue[rear]=*new_task;
        rear=(rear+1)%MAX_TASKS;
        task_count++;

        pthread_mutex_unlock(&mutex); // 解锁
    }
}