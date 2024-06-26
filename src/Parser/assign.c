




#include "identFunc.h"
#include "expFunc.h"
#include "assign.h"
#include "lexer.h"
#include "designator.h"
#include "ioFunc.h"
#include "ioVar.h"
#include "arrays.h"
#include <string.h>
#include <assert.h>

#include "hashTable.h"
#include "ssaInstr.h"
#include "ssaPhi.h"
#include "ssaInstrRename.h"
#include "symTable.h"
#include <stdlib.h>

void Assign()
{
    nextToken();

    assert(strcmp(buffer,"let") == 0);
    
    ident();

    char variable[100];
    int isArr;
    strcpy(variable,buffer);

    Designator();
    struct Instruction* oldValue = GetInstr();
    struct Instruction* adda = Get_Adda_Instr();
    isArr = isVarArray(variable) ? 1 : 0;
    nextToken();
    assert(strcmp(buffer,"<-") == 0);
    ExpString();
    if (isArr)
    {
        Create_Store_Instruction(variable,GetInstr(),adda);
        Add_Exp_To_Chain("load",GetInstr(),"\0");
        return;
    }
    if (isIdent)
    {
        struct Instruction* copyPropag = createInstruction(variable,NULL,NULL);
        AddExpression_To_Rename(buffer,NULL,copyPropag);
    }
    //Get_Ident_Instr(variable)
    Insert_Phi_Function(GetJBlock(),GetInstr(),
                        oldValue,variable,isIdent,buffer);
    //UpdateTable(variable, GetInstr());
    Update_Int_Table(variable,GetInstr());
    return;
}