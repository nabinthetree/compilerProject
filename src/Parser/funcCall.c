





#include "lexer.h"
#include "funcCall.h"
#include "identFunc.h"
#include "expFunc.h"
#include "ioVar.h"
#include "ioFunc.h"
#include "ssaInstr.h"
#include "ssaInstrRename.h"
#include <string.h>
#include <assert.h>


void Check_WriteNL_Read();
int is_WriteOP();

/* the token "call" has already been parsed when
calling FuncCall, it proceeds according to the
smpl grammar rules */

void FuncCall()
{
    assert(strcmp("call",buffer) == 0);
    ident();

    Check_WriteNL_Read();
    int isWriteOutput = is_WriteOP();

    if (sym[0] != '(')
    {
        return;
    }
    nextToken();
    if (sym[0] == ')')
    {
        nextToken();
    }
    else 
    {
        ExpString();
        char ident[100];
        isIdent ? strcpy(ident,buffer) : strcpy(ident, "\0");
        if (isWriteOutput)
        {
            SetInstr(createInstruction("write",GetInstr(),NULL));
            AddInstr();
            AddExpression_To_Rename(ident,NULL,GetInstr());
        }
        while(sym[0] == ',')
        {
            nextToken();
            Exp();
        }
        assert(strcmp(")",sym) == 0);
        nextToken();
    }
}

void Check_WriteNL_Read()
{

    if ( strcmp(buffer, "InputNum") == 0)
    {
        SetInstr(createInstruction("read",NULL,NULL)); 
        AddInstr(); 
    }
    else if (strcmp(buffer,"OutputNewLine") == 0)
    {
        SetInstr(createInstruction("writeNL",NULL,NULL));
        AddInstr();
    }
    setFlags(0,0,0);
    return;
}

int is_WriteOP()
{
    return strcmp("OutputNum",buffer) == 0;
}