


#include "ssaInstr.h"
#include "ssaComSubExp.h"
#include "container.h"
#include "hashTable.h"
#include "instruction.h"
#include "symTable.h"
#include "keyString.h"
#include "ioVar.h"
#include "valueInt.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>


struct HashTable* identifiers;
struct HashTable* constants;


struct Container* smplProgram;
struct Container* joinBlocks;

struct Container* curProced;
struct Container* curBlock;
struct Instruction* curInstr;
unsigned int initSize = 10;
int isWhileNested = 0;

void SSA_Initialize_Program()
{
    smplProgram = Create_Container(initSize, &conAssn);
    joinBlocks = Create_Container(initSize,&conAssn);

    constants = Create_HashTable(30,&isStrEqual,&strAssn,
                                    &instrAssn,&hashFunction_String);

    return;
}

void SSA_Initialize_Instructions()
{    
  
    identifiers = Create_HashTable(20,&isStrEqual,&strAssn,
                                    &instrAssn,&hashFunction_String);

    curBlock = NULL;    
    curInstr = NULL;
    curProced = NULL;


    return;
}


void AddProced()
{
    SSA_Initialize_Instructions();
    struct Container* proced = Create_Container(initSize,&conAssn);
    AddValue(smplProgram, proced);
    curProced = proced;
}
/* add block expects that addProced has been called before */
void AddBlock()
{
    struct Container* block = Create_Container(initSize,&instrAssn);
    AddValue(curProced,block);
    curBlock = block;
    AddValue(curBlock,createInstruction("empty",NULL,NULL));
}

/* add instr expects that curInstr has been dynamically allocated*/
/* AddInstr can be called on NULL curInstr but not sure what that would
mean */
void AddInstr()
{
    /* common subexpression elimination should be done here */
    struct Instruction* lastInstr = Top(curBlock);
    if(strcmp(curInstr->instr,"const") == 0)
    {
        return;
    }

    if (lastInstr != NULL && strcmp(lastInstr->instr,"empty") == 0)
    {
        lastInstr->instr = curInstr->instr;
        lastInstr->op1 = curInstr->op1;
        lastInstr->op2 = curInstr->op2;
        curInstr = instrAssn(curInstr,lastInstr);
    }
    else 
    {
        AddValue(curBlock,curInstr);
    }
}

void AddJBlock()
{
    struct Container* jBlock = Create_Container(initSize,&instrAssn);
    AddValue(joinBlocks,jBlock);
}
void Move_JBlock_To_CurProced()
{
    if (joinBlocks->size == 0)
    {
        return;
    }
    int topIndex = joinBlocks->size - 1;
    struct Container* jBlock = joinBlocks->values[topIndex];
    AddValue(curProced,jBlock);
    curBlock = jBlock;
}

void RemoveBlock(int numToRemove)
{
    assert(curProced != NULL);
    if (numToRemove >= curProced->size)
    {
        numToRemove = curProced->size - 1;
    }
    for(int i = 1; i <= numToRemove; ++i)
    {    
        RemoveBack(curProced);
    }
    curBlock = Top(curProced);
}

void ResetBlocks()
{
    assert(curProced != NULL);
    RemoveBlock(curProced->size - 1);
    
}

void RemoveJBlock()
{
    RemoveBack(joinBlocks);
}

/* this uses curInstr and the instruction passed in to
to get new Instruction */

void UpdateInstr(struct Instruction* op1, char* operation,char* stringExp)
{

    if(!isWhileNested && isConst(op1) && isConst(curInstr))
    {
        Compute_Literal(op1->op1,curInstr->op1, operation);
        if (Get_Last_Parsed_Instr() != NULL)
        {
            SetInstr(Get_Last_Parsed_Instr());
            return;
        }
        int boolVal = Compute_Boolean(op1->op1,curInstr->op1,operation);
    }
    struct Instruction newInstr;
    instrMemAssn(&newInstr,op1,curInstr,operation);
    struct Instruction* i = FindComExp(&newInstr,stringExp);
 /* NULL IS USED FOR COM LOAD EXPRESSION , REGULAR EXPRESSIONS 
 * SHOULD ALSO RETURN NULL, YOU SHOULD FIX THAT */
    if ( i == &newInstr || i == NULL)
    {
        curInstr = ComputeInstruction(op1,curInstr,operation);
        AddInstr();
        Add_Exp_To_Chain(curInstr->instr,curInstr,stringExp);
        return;
    }
    //free(curInstr);
    curInstr = i;
    
}

/* the following two functions will be using the buffer
when ident or num is called to create or retrieve an identifiers
instruction or a constant instruction that already exits */

void const_Instr_Parsed() 
{
    if(getKeyIndex(constants,buffer) == -1)
    {
        int value = atoi(buffer);
        int* pValue = malloc(sizeof(int));
        *pValue = value;
        struct Instruction* newInstr = createInstruction("const",pValue,NULL);
        insert_KeyValue(constants, buffer, newInstr);
        struct Container* p = smplProgram->values[0];
        AddValue(p->values[0],newInstr);
    }
    curInstr = get_KeyValue(constants,buffer);
    return;
}

void ident_Instr_Parsed() 
{
    if(getKeyIndex(identifiers,buffer) == -1)
    {
        char identifer[100];
        strcpy(identifer,buffer);
        strcpy(buffer, "0");
        const_Instr_Parsed();
        struct Instruction* zero = get_KeyValue(constants,"0");
        insert_KeyValue(identifiers,identifer,zero);
        strcpy(buffer,identifer);
    }
    curInstr = get_KeyValue(identifiers,buffer);
}

struct Instruction* GetInstr()
{
    return curInstr;
}


void SetInstr(struct Instruction* newInstr)
{
    curInstr = newInstr;
}

void Connect_Blocks(int isLBranch)
{
    if (curProced == NULL || curProced->size < 2)
    {
        return;
    }
    struct Container* parent = curProced->values[curProced->size - 2];
    struct Container* child = Top(curProced);
    if (isLBranch)
    {
        parent->lbranch = child;
        return;
    }
    parent->rbranch = child;
    return;
}

void Connect_JBlock(int islBranch, int isParent)
{
    assert(curProced != NULL && joinBlocks != NULL);
    if ( curProced->size < 1 || joinBlocks-> size < 1)
    {
        return;
    }
    struct Container* block = curProced->values[curProced->size - 1];
    struct Container* jBlock = joinBlocks->values[joinBlocks->size - 1];

    struct Container* parent = isParent ? jBlock : block;
    struct Container* child = isParent ? block : jBlock;
    

    if(islBranch)
    {
        parent->lbranch = child;
        return;
    }
    parent->rbranch = child;
    return;
}

void UpdateTable(void* key, void* value)
{
    assert(get_KeyValue(identifiers,key) != NULL);
    insert_KeyValue(identifiers,key,value);
    return;
}

struct Container* GetJBlock() { return Top(joinBlocks);}
struct Container* GetBlock() { return curBlock;}
struct Container* GetProced() { return curProced;}

struct Instruction* Get_Ident_Instr(char* key) { 
    return get_KeyValue(identifiers,key); 
}

void Set_While_Nest(int cond) { isWhileNested = cond;}
int Get_While_Nest() { return isWhileNested;}

void AddLiteral(struct Instruction* literal)
{
    struct Container* p = smplProgram->values[0];
    AddValue(p->values[0],literal);
}


int isConst(struct Instruction* i) { return strcmp("const",i->instr) == 0;}