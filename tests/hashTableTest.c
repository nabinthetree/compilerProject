







#include "container.h"
#include "instruction.h"
#include "hashTable.h"
#include "keyString.h"
#include "valueInt.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

void Test_KeyAssn_HashTable()
{
    struct HashTable* h1 = Create_HashTable(10,&isStrEqual,&strAssn,&intAssn,
                                                    &hashFunction_String);
    struct HashTable* h2 = Create_HashTable(10,&isStrEqual,&strAssn,&intAssn,
                                                    &hashFunction_String);
    struct HashTable* h3 = Create_HashTable(10,&isStrEqual,&strAssn,&intAssn,
                                                    &hashFunction_String);
    char* alphabet[10] = {"a","b","c","d","e","f","g","h","i","j"};
    char* alphabet2[10] = {"k","l","m","n","o","p","q","r","s","t"};
    
    for (unsigned int i = 0; i < 10; ++i)
    {
        insert_KeyValue(h2,alphabet[i],&i);
        insert_KeyValue(h3,alphabet2[i],&i);
    }
    keyAssn(h1,h2);

    for (unsigned int i = 0; i < 10; ++i)
    {
        assert(isStrEqual(h1->keys[i],h2->keys[i]));
    }
    keyAssn(h1,h3);

    for (unsigned int i = 0; i < 10; ++i)
    {
        assert(isStrEqual(h1->keys[i],h3->keys[i]));
        assert(isStrEqual(h1->keys[i],h2->keys[i]) == 0);
    }

    printf("Test KeyAssn Hash Passed\n");
    return;
}

void Test_Clear_Hash()
{

    struct HashTable* h1 = Create_HashTable(15,&isStrEqual,&strAssn,&intAssn,&hashFunction_String);
    
    char* nouns[15] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o"};
    int* values[15];
    char* removed[15];
    for (int i = 0; i < 15; ++i)
    {
        insert_KeyValue(h1,nouns[i],&i);
        values[i] = get_KeyValue(h1,nouns[i]);
        removed[i] = h1->keys[getKeyIndex(h1,nouns[i])];
    }
    clear(h1);
    for (int i = 0; i < 15; ++i)
    {
        assert(*values[i] == i);
        /* printf removed will not work when line 67 is called */
        //printf("%s",removed[i]);
    }
    printf("Test Clear Hash Passed\n");
}

void Test_Int_Hash()
{
    int d = 5;

    assert(hashInt(&d,32) == 8);
    d = 12;

    assert(hashInt(&d,32) == 20);

    int* x = malloc(sizeof(int));
    *x = 10;
    assert(!intComp(&d,x));
    int* y = malloc(sizeof(int));
    *y = 10;
    assert(intComp(x,y));
    assert(!intComp(NULL,y));
    printf("Test Int Hash Passed");

    struct HashTable* literals = Create_HashTable(15,&intComp,&intAssn,&instrAssn,&hashInt);

    assert(getKeyIndex(literals,y) == -1);
    insert_KeyValue(literals,y,createInstruction("const",x,NULL));
    assert(((struct Instruction*)(get_KeyValue(literals,y)))->op1 == x);
    assert(getKeyIndex(literals,y) == 10);
    free(y);
    *x = 10;
    assert(getKeyIndex(literals,x) == 10);
}


int main()
{

    Test_KeyAssn_HashTable();
    Test_Clear_Hash();

    Test_Int_Hash();

    return 0;
}