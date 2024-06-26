




#include "while.h"
#include "relation.h"
#include "keyString.h"
#include "statSeq.h"
#include "lexer.h"
#include "ioVar.h"
#include "instruction.h"
#include "container.h"
#include "ssaInstr.h"
#include "ssaPhi.h"
#include "ssaInstrRename.h"
#include "ssaComSubExp.h"
#include "kills.h"
#include <string.h>

#include <stdio.h>
#include <assert.h>

void While_SSA_Cond_Parse();
struct Instruction* While_SSA_Do_Parse();
void While_Exit(struct Instruction*, struct Container*);



void whileStatement()

{
    nextToken();

    assert(strcmp(buffer,"while") == 0);

    int prevWhileNest = Get_While_Nest();
    if(!prevWhileNest)
    {
        if (GetJBlock() == NULL) {Remove_ComSubExp_Structure();}
        Add_RenamingStructure();
    }
    Add_Kill_Block();
    Add_ComSubExp_Structure();
    Set_While_Nest(1);

    While_SSA_Cond_Parse();

    Relation();
    nextToken();

    assert(strcmp(buffer,"do") == 0);
    
    struct Instruction* condBra = While_SSA_Do_Parse();
    struct Container* body = GetBlock();

    int prevSize = GetNumOfStructures();
    statSeq();

    if ( prevSize != GetNumOfStructures()) {Remove_ComSubExp_Structure();}
    Go_To_Prev_Chains_Sizes();

    While_Exit(condBra,body);
    nextToken();
    assert(strcmp(buffer,"od") == 0);

    if(!prevWhileNest)
    {
        Set_While_Nest(0);
        Remove_RenamingStructure();
    }
    Create_Loads(GetBlock());
    Remove_Kill_Block();
}

void While_SSA_Cond_Parse()
{
    AddJBlock();
    Move_JBlock_To_CurProced();
    Connect_Blocks(1);

    return;
}
/* before the body is parsed */
struct Instruction* While_SSA_Do_Parse()
{
    struct Instruction* condBranch = createInstruction(strAssn(NULL,GetBranch()),Top(GetBlock()),NULL);
    SetInstr(condBranch);
    AddInstr();
    RemoveBlock(1);
    AddBlock();
    Connect_JBlock(1,1);
    AddBranch(1);

    /* comSubExp Elimination */
    Remember_Current_Chain_Sizes();
    return condBranch;
}


void While_Exit(struct Instruction* b, struct Container* body)
{
    Connect_JBlock(0,0);
    struct Instruction* bra;
    struct Container* phiFunctions = Get_Phi_Functions(GetJBlock());
    if ( phiFunctions == NULL)
    {
        bra = createInstruction("bra",Top(GetJBlock()),NULL);
    }
    else 
    {
        bra = createInstruction("bra",phiFunctions->values[0],NULL);
    }
    AddValue(GetBlock(),bra);
    RemoveBlock(2);
    AddBlock();
    Connect_JBlock(0,1);
    b->op2 = Top(GetBlock());
    struct Container* innerJBlock = GetJBlock();
    RemoveJBlock();
    RemoveBranch();
    Commit_Values(innerJBlock);
    return;
}