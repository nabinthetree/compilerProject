

#include "blocks.h"
#include <stdlib.h>







struct Instruction* createInstruction(char* instr,struct Instruction* op1, struct Instruction* op2, char* val)
{
    struct Instruction* instruction = malloc(sizeof(struct Instruction));
    
    instruction->instr = instr;
    instruction->op1 = op1;
    instruction->op2 = op2;
    instruction->val = val;
    
    return instruction;
}