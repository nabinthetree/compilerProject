



#include "dotGraph.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

void Test_DotGraph_Initialize()
{

    Initialize_Graph_Creater();
    assert(bNum == 0);
    assert(iNum == 0);
    assert(dotGraph != NULL );
    printf("Test DotGraph Initialize Passed\n");
}

void Test_Write_Edge()
{
    struct Container* p = Create_Container(15,&conAssn);
    struct Container* c = Create_Container(15,&conAssn);

    int pBNum = 1;
    int cBNum = 2;

    insert_KeyValue(blockNum,p,&pBNum);
    insert_KeyValue(blockNum,c,&cBNum);

    Write_Edge(p,c,"fall-through");
    cBNum = 3;
    insert_KeyValue(blockNum,c,&cBNum);
    assert(blockNum->size == 2);

    Write_Edge(p,c,"branch");

    printf("Test Write Edge Passed\n");

    return;
}

void Test_Write_Edges()
{
    Initialize_Graph_Creater();
    int bNum = 0;
    struct Container* program[15];

    for(unsigned int i = 0; i <= 14; ++i)
    {
        int* num = malloc(sizeof(int));
        *num = i;
        program[i] = Create_Container(15,&conAssn);
        insert_KeyValue(blockNum,program[i],num);
    }

    for(unsigned int i = 0; 2 * i + 2 < 15; ++i)
    {
        program[i]->lbranch = program[2 * i + 1];
        program[i]->rbranch = program[2 * i + 2];

    }
    Write_Edges();
    printf("Test Write Edges Passed");
}

void Test_Write_Instruction()
{
    Initialize_Graph_Creater();
    struct Instruction* read = createInstruction("read",NULL,NULL);
    int fiveConst = 5;
    struct Instruction* five = createInstruction("const",&fiveConst,0);
    struct Instruction* add = ComputeInstruction(read,five,"add");
    struct Instruction* mul = ComputeInstruction(add,read,"mul");
    Write_Instruction(read);
    Write_Instruction(add);
    Write_Instruction(five);
    Write_Instruction(mul);
    assert(iNum == 4);

    printf("Test Write Instruction Passed");


    return;
}

void Test_Write_Operand()
{
    Initialize_Graph_Creater();
    struct Instruction* read = createInstruction("read",NULL,NULL);
    int five = 5;
    struct Instruction* fiveConst = createInstruction("const",&five,NULL);

    Write_Operand(fiveConst);
    Write_Operand(read);
    
    printf("Test Write Operand Passed\n");
}

void Test_Write_Instructions()
{
    Initialize_Graph_Creater();
    struct Container* block = Create_Container(15,&conAssn);
    //Write_Instructions(block);
    int x = 17;
    struct Instruction* numConst = createInstruction("const",&x,NULL);
    AddValue(block,numConst);
    struct Instruction* read = createInstruction("read",NULL,NULL);
    struct Instruction* add = ComputeInstruction(read,read,"add");
    struct Instruction* cmp = createInstruction("cmp",add,numConst);
    AddValue(block,read);
    AddValue(block,add);
    AddValue(block,cmp);

    Write_Instructions(block);
    printf("Test Write Instructions Passed\n");
}

void Test_Write_Block()
{
    Initialize_Graph_Creater();
    struct Container* block = Create_Container(15,&conAssn);
    //Write_Instructions(block);
    int x = 17;
    struct Instruction* numConst = createInstruction("const",&x,NULL);
    struct Instruction* read = createInstruction("read",NULL,NULL);
    struct Instruction* add = ComputeInstruction(read,read,"add");
    struct Instruction* cmp = createInstruction("cmp",add,numConst);
    struct Instruction* instrs[4] = {numConst,read,add,cmp};
    
    for (unsigned int i = 0; i < 4; ++i)
    {
        AddValue(block,instrs[i]);
    }
    struct Container* block2 = Create_Container(15,&conAssn);

    struct Container* block3 = Create_Container(15,&conAssn);
    struct Instruction* add2 = createInstruction("add",add,add);
    AddValue(block3,add2);
    struct Container* block4 = Create_Container(15,&conAssn);
    struct Instruction* phi = createInstruction("phi",add2,add);
    struct Instruction* phi2  = createInstruction("phi",add2,read);
    struct Instruction* write = createInstruction("write",phi2,NULL);

    AddValue(block4,phi);
    AddValue(block4,phi2);
    AddValue(block4,write);

    block->rbranch = block2;
    block->lbranch = block3;
    block2->lbranch = block4;
    block3->rbranch = block4;

    Write_Block(block);

    printf("Test Write Block Passed");
    return;
}

void Test_Create_Graph()
{

    Initialize_Graph_Creater();
    struct Container* block = Create_Container(15,&conAssn);
    //Write_Instructions(block);
    int x = 17;
    struct Instruction* numConst = createInstruction("const",&x,NULL);
    struct Instruction* read = createInstruction("read",NULL,NULL);
    struct Instruction* add = ComputeInstruction(read,read,"add");
    struct Instruction* cmp = createInstruction("cmp",add,numConst);
    struct Instruction* instrs[4] = {numConst,read,add,cmp};
    
    for (unsigned int i = 0; i < 4; ++i)
    {
        AddValue(block,instrs[i]);
    }
    struct Container* block2 = Create_Container(15,&conAssn);

    struct Container* block3 = Create_Container(15,&conAssn);
    struct Instruction* add2 = createInstruction("add",add,add);
    AddValue(block3,add2);
    struct Container* block4 = Create_Container(15,&conAssn);
    struct Instruction* phi = createInstruction("phi",add2,add);
    struct Instruction* phi2  = createInstruction("phi",add2,read);
    struct Instruction* write = createInstruction("write",phi2,NULL);

    AddValue(block4,phi);
    AddValue(block4,phi2);
    AddValue(block4,write);

    block->rbranch = block2;
    block->lbranch = block3;
    block2->lbranch = block4;
    block3->rbranch = block4;

    struct Container* program = Create_Container(15, &conAssn);
    struct Container* procedure = Create_Container(15,&conAssn);
    AddValue(procedure,block);
    AddValue(program,procedure);

    Create_Graph(program);

    printf("Test Create Graph Passed");
    return;
}

int main()

{

    //Test_DotGraph_Initialize();
    //Test_Write_Edge();
    //Test_Write_Edges();
    //Test_Write_Operand();
    //Test_Write_Instruction();
    //Test_Write_Instructions();
    //Test_Write_Block();
    Test_Create_Graph();


    return 0;
}