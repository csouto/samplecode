#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
float o;
int q = 25;
int d = 10;
int n = 5;
int p = 1;
int sq = 0;
int sd = 0;
int sn = 0;
int sp = 0;

    do
    {
       printf("O hai! How much change is owed?\n"); 
       o = get_float();
    }
    while(o < 0);

float (x) = o * 100; 
int c = round(x);

    if (c >= q)
    {
        sq = c / q;
        c = c % q;
    }
    if (c >= d)
    {
        sd = c / d;
        c = c % d;
    }
    if(c >= n)
    {
        sn = c / n;
        c = c % n;
    }
    if(c >= p)
    {
        sp = c / p;
    }
    
int t = sq + sd + sn + sp;
printf("%i\n", t);
    
}