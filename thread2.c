#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

#define MATRIX_MAX 5

struct myqueue{
    int ***matrix;
    int front;
    int rear;
};

int main()
{
    struct myqueue *queue=(struct myqueue *)malloc(sizeof(struct myqueue));
    queue->matrix=(int ***)malloc(sizeof(int**)*MATRIX_MAX);
    int row,column;
    printf("请输入矩阵的行数与列数：\n");
    scanf("%d %d",&row,&column);
    for(int i=0;i<MATRIX_MAX;i++)
    {
        queue->matrix[i]=(int**)malloc(sizeof(int*)*row);
    }
    for(int i=0;i<MATRIX_MAX;i++)
    {
        for(int j=0;j<row;j++)
        {
            queue->matrix[i][j]=(int *)malloc(sizeof(int)*column);
        }
    }
    
    //初始化矩阵
    srand(time(NULL));
    for(int i=0;i<MATRIX_MAX;i++)
    {
        for(int j=0;j<row;j++)
        {
            for(int k=0;k<column;k++)
            {
                queue->matrix[i][j][k]=rand()%10;
            }
        }
    }

    //测试输出
    // for(int i=0;i<MATRIX_MAX;i++)
    // {
    //     for(int j=0;j<row;j++)
    //     {
    //         for(int k=0;k<column;k++)
    //         {
    //             printf("%d ",queue->matrix[i][j][k]);
    //         }
    //         printf("\n");
    //     }
    //     printf("\n");
    // }
    
    for(int i=0;i<MATRIX_MAX;i++)
    {
        for(int j=0;j<row;j++)
        {
            free(queue->matrix[i][j]);
        }
    }
    for(int i=0;i<MATRIX_MAX;i++)
    {
        free(queue->matrix[i]);
    }
    free(queue->matrix);
    free(queue);

    return 0;
}