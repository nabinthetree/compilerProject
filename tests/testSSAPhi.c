




#include "container.h"
#include "instruction.h"
#include "hashTable.h"
#include "ssaPhi.h"
#include "ssaInstr.h"

#include <string.h>
#include <stdio.h>
#include <assert.h>


void Test_SSA_Phi_Init()
{

    SSA_Phi_Initialize();

    assert(oldValues != NULL);
    assert(blockPhiFunctions != NULL);
    assert(branches != NULL);

    printf("Test SSA Init Passed\n---------\n");
}

void Test_BackUpValue()
{
    SSA_Phi_Initialize();
    struct Instruction* lBranch = createInstruction("read",NULL,NULL);
    int five = 5;
    struct Instruction* read2 = createInstruction("read",NULL, NULL);

    struct Instruction* fiveConst = createInstruction("const",&five,0);

    struct Instruction* rBranch = ComputeInstruction(read2,fiveConst,"mul");

    struct Instruction* phiFunc = createInstruction("phi",lBranch,rBranch);

    char* var1 = "average";

    Back_Up_Value(phiFunc,var1,read2);
    Back_Up_Value(rBranch,"a",read2);

    printf("Test Back Up Value Passed\n---------\n");

    void** backUpInfo = get_KeyValue(oldValues,phiFunc);
    assert(strcmp(backUpInfo[0],var1) == 0);
    assert(instrCmp(read2,backUpInfo[1]) == 1);

    backUpInfo = get_KeyValue(oldValues,rBranch);
    assert(strcmp(backUpInfo[0],"a") == 0);
    assert(instrCmp(read2,backUpInfo[1]) == 1);

    return;
}

void Test_RestoreBackUps()
{
    SSA_Initialize_Instructions();
    SSA_Phi_Initialize();
    int five = 5;
    struct Instruction* fiveConst = createInstruction("const",&five,NULL);
    struct Container* block = Create_Container(15,&Direct_Assn);
    struct Instruction* read = createInstruction("read",NULL,NULL);
    struct Instruction* lBranch = ComputeInstruction(read,fiveConst, "div");
    struct Instruction* read2 = createInstruction("read",NULL,NULL);
    struct Instruction* phi = createInstruction("phi",read,read2);
    struct Instruction* phi2 = createInstruction("phi",lBranch,read2);
    struct Container* phiFunctions = Create_Container(15,&Direct_Assn);
    AddValue(phiFunctions,phi);
    AddValue(phiFunctions,phi2);
    insert_KeyValue(blockPhiFunctions,block,phiFunctions);
    /*
    void* backUpInfo[2] = {"d",fiveConst};
    insert_KeyValue(oldValues,phi,backUpInfo);
    */
    UpdateTable("d",phi);
    Back_Up_Value(phi,"d",fiveConst);
    Back_Up_Value(phi2,"c",read);
    assert(get_KeyValue(identifiers,"d") == phi);
   
    Restore_Back_Ups(block);
    assert(get_KeyValue(identifiers, "d") == fiveConst);
    assert(get_KeyValue(identifiers,"c") == read);

    printf("Test Restore BackUps");
}
void Test_Get_PhiFunc()
{
    SSA_Phi_Initialize();
    struct Container* block = Create_Container(15,&Direct_Assn);
    struct Instruction* read = createInstruction("read",NULL,NULL);
    struct Instruction* read2 = createInstruction("read",NULL,NULL);
    struct Instruction* read3 = createInstruction("read",NULL,NULL);
    struct Instruction* read4 = createInstruction("read",NULL,NULL);

    struct Instruction* phi = createInstruction("phi",read,read2);

    struct Instruction* phiInstr = Get_Ident_Phi_Func(block,"a");
    assert(phiInstr == NULL);

    AddValue(block,phi);
    Back_Up_Value(phi,"a",read3);
    phiInstr = Get_Ident_Phi_Func(block,"b");
    assert(phiInstr == NULL);
    phiInstr = Get_Ident_Phi_Func(block,"a");
    assert(phiInstr == phi);
    printf("Test Get PhiFunc Passed");
    return;
}

void Test_Insert_Phi()
{
    SSA_Phi_Initialize();
    AddBranch(1);
    int five = 5;
    struct Instruction* fiveConst = createInstruction("const",&five,NULL);
    struct Instruction* read = createInstruction("read",NULL,NULL);
    
    struct Instruction* add = ComputeInstruction(fiveConst,read,"sub");

    struct Container* block = Create_Container(15,&Direct_Assn);
    Insert_Phi_Function(block,add,fiveConst,"a",0,NULL);

    struct Container* phiFunctions = get_KeyValue(blockPhiFunctions,block);
    assert(phiFunctions != NULL);
    assert(phiFunctions->size == 1);
    struct Instruction* phi = phiFunctions->values[0];
    assert(phi->op1 == add);
    void** backUp = get_KeyValue(oldValues,phi);
    assert(strcmp(backUp[0],"a") == 0);
    assert(instrCmp(fiveConst,backUp[1]) == 1);
    printf("Test Insert Phi Passed");
}
int main()

{
    //Test_SSA_Phi_Init();
    //Test_BackUpValue();
    //Test_RestoreBackUps();
    //Test_Get_PhiFunc();
    Test_Insert_Phi();
    return 0;
}
