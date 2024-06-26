





#include "hashTable.h"
#include "blocks.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int tableSize = 10;
int numKeys = 0;
char* keys[10];
struct Instruction* keyInstr[10];


int hashFunction(char* key)
{
    int sum = 0;
    while (*key != '\0')
    {
        sum += *key;
        ++key;
    }
    return sum % tableSize;
}

int getKeyIndex(char* key)
{

    int hashVal = hashFunction(key);
    int isKeyInserted = 0;
    for(int i = 0; i < tableSize; ++i)
    {
        int index = (hashVal + i) % tableSize;

        if (keys[index] == NULL)
        {
            keys[index] = malloc(strlen(key));
            strcpy(keys[index],key);
            ++numKeys;
            return index;
        }
        if (strcmp(keys[index],key) == 0)
        {
            return index;
        }
    }
    return -1;
}


void setKeyInstruction(char* key, struct Instruction* instr)
{
    int index = getKeyIndex(key);
    if ( index < 0)
    {
        printf("key is not in the HashTable");
    }
    keyInstr[getKeyIndex(key)] = instr;
}

struct Instruction* getKeyInstruction(char* key)
{
    
    return keyInstr[getKeyIndex(key)];
}


