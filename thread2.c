#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

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
int front;
int rear;

void submit_task(Task *arg);
void *calculate(void *arg);
int main()
{
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

    //输出计算前矩阵
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

    //创建线程计算矩阵相乘
    pthread_t tid[MAX_THREADS];
    for(int i=0;i<MAX_THREADS;i++)
    {
        pthread_create(&tid[i],NULL,calculate,NULL);
    }
    for(int i=0;i<10;i++)
    {
        srand(time(NULL));
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
        submit_task(arg);
    }


    //输出计算后矩阵
    // printf("两两相乘得：\n");
    // for(int i=0;i<MAX_MATRIX/2;i++)
    // {
    //     printf("第%d个结果:\n",i+1);
    //     for(int j=0;j<arg->row;j++)
    //     {
    //         for(int k=0;k<arg->row;k++)
    //         {
    //             printf("%4d",arg->nmatrix[i][j][k]);
    //         }
    //         printf("\n");
    //     }
    //     printf("\n");
    // }
    
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
        if(front!=rear)
        {
            task=task_queue[front];
            front=(front+1)%MAX_TASKS;
            
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
        }
        else
        {
            // 如果没有任务可执行，线程休眠一段时间
            usleep(10000);
        }
    }
    return NULL;
}
void submit_task(Task *arg)
{
    if(task_count<MAX_TASKS)
    {
        // 将任务添加到队列尾部
        task_queue[rear]=*arg;
        rear=(rear+1)%MAX_TASKS;
        task_count++;
    }
}