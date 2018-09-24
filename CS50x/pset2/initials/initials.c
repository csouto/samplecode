#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

string name;
int main(void)
{
    
//Asks for name    
name = get_string();

// Define length of input
int n =  strlen(name);

    //Iterates over each char in string
    for (int i = 0; i < n; i++)
    {
        
        //Looks for the first char and prints it (always in uppercase)
        if (i == 0)
        {
            printf("%c", toupper(name[0]));
        }

            //Looks for a 'space'in input and prints the next char (always in uppercase)
            if (name[i] == ' ')
            {
                printf("%c", toupper (name[i+1]));    
            }
    }
printf("\n");    
}