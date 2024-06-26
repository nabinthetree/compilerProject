





#include "expFunc.h"
#include "reOp.h"
#include "relation.h"
#include "instruction.h"
#include "lexer.h"
#include "ssaInstr.h"
#include "ioVar.h"
#include "ssaInstrRename.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>


void Relation()
{
    char ident[100];
    char ident2[100];
    char compString[100];
    compString[0] = '\0';
    Exp();
    strncat_s(compString,100,expString,strlen(expString));
    isIdent ? strcpy(ident,buffer) : strcpy(ident,"\0");
    struct Instruction* op1 = GetInstr();
    ReOp();
    strncat_s(compString,100,buffer,strlen(buffer));
    Exp();
    strncat_s(compString,100,expString,strlen(expString));
    strcpy_s(expString,100,compString);
    isIdent ? strcpy(ident2,buffer) : strcpy(ident2,buffer);
    UpdateInstr(op1,"cmp",expString);
    //AddInstr();
    AddExpression_To_Rename(ident,ident2,GetInstr());
    return;
}