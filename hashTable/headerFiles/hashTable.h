


#ifndef HASH_TABLE_H
#define HASH_TABLE_H



struct HashTable 
{
    unsigned int size;
    unsigned int capacity;

    void** keys;
    void** values;

    int(*keyComp)(void*a,void*b);
    void*(*keyAssn)(void*a,void*b);
    void*(*valueAssn)(void*a,void*b);
    int(*hashFunc)(void*a,int b);
    
};


struct HashTable* Create_HashTable(unsigned int, int(*)(void*,void*),
                                    void*(*)(void*,void*),
                                    void*(*)(void*,void*),
                                    int(*)(void*,int));

int getKeyIndex(struct HashTable*, void*);

int insert_KeyValue(struct HashTable*, void*,void*);

void* get_KeyValue(struct HashTable*,void*);

unsigned int size(struct HashTable*);

void clear(struct HashTable*);

void keyAssn(struct HashTable* des, struct HashTable* source);

void* Direct_Assn(void*a, void*b);










#endif