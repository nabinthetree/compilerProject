



#include "ssaPhi.h"
#include "hashTable.h"
#include "instruction.h"
#include "valueInt.h"
#include "keyString.h"
#include "ssaInstr.h"
#include "ssaInstrRename.h"
#include "symTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct HashTable* oldValues;
struct HashTable* blockPhiFunctions;
struct Container* branches;

int hashLength = 32;

void SSA_Phi_Initialize()
{
    blockPhiFunctions = Create_HashTable(hashLength,&conCmp,
                                &conAssn,&Direct_Assn,&hashFunc);
    oldValues = Create_HashTable(hashLength,&instrCmp,
                                &instrAssn,&instrAssn,&hashFunc);
    branches = Create_Container(15,&intAssn);
    
}

/* the oldvalues hashTable is string => array of two void* */

void Back_Up_Value(struct Instruction* phiFunc,char* ident,
                    struct Instruction* oldValue) 
{
    if (getKeyIndex(oldValues,phiFunc) == -1)
    {
        void** backInfo = malloc(sizeof(void*) * 2);
        backInfo[0] = NULL;
        backInfo[1] = NULL;
        insert_KeyValue(oldValues,phiFunc,backInfo);
    }
    void** backUpInfo = get_KeyValue(oldValues,phiFunc);
    backUpInfo[0] = strAssn(backUpInfo[0],ident);
    backUpInfo[1] = instrAssn(backUpInfo[1],oldValue);
    

    return;
}

/* expects block to be hashed by blockPhiFunctions */
/* expects each phi function in the hashed block to 
    be hashed by oldvalues already */
void Restore_Back_Ups(struct Container* block)
{
    if (getKeyIndex(blockPhiFunctions,block) == -1)
    {
        return;
    }
    //assert(getKeyIndex(blockPhiFunctions,block) != -1);
    struct Container* phiFunctions = get_KeyValue(blockPhiFunctions,block);
    assert(phiFunctions != NULL);
    for (unsigned int i = 0; i < phiFunctions->size; ++i)
    {
        if ( phiFunctions->values[i] == NULL)
        {
            continue;
        }
        struct Instruction* phiFunc = phiFunctions->values[i];
        assert(getKeyIndex(oldValues,phiFunc) != -1);
        void** backUpInfo = get_KeyValue(oldValues,phiFunc);
        //UpdateTable(backUpInfo[0],backUpInfo[1]);
        Update_Int_Table(backUpInfo[0],backUpInfo[1]);
    }

    return;
}

void Insert_Phi_Function(struct Container* block,
                        struct Instruction* newValue,
                        struct Instruction* oldValue,
                        char* ident, int isCopyPropag,char*varCopyPropag)
{
    if (branches->size == 0 || block == NULL)
    {
        return;
    }
    if (oldValue == NULL)
    {
        Parse_Literal(0);
        oldValue = Get_Last_Parsed_Instr();
    }
    struct Container* phiFunctions = NULL;
    if (getKeyIndex(blockPhiFunctions,block) == -1)
    {
        phiFunctions = Create_Container(15,&Direct_Assn);
        insert_KeyValue(blockPhiFunctions,block,phiFunctions);
    }
    phiFunctions = get_KeyValue(blockPhiFunctions,block);
    struct Instruction* phi = Get_Ident_Phi_Func(phiFunctions,ident);
    if ( phi == NULL)
    {
        phi = createInstruction("phi",oldValue,oldValue);
        AddValue(phiFunctions,phi);
        Back_Up_Value(phi,ident,oldValue);
        if (block->lbranch != NULL)
        {
            Rename_Expressions(ident,phi);
        }
        if(isCopyPropag)
        {
           AddExpression_To_Rename(varCopyPropag,NULL,phi);
        }

    }

    int isLbranch = *(int*)Top(branches);
    if (isLbranch)
    {
        phi->op1 = newValue;
        

    }
    else 
    {
        phi->op2 = newValue;
    }

  
    return;
}


struct Instruction* Get_Ident_Phi_Func(struct Container* phiFunctions,char* ident)
{

    for(unsigned int i = 0; i < phiFunctions->size; ++i)
    {
        struct Instruction* phi = phiFunctions->values[i];
        void** backUpInfo = get_KeyValue(oldValues,phi);
        assert(backUpInfo != NULL);
        if (strcmp(backUpInfo[0],ident) == 0)
        {
            return phi;
        }
    }
    return NULL;
}


void AddBranch(int islBranch)
{
    AddValue(branches,&islBranch);
}

void RemoveBranch() { RemoveBack(branches);}


struct Container* Get_Phi_Functions(struct Container* c) { return get_KeyValue(blockPhiFunctions,c);}

void Commit_Values(struct Container* innerBlock)
{
    struct Container* outerBlock = GetJBlock();
    if (innerBlock == NULL)
    {
        return;
    }
    struct Container* phiFunctions = get_KeyValue(blockPhiFunctions,innerBlock);
    if (phiFunctions == NULL)
    {
        return;
    }
    
    for( unsigned int i = 0; i < phiFunctions->size; ++i)
    {
        struct Instruction* newVal = phiFunctions->values[i];
        void** phiInfo = get_KeyValue(oldValues,newVal);
        char* ident = phiInfo[0];
        struct Instruction* oldVal = phiInfo[1];
        //UpdateTable(ident,newVal);
        Update_Int_Table(ident,newVal);
        Insert_Phi_Function(GetJBlock(),newVal,oldVal,ident,0,NULL);
        
    }
}