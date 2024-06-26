



#include "if.h"
#include "lexer.h"
#include "ioVar.h"
#include "ioFunc.h"
#include "relation.h"
#include "statSeq.h"

#include "ssaInstr.h"
#include "ssaPhi.h"
#include "ssaComSubExp.h"
#include "container.h"
#include "instruction.h"
#include "kills.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void SSA_If_SetUp();
struct Instruction* SSA_Then_Parse();
void SSA_Else_Parse(struct Instruction*);

void ifStatement()
{
    Add_Kill_Block();
    nextToken();
    assert(strcmp(buffer,"if") == 0);
    
    Relation();
    SSA_If_SetUp();

    nextToken();
    assert(strcmp(buffer,"then") == 0);
    int prevSize = GetNumOfStructures();
    statSeq();
    if ( prevSize != GetNumOfStructures()) { Remove_ComSubExp_Structure();}
    struct Instruction* branch = SSA_Then_Parse();

    nextToken();
    if (strcmp(buffer,"else") == 0)
    {
        statSeq();
        nextToken();
    }
    if (prevSize != GetNumOfStructures()) { Remove_ComSubExp_Structure();}
    SSA_Else_Parse(branch);

    assert(strcmp(buffer,"fi") == 0);
    Create_Loads(GetBlock());
    Remove_Kill_Block();
}

void SSA_If_SetUp()
{
    /* used to for ComSubExp Elimination */
    Remember_Current_Chain_Sizes();

    /* set the expression on what to branch in the branch instruction */
    struct Instruction* cmp = Top(GetBlock());
    struct Instruction* branch = createInstruction(GetBranch(),cmp,NULL);
    SetInstr(branch);
    AddInstr();
    /*---------*/
    /* this is for SSA, the joinBlock before then is parsed */
    AddJBlock();
    AddBlock();
    Connect_Blocks(1);
    AddBranch(1);
    /*----------------------------*/
    return;
}
/* this is called after then is parsed */
struct Instruction* SSA_Then_Parse()
{
    
    struct Instruction* branch = createInstruction("bra",NULL,NULL);
    SetInstr(branch);
    AddInstr();
    branch = Top(GetBlock());
    

    /* SSA after then is parsed*/
    RemoveBranch();
    AddBranch(0);
    Restore_Back_Ups(GetJBlock());

    Connect_JBlock(0,0);
    RemoveBlock(1);

    struct Instruction* condBranch = Top(GetBlock());
    /*-------------------*/
    /* SSA for the else Block, whether its there or not,
    container is created */
    AddBlock();
    Connect_Blocks(0);
    condBranch->op2 = Top(GetBlock());
    /* ----------------------- */
    Go_To_Prev_Chains_Sizes();
    return branch;
}

void SSA_Else_Parse(struct Instruction* bra)
{

    RemoveBranch();
    Connect_JBlock(1,0);


    RemoveBlock(2);
    Move_JBlock_To_CurProced();

    if(Get_Phi_Functions(GetBlock()) ==  NULL)
    {
        AddValue(GetBlock(),createInstruction("empty",NULL,NULL));
        bra->op1 = Top(GetBlock());
    }
    else 
    {
        bra->op1 = Get_Phi_Functions(GetBlock())->values[0];
    }
    
    RemoveJBlock();
    Commit_Values(GetBlock());

    Go_To_Prev_Chains_Sizes();
    Pop_Current_Prev_Size();
    return;
}