

#ifndef DOT_GRAPH_H
#define DOT_GRAPH_H

#include "container.h"
#include "instruction.h"
#include "hashTable.h"
#include "stdio.h"




/* hash maps; map containers/instructions => integers */

extern struct HashTable* blockNum;
extern struct HashTable* instrNum;

/* hash maps; map containers => NULL */
extern struct HashTable* visitedBlocks;
extern FILE* dotGraph;
extern int bNum;
extern int iNum;

void Initialize_Graph_Creater();
void Create_Graph(struct Container*);

void Write_Block(struct Container*);
void Write_Instructions(struct Container*);

void Write_Instruction(struct Instruction* instr);
void Write_Operand(struct Instruction* op);

void Write_Edges();
void Write_Edge(struct Container*p,struct Container*c,char* label);








#endif