


#include "ssaInstrRename.h"
#include "container.h"
#include "instruction.h"
#include "hashTable.h"
#include "keyString.h"
#include "valueInt.h"
#include "ssaInstr.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct Container* whileLoopExps;



void SSA_Renaming_Initialize()
{
    whileLoopExps = Create_Container(15,&Direct_Assn);
}

void Add_RenamingStructure()
{
    // hashTable is strings => containers
    struct HashTable* newExps = Create_HashTable(15,&isStrEqual,&strAssn,
                                                &conAssn,&hashFunction_String);
    AddValue(whileLoopExps,newExps);

    return;
}

void Remove_RenamingStructure()
{
    RemoveBack(whileLoopExps);
    return;
}
/* this function expects to be called on 
a non empty whileLoop container, non NULL exp and ident */
void AddExpression_To_Ident_Chain(int branch, char* ident, struct Instruction* exp)
{
    assert(ident != NULL);
    assert(exp != NULL);

    struct HashTable* expressions = Top(whileLoopExps);
    assert(expressions != NULL);

    if ( get_KeyValue(expressions,ident) == NULL)
    {
        struct Container* chain = Create_Container(15,&Direct_Assn);
        insert_KeyValue(expressions,ident,chain);
    }
    struct Container* identChain = get_KeyValue(expressions,ident);

    void** chainInfo = malloc(2*sizeof(void*));
    chainInfo[0] = exp;
    chainInfo[1] = NULL;
    chainInfo[1] = intAssn(chainInfo[1],&branch);

    AddValue(identChain,chainInfo);
    return;
}

/* expressions do not to be added if a while loop is not encountered */
void AddExpression_To_Rename(char* ident1, char* ident2,struct Instruction* exp)
{

    if (Top(whileLoopExps) == NULL)
    {
        return;
    }

    if (ident1 != NULL && strcmp(ident1,"\0") != 0)
    {
        AddExpression_To_Ident_Chain(0,ident1,exp);
    }
    if (ident2 != NULL && strcmp(ident2,"\0") != 0)
    {
        AddExpression_To_Ident_Chain(1,ident2,exp);
        
    }

    return;
}


/* rename_expressions is called if phi function in while loop is created */


void Rename_Expressions(char* ident, struct Instruction* newInstr)
{

    assert(ident != NULL);
    assert(newInstr != NULL);
    assert(Top(whileLoopExps) != NULL);
    struct HashTable* expression = Top(whileLoopExps);

    struct Container* identChain = get_KeyValue(expression,ident);
    if (identChain == NULL) { return;}
    assert(identChain != NULL);

    for(unsigned int i = 0; i < identChain->size; ++i)
    {
        void** instrInfo = identChain->values[i];
        assert(instrInfo != NULL);
        struct Instruction* renamedInstr = instrInfo[0];
        int opBra = *(int*)(instrInfo[1]);

        if(renamedInstr->op1 == NULL && renamedInstr->op2 == NULL)
        {
            UpdateTable(renamedInstr->instr,newInstr);
        }

        if (opBra == 0)
        {
            renamedInstr->op1 = newInstr;
        }
        else 
        {
            renamedInstr->op2 = newInstr;
        }
    }
    Clear(identChain);
}


