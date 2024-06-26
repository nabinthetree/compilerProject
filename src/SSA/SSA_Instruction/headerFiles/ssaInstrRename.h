



#ifndef SSAINSTR_RENAME_H
#define SSAINSTR_RENAME_H


#include "container.h"
#include "instruction.h"

extern struct Container* whileLoopExps;

void SSA_Renaming_Initialize();

void AddExpression_To_Rename(char* ident1,char* ident2, struct Instruction* exp);

/* helper function */
void AddExpression_To_Ident_Chain(int branch, char* ident, struct Instruction* exp);

void Add_RenamingStructure();
void Remove_RenamingStructure();

void Rename_Expressions(char* ident, struct Instruction* newInstr);















#endif