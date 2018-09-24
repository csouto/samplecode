/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames and n (factor)
    int n = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];
    

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // Save padding for infile
    int oldpadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    //Save original dimensions
    int w = bi.biWidth;
    int h = abs(bi.biHeight);
    
    // Modify headers 
    
    bi.biWidth *= n;
    bi.biHeight *= n;

    // Calculate padding for new file
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // Recalculate biSizeImage and bfSize with new padding
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + padding) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    // iterate over each scanline
    for (int i = 0; i < h; i++)
    {
        // Set counter to store pixels's order
        int r = 0;
        
        // Array to store an entire line
        RGBTRIPLE row[bi.biWidth];
        
        // iterate over pixels in scanline
        for (int j = 0; j < w; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
            // Save pixel n times on array
            for (int x = 0; x < n; x++)
            {
                row[r] = triple;
                r++;
            }
        }
        // Repeat n times
        for (int y = 0; y < n; y++)
        {
            // Write array on file
            for (int a = 0; a < bi.biWidth; a++)
            {
                    RGBTRIPLE tpl;
                    tpl = row[a];
                    fwrite(&tpl, sizeof(RGBTRIPLE), 1, outptr);
            }
            // Write padding 
            for (int k = 0; k < padding; k++)
            {
                fputc(0x00, outptr);
            }
        }
        // skip over padding (infile), if any
        fseek(inptr, oldpadding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);
    // success
    return 0;
}
