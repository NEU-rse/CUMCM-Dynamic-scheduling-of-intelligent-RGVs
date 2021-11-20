// 附录一、假设无故障的基本算法（先到先服务原则）一道工序的C语言程序
#include <stdio.h>
#include <stdlib.h>

int Tplus(int k, int p[], int q)    ///对CNC的时间计数器加上下料与清洗时间等。
{
    int m;
    for(m=0;m<8;m++)
    {
        p[m]+=k;
    }
    p[q-1]-=k;      ///本CNC的上下料、清洗时间不计。
}
int Tplus1(int k, int p[])
{
    int m;
    for(m=0;m<8;m++)
    {
        p[m]+=k;
    }
}
int Rankmove(int m[])
{
    int z;
    for(z=0;z<7;z++)
    {
        m[z]=m[z+1];
    }
    m[7]=0;
}
int main()
{
    int t_sxl[2], t_qx, t_work, t_mov[3], i, tmr=0, t_CNC[8]={0}, Rank[8]={0}, j=0, place=1, N=1, A, k, B, TN_CNC[8][200]={0}, N_CNC[8]={1,1,1,1,1,1,1,1};     ///定义执行各种子工序的时间变量
    printf("Please input t_sxl[2], t_qx, t_work, t_mov[3] in this order:");
    scanf("%d%d%d%d", &t_sxl[0], &t_sxl[1], &t_qx, &t_work);    ///从键盘输入各组的时间变量
    // demo input: 28 31 25 560 20 33 46
    for(i=0; i<3; i++)
    {
        scanf("%d", &t_mov[i]);
    }
    //printf("%d %d %d %d %d %d %d",t_sxl[0], t_sxl[1], t_qx, t_work, t_mov[0], t_mov[1], t_mov[2]);
    t_CNC[7]=0;
    for(i=6;i>=0;i--)
    {
        if(i%2 == 0)
        {
            t_CNC[i] = t_CNC[i+1] + t_sxl[0];
        }
        else
        {
            t_CNC[i] = t_CNC[i+1] + t_sxl[1] + t_mov[0];
        }
    }
    for(k=0;k<8;k++)
    {
        printf("t_CNC[%d]=%d ",k,t_CNC[k]);
    }
    place=1;    ///将RGV从位置4移动回到初始位置（这段时间远小于首台CNC加工完的时间）。
    printf("\n");
    for(tmr=t_CNC[0]+t_sxl[1]; tmr<=28800; tmr++)   ///宏观上以离散的秒数计时，步长为1s。
    {
        for(i=0;i<8;i++)    ///检测各CNC是否已完成加工
        {
            if(t_CNC[i] / t_work >= 1)
            {
                Rank[j++] = i+1;    ///第i+1台CNC发出待服务指令。
                /*for(k=0;k<8;k++)
                {
                    printf("Rank[%d]=%d ",k,Rank[k]);
                }*/
                t_CNC[i] = -1000;
            }
            else
            {
                t_CNC[i]++;     ///否则对第i台CNC的时间计时器+1s。
            }
        }
        if(Rank[0] == 0);    ///检测是否有CNC为待服务状态。
        else
        {
            A = place*2 - Rank[0];
            if(A==0 || A==1)  ///判断RGV是否与待服务CNC在同一工位
            {
                B=t_sxl[A] + t_qx;
                TN_CNC[Rank[0]-1][N_CNC[Rank[0]-1]++]=tmr;
                printf("%d material. time down: %d Time up: %d CNC%d#\n", N, tmr, TN_CNC[Rank[0]-1][N_CNC[Rank[0]-1]-2], Rank[0]);    ///打印第N个物料的上料时间。
                tmr = tmr + t_sxl[A] + t_qx;
                N++;    ///又放入第N+1个新物料。
                Tplus(B, t_CNC, Rank[0]);
                t_CNC[Rank[0]-1] = t_qx;
            }
            else if(A>1)
            {
                B=t_mov[A/2-1] + t_sxl[A%2] + t_qx;
                TN_CNC[Rank[0]-1][N_CNC[Rank[0]-1]++]=tmr+t_mov[A/2-1];
                printf("%d material. time down: %d Time up: %d CNC%d#\n", N, TN_CNC[Rank[0]-1][N_CNC[Rank[0]-1]-1], TN_CNC[Rank[0]-1][N_CNC[Rank[0]-1]-2], Rank[0]);
                tmr = tmr + t_mov[A/2-1] + t_sxl[A%2] + t_qx;
                N++;
                Tplus(B, t_CNC, Rank[0]);
                place-=(A/2);   ///RGV向左移动位置。
                t_CNC[Rank[0]-1] = t_qx;
            }
            else
            {
                B=t_mov[-((A-1)/2)] + t_sxl[(-A)%2] + t_qx;
                TN_CNC[Rank[0]-1][N_CNC[Rank[0]-1]++]=tmr+t_mov[-((A-1)/2)];
                printf("%d material. time down: %d Time up: %d CNC%d#\n", N, tmr+t_mov[-((A-1)/2)], TN_CNC[Rank[0]-1][N_CNC[Rank[0]-1]-2], Rank[0]);
                tmr = tmr + B;
                Tplus(B, t_CNC, Rank[0]);
                place+=(1-A)/2;     ///RGV向右移动位置。
                N++;
                t_CNC[Rank[0]-1] = t_qx;
            }
            Rankmove(Rank);
            j--;
        }
    }
}
