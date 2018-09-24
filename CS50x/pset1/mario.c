#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int a;
    int b = 0;
    
    do
    {
        printf("Height:");
        a = get_int();
    }
    while (a < 0 || a > 23);
    
    for (b = 0; b < a; b++)
    {
    int n = 0;
    int j = 0;
        for(n = 0; n < a - b - 1; n++)
        {
            printf(" ");
        }
   
        for(j = 0; j <= b + 1; j++)
        {
            printf("#");
        }
        
    printf("\n"); 
        
    }    
    

}
