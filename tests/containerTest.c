



#include "instruction.h"
#include "container.h"
#include "valueInt.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <Windows.h>


void Test_Create_Container()
{
    struct Container* block = Create_Container(15, &instrAssn);
    struct Instruction* read = createInstruction("read",NULL,NULL);
    struct Instruction* op1 = createInstruction("const","45",NULL);

    read = block->valAssn(read,op1);
    assert(read == op1);
    assert(block->size == 0);
    assert(block->capacity = 15);
    assert(block->valAssn == &instrAssn);

    for (unsigned int i = 0; i < block->capacity; ++i)
    {
        assert(block->values[i] == NULL);
    }

    printf("5/5 Test Create Container Passed\n-----------\n");
    return;
}

void Test_Add_Value()
{
    int five = 5;
    int* f = &five;
    struct Container* c = Create_Container(15,&instrAssn);

    struct Instruction* read = createInstruction("read",NULL,NULL);
    struct Instruction* fiveConst = createInstruction("const",f,NULL);
    struct Instruction* product = ComputeInstruction(fiveConst,read,"mul");

    struct Instruction* read2 = createInstruction("read",NULL,NULL);
    struct Instruction* diff = ComputeInstruction(product,read2,"sub");

    AddValue(c,read);
    AddValue(c,fiveConst);
    AddValue(c,product);

    assert(c->values[0] == read);
    assert(c->values[1] == fiveConst);
    assert(c->values[2] == product);

    assert(c->size == 3);

    AddValue(c,read2);
    AddValue(c,diff);

    assert(c->size == 5);
    assert(c->values[3] == read2);
    assert(c->values[4] == diff);

    
    
    printf("7/7 Test Add Value Passed");

    return;
}

void Test_Remove_Back()
{
    int x = 4;
    int* d = &x;

    struct Container* c = Create_Container(15,instrAssn);
    struct Instruction* read = createInstruction("read",NULL,NULL);
    struct Instruction* fourConst = createInstruction("const",d,NULL);

    RemoveBack(c);
    assert(c->size == 0);

    AddValue(c,read);
    AddValue(c,fourConst);

    RemoveBack(c);
    assert(c->size == 1);
    assert(c->values[0] == read);

    RemoveBack(c);
    assert(c->size == 0);

    printf("\n--------\n4/4 Test Remove Block\n--------\n");

    return;
}

void Test_Extend_Container()
{
    int zero = 0;
    int* z = &zero;

    int one = 1;
    int* o = &one;

    struct Instruction* read =  createInstruction("read",NULL,NULL);
    struct Instruction* zeroConst = createInstruction("const",z,NULL);
    struct Instruction* product = ComputeInstruction(read,zeroConst, "mul");
    
    struct Instruction* read2 = createInstruction("read",NULL,NULL);
    struct Instruction* div = createInstruction("div",product, read2);

    struct Container* c = Create_Container(2, &instrAssn);
    AddValue(c,read);
    AddValue(c,zeroConst);
    assert(c->values[0] == read);
    //Extend(c);
    
    AddValue(c,product);

    assert(c->capacity == 4);
    assert(c->size == 3);

    AddValue(c,read2);

    assert(c->capacity == 4);
    assert(c->size == 4);

    AddValue(c,div);
    assert(c->capacity == 8);
    assert(c->size == 5);
    
    assert(c->values[0] == read);
    assert(c->values[1] == zeroConst);
    assert(c->values[2] == product);

    printf("10/10   Test Extend Container Passed\n----------\n");
    return;
}

void Test_HashFunc_Container()
{

    for (unsigned int i = 0; i < 100; ++i)
    {
        struct Container* key = Create_Container(15,&conAssn);
        int hashVal = hashFunc(key,32);

        assert(hashVal >= 0 && hashVal <= 31);

    }


    printf("Test HashFun Container Passed\n------\n");
}

void Test_Top_Container()
{
    struct Container* c = Create_Container(15,&intAssn);
    //Top(c); should throw an assertion
    int d = 10;
    AddValue(c,&d);

    
    assert(d == *(int*)Top(c));

    d = 15;
    AddValue(c,&d);
    assert(d == *(int*)Top(c));

    printf("Test Top Container Passed\n");
}

void Test_Free_Container()
{
    struct Container* temp = Create_Container(15,&intAssn);

    int d = 10;
    AddValue(temp,&d);
    d = 5;
    AddValue(temp,&d);
    int* val = temp->values[0];
    int* val2 = temp->values[1];
    Free_Container(temp);
    printf("%lu\n",temp->size);
    printf("%lu\n",temp->capacity);
    printf("%i %i\n",*val,*val2);
    printf("Test Free Container\n");
}

int main()
{
    Test_Create_Container();
    Test_Add_Value();
    Test_Remove_Back();
    Test_Extend_Container();

    Test_HashFunc_Container();
    Test_Top_Container();
    Test_Free_Container();

    return 0;
}