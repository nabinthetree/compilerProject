




#include "instruction.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>


void Test_Create_Instruction()
{
    int x = 7;
    int* y = &x;
    struct Instruction* zeroConst = createInstruction("const",y,NULL);
    struct Instruction* read1 = createInstruction("read",NULL,NULL);

    struct Instruction* add1 = createInstruction("add",zeroConst,read1);

    assert(strcmp("add", add1->instr) == 0);
    assert(strcmp("const", zeroConst->instr) == 0);
    assert(strcmp("read",read1->instr) == 0);

    assert(NULL != zeroConst->op1 && NULL == zeroConst->op2);
    assert(*(int*)(zeroConst->op1) == 7);

    printf("\n--------------\n5/5 Test_Create_Instruction Passed\n");
}

void Test_InstrAssn()
{

    struct Instruction* des = NULL;

    struct Instruction* dividend = createInstruction("const","24",NULL);
    struct Instruction* divisor = createInstruction("const","4",NULL);
    struct Instruction* quotient = createInstruction("div",dividend,divisor);

    des = instrAssn(des,quotient);

    assert(des == quotient);

    printf("---------------\n1/1 Test_InstrAssn Passed\n---------------\n");
}


void Test_instrCmp()
{

    struct Instruction* op1 = createInstruction("const","17",NULL);
    struct Instruction* op2 = createInstruction("const","7",NULL);

    struct Instruction* product = createInstruction("mul",op1,op2);
    struct Instruction* product2 = createInstruction("mul",op2,op1);

    assert(instrCmp(op1,op2) == 0);
    assert(instrCmp(product,product2) == 1);
    assert(instrCmp(product,product));
    assert(instrCmp(op1,product) == 0);

    assert(instrCmp(op1,op1) == 1);

    struct Instruction* diff = createInstruction("sub",op1,op2);
    struct Instruction* diff2 = createInstruction("sub",op2,op1);

    assert(instrCmp(diff,diff) == 1);
    assert(instrCmp(diff,diff2) == 0);

    printf("7/7 Test_InstrComp Passed\n");

}

void Test_Compute_Instruction()
{
    int* x = malloc(sizeof(int));
    int* y = malloc(sizeof(int));

    *x = 10;
    *y = 17;

    struct Instruction* op1 = createInstruction("const",x,NULL);
    struct Instruction* op2 = createInstruction("const",y,NULL);

    struct Instruction* read = createInstruction("read",NULL,NULL);
    
    struct Instruction* result = ComputeInstruction(op1,op2,"mul");
    assert(*(int*)result->op1 == 170);
    assert(strcmp(result->instr,"const") == 0);
    assert(result->op2 == NULL);

    struct Instruction* result1 = ComputeInstruction(result,read, "add");
    struct Instruction* result2 = ComputeInstruction(read, result, "add");
    assert(instrCmp(result1,result2) == 1);
    assert(instrCmp(result1->op1, result) == 1);
    assert(instrCmp(result1->op2,read) == 1);
   
    assert(strcmp(result1->instr,"add") == 0);


    printf("-------------\n7/7 Test Compute Instruction Passed\n--------------\n");

    return;
}

void Test_instrMemAssn()
{
    struct Instruction newInstr;

    void* operand = createInstruction("read",NULL,NULL);
    void* operand2 = createInstruction("read",NULL,NULL);
    char* operation = "add";

    instrMemAssn(&newInstr,operand,operand2,operation);

    assert(newInstr.op1 == operand);
    assert(newInstr.op2 == operand2);
    assert(strcmp(newInstr.instr,operation) == 0);

    printf("\nTest Instruction Member Assn Passed\n");

    return;
}

int main()


{

    Test_Create_Instruction();
    Test_InstrAssn();
    Test_instrCmp();
    Test_instrMemAssn();

    return 0;
}