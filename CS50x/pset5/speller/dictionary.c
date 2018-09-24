/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include "dictionary.h"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <strings.h>
// Size of hashtable
#define SIZE 65536

// Prototype of hash function
int hash(const char* word);

// Define node structure 
typedef struct node
{
    char word[LENGTH+1];
    struct node* next ;
}
node;

// Used to store the dictionary size
unsigned int dict = 0;

// Create array of node*
node *hashtable[SIZE];

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // Hash the word and create a pointer to bucket where it's (supposedly) stored
    node *cursor = hashtable[hash(word)]; 
    
    // Iterate over linked list
    while (cursor != NULL)
    {
        // Compare word from text with words in linked list (ignoring case)
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
        // Move to next node                
        cursor = cursor->next;
    }
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // Open dictonary file
    FILE *dc = fopen(dictionary, "r");
    if (dc == NULL)
    {
        printf("Could not open %s", dictionary);
        unload();
        return false;
    }
    // Array to temporaly store words from dictionary file
    char word[LENGTH+1];
    
    // Populate hashtable w/ pointers to nodes
    for (int i = 0; i < SIZE; i++)
    {
        hashtable[i] = malloc(sizeof(node));
        
        // Initialize pointees
        hashtable[i]->next = NULL;
        strcpy(hashtable[i]->word, "cs50");
        
        if (hashtable[i] == NULL)
        {
            // unload();
            dict = 0;
            return false;
        }
    }
    
    // Scan dict file word by word, storing content in word
    while (fscanf(dc, "%s", word) != EOF)
    {
        // Increase word counter
        dict++;
        
        // Hash current word
        unsigned int pos = hash(word);
        
        // Create new node
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            // unload(); // use free instead?
            dict = 0;
            return false;
        }
        
        //Copy current word into node
        strcpy(new_node->word, word);
        
        // // Insert new node into hashtable
        new_node->next = hashtable[pos];
        hashtable[pos] = new_node;
    }
    fclose(dc);
    return true;
    
    
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return dict;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // Iterate over hashtable
    for (int i = 0; i < SIZE; i++)
    {
        // Point cursor to the head of linked list
        node *cursor = hashtable[i];
        
        // Walk through linked list, create a temp pointer (avoid losing track), move to next node and free the memory space.
        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    return true;    

    
}


// Hash function found (and slightly modified) in https://www.reddit.com/r/cs50/comments/1x6vc8/pset6_trie_vs_hashtable/cf9nlkn/
int hash(const char* word)
{
    unsigned int hashval = 0;
    for (int i=0, n=strlen(word); i<n; i++)
    {
        hashval = (hashval << 2) ^ tolower(word[i]);
    }
    return hashval % SIZE;
}
