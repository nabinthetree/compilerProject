



#include "keyString.h"
#include "hashTable.h"


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


struct HashTable* Create_HashTable(unsigned int cap,
                                int(*kComp)(void*a,void*b),
                                void*(*kAssn)(void*a,void*b),
                                void*(*vAssn)(void*a,void*b),
                                int(*hFunc)(void*a,int b))
{
    size_t numBytes = 0;
    struct HashTable* hashTable;
    numBytes += 2 * (sizeof(unsigned int) + sizeof(void*));
    numBytes += sizeof(hashTable->keyComp) + sizeof(hashTable->keyAssn);
    numBytes += sizeof(hashTable->hashFunc) + sizeof(hashTable->valueAssn);

    hashTable = malloc(numBytes);
    hashTable->size = 0;
    hashTable->capacity = cap;
    hashTable->keys = malloc(cap * sizeof(void*));
    hashTable->values = malloc(cap* sizeof(void*));

    hashTable->keyAssn = kAssn;
    hashTable->hashFunc = hFunc;
    hashTable->valueAssn = vAssn;
    hashTable->keyComp = kComp;
    
    for(unsigned int i = 0; i < hashTable->capacity; ++i)
    {
        hashTable->keys[i] = NULL;
        hashTable->values[i] = NULL;
    }

    return hashTable;
}

int getKeyIndex(struct HashTable* t, void* key)
{
    if ( t == NULL || key == NULL)
    {
        return -1;
    }
    unsigned int hashIndex = (*t->hashFunc)(key,t->capacity);
    for(unsigned int i = 0; i < t->capacity; ++i)
    {
        int curIndex = (hashIndex + i) % t->capacity;
        void* curKey = t->keys[curIndex];

        if (curKey == NULL)
        {
            continue;
        }
        if ((*t->keyComp)(curKey,key))
        {
            return curIndex;
        }
    }
    return -1;
}

/* returns 0 if the key value insertion was succesful
return -1 otherwrise */

int insert_KeyValue(struct HashTable* t, void*key, void*value)
{
    int index = getKeyIndex(t,key);
    if ( index != -1)
    {
        t->values[index] = value;
        return 0;
    }

    int hashIndex = t->hashFunc(key,t->capacity);
    for (unsigned int i = 0; i < t->capacity; ++i)
    {
        int curIndex = (hashIndex + i) % t->capacity;
        if(t->keys[curIndex] == NULL)
        {
            t->keys[curIndex] = t->keyAssn(t->keys[curIndex],key);
            t->values[curIndex] = t->valueAssn(t->values[curIndex],value);
            ++t->size;
            return 0;
        }
    }
    return -1;
}

/* is key is not found then return NULL */
void* get_KeyValue(struct HashTable* t, void* key)
{
    int index = getKeyIndex(t,key);

    if ( index != -1)
    {
        return t->values[index];
    }
    return NULL;
}


/* this frees the data members but also 
the key values themselves from the heap */

void clear(struct HashTable* t)
{
    assert( t != NULL);
    for(unsigned int i = 0; i < t->capacity; ++i)
    {
        void* key = t->keyAssn(t->keys[i],t->keys[i]);
        free(key);
    }
    struct HashTable* temp = Create_HashTable(0,NULL,NULL,NULL,NULL);
    free(temp);
    free(t);
    return;
}

void keyAssn(struct HashTable* des, struct HashTable* source)
{
    assert(des != NULL && source != NULL);
    assert(des->capacity == source->capacity);
    assert(des->keyAssn == source->keyAssn);

    for (unsigned int i = 0; i < des->capacity; ++i)
    {
        des->keys[i] = des->keyAssn(des->keys[i],source->keys[i]);
    }
    return; 
}


void* Direct_Assn(void*a, void*b)
{
    return b;
}