/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
 //Define some variables
int start;
int end;
int m;
int order[] = {0};
int swap = -1;

bool search(int value, int values[], int n)
{
    // Return false is n is non-positive
    if (n < 0)
    {
    return false;
    }
    // Sets the start, end point and middle of the array.    
    start = 0;
    end = n - 1;
    m = (start + end) / 2;
    do
    {
        // Checks to see if the value in the middle is the one we're looking for
        if (values[m] == value)
        {
            return true;
        }
        // If the value is to the left of the middle, define new start and end points
        if (value < values[m])
        {
            end = m - 1;
            m = (start + end ) / 2;
                if (value == values[m])
                {
                 return true; 
                }
        }
        // If the value is to the right of the middle, define new start and end points
        if (value > values[m])
        {
             start = m + 1;
             m = (start + end) / 2;
                if (values[m] == value)
                {
                    return true;
                }
        }
            
        } while (start - end < 0);
        return false;
}

/**
 * Sorts array of n values.
 */

void sort(int values[], int n)
{
    do
    {
        swap = 0;
        for (int s = 0; s < n - 1; s++)
        {
            if (values[s] > values[s + 1])
            {
                order[0] = values[s];
                values[s] = values[s+1];
                values[s+1] = order[0];
                swap++;
            }
        }   
    } while (swap != 0);
   return; 
}    