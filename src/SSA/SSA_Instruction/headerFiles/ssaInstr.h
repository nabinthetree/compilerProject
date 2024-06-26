

#ifndef SSA_INSTR_H
#define SSA_INSTR_H

#include "container.h"
#include "instruction.h"
#include "hashTable.h"

extern struct HashTable* identifiers;
extern struct HashTable* constants;


extern struct Container* smplProgram;
extern struct Container* joinBlocks;

extern struct Container* curBlock;
extern struct Instruction* curInstr;
extern struct Container* curProced;

extern unsigned int initSize;
extern int isWhileNested;
/* these functions assume at least 2 blocks
have been allocated for a current procedure */
void Connect_Blocks(int isLeftBranch);
void Connect_JBlock(int islBranch, int isParent);

void AddProced();
void AddBlock();
void AddInstr();
void AddLiteral(struct Instruction*);

void AddJBlock();
void Move_JBlock_To_CurProced();

void RemoveBlock(int);
void ResetBlocks();
void RemoveJBlock();


void SSA_Initialize_Program();
void SSA_Initialize_Instructions();


void UpdateInstr(struct Instruction* op1, char* operation,char* stringExp);
void UpdateTable(void* key, void* value);

void const_Instr_Parsed();
void ident_Instr_Parsed();

struct Instruction* GetInstr();
struct Container* GetJBlock();
struct Container* GetBlock();
struct Container* GetProced();
struct Instruction* Get_Ident_Instr(char* key);

void SetInstr(struct Instruction*);
//void Set_Instr_Ident(void* key, void * value);
void Set_While_Nest(int cond);
int Get_While_Nest();

int isConst(struct Instruction* i);

#endif