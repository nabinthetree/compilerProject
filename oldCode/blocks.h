

#ifndef BLOCKS_H
#define BLOCKS_H


struct Instruction 
{
    
    char* instr;
    struct Instruction* op1;
    struct Instruction* op2;

    char* val;
    

    /* Why do we need the third
    operand. I forgot :( */

    //Instruction op3;
};
struct Blocks
{

    struct Instruction* instr;
};

struct Instruction* createInstruction(char*,struct Instruction*,struct Instruction*,char*);




#endif