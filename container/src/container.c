




#include "container.h"

#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>


struct Container* Create_Container(unsigned int cap, 
                                    void*(*vAssn)(void*a,void*b))
{
    struct Container* c = NULL;
    c = malloc(sizeof(struct Container));
    c->values = malloc(sizeof(void*) * cap);
    for(unsigned int i = 0; i < cap; ++i)
    {
        c->values[i] = NULL;
    }
    c->size = 0;
    c->capacity = cap;
    c->valAssn = vAssn;
    c->lbranch = NULL;
    c->rbranch = NULL;
    return c;
}

void AddValue(struct Container* c, void* value)
{

    if ( c->size >= c->capacity)
    {
        /* call extend here :D */
        Extend(c);
    }
    c->values[c->size] = c->valAssn(c->values[c->size],value);
    ++c->size;
    return;
}


void RemoveBack(struct Container* c)
{

    if ( c->size >= 1)
    {
        --c->size;
    }

    return;
}

void Extend(struct Container* c)
{
    if ( c->size >= c->capacity)
    {
        void** newValues = malloc(sizeof(void*) * 2 * c->capacity);

        for (unsigned int i = 0; i < c->size; ++i)
        {
            newValues[i] = c->valAssn(newValues[i],c->values[i]);
        }
        void** val = malloc(sizeof(void*) * c->capacity);
        free(c->values);
        free(val);
        c->capacity = c->capacity * 2;
        c->values = newValues;   
    }  
}

void* conAssn(struct Container* des, struct Container* source)
{
    return source;
}

int hashFunc(void* key, int size)
{

    unsigned long int address = pow((unsigned long int)key,2);

    unsigned long int midIndex = sizeof(unsigned long int) * 4; // 8 bits but then divide by 4 since I want the midposition
    unsigned int middle_r = (unsigned int)log2(size) / 2;
    int lefPos = midIndex - middle_r;
    address = address<<(lefPos - 1);
    address = address>>(2*lefPos - 1);

    return address;
    
}

int conCmp(struct Container* con1, struct Container* con2)
{
    return con1 == con2;
}

void* Top(struct Container* c)
{
    if ( c == NULL || c->size <= 0)
    {
        return NULL;
    }
    //assert(c != NULL);
    //assert(c->size > 0);
    return c->values[c->size - 1];
}

void Clear(struct Container* c)
{
    while(c->size > 0)
    {
        RemoveBack(c);
    }
}



void Free_Container(struct Container* c)
{
    /* this does not remove memory if
    void* are dynamically as well */ 
    free(c->values);
    free(c);

}