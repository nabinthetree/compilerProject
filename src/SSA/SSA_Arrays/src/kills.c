





#include "kills.h"
#include "container.h"
#include "instruction.h"
#include "ssaComSubExp.h"
#include "ssaInstr.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>


struct Container* killBlocks;

void Initialize_Kills()
{
    killBlocks = Create_Container(15,&instrAssn);
}

void Add_Kill_Block()
{
    AddValue(killBlocks,Create_Container(15,&instrAssn));
}
/* not very efficient, checking kill instructions every time */
void Add_Kill(struct Instruction* adda)
{
    struct Container* block = Top(killBlocks);
    assert(block != NULL);
    for (unsigned int i = 0; i < block->size; ++i )
    {
        if (instrCmp(block->values[i],adda))
        {
            return;
        }
    }
    AddValue(block,adda);
}

void Remove_Kill_Block()
{
    RemoveBack(killBlocks);
}
/* expects to be called after if and while parse functions have ended */
/* few assumptions made from this function which is bad programming */
void Create_Loads(struct Container* block)
{
    struct Instruction* load;
    struct Container* kills = Top(killBlocks);
    struct Instruction* adda;
    struct Instruction* comAdda;
    if (kills->size == 0) { return;}
    /* kills might contain an empty instruction, needs to get handled */
    unsigned int killIndex = 0;
    struct Instruction* empty = block->values[0];
    if (strcmp(empty->instr,"empty") == 0 )
    {
        while(killIndex < kills->size)
        {
            adda = kills->values[killIndex];
            comAdda = FindComExp(adda,"\0");
            if ( comAdda != NULL)
            {
                instrMemAssn(empty,adda,NULL,"load");
                Add_Exp_To_Chain("load",empty,"\0");
                break;
            }
            ++killIndex;
        }
    }
    for (++killIndex; killIndex < kills->size; ++killIndex)
    {
        adda = kills->values[killIndex];
        /* come back to fix this */
        comAdda = FindComExp(adda,"\0");

        if ( comAdda == NULL)
        {
            continue;
        }
  
        load = createInstruction("load",comAdda,NULL);
        
        AddValue(block,load);
        /* come back to fix arrays */
        Add_Exp_To_Chain("load",load,"\0");
    }
    return;
}

struct Container* Get_Kill_Block() { return Top(killBlocks);}