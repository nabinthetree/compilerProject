





#include "dotGraph.h"
#include "container.h"
#include "instruction.h"
#include "hashTable.h"
#include "valueInt.h"
#include "ssaPhi.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct HashTable* leftBranch;
struct HashTable* rightBranch;

struct HashTable* blockNum;
struct HashTable* instrNum;

struct HashTable* visitedBlocks;
FILE* dotGraph;
int bNum;
int iNum;

struct Instruction* t1;
struct Instruction* t2;

void Initialize_Graph_Creater()
{
    bNum = 0;
    iNum = 0;
    dotGraph = fopen("dotGraph.txt","w");
    if (dotGraph == NULL)
    {
        printf("Error Opening File");
        exit(1);
    }
    int hashSize = 128;
    /* could be done in a single has map
    where the values are arrays of size 3 storing
    v,b,i */
    visitedBlocks = Create_HashTable(hashSize,&conCmp,
                                &conAssn,&conAssn,&hashFunc);
    
    blockNum = Create_HashTable(hashSize,&conCmp,
                                &conAssn,&intAssn,&hashFunc);
    instrNum = Create_HashTable(hashSize,&conCmp,
                                &conAssn,&intAssn,&hashFunc);
    return;
}

void Write_Edge(struct Container* p, struct Container*c, char* label)
{
    if (p == NULL || c == NULL || label == NULL)
    {
        return;
    }
    int parentBNum = *(int*)get_KeyValue(blockNum,p);
    int childBNum = *(int*)get_KeyValue(blockNum,c);
    fprintf(dotGraph,"bb%i:s -> bb%i:n [label=\"%s\"];\n",
                        parentBNum,childBNum,label);
    return;
}


void Write_Edges()
{
    
    for(unsigned int i = 0; i < blockNum->capacity; ++i)
    {
        struct Container* block = blockNum->keys[i];
        if (block == NULL)
        {
            continue;
        }
        Write_Edge(block,block->lbranch,"fall-through");
        Write_Edge(block,block->rbranch,"branch");
    }

    return;

}

void Write_Instruction(struct Instruction* instr)
{
    
    if (instr == NULL)
    {
        return;
    }

    if (getKeyIndex(instrNum,instr) == -1)
    {
        ++iNum;
        insert_KeyValue(instrNum,instr,&iNum);
    }
    int curINum = *(int*)get_KeyValue(instrNum,instr);

    fprintf(dotGraph,"%i: %s ",curINum,instr->instr);
    if (strcmp(instr->instr,"const") == 0)
    {
        fprintf(dotGraph,"#%i ",*(int*)instr->op1);
        return;
    }
    Write_Operand(instr->op1);
    Write_Operand(instr->op2);
    return;
}

void Write_Operand(struct Instruction* operand)
{
    if (operand != NULL)
    {
        if (getKeyIndex(instrNum, operand) == -1)
        {
            ++iNum;
            insert_KeyValue(instrNum,operand,&iNum);
        }
        
        int curINum = *(int*)get_KeyValue(instrNum, operand);
        fprintf(dotGraph,"(%i) ",curINum);
    }
    return;
}

void Write_Instructions(struct Container* block)
{
    if ( block == NULL)
    {
        return;
    }
    if (block->size == 0)
    {
        return;
    }

    Write_Instruction(block->values[0]);
    for (unsigned int i = 1; i < block->size; ++i)
    {
        fprintf(dotGraph, " | ");
        Write_Instruction(block->values[i]);
    }
    return;
}

void Write_Block(struct Container* block)
{

    if ((block == NULL) || (getKeyIndex(visitedBlocks,block) != -1))
    {
        return;
    }
    insert_KeyValue(visitedBlocks,block,NULL);
    ++bNum;
    insert_KeyValue(blockNum,block,&bNum);

    fprintf(dotGraph,"bb%i [ shape=record, label=\"<b>BB%i |  {\n",
                        bNum,bNum);
    struct Container* phiFunctions = Get_Phi_Functions(block);
    Write_Instructions(phiFunctions);
    if ( phiFunctions != NULL && block->size != 0)
    {
        fprintf(dotGraph, "| ");
    }
    Write_Instructions(block);
    fprintf(dotGraph,"}\"];\n");

    Write_Block(block->lbranch);
    Write_Block(block->rbranch);

    return;

}
/* create_graph expects a container whose values are containers
and those containers values are containers of instructions 
program => procedures => blocks => instructions */


void Create_Graph(struct Container* program)
{
    Initialize_Graph_Creater();
    if (program == NULL)
    {
        return;
    }
    fprintf(dotGraph,"digraph G {\n\n\n");

    for(unsigned int i = 0; i < program->size; ++i)
    {
        struct Container* proced = program->values[i];
        struct Container* intialBlock = proced->values[0];
        Write_Block(intialBlock);
    }
    fprintf(dotGraph,"\n\n");
    Write_Edges();
    fprintf(dotGraph, "\n\n}\n\n");
    
}