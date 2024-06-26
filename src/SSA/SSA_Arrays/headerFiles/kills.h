






#ifndef KILLS_H
#define KILLS_H

#include "container.h"
#include "instruction.h"
#include "ssaComSubExp.h"

extern struct Container* killBlocks;

void Initialize_Kills();
void Add_Kill_Block();
void Add_Kill(struct Instruction* adda);
void Remove_Kill_Block();
void Create_Loads(struct Container* block);
struct Container* Get_Kill_Block();


#endif