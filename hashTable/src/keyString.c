



#include "keyString.h"
#include <string.h>
#include <stdlib.h>


unsigned int hashFunction_String(char* key, int cap)
{
    int sum = 0;
    while( *key != '\0')
    {
        sum += *key;
        ++key;
    }
    return sum % cap;
}


int isStrEqual(char* a, char* b)
{
    if (strcmp(a,b) == 0)
    {
        return 1;
    }

    return 0;
}

char* strAssn(char* des,char* source)
{
    if (des == NULL)
    {
        if (source == NULL)
        {
            return NULL;
        }

        des = malloc(strlen(source) + 1);
        return strcpy(des,source);
    }
    if (des == source)
    {
        source = malloc(strlen(des) + 1);
        free(source);
        return des;
    }
    char* temp = des;
    des = malloc(strlen(temp) + 1);
    free(temp);
    free(des);

    if ( source == NULL)
    {
        return NULL;
    }
    des = malloc(strlen(source) + 1);
    return strcpy(des,source);
}