#include <cs50.h>
#include <stdio.h>

int main(void)
{
    printf("Minutes: "); 
    int min = get_int(); 
    int btt = min * 12;
    printf("Bottles: %i\n", btt); 
}