/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */

void init(void)
{
    // Sets the highest number
    int n = d * d - 1;
    // Iterates over each row
    for (int i = 0; i < d; i++)
    {
        // Then iterates over each column
        for (int j = 0; j < d; j++)
        {
            // Set value in each tie, decreasing its value
            board[i][j] = n;
            n--;
        }
    }
// Invert 2 and 1 if table's dimension is even
    if(d * d % 2 == 0)
    {
        board[d - 1][d - 3] = 1;
        board[d - 1][d - 2] = 2;
    }
    
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // Iterate over each row
    for (int i = 0; i < d; i++)
    {
        // Iterate over each column
        for (int j = 0; j < d; j++)
        {
            // Checks if the current char is zero and prints a underscore
            if (board[i][j] == 0)
            {
                printf(" _ ");
            }
            // If the current char is not zero, print it
            else
            {
            printf("%2i", board[i][j]);
            printf(" ");
            }
            
        }
    printf("\n");
        
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
int blankr;
int blankc;
int tiler;
int tilec;
    // Find tile
    // Iterate over each row
    for (int i = 0; i < d; i++)
    {
        // Iterate over each column
        for (int j = 0; j < d; j++)
        {
            // Look for the blank tile
            if (board[i][j] == 0)
            {
                // Store location of blank tile
                blankr = i;
                blankc = j;
            }
            // Look for the tile that will be moved
            if (board[i][j] == tile)
            {
                // Store the location of tile
                tiler = i;
                tilec = j;
            }
        }
    }
    // Check if the move is legal
    if ((tiler == (blankr - 1) && tilec == blankc) || (tiler == (blankr + 1) && tilec == blankc) || (tiler == blankr && tilec == (blankc + 1)) || (tiler == blankr && tilec == (blankc - 1)))
    {
        // Swap
        board[blankr][blankc] = tile;
        board[tiler][tilec] = 0;
        return true;
    }
    else
    {
    return false;
    }
    
    
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
// Start the counter (Tiles are in increasing order, from 1 to d * d - 1) 
int c = 0;
   // Iterate over each row
   for (int i = 0; i < d; i++)
   {
       // Iterate over each column
       for (int j = 0; j < d; j++)
       {
            // Proceeds only if current tile is not the last (board[d-1][d-1])
            if (!(i == d - 1 && j == d - 1))
            {
               c++;
                // Check if the current tile is different from counter
                if (board[i][j] != c)
                {
                    return false;
                }
            }
       }
   }
   return true;
}
