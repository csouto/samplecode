#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main (int argc, string argv[])
{
    // If no argument is provided, return exit code 1
    if (argc < 2 || argc > 2)
    {
        printf("Please use one argument\n"); 
        return 1;
    }
    
// Ask for plain text    
printf("plaintext: ");
string ptxt = get_string();

// Convert string to int
int k = atoi(argv[1]);

int n = strlen(ptxt);

printf("ciphertext: ");    
    // Iterates over each character
    for (int i = 0; i <= n; i++)
    {
        // Ckeck if the i'th character is in lowercase
        if (islower (ptxt[i]))
        {
            // Define alphabetical order (alpha == position in alphabet minus ASCII code for the first letter - in lowercase or uppercase)
            int alpha = (int)ptxt[i] - 97;
            // Convert char to ASCII again and print the char rotation by k positions using the algorithm C = (p + k) % 26 
            printf("%c", (alpha + k) % 26 + 97);
        }
        // Check if the i'th char is in uppercase
        if (isupper (ptxt[i]))
        {
            // Same as above but in one line. I'th char - 65 (Get alphabetical position) rotated by k mod 26 + 65 (Convert back to ASCII)
            printf("%c", ( (int)ptxt[i] - 65 + k ) % 26 + 65);
        }
        // Check if the char is a space or punctuation and print without any modification
        if( isspace(ptxt[i]) || ispunct(ptxt[i]) )
        {
            printf("%c", ptxt[i]);
        }
        
    }
    printf("\n");
}