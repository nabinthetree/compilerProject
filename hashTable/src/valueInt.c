


#include "valueInt.h"
#include <stdlib.h>
#include <stdio.h>



int* intAssn(int* des, int* source)
{
    int * temp = malloc(sizeof(int));
    if ( des != source)
    {
        free(des);
    }
    free(temp);
    
    des = malloc(sizeof(int));
    *des = *source;

    return des;
}

int intComp(int* val1, int* val2)
{
    if (val1 == NULL || val2 == NULL)
    {
        return 0;
    }
    return *val1 == *val2;
}


int hashInt(int* key, int size)
{
    int hashKey = *key;

    return (hashKey * (hashKey + 3)) % size;
}