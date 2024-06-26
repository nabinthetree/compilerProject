






#include "ssaComSubExp.h"
#include "ssaInstr.h"
#include "instruction.h"
#include "container.h"
#include "hashTable.h"
#include "keyString.h"


#include <stdlib.h>
#include <stdio.h>



extern struct HashTable* arrays;
extern struct Container* curVarDimens;
extern struct Container* arrayIndices;
extern struct Container* indicesToRename;
extern struct Instruction* varBaseOffset;
extern struct Instruction* addaInstr;

int isVarArray(char* var);
void Initialize_Arrays();
void Add_Dimension(int dim);
void Add_Index(struct Instruction* index,char* var);
void Clear_Indices();
void Clear_Dimensions();
void Insert_Array(char* arrayName,struct Container* dimens, struct Instruction* baseOffset);
void Calculate_Array_Index(char* arrayName, struct Container** indices);

struct Instruction* Get_Adda_Instr();
struct Instruction* Get_BaseOffSet();
struct Container* Get_Dimensions();
struct Container* Get_Indices();

void SetBaseOffSet();
void Create_Store_Instruction(char* symbol,struct Instruction* valToStore, struct Instruction* adda);
void Create_Load_Instruction(char* symbol);
void arrayParse(char* symbol);