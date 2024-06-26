




#include "ssaInstr.h"
#include "instruction.h"
#include "container.h"
#include "hashTable.h"
#include "ioVar.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>



void Test_SSA_Init_Proced()
{

    SSA_Initialize_Program();
    assert(smplProgram->capacity == initSize);
    assert(smplProgram->size == 0);
    assert(joinBlocks->size == 0);
    assert(joinBlocks->capacity  == initSize);
    assert(constants != NULL);
    for(unsigned int i = 0; i < smplProgram->capacity; ++i)
    {
        assert(smplProgram->values[i] == NULL);
        assert(joinBlocks->values[i] == NULL);
    }

    printf("\n---------\n6/6 Test SSA Init Procedure Passed\n---------\n");

}

void Test_SSA_Initialize_Instr()
{
    SSA_Initialize_Instructions();
    assert(curBlock == NULL);
    assert(curInstr == NULL);
    assert(curProced == NULL);

    assert(identifiers != NULL);
    printf("5/5 Test SSA Init Instr Passed\n--------\n");
    
}


void Test_AddProced()
{

    AddProced();

    assert(curProced != NULL);
    assert(smplProgram->values[0] != NULL);
    assert(smplProgram->values[0] == curProced);

    printf("3/3 Test Add Proced Pased\n------\n");

}

void Test_AddBlock()
{
    /* this is the second time add proced is being called
    watch out :), these test cases are using the 2nd added procedure */

    AddProced();

    assert(smplProgram->values[1] != NULL);
    assert(curProced != NULL);
    assert(smplProgram->values[1] == curProced);

    AddBlock();

    struct Container* proced = smplProgram->values[1];
    assert(curBlock == proced->values[0]);


    printf("Test Add Block Passed\n-------\n");
}

void Test_AddInstr()
{
    AddProced();
    AddBlock();

    assert(smplProgram->size == 3);
    
    struct Container* proced = smplProgram->values[2];
    assert(proced->values[0] == curBlock);

    curInstr = createInstruction("read",NULL,NULL);

    struct Container* block = proced->values[0];
    AddInstr();
    assert(block->values[0] == curInstr);

    printf("3/3 Test Add Instr Passed\n------\n");
}

void Test_Constant_Instr()
{
    struct Instruction* instr = curInstr;
    strcpy(buffer,"0");
    const_Instr_Parsed();
    assert(instr != curInstr);

    instr = curInstr;
    const_Instr_Parsed();
    assert(instr == curInstr);

    strcpy(buffer,"17");
    const_Instr_Parsed();
    assert(instr != curInstr);

    int x = 0;
    int y = 17;

    assert(instrCmp(instr,createInstruction("const",&x,NULL)) == 1);
    assert(instrCmp(curInstr,createInstruction("const",&y,NULL)) == 1);

    printf("5/5 Test Constant Instr Passed\n-------\n");
    
}

void Test_Identifer_Instr()
{
    strcpy(buffer,"stddev");
    ident_Instr_Parsed();

    assert(strcmp(buffer,"stddev") == 0);
    int zero = 0;
    struct Instruction* zeroInstr = createInstruction("const",&zero,NULL);

    assert(curInstr != NULL);
    assert(instrCmp(zeroInstr,curInstr) == 1);
    assert(getKeyIndex(identifiers,buffer) != -1);
    assert(getKeyIndex(constants,"0") != -1);

    assert(instrCmp(get_KeyValue(identifiers,buffer),get_KeyValue(constants,"0")));

    printf("6/6 Test Instr Parsed Passed\n--------\n");
}

void Test_UpdateInstr()
{
    int zero = 0;
    strcpy(buffer,"17");
    const_Instr_Parsed();

    struct Instruction* input = curInstr;

    strcpy(buffer,"slope");
    ident_Instr_Parsed();

    assert(instrCmp(input,curInstr) != 1);
    input = curInstr;

    UpdateInstr(input,"mul","\0");

    assert(instrCmp(input,curInstr) == 1);

    strcpy(buffer,"17");
    const_Instr_Parsed();
    input = curInstr;
    struct Instruction* read = createInstruction("read",NULL,NULL);

    UpdateInstr(read,"add","\0");

    assert(instrCmp(curInstr, createInstruction("add",input,read)) == 1);


    printf("Test Update Instr Passed\n------\n");
}

void Test_Update_Table()
{
    struct Instruction* read = createInstruction("read",NULL,NULL);
    UpdateTable("a",read);
    assert(instrCmp(read,get_KeyValue(identifiers,"a")) == 1);
    printf("Test Update Table Passed");
    return;
}

int main()
{
   // Initialize();
   Test_SSA_Init_Proced();
   Test_SSA_Initialize_Instr();
   Test_AddProced();
   Test_AddBlock();
   Test_AddInstr();

   Test_Constant_Instr();
   Test_Identifer_Instr();
   Test_UpdateInstr();
   Test_Update_Table();

    return 0;
}