




#include "ssaInstrRename.h"
#include "hashTable.h"

#include <assert.h>
#include <stdio.h>


void Test_SSA_RenameInitialize()
{

    SSA_Renaming_Initialize();
    assert(whileLoopExps != 0);

    printf("Test SSA Rename Initialize Passed");

    return;
}

void Test_SSA_Add_RenamingStructure()
{
    SSA_Renaming_Initialize();
    assert(whileLoopExps->size == 0);


    for (unsigned int i = 0; i < 4; ++i)
    {
        Add_RenamingStructure();
        assert(whileLoopExps->size == i + 1);
        struct HashTable* expression = whileLoopExps->values[i];

        assert(expression->size == 0);
        assert(expression->capacity == 15);
        assert(expression->keys != NULL);
        assert(expression->values != NULL);
    }

    printf("Test Add Renaming Structure Passed");
    return;
}

void Test_SSA_Remove_RenamingStructure()
{
   SSA_Renaming_Initialize();
    
    Remove_RenamingStructure();
    assert(whileLoopExps->size == 0);

    for(unsigned int i = 1; i <= 4; ++i)
    {
        if (i % 2 != 0)
        {
            Add_RenamingStructure();
        }
        else 
        {
            Remove_RenamingStructure();
            assert(whileLoopExps->size == 0);
        }
    }
    printf("Test Remove Renaming Structure Passed");
}

void Test_AddExpression_Ident_Chain()
{
    SSA_Renaming_Initialize();
    char* numDays = "numDaysL";
    char* yIntercept = "yIntercept";
    int constant = 44;
    struct Instruction* c = createInstruction("const",&constant,NULL);
    struct Instruction* read = createInstruction("read",NULL,NULL);
    struct Instruction* cmp = createInstruction("cmp",c,read);
    struct Instruction* add = createInstruction("add",read,c);
    Add_RenamingStructure();

    /* This is testing unit testing AddExpression */
    //AddExpression_To_Ident_Chain(0,numDays,cmp);
    //AddExpression_To_Ident_Chain(1,yIntercept,cmp);
    //AddExpression_To_Ident_Chain(1,numDays,add);
    //AddExpression_To_Ident_Chain(0,yIntercept,add);
    /*---------------*/

    /* this is testing AddExpression_To_Rename */
    AddExpression_To_Rename(numDays,yIntercept,cmp);
    AddExpression_To_Rename(yIntercept,numDays,add);
    /*------------------------------*/

    struct HashTable* expressions = Top(whileLoopExps);
    struct Container* identChain = get_KeyValue(expressions,numDays);
    struct Container* identChain2 = get_KeyValue(expressions,yIntercept);
    struct Container* chains[2] = {identChain, identChain2};

    for(unsigned int i = 0; i < 2; ++i)
    {
        assert(chains[i]->size == 2);

        void** instrInfo = chains[i]->values[0];
    
        assert(*(int*)(instrInfo[1]) == i);
        assert((struct Instruction*)(instrInfo[0]) == cmp);

        instrInfo = chains[i]->values[1];
        assert(*(int*)(instrInfo[1]) == 1 - i);
        assert((struct Instruction*)(instrInfo[0]) == add);
    }

    printf("Test AddExpression Ident Chain Passed");
}

void Test_Rename_Expression()
{
    SSA_Renaming_Initialize();
    char* a = "a";
    int oneConst = 1;
    int tenConst = 10;
    struct Instruction* constInstr = createInstruction("const",&oneConst,NULL);
    struct Instruction* ten = createInstruction("const",&tenConst,NULL);

    struct Instruction* read = createInstruction("read",NULL,NULL);
    struct Instruction* cmp = createInstruction("cmp",read,ten);
    struct Instruction* add = createInstruction("add",constInstr,read);
    struct Instruction* phi = createInstruction("phi",add,read);
    Add_RenamingStructure();
    
    AddExpression_To_Rename(a,NULL,cmp);
    AddExpression_To_Rename(NULL,a,add);

    Rename_Expressions(a,phi);
    assert(cmp->op1 == phi);
    assert(add->op1 == constInstr);
    assert(cmp->op2 == ten);
    assert(add->op2 == phi);
    printf("Test Rename Expressions Passed");
}

int main()
{

    //Test_SSA_RenameInitialize();
    //Test_SSA_Add_RenamingStructure();
    //Test_SSA_Remove_RenamingStructure();
    //Test_AddExpression_Ident_Chain();

    Test_Rename_Expression();

    return;
}