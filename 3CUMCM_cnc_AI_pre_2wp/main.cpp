//附录三、假设无故障且加入“智能预测”算法的两道工序C语言程序：

#include <stdio.h>
#include <stdlib.h>
int Tplus(int k, int p[], int q)    ///统一对CNC的时间计数器加上下料与清洗时间等。
{
    int m;
    for(m=0;m<4;m++)
    {
        p[m]+=k;
    }
    p[q-1]-=k;      ///本CNC的上下料、清洗时间不计。
}
int Tplus1(int k, int p[])      ///智能预测时对所有CNC的timer加RGV的移动时间等。
{
    int m;
    for(m=0;m<4;m++)
    {
        p[m]+=k;
    }
}
int Rankmove(int m[])   ///将待服务队列中每个元素前移一个单位。
{
    int z;
    for(z=0;z<4;z++)
    {
        m[z]=m[z+1];
    }
    m[3]=0;
}
int main()
{
    int odd[4]={1,3,5,7}, even[4]={2,4,6,8}, tmr=0, t_sxl[2], t_qx, t_work[2], t_mov[3], zjbl, t_CNC_1[4]={0}, t_CNC_2[4]={-1000,-1000,-1000,-1000}, i, Rank1[4]={0}, Rank2[4]={1,2,3,4}, j=0, k=3, p, q, place=1, A, N=1, TN_CNC1[4][200]={0}, N_CNC1[4]={1,1,1,1}, TN_CNC2[4][200]={0}, N_CNC2[4]={1,1,1,1};     ///定义执行各道工序的时间变量
    printf("Please input t_sxl[2], t_qx, t_work[2], t_mov[3] in this order:");
    scanf("%d%d%d%d%d", &t_sxl[0], &t_sxl[1], &t_qx, &t_work[0], &t_work[1]);    ///从键盘输入各组的时间变量
    for(i=0; i<3; i++)
    {
        scanf("%d", &t_mov[i]);
    }
    if(t_work[0]<t_work[1])     ///调整第一、第二道工序位于传送带上下位置（默认第一道工序CNC在上料传送带一侧）
    {
        zjbl = t_sxl[0];
        t_sxl[0] = t_sxl[1];
        t_sxl[1] = zjbl;
        for(i=0;i<4;i++)    ///调换标记数组。
        {
            zjbl = odd[i];
            odd[i]=even[i];
            even[i]=zjbl;
        }
    }
    t_CNC_1[3]=0;   ///第一次把第一道工序的4台CNC全部加装物料后，各CNC的已工作时间计算。
    for(i=2;i>=0;i--)
    {
        t_CNC_1[i] = t_CNC_1[i+1] + t_mov[0] + t_sxl[0];
    }
    for(tmr = t_CNC_1[0] + t_sxl[0]; tmr<=28800; tmr++)    ///将时间变量离散化计数。
    {
        for(i=0;i<4;i++)    ///检测第一道工序各CNC是否已完成加工
        {
            if(t_CNC_1[i] / t_work[0] >= 1)
            {
                Rank1[j++] = i+1;    ///第i+1台CNC发出待服务指令。
                t_CNC_1[i] = -1000;
            }
            else
            {
                t_CNC_1[i]++;     ///否则对第i+1台CNC的时间计时器+1s。
                t_CNC_2[i]++;
            }
        }
        if(Rank1[0] == 0)
        {
            q=1;
            for(p=0;p<3;p++)    ///获取最快即将完成第一道加工的CNC编号。
            {
                if(t_CNC_1[p] < t_CNC_1[p+1])
                {
                    q = p+1;
                }
            }
            if(place == q);     ///判断该CNC与RGV的距离关系。
            else if(place-q>0)      ///RGV左移。
            {
                tmr+=t_mov[place-q];
                Tplus1(t_mov[place-q],t_CNC_1);
                Tplus1(t_mov[place-q],t_CNC_2);
                place-=q;
            }
            else    ///RGV右移。
            {
                tmr+=t_mov[q-place];
                Tplus1(t_mov[q-place],t_CNC_1);
                Tplus1(t_mov[q-place],t_CNC_2);
                place+=q;
            }
        }
        else    ///服务队列1中有CNC待服务。
        {
            if(place == Rank1[0])
            {
                A = t_sxl[0];
                TN_CNC1[Rank1[0]-1][N_CNC1[Rank1[0]-1]++] = tmr;
                tmr = tmr + A;
                Tplus(A,t_CNC_1,Rank1[0]);
                Tplus1(A,t_CNC_2);
                t_CNC_1[Rank1[0]-1] = 0;
            }
            else if(place - Rank1[0]>0)
            {
                A = t_mov[place-Rank1[0]] + t_sxl[0];
                TN_CNC1[Rank1[0]-1][N_CNC1[Rank1[0]-1]++] = tmr + t_mov[place-Rank1[0]];
                tmr += A;
                Tplus(A,t_CNC_1,Rank1[0]);
                Tplus1(A,t_CNC_2);
                t_CNC_1[Rank1[0]-1] = 0;
                place-=Rank1[0];
            }
            else
            {
                A = t_mov[Rank1[0]-place] + t_sxl[0];
                TN_CNC1[Rank1[0]-1][N_CNC1[Rank1[0]-1]++] = tmr + t_mov[Rank1[0]-place];
                tmr+=A;
                Tplus(A,t_CNC_1,Rank1[0]);
                Tplus1(A,t_CNC_2);
                t_CNC_1[Rank1[0]-1] = 0;
                place+=Rank1[0];
            }
            zjbl=Rank1[0];
            Rankmove(Rank1);
            j--;
            for(i=0;i<4;i++)
            {
                if(t_CNC_2[i] / t_work[1] >= 1)
                {
                    Rank2[k++] = i+1;
                    t_CNC_2[i] = -1000;
                }
                else
                {
                    t_CNC_1[i]++;
                    t_CNC_2[i]++;
                }
            }
            if(Rank2[0] == 0)
            {
                q=0;
                for(p=0;p<3;p++)    ///获取即将完成第二道加工的CNC编号。
                {
                    if(t_CNC_2[p] < t_CNC_2[p+1])
                    {
                        q = p+1;
                    }
                }
                if(place == p);
                else if(place-p>0)
                {
                    tmr+=t_mov[place-p];
                    Tplus1(t_mov[place-p],t_CNC_1);
                    Tplus1(t_mov[place-p],t_CNC_2);
                    place-=p;
                }
                else
                {
                    tmr+=t_mov[p-place];
                    Tplus1(t_mov[p-place],t_CNC_1);
                    Tplus1(t_mov[p-place],t_CNC_2);
                    place+=p;
                }
            }
            else
            {
                if(Rank2[0] == place)
                {
                    A = t_sxl[1] + t_qx;
                    TN_CNC2[Rank2[0]-1][N_CNC2[Rank2[0]-1]++] = tmr;
                    printf("%d material CNC1: CNC%d# First step Time down: %5d up: %5d CNC2: CNC%d# Second Time down: %5d up: %5d\n",N, odd[zjbl-1], TN_CNC1[zjbl-1][N_CNC1[zjbl-1]-1], TN_CNC1[zjbl-1][N_CNC1[zjbl-1]-2], even[Rank2[0]-1], TN_CNC2[Rank2[0]-1][N_CNC2[Rank2[0]-1]-1], TN_CNC2[Rank2[0]-1][N_CNC2[Rank2[0]-1]-2]);
                    tmr = tmr + A;
                    Tplus(A,t_CNC_2,Rank2[0]);
                    Tplus1(A,t_CNC_1);
                    t_CNC_2[Rank2[0]-1] = t_qx;
                    N++;
                }
                else if(place-Rank2[0]>0)
                {
                    A = t_mov[place-Rank2[0]] + t_sxl[1] + t_qx;
                    TN_CNC2[Rank2[0]-1][N_CNC2[Rank2[0]-1]++] = tmr + t_mov[place-Rank2[0]];
                    printf("%d material CNC1: CNC%d# First step Time down: %5d up: %5d CNC2: CNC%d# Second Time down: %5d up: %5d\n",N, odd[zjbl-1], TN_CNC1[zjbl-1][N_CNC1[zjbl-1]-1], TN_CNC1[zjbl-1][N_CNC1[zjbl-1]-2], even[Rank2[0]-1], TN_CNC2[Rank2[0]-1][N_CNC2[Rank2[0]-1]-1], TN_CNC2[Rank2[0]-1][N_CNC2[Rank2[0]-1]-2]);
                    tmr += A;
                    Tplus(A,t_CNC_2,Rank2[0]);
                    Tplus1(A,t_CNC_1);
                    t_CNC_2[Rank2[0]-1] = t_qx;
                    place-=Rank2[0];
                    N++;
                }
                else
                {
                    A = t_mov[Rank2[0]-place] + t_sxl[1] + t_qx;
                    TN_CNC2[Rank2[0]-1][N_CNC2[Rank2[0]-1]++] = tmr + t_mov[Rank2[0]-place];
                    printf("%d material CNC1: CNC%d# First step Time down: %5d up: %5d CNC2: CNC%d# Second Time down: %5d up: %5d\n",N, odd[zjbl-1], TN_CNC1[zjbl-1][N_CNC1[zjbl-1]-1], TN_CNC1[zjbl-1][N_CNC1[zjbl-1]-2], even[Rank2[0]-1], TN_CNC2[Rank2[0]-1][N_CNC2[Rank2[0]-1]-1], TN_CNC2[Rank2[0]-1][N_CNC2[Rank2[0]-1]-2]);
                    tmr += A;
                    Tplus(A,t_CNC_2,Rank2[0]);
                    Tplus1(A,t_CNC_1);
                    t_CNC_2[Rank1[0]-1] = t_qx;
                    place += Rank2[0];
                    N++;
                }
                Rankmove(Rank2);
                k--;
            }
        }
    }
}

