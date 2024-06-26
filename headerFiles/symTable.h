


#ifndef SYMTABLE_H
#define SYMTABLE_H


#include "hashTable.h"



extern struct HashTable* literals;
extern struct HashTable* integers;
extern struct Instruction* lastParsedInstr;


void Initialize_Tables();
void Parse_Literal(int val);
void Parse_Int_Desg(char*var);
void Parse_Int_Decl(char* var);
void Set_Unitialized_Zero(char* var);
void Update_Int_Table(char* var, struct Instruction* value);

void Compute_Literal(int* op1, int* op2,char* operation);
int Compute_Boolean(int* op1, int* op2, char* operation);

struct Instruction* Get_Last_Parsed_Instr();

#endif