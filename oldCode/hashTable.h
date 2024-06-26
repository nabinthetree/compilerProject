




#include "blocks.h"


extern int tableSize;
extern char* keys[10];
extern struct Instruction* keyInstr[10];

int getKeyIndex(char*);
int hashFunction(char*);
void setKeyInstruction(char*,struct Instruction*);
struct Instruction* getKeyInstruction(char*);
void expand_Hash_Table();
