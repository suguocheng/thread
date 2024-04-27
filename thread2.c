#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

#define MATRIX_MAX 4

struct myqueue{
    int ***matrix;
    int front;
    int rear;
};

typedef struct{
    pthread_t *tid;

}thread_pool;

int main()
{
    struct myqueue *queue=(struct myqueue *)malloc(sizeof(struct myqueue));
    queue->matrix=(int ***)malloc(sizeof(int**)*MATRIX_MAX);
    int row;
    printf("请输入矩阵的行列数：\n");
    scanf("%d",&row);
    for(int i=0;i<MATRIX_MAX;i++)
    {
        queue->matrix[i]=(int**)malloc(sizeof(int*)*row);
    }
    for(int i=0;i<MATRIX_MAX;i++)
    {
        for(int j=0;j<row;j++)
        {
            queue->matrix[i][j]=(int *)malloc(sizeof(int)*row);
        }
    }
    
    //初始化矩阵
    srand(time(NULL));
    for(int i=0;i<MATRIX_MAX;i++)
    {
        for(int j=0;j<row;j++)
        {
            for(int k=0;k<row;k++)
            {
                queue->matrix[i][j][k]=rand()%10;
            }
        }
    }

    //输出计算前矩阵
    for(int i=0;i<MATRIX_MAX;i++)
    {
        printf("第%d个矩阵:\n",i+1);
        for(int j=0;j<row;j++)
        {
            for(int k=0;k<row;k++)
            {
                printf("%4d",queue->matrix[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }

    //矩阵相乘
    int ***nmatrix=(int ***)malloc(sizeof(int**)*MATRIX_MAX/2);
    for(int i=0;i<MATRIX_MAX/2;i++)
    {
        nmatrix[i]=(int**)malloc(sizeof(int*)*row);
    }
    for(int i=0;i<MATRIX_MAX/2;i++)
    {
        for(int j=0;j<row;j++)
        {
            nmatrix[i][j]=(int *)malloc(sizeof(int)*row);
        }
    }
    int value=0;
    int len=0;
    int count=0;
    for(int a=0;a<MATRIX_MAX-1;a++)
    {
        for(int i=0;i<row;i++)
        {
            for(int j=0;j<row;j++)
            {
                for(int k=0;k<row;k++)
                {
                    //行、列 元素的乘积之和 
                    value+=queue->matrix[a][i][k]*queue->matrix[a+1][k][j];
                }
                nmatrix[count][i][len++]=value;
                value=0;
                //len==a1 指的是二维数组已经到达当行的最后元素
                if(len==row)
                {
                    len=0;
                }
            }
        }
        a++;
        count++;
    }

    //输出计算后矩阵
    printf("两两相乘得：\n");
    for(int i=0;i<MATRIX_MAX/2;i++)
    {
        printf("第%d个结果:\n",i+1);
        for(int j=0;j<row;j++)
        {
            for(int k=0;k<row;k++)
            {
                printf("%4d",nmatrix[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
    
    for(int i=0;i<MATRIX_MAX/2;i++)
    {
        for(int j=0;j<row;j++)
        {
            free(nmatrix[i][j]);
        }
    }
    for(int i=0;i<MATRIX_MAX/2;i++)
    {
        free(nmatrix[i]);
    }
    free(nmatrix);
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