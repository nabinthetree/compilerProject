

#include "ssaComSubExp.h"
#include "ssaInstr.h"
#include "hashTable.h"
#include "keyString.h"
#include "container.h"
#include "valueInt.h"
#include "instruction.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

struct Container* hashOperChains = NULL;
struct Container* hashPrevSizes = NULL;
struct Container* expStrings;

struct HashTable* operChains = NULL;
struct HashTable* prevSizes = NULL;
struct HashTable* stringChain = NULL;
unsigned int hashChainSize = 25;


char* instructions[14] = {"neg", "add", "sub","mul", "div","cmp",
                    "load","bge","bgt","bra","ble","blt","beq","adda"};
unsigned int numInstrs = 14;


void SSA_ComSubExp_Initialize()
{

    if (hashOperChains == NULL)
    {
        hashOperChains = Create_Container(15,&Direct_Assn);
        hashPrevSizes = Create_Container(15,&Direct_Assn);
        expStrings = Create_Container(15,&Direct_Assn);
        return;
    }
    while (Top(hashOperChains) != NULL)
    {
        clear(Top(hashOperChains));
        clear(Top(hashPrevSizes));\
        /* the keys for expString should not be removed */
        RemoveBack(expStrings);
        RemoveBack(hashOperChains);
        RemoveBack(hashPrevSizes);
    }

    return;
}

void Add_Keys(struct HashTable* h )
{
    assert(h != NULL);
    assert(h->capacity == numInstrs);
    for (unsigned int i = 0; i < numInstrs; ++i)
    {
        insert_KeyValue(h,instructions[i],NULL);
    }

    return;
}

void Add_Container_Values(struct HashTable* h,
                        void*(*valueAssn)(void* des,void* source))
{
    assert(h != NULL);
    assert(h->capacity == numInstrs);
    for (unsigned int i = 0; i < h->capacity; ++i)
    {
        h->values[i] = Create_Container(15,valueAssn);
    }
}

void Add_ComSubExp_Structure()
{
    assert(hashOperChains != NULL);
    
    struct HashTable* t = Create_HashTable(numInstrs,&isStrEqual,&strAssn,
                                        &conAssn,&hashFunction_String);
    operChains == NULL ? Add_Keys(t) : keyAssn(t,operChains);
    Add_Container_Values(t,&instrAssn);
    AddValue(hashOperChains,t);
    operChains = t;

    t = Create_HashTable(numInstrs,&isStrEqual,&strAssn,&conAssn,
                            &hashFunction_String);
    keyAssn(t,operChains);
    Add_Container_Values(t, &intAssn);
    AddValue(hashPrevSizes,t);
    prevSizes = t;

    t = Create_HashTable(32,&conCmp,&instrAssn,&strAssn,&hashFunc);
    AddValue(expStrings,t);
    stringChain = t;
    return;
}

void Remove_ComSubExp_Structure()
{
    clear(operChains);
    clear(prevSizes);
    RemoveBack(hashOperChains);
    RemoveBack(hashPrevSizes);
    RemoveBack(expStrings);

    operChains = Top(hashOperChains);
    prevSizes = Top(hashPrevSizes);
    stringChain = Top(expStrings);
    return;
}



void Remember_Current_Chain_Sizes()
{
    for (unsigned int i = 0; i < numInstrs; ++i)
    {
        char* key = instructions[i];
        struct Container* chain = get_KeyValue(operChains,key);
        struct Container* chainSizes = get_KeyValue(prevSizes,key);
        assert(chain != NULL && chainSizes != NULL);
        AddValue(chainSizes,&chain->size);
    }
    return;
}
/* expects that prevSize containers are not empty */
/* makes no sense to call this function before calling remember */
void Go_To_Prev_Chains_Sizes()
{

    for (unsigned int i = 0; i < numInstrs; ++i)
    {
        char* key = instructions[i];
        struct Container* instrChain = get_KeyValue(operChains,key);
        assert(instrChain != NULL);
        struct Container* chainSizes = get_KeyValue(prevSizes,key);
        unsigned int curSize = instrChain->size;
        unsigned int prevSize = *(unsigned int*)(Top(chainSizes));

        for (unsigned int j = 1; j <= curSize - prevSize; ++j)
        {
            RemoveBack(instrChain);
        }
    }

    return;
}

void Pop_Current_Prev_Size()
{
    assert(prevSizes != NULL);

    for(unsigned int i = 0; i < numInstrs; ++i)
    {
        struct Container* chain = get_KeyValue(prevSizes,instructions[i]);
        assert(chain != NULL);
        RemoveBack(chain);
    }

    return;
}




/* expects that curInstr was create with malloc before this call */
/* i do not know if this good programming or bad */
struct Instruction* FindComExp(struct Instruction* temp, char* stringExp)
{
    if (isStrEqual(temp->instr,"load")) { return FindComLoad(temp);}
    assert(temp != NULL);
    char* operation = temp->instr;
    struct Container* chain = get_KeyValue(operChains,operation);
    assert(chain != NULL);
    struct Instruction* pastInstr;
    for (int i = chain->size - 1; i >= 0; --i)
    {
        pastInstr = chain->values[i];
        if (instrCmp(temp,pastInstr))
        {
            
            char* tempString = get_KeyValue(stringChain,pastInstr);
            assert(tempString != NULL);
            if (!Get_While_Nest() || strcmp(tempString,stringExp) == 0)
            {
                return pastInstr;
            }
        }
    }
    return NULL;
    //AddValue(chain,curInstr);
    //return temp;
}

void Add_Exp_To_Chain(char* operation, struct Instruction* newExp, char* stringExp)
{
    assert(operChains != NULL);
    struct Container* chain = get_KeyValue(operChains,operation);

    assert(chain != NULL);
    AddValue(chain,newExp);
    insert_KeyValue(stringChain,newExp,stringExp);
}

unsigned int GetNumOfStructures() { return hashOperChains->size;}


struct Instruction* FindComLoad(struct Instruction* load)

{
    struct Container* arrayChain = get_KeyValue(operChains,"load");
    struct Instruction* tempInstr;
    assert(arrayChain != NULL);
    for (int i = arrayChain->size - 1; i >= 0; --i)
    {
        tempInstr = arrayChain->values[i];

        if (isStrEqual(tempInstr->instr,"store") &&
            instrCmp(tempInstr->op1,load->op1))
            {
                return (void*)(0);
            }
        
        if (isStrEqual("load",tempInstr->instr) &&
            instrCmp(tempInstr->op1,load->op1))
            {
                return tempInstr;
            }
    }
    return (void*)(0);
}

char* Get_Exp_String(struct Instruction* instr)
{
    assert(stringChain != NULL);
    char* stringExp = get_KeyValue(stringChain,instr);
    assert(stringExp!= NULL);
    return stringExp;
}