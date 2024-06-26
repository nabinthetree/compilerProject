



#ifndef INSTRUCTION_H
#define INSTRUCTION_H




struct Instruction 
{
    
    char* instr;
    void* op1;
    void* op2;

    /* Why do we need the third
    operand. I forgot :( */

    //Instruction op3;
};

struct Instruction* instrAssn(struct Instruction* des, struct Instruction* source);

int instrCmp(struct Instruction* instr1, struct Instruction* instr2);


struct Instruction* createInstruction(char* instr,
                                        void*,
                                        void*);

struct Instruction* ComputeInstruction(struct Instruction* instr1, struct Instruction* instr2, char* operation);

void instrMemAssn(struct Instruction* des, void* op1, void* op2, char* instr);





#endif