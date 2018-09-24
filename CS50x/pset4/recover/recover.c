#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    // Check if arguments are correct
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover filename\n");
        return 1;
    }
    
    // Create file pointer/open file
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Unable to open file\n");
        return 2;
    }
        
    // Create buffer
    unsigned char buffer[512];
    
    // Array to store filename
    char filename[8];

    // JPEG counter, increased each time a JPEG header is found
    int c = 0;
    
    // Define filename, create file pointer and open file
    sprintf(filename, "%03i.jpg", c);
    FILE *outptr = fopen(filename, "w");
    
    // Loop until EOF
    while((fread(&buffer, 512, 1, inptr)) == 1)
    {

        // Look for first bits of a JPEG file
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // If a JPEG header was found previously, close the previous file and start a new one
            if (c > 0)
            {
                fclose(outptr);
                sprintf(filename, "%03i.jpg", c);
                fopen(filename, "w");
                fwrite(&buffer, 512, 1, outptr);
            } 
            // If its the first JPEG, write its header
            if (c == 0)
            {
                fwrite(&buffer, 512, 1, outptr);
            }
            // Increase the counter of JPEG headers
            c++;
        }    
        // Condition to deal with blocks that are not headers. (Write only if a header was found previously)
        else
        {
            if (c > 0)
            {
                fwrite(&buffer, 512, 1, outptr);
            }
        }
        
    }    
    // Close the remaining files
    fclose(inptr);
    fclose(outptr);
}