//��¼����������1%�ĸ��ʳ��ֹ����Ҽ��롰����Ԥ�⡱�롰�ͽ�ԭ���㷨����������C���Գ���

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int Tplus(int k, int p[], int q)    ///ͳһ��CNC��ʱ�������������������ϴʱ��ȡ�
{
    int m;
    for(m=0;m<4;m++)
    {
        p[m]+=k;
    }
    p[q-1]-=k;      ///��CNC�������ϡ���ϴʱ�䲻�ơ�
}
void SmartRankmove(int m[], int pla)
{
    int x;
    for(x = pla-1;x<3;x++)
    {
        m[x]=m[x+1];
    }
    m[3]=0;
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
    int odd[4]={1,3,5,7}, even[4]={2,4,6,8}, a, b, c, d, BREDOW[200][4], BREDOWs[200][4], h=0, g=0, l, m, pl, tmr=0, t_sxl[2], t_qx, t_work[2], t_mov[3], zjbl, t_CNC_1[4]={0}, t_CNC_2[4]={-1000,-1000,-1000,-1000}, i, Rank1[4]={0}, Rank2[4]={1,2,3,4}, j=0, k=3, p, q, place=1, A, N=1, TN_CNC1[4][200]={0}, N_CNC1[4]={1,1,1,1}, TN_CNC2[4][200]={0}, N_CNC2[4]={1,1,1,1};     ///����ִ�и��������ʱ�����
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
                c=rand();
                d=rand();
                ///printf("c,d:%d %d\n",c, d);
                if(c%100 == 99)     /// ����CNC������ϻ���
                {
                    t_CNC_1[i] = -(600 + (int)(600*(d/(RAND_MAX+1.0))));
                    BREDOW[h][0]=N+1;
                    BREDOW[h][1]=i+1;
                    BREDOW[h][2]=tmr;
                    BREDOW[h++][3]=tmr-t_CNC_1[i]; ///��¼������������
                }
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
            pl=1;
            for(l=0;Rank1[l] != 0;l++);     ///���Թ���1�ķ��������CNC������
            for(m=0;m<(l-1);m++)    ///�ҵ������о���RGV�����CNC��
            {
                if(abs(place - Rank1[m])>abs(place - Rank1[m+1]))
                {
                    pl=m+2;
                }
            }

            if(place == Rank1[pl-1])
            {
                A = t_sxl[0];
                TN_CNC1[Rank1[pl-1]-1][N_CNC1[Rank1[pl-1]-1]++] = tmr;
                tmr = tmr + A;
                Tplus(A,t_CNC_1,Rank1[pl-1]);
                Tplus1(A,t_CNC_2);
                t_CNC_1[Rank1[pl-1]-1] = 0;
            }
            else if(place - Rank1[pl-1]>0)
            {
                A = t_mov[place-Rank1[pl-1]] + t_sxl[0];
                TN_CNC1[Rank1[pl-1]-1][N_CNC1[Rank1[pl-1]-1]++] = tmr + t_mov[place-Rank1[pl-1]];
                tmr += A;
                Tplus(A,t_CNC_1,Rank1[pl-1]);
                Tplus1(A,t_CNC_2);
                t_CNC_1[Rank1[pl-1]-1] = 0;
                place-=Rank1[pl-1];
            }
            else
            {
                A = t_mov[Rank1[pl-1]-place] + t_sxl[0];
                TN_CNC1[Rank1[pl-1]-1][N_CNC1[Rank1[pl-1]-1]++] = tmr + t_mov[Rank1[pl-1]-place];
                tmr+=A;
                Tplus(A,t_CNC_1,Rank1[pl-1]);
                Tplus1(A,t_CNC_2);
                t_CNC_1[Rank1[pl-1]-1] = 0;
                place+=Rank1[pl-1];
            }
            zjbl=Rank1[pl-1];
            SmartRankmove(Rank1, pl);
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
                    a=rand();
                    b=rand();
                    if(a%100 == 99) /// ����CNC������ϻ���
                    {
                        t_CNC_2[i] = -(600 + (int)(600*(b/(RAND_MAX+1.0))));
                        BREDOWs[g][0]=N+1;
                        BREDOWs[g][1]=i+1;
                        BREDOWs[g][2]=tmr;
                        BREDOWs[g++][3]=tmr-t_CNC_1[i]; ///��¼����2������������
                    }
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
                pl=1;
                for(l=0;Rank2[l] != 0;l++);     ///���Թ���1�ķ��������CNC������
                for(m=0;m<(l-1);m++)    ///�ҵ������о���RGV�����CNC��
                {
                    if(abs(place - Rank2[m])>abs(place - Rank2[m+1]))
                    {
                        pl=m+2;
                    }
                }
                if(Rank2[pl-1] == place)
                {
                    A = t_sxl[1] + t_qx;
                    TN_CNC2[Rank2[pl-1]-1][N_CNC2[Rank2[pl-1]-1]++] = tmr;
                    printf("%d material CNC1: CNC%d# First step Time down: %5d up: %5d CNC2: CNC%d# Second Time down: %5d up: %5d\n",N, odd[zjbl-1], TN_CNC1[zjbl-1][N_CNC1[zjbl-1]-1], TN_CNC1[zjbl-1][N_CNC1[zjbl-1]-2], even[Rank2[pl-1]-1], TN_CNC2[Rank2[pl-1]-1][N_CNC2[Rank2[0]-1]-1], TN_CNC2[Rank2[0]-1][N_CNC2[Rank2[0]-1]-2]);
                    tmr = tmr + A;
                    Tplus(A,t_CNC_2,Rank2[pl-1]);
                    Tplus1(A,t_CNC_1);
                    t_CNC_2[Rank2[pl-1]-1] = t_qx;
                    N++;
                }
                else if(place-Rank2[pl-1]>0)
                {
                    A = t_mov[place-Rank2[pl-1]] + t_sxl[1] + t_qx;
                    TN_CNC2[Rank2[pl-1]-1][N_CNC2[Rank2[pl-1]-1]++] = tmr + t_mov[place-Rank2[pl-1]];
                    printf("%d material CNC1: CNC%d# First step Time down: %5d up: %5d CNC2: CNC%d# Second Time down: %5d up: %5d\n",N, odd[zjbl-1], TN_CNC1[zjbl-1][N_CNC1[zjbl-1]-1], TN_CNC1[zjbl-1][N_CNC1[zjbl-1]-2], even[Rank2[pl-1]-1], TN_CNC2[Rank2[pl-1]-1][N_CNC2[Rank2[pl-1]-1]-1], TN_CNC2[Rank2[pl-1]-1][N_CNC2[Rank2[pl-1]-1]-2]);
                    tmr += A;
                    Tplus(A,t_CNC_2,Rank2[pl-1]);
                    Tplus1(A,t_CNC_1);
                    t_CNC_2[Rank2[pl-1]-1] = t_qx;
                    place-=Rank2[pl-1];
                    N++;
                }
                else
                {
                    A = t_mov[Rank2[pl-1]-place] + t_sxl[1] + t_qx;
                    TN_CNC2[Rank2[pl-1]-1][N_CNC2[Rank2[pl-1]-1]++] = tmr + t_mov[Rank2[pl-1]-place];
                    printf("%d material CNC1: CNC%d# First step Time down: %5d up: %5d CNC2: CNC%d# Second Time down: %5d up: %5d\n",N, odd[zjbl-1], TN_CNC1[zjbl-1][N_CNC1[zjbl-1]-1], TN_CNC1[zjbl-1][N_CNC1[zjbl-1]-2], even[Rank2[pl-1]-1], TN_CNC2[Rank2[pl-1]-1][N_CNC2[Rank2[pl-1]-1]-1], TN_CNC2[Rank2[pl-1]-1][N_CNC2[Rank2[pl-1]-1]-2]);
                    tmr += A;
                    Tplus(A,t_CNC_2,Rank2[pl-1]);
                    Tplus1(A,t_CNC_1);
                    t_CNC_2[Rank1[pl-1]-1] = t_qx;
                    place += Rank2[pl-1];
                    N++;
                }
                SmartRankmove(Rank2, pl);
                k--;
            }
        }
    }
    for(p=0;p<h;p++)
    {
        for(m=0;m<4;m++)
        {
            printf("%d ", BREDOW[p][m]);
        }
        printf("\n");
    }
    printf("--------\n");
    for(p=0;p<g;p++)
    {
        for(m=0;m<4;m++)
        {
            printf("%d ", BREDOWs[p][m]);
        }
        printf("\n");
    }
}

