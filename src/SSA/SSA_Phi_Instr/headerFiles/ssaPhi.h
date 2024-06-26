

#ifndef SSA_PHI_H
#define SSA_PHI_H



#include "hashTable.h"
#include "container.h"
#include "instruction.h"

/* oldValues maps strings to array of size 2 of void* */
struct HashTable* oldValues;

/* blockPhiFunctions maps containers to containers */
extern struct HashTable* blockPhiFunctions;
extern struct Container* branches;



void SSA_Phi_Initialize();
void Insert_Phi_Function(struct Container* block,
                        struct Instruction* newValue,
                        struct Instruction* olValue,
                        char* ident, int isCopyPropag,char* varCopyPropag);
void Back_Up_Value(struct Instruction* phiFunc, char* ident,
                    struct Instruction* oldValue);

void Restore_Back_Ups(struct Container* block);

void Commit_Values(struct Container* innerBlock);
void Update_Current_Values();
void AddBranch(int islBranch);
void RemoveBranch();
struct Container* Get_Phi_Functions(struct Container* c);

//helper Function
struct Instruction* Get_Ident_Phi_Func(struct Container* phiFunctions,char* ident);

















#endif