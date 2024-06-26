





#include "hashTable.h"
#include "blocks.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void TestHashFunction()
{

    char* key = "numDays";
    char* key2 = "average";
    char* key3 = "quickSort";
    
    assert(7 == hashFunction(key));
    assert(1 == hashFunction(key2));
    assert(5 == hashFunction(key3));

    printf("3/3 Tests Passed\n");
}

void TestGetKey()
{
    char* key = "numDays";
    char* key2 = "stddev";
    char* key3 = "microfacet";

    int keyIndex = hashFunction(key);
    assert(keyIndex == getKeyIndex(key));

    int keyIndex2 = hashFunction(key2);

    if ( keyIndex2 == keyIndex)
    {
        assert(keyIndex2 + 1 == getKeyIndex(key2));
        keyIndex2 += 1;
    }
    else 
    {
        assert(keyIndex2 == getKeyIndex(key2));
    }

    int keyIndex3 = hashFunction(key3);
    if ( keyIndex2 == keyIndex3)
    {
        assert(keyIndex3 + 1 == getKeyIndex(key3));
    }
    else 
    {
        assert(keyIndex3 == getKeyIndex(key3));
    }
    getKeyIndex("a");
    getKeyIndex("b");
    getKeyIndex("c");
    getKeyIndex("d");
    getKeyIndex("e");
    getKeyIndex("f");
    getKeyIndex("h");
    assert(-1 == getKeyIndex("i"));

    printf("3/3 Tests Passed\n");
   for (int i = 0; i < tableSize; ++i)
   {
        assert(i == getKeyIndex(keys[i]));
   }
    
}

void Test_Set_Instruction()
{
    
    struct Instruction* instr1 = createInstruction("const",NULL,NULL,"8");
    setKeyInstruction("a",instr1);
    instr1 = getKeyInstruction("a");

    struct Instruction* instr2 = createInstruction("const",NULL,NULL,"9");
    setKeyInstruction("c",instr2);
    instr2 = getKeyInstruction("c");

    struct Instruction* instr3 = createInstruction("add",instr1,instr2,NULL);
    setKeyInstruction("f",instr3);
    instr3 = getKeyInstruction("f");

    assert(strcmp(instr1->instr,"const") == 0);
    assert(strcmp(instr2->instr,"const") == 0);

    assert(strcmp(instr3->instr,"add") == 0);

    assert(instr3->op1 == instr1);
    assert(instr3->op2 == instr2);
    
}

int main()
{

    TestHashFunction();
    TestGetKey();
    Test_Set_Instruction();

    return 0;
}
