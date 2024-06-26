




#include "ssaComSubExp.h"
#include "instruction.h"
#include "valueInt.h"
#include "keyString.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdlib.h>


void Test_SSA_Com_Initialize()
{
    SSA_ComSubExp_Initialize();

    int x = 5;

    assert(hashPrevSizes != NULL);
    assert(hashOperChains != NULL);
    assert(operChains == NULL);
    assert(prevSizes == NULL);
    
    Add_ComSubExp_Structure();
    Add_ComSubExp_Structure();
    assert(hashOperChains->size == 2);
    Remove_ComSubExp_Structure();
    assert(hashOperChains->size == 1);
    SSA_ComSubExp_Initialize();
    assert(hashOperChains->size == 0);
    printf("Test Com Initialized Passed\n");
}

void Test_Add_Keys()
{
    struct HashTable* h = NULL;
    h = Create_HashTable(numInstrs,&isStrEqual,&strAssn,&Direct_Assn,&hashFunction_String);
    Add_Keys(h);
    for (unsigned int i = 0; i < numInstrs; ++i)
    {
        assert(getKeyIndex(h,instructions[i]) != -1);
    }
    printf("Test Add Instructions Passed\n");
    return;
}

void Test_Add_Container_Values()
{

    struct HashTable* h = Create_HashTable(15,&isStrEqual,&strAssn,&conAssn,&hashFunction_String);
    Add_Container_Values(h,&intAssn);
    
    for (unsigned int i = 0; i < h->capacity; ++i)
    {
        AddValue(h->values[i],&i);
        assert(*(unsigned int*)(Top(h->values[i])) == i);
    }
    Add_Container_Values(h,&instrAssn);
    for (unsigned int i = 0; i < h->capacity; ++i)
    {
        struct Instruction* r = createInstruction("read",NULL,NULL);
        AddValue(h->values[i],r);
        assert(Top(h->values[i]) == r);
    }

    printf("Test Add Container Values Passed");
}

void Test_Add_ComSubExp_Structure()
{
    SSA_ComSubExp_Initialize();

    Add_ComSubExp_Structure();
    assert(hashOperChains->size == 1);
    assert(hashPrevSizes->size == 1);

    struct HashTable* h = Top(hashOperChains);
    struct HashTable* h2 = Top(hashPrevSizes);

    for (unsigned int i = 0; i < numInstrs; ++i)
    {
        assert(isStrEqual(h->keys[i],h2->keys[i]));
        assert(h->keys[i] != h2->keys[2]);
        assert(h->values[i] != h2->values[i]);
    }

    printf("Test Add ComSubExp Structure Passed");
}


void Test_Remove_Structure()
{

    SSA_ComSubExp_Initialize();
    Add_ComSubExp_Structure();

    struct Instruction* r = createInstruction("read",NULL,NULL);
    struct Instruction* r2 = createInstruction("read",NULL,NULL);
    struct Instruction* add = createInstruction("add",r,r2);

    AddValue(get_KeyValue(operChains,"add"),add);
    Remove_ComSubExp_Structure();

    assert(isStrEqual(add->instr,"add"));

    assert(hashOperChains->size == 0);
    assert(hashPrevSizes->size == 0);



    printf("Test Remove ComSubExp Passed\n");
}



void Test_SSA_Remember_Chain()
{
    SSA_ComSubExp_Initialize();
    Add_ComSubExp_Structure();
    Remember_Current_Chain_Sizes();

    for (unsigned int j = 0; j < 10; ++j)
    {
        for (unsigned int i = 0; i < numInstrs; ++i)
        {
            char* key = instructions[i];
            struct Container* chain = get_KeyValue(operChains,key);
            struct Container* sizes = get_KeyValue(prevSizes,key);

            assert(chain->size == 0 + j);
            assert(sizes->size == 1 + j);
            AddValue(chain,createInstruction("read",NULL,NULL));
            unsigned int* lastSize = Top(get_KeyValue(prevSizes,key));
            assert(*lastSize == 0 + j);
        }
        Remember_Current_Chain_Sizes();
    }

    printf("Test Remeber SSA Chain Passed\n");
    return;
}

