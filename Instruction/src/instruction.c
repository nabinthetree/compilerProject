


#include "instruction.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


struct Instruction* createInstruction(char* instr,void* op1, void* op2)
{
    struct Instruction* instruction = malloc(sizeof(struct Instruction));
    
    instruction->instr = instr;
    instruction->op1 = op1;
    instruction->op2 = op2;

    return instruction;
}


struct Instruction* instrAssn(struct Instruction* des, struct Instruction* source)
{
    return source;
}


int instrCmp(struct Instruction* instr1, struct Instruction* instr2)
{
    
    if (strcmp(instr1->instr, instr2->instr) != 0)
    {
        return 0;
    }
    if (strcmp(instr1->instr,"div") == 0 || strcmp(instr1->instr,"sub") == 0)
    {
        return (instr1->op1 == instr2->op1) && (instr1->op2 == instr2->op2);
    }
    if (strcmp(instr1->instr,"add") == 0 || strcmp(instr1->instr,"mul") == 0
        || strcmp(instr1->instr,"adda") == 0)
    {
        if (instr1->op1 == instr2->op1)
        {
            return instr1->op2 == instr2->op2;

        }
        if (instr1->op1 == instr2->op2)
        {
            return instr1->op2 == instr2->op1;
        }
        return 0;
    }

    if (strcmp(instr1->instr,"const") == 0)
    {
        return *(int*)instr1->op1 == *(int*)instr2->op1;
    }
    if (instr1 != instr2)
    {
        return 0;
    }

    if ( instr1->op1 == instr2->op1)
    {
        return instr1->op2 == instr2->op2;
    }
    return 0;
}


struct Instruction* ComputeInstruction(struct Instruction* op1, struct Instruction* op2, char* operation)
{
    /*
    if (op1 == NULL || op2 == NULL)
    {
        return createInstruction(operation,op1,op2);
    }
    */
    return createInstruction(operation,op1,op2);
    /*
    if (strcmp(op1->instr,"const") != 0 || strcmp(op2->instr,"const") != 0)
    {
        return createInstruction(operation,op1,op2);
    }
    int oper1 = *(int*)op1->op1;
    int oper2 = *(int*)op2->op1;
    int* operResult = malloc(sizeof(int));
    struct Instruction* result = NULL;

    if (strcmp(operation,"add") == 0)
    {
        
        *operResult = oper1 + oper2;
        result = createInstruction("const",operResult,NULL);
        return result;
    }
    if (strcmp(operation,"sub") == 0)
    {
        *operResult = oper1 - oper2;
        result = createInstruction("const",operResult,NULL);
        return result;
    }

    if (strcmp(operation, "div") == 0)
    {
        *operResult = oper1 / oper2;
        result = createInstruction("const",operResult,NULL);
        return result;
    }

    if (strcmp(operation,"mul") == 0)
    {
        
        *operResult = oper1 * oper2;
        result = createInstruction("const",operResult,NULL);
        return result;
    }

    if (strcmp(operation, "cmp") == 0)
    {
        return createInstruction(operation,op1,op2);
    }
    
    return NULL;
    */
}

void instrMemAssn(struct Instruction* des, void* op1,void* op2,char* instr)
{
    des->instr = instr;
    des->op1 = op1;
    des->op2 = op2;
}