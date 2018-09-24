#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main (int argc, string argv[]) 
{
    // Allow only two arguments, exiting program otherwise
    if (argc != 2)
    {
        printf("Wrong arguments!\n");
        return 1;
    }

// Calculates the length of argument one (key)
int cl = strlen(argv[1]);

    // Iterates over each char in argument one (key) to identify invalid chars
    for (int ia = 0; ia < cl; ia++)
    {
        if (ispunct (argv[1][ia]) || isdigit (argv[1][ia]))
        {
            printf("Invalid char!\n");
            return 1;
        }
    }

// Print word "plaintext and waits for user's input
printf("plaintext:");
string ptxt = get_string();

int pl = strlen(ptxt); // Length of plaintext

int order[200]; // Define an int array to create a new index for alphabetical characters

printf("ciphertext:");
int k = 0; // Int k will be used to iterate over characters in key (k)

    // Iterates over characters in argv (k) and ptxt (p) to apply the algorithm C = (pi + kj) % 26
    for (int i = 0; i < pl; i++)
    {
        // Create an index for characters in argv[1] (key). From 0 to 26, characters will be numbered in alphabetical order.
        if (islower(argv[1][i])) 
        {
          order[i] = (int)(char)argv[1][i] - 97;
        }
        if (isupper(argv[1][i]))
        {
            order[i] = (int)(char)argv[1][i] - 65;
        }
        
        // Print characters applying algorithm (reusing letters in key). Int k is used here to only iterate over key if the i'th 
        // character of plaintext is a letter. Indexed alpha characters are converted back to ASCII.
        if (islower(ptxt[i]))
        {
            printf("%c", (ptxt[i] - 97 + order[k % cl]) % 26 + 97);
            k++;
        }
        if (isupper(ptxt[i]))
        {
            printf("%c", (ptxt[i] - 65 + order[k % cl]) % 26 + 65);
            k++;
        }
        
        // If the i'th character of plain text is not a letter, print unchanged.
        if (ispunct(ptxt[i]) || isspace(ptxt[i]))
        {
            printf("%c", ptxt[i]);
        }

    }
//Print a newline.
printf("\n"); 
    
}
