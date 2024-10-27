// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Word Counter
int word_counter=0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    unsigned int index = hash(word);

    node *ptr = table[index];

    while(ptr != NULL){
        if(strcasecmp(ptr->word, word)== 0){
            return true;
        }
        ptr = ptr->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return toupper(word[0]) - 'A';
    
    // unsigned int hash = 0;
    // for(int i=0; word[i]!='\0'; i++){
    //     hash = (hash << 2)^word[i];
    // }

    // return hash%N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *file = fopen(dictionary, "r");
    if(file == NULL){
        // printf("Could not open Dictionary");
        return false;
    }

    char word[LENGTH+1];
    // char word;
    while (fscanf(file, "%s", word)!= EOF){
        node *new_word = malloc(sizeof(node));

        if(new_word == NULL){
            fclose(file);
            return false;
        }

        strcpy(new_word->word, word);
        new_word->next = NULL;

        unsigned int index = hash(word);
        new_word->next = table[index];
        table[index] = new_word;
        word_counter++;

    }

    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return word_counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for(int i=0; i<N; i++){
        node *ptr = table[i];

        while(ptr != NULL){
            node *temp = ptr;
            ptr = ptr->next;
            free(temp);
        }
    }

    return true;
}
