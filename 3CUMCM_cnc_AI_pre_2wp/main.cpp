//��¼���������޹����Ҽ��롰����Ԥ�⡱�㷨����������C���Գ���

#include <stdio.h>
#include <stdlib.h>
int Tplus(int k, int p[], int q)    ///ͳһ��CNC��ʱ�������������������ϴʱ��ȡ�
{
    int m;
    for(m=0;m<4;m++)
    {
        p[m]+=k;
    }
    p[q-1]-=k;      ///��CNC�������ϡ���ϴʱ�䲻�ơ�
}
int Tplus1(int k, int p[])      ///����Ԥ��ʱ������CNC��timer��RGV���ƶ�ʱ��ȡ�
{
    int m;
    for(m=0;m<4;m++)
    {
        p[m]+=k;
    }
}
int Rankmove(int m[])   ///�������������ÿ��Ԫ��ǰ��һ����λ��
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
    int odd[4]={1,3,5,7}, even[4]={2,4,6,8}, tmr=0, t_sxl[2], t_qx, t_work[2], t_mov[3], zjbl, t_CNC_1[4]={0}, t_CNC_2[4]={-1000,-1000,-1000,-1000}, i, Rank1[4]={0}, Rank2[4]={1,2,3,4}, j=0, k=3, p, q, place=1, A, N=1, TN_CNC1[4][200]={0}, N_CNC1[4]={1,1,1,1}, TN_CNC2[4][200]={0}, N_CNC2[4]={1,1,1,1};     ///����ִ�и��������ʱ�����
    printf("Please input t_sxl[2], t_qx, t_work[2], t_mov[3] in this order:");
    scanf("%d%d%d%d%d", &t_sxl[0], &t_sxl[1], &t_qx, &t_work[0], &t_work[1]);    ///�Ӽ�����������ʱ�����
    for(i=0; i<3; i++)
    {
        scanf("%d", &t_mov[i]);
    }
    if(t_work[0]<t_work[1])     ///������һ���ڶ�������λ�ڴ��ʹ�����λ�ã�Ĭ�ϵ�һ������CNC�����ϴ��ʹ�һ�ࣩ
    {
        zjbl = t_sxl[0];
        t_sxl[0] = t_sxl[1];
        t_sxl[1] = zjbl;
        for(i=0;i<4;i++)    ///����������顣
        {
            zjbl = odd[i];
            odd[i]=even[i];
            even[i]=zjbl;
        }
    }
    t_CNC_1[3]=0;   ///��һ�ΰѵ�һ�������4̨CNCȫ����װ���Ϻ󣬸�CNC���ѹ���ʱ����㡣
    for(i=2;i>=0;i--)
    {
        t_CNC_1[i] = t_CNC_1[i+1] + t_mov[0] + t_sxl[0];
    }
    for(tmr = t_CNC_1[0] + t_sxl[0]; tmr<=28800; tmr++)    ///��ʱ�������ɢ��������
    {
        for(i=0;i<4;i++)    ///����һ�������CNC�Ƿ�����ɼӹ�
        {
            if(t_CNC_1[i] / t_work[0] >= 1)
            {
                Rank1[j++] = i+1;    ///��i+1̨CNC����������ָ�
                t_CNC_1[i] = -1000;
            }
            else
            {
                t_CNC_1[i]++;     ///����Ե�i+1̨CNC��ʱ���ʱ��+1s��
                t_CNC_2[i]++;
            }
        }
        if(Rank1[0] == 0)
        {
            q=1;
            for(p=0;p<3;p++)    ///��ȡ��켴����ɵ�һ���ӹ���CNC��š�
            {
                if(t_CNC_1[p] < t_CNC_1[p+1])
                {
                    q = p+1;
                }
            }
            if(place == q);     ///�жϸ�CNC��RGV�ľ����ϵ��
            else if(place-q>0)      ///RGV���ơ�
            {
                tmr+=t_mov[place-q];
                Tplus1(t_mov[place-q],t_CNC_1);
                Tplus1(t_mov[place-q],t_CNC_2);
                place-=q;
            }
            else    ///RGV���ơ�
            {
                tmr+=t_mov[q-place];
                Tplus1(t_mov[q-place],t_CNC_1);
                Tplus1(t_mov[q-place],t_CNC_2);
                place+=q;
            }
        }
        else    ///�������1����CNC������
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
                for(p=0;p<3;p++)    ///��ȡ������ɵڶ����ӹ���CNC��š�
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

