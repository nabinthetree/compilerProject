

#ifndef SSA_COM_SUB_EXP_H
#define SSA_COM_SUB_EXP_H


#include "hashTable.h"
#include "instruction.h"
#include "container.h"

extern struct Container* hashOperChains;
extern struct Container* hashPrevSizes;
extern struct Container* expStrings;
/* hash maps that  map strings => containers */
extern struct HashTable* operChains;
extern struct HashTable* prevSizes;
extern struct HashTable* stringChain;
extern unsigned int hashChainSize;

extern char* instructions[14];
extern unsigned int numInstrs;

struct Instruction* FindComExp(struct Instruction* curInstr, char* stringExp);
struct Instruction* FindComLoad(struct Instruction* load);

void Add_Exp_To_Chain(char* operation, struct Instruction*,char* stringExp);
void Go_To_Prev_Chains_Sizes();
void Pop_Current_Prev_Size();
void Remember_Current_Chain_Sizes();
void SSA_ComSubExp_Initialize();

void Add_ComSubExp_Structure();
void Remove_ComSubExp_Structure();

unsigned int GetNumOfStructures();

/* meant for using at time of hashTable initialization*/
void Add_Keys(struct HashTable* h);
void Add_Container_Values(struct HashTable* h, 
                        void*(*valueAssn)(void* des,void* source));
char* Get_Exp_String(struct Instruction* instr);

#endif