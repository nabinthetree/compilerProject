



#ifndef CONTAINER_H
#define CONTAINER_H


struct Container
{

    unsigned int size;
    unsigned int capacity;

    void** values;

    void*(*valAssn)(void*a,void*b);

    struct Container* lbranch;
    struct Container* rbranch;
};

struct Container* Create_Container(unsigned int cap,
                                void*(*)(void*,void*));

void AddValue(struct Container* c, void* value);

void RemoveBack(struct Container* c);

void Extend(struct Container*c);

void* conAssn(struct Container* des, struct Container* source);

int hashFunc(void* key, int size);

int conCmp(struct Container* con1, struct Container* con2);

void* Top(struct Container* c);

void Clear(struct Container* c);

void Free_Container(struct Container* c);

#endif