void Test_PrevChain_Sizes()
{
    SSA_ComSubExp_Initialize();
    Add_ComSubExp_Structure();
    time_t t;
    srand((unsigned) time(&t));

    int numOperationInstrs;

    for (unsigned int z = 1; z <= 3; ++z)
    {
        Remember_Current_Chain_Sizes();
        for (unsigned int i = 0; i < numInstrs; ++i)
        {
            char* operation = instructions[i];
            struct Container* chain = get_KeyValue(operChains,operation);
            numOperationInstrs = rand() % 20;
            for (int j = 1; j < numOperationInstrs; ++j)
            {
                AddValue(chain, createInstruction("read",NULL,NULL));
            }
        }
    }
    for (unsigned int z = 1; z <= 3; ++z)
    {
        Go_To_Prev_Chains_Sizes();
        for (unsigned int h = 0; h < numInstrs; ++h)
        {
            char* key = instructions[h];
            struct Container* chain = get_KeyValue(operChains,key);
            struct Container* sizes = get_KeyValue(prevSizes,key);
            assert(chain->size == *(unsigned int*)(Top(sizes)));
            //printf("%u %u\n", chain->size, *(unsigned int*)(Top(sizes)));
            RemoveBack(sizes); /* used for unit testing */
        }
        /* used to see if all functions work correctly together remove line 180 to do this call */
        //Pop_Current_Prev_Size();
    }
    printf("Test Prev Chain Passed\n");
    
}


void Test_ComSubExp()
{
    SSA_ComSubExp_Initialize();
    Add_ComSubExp_Structure();
    int val = 2;
    struct Instruction* twoConst = createInstruction("const",&val,NULL);
    struct Instruction* read = createInstruction("read",NULL,NULL);
    struct Instruction* add = createInstruction("add",twoConst,read);

    struct Instruction* result = FindComExp(add,"\0");
    assert(result == add);
    Add_Exp_To_Chain(result->instr,result,"\0");
    struct Instruction* add2 = createInstruction("add",read,twoConst);
    result = FindComExp(add2,"\0");
    assert(result == add);

    struct Instruction* sub = createInstruction("sub",twoConst,read);
    struct Instruction* sub2 = createInstruction("sub",read,twoConst);

    result = FindComExp(sub,"\0");
    assert(result == sub);
    Add_Exp_To_Chain(result->instr,result,"\0");
    result = FindComExp(sub2,"\0");
    assert(result == sub2);
    Add_Exp_To_Chain(result->instr,result,"\0");
    printf("Test Common Sub Expression Passed\n");
}


void Test_Pop_Current_Size()
{
    SSA_ComSubExp_Initialize();
    Add_ComSubExp_Structure();
    Remember_Current_Chain_Sizes();
    Pop_Current_Prev_Size();

    for (unsigned int i = 0; i < numInstrs; ++i)
    {
        struct Container* chain = get_KeyValue(prevSizes,instructions[i]);
        assert(chain->size == 0);
    }


    printf("Test Pop Current Sizes Passed");
}

void Test_FindComLoad()
{

    SSA_ComSubExp_Initialize();
    Add_ComSubExp_Structure();
    int fp = 28;

    int index  = 20;

    struct Instruction* baseOff = createInstruction("baseAddr",&fp,NULL);
    struct Instruction* indice = createInstruction("const",&index,NULL);

    struct Instruction* adda = createInstruction("adda",indice,baseOff);
    struct Instruction* load = createInstruction("load",adda,NULL);
    struct Instruction* temp = FindComLoad(load);
    struct Instruction* add = createInstruction("add",indice,indice);
    struct Instruction* store = createInstruction("store",adda,add);

    assert(temp == NULL);
    Add_Exp_To_Chain("array",load,"\0");
    temp = FindComLoad(load);
    assert (load == temp);
    Add_Exp_To_Chain("array",store,"\0");
    temp = FindComLoad(load);
    assert(temp == NULL);
    printf("Test FindComLoad passed");
}

int main()
{


    //Test_SSA_Com_Initialize();
    //Test_Add_Keys();
    //Test_Add_Container_Values()
    //Test_Add_ComSubExp_Structure();

    //Test_Remove_Structure();
    //Test_SSA_Remember_Chain();
    //Test_PrevChain_Sizes();

    //Test_Pop_Current_Size();

    //Test_ComSubExp();
    Test_FindComLoad();

    return 0;
}