



#include "arrays.h"
#include "container.h"
#include "instruction.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>



void Test_Initialize_Array()
{
    assert(arrays == NULL);
    Initialize_Arrays();
    assert(arrays != NULL);
    Initialize_Arrays();
    printf("Test Initialize Array Passed");
}

void Test_Add_Dimension()
{
    Initialize_Arrays();
    
    for(int i = 1; i <= 3; ++i)
    {
        Add_Dimension(i);
        struct Instruction* dimen = curVarDimens->values[i-1];
        assert(*(int*)(dimen->op1) == i);
        assert(curVarDimens->size == i);
    }
    printf("Test Add Dimension Passed");
}

void Test_Clear_Dimension()
{

    Initialize_Arrays();
    assert(curVarDimens == NULL);
    Add_Dimension(2);
    assert(curVarDimens != NULL);
    Clear_Dimensions();
    assert(curVarDimens == NULL);
    printf("Test Clear Dimension Passed");

    return;
}

void Test_Insert_Array()
{
    Initialize_Arrays();

    struct Instruction* add = createInstruction("add","FP","idMatrix");
    
    Add_Dimension(2);
    Add_Dimension(2);
    SetBaseOffSet(add);
    Insert_Array("idMatrix",Get_Dimensions(),Get_BaseOffSet());

    void** arrayInfo = get_KeyValue(arrays,"idMatrix");
    assert(arrayInfo != NULL);
    assert(arrayInfo[0] == Get_Dimensions());
    assert(arrayInfo[1] == add);

    printf("Test Insert Array Passed");


    return;
}

void Test_Calculate_Array_Index()
{
    Initialize_Arrays();

    Add_Dimension(7);
    Add_Dimension(5);
    Add_Dimension(6);
    
    struct Instruction* add = createInstruction("add","FP","a");
    struct Instruction* add2 = createInstruction("add","FP","b");
    SetBaseOffSet(add);

    Insert_Array("a",Get_Dimensions(),Get_BaseOffSet());

    int row = 6;
    int col = 4;
    int height = 5;
    struct Container* indices = Create_Container(2,&instrAssn);
    AddValue(indices,createInstruction("const",&row,NULL));
    AddValue(indices,createInstruction("const",&col,NULL));
    AddValue(indices,createInstruction("const",&height,NULL));
    /* this test has changed, we need to go back here and test it*/
    //Calculate_Array_Index("a",indices);

    struct Instruction* adda = Get_Adda_Instr();
    assert(836 == *(int*)(adda->op1));



    printf("Test Calculate Array Index Passed");

}

void Test_Indices() { 
    
    Initialize_Arrays();
    struct Instruction* read = createInstruction("read",NULL,NULL);
    struct Instruction* read2 = createInstruction("read",NULL,NULL);
    
    Add_Index(read,"\0");
    Add_Index(read2,"\0");

    struct Container* indices = Get_Indices();
    assert(indices->values[0] == read);
    assert(indices->values[1] == read2);

    Clear_Indices();

    assert(indices->values[0] == read);
    assert(indices->values[1] == read2);
    assert(indices->size == 0);

    return;
}

int main()

{
    SSA_Initialize_Program();
    SSA_ComSubExp_Initialize();
    Add_ComSubExp_Structure();
    AddProced();
    AddBlock();

    // Test_Initialize_Array();
    // Test_Add_Dimension();
    // Test_Clear_Dimension();
    // Test_Insert_Array();

    //Test_Calculate_Array_Index();
    Test_Indices();

    return 0;
}