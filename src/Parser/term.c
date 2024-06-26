



#include "termFunc.h"
#include "ioFunc.h"
#include "facFunc.h"

#include "ioVar.h"
#include "lexer.h"

#include "instruction.h"
#include "ssaInstr.h"
#include "ssaInstrRename.h"
#include <string.h>

void Term()
{
    Fac();
    char ident1[100];
    char ident2[100];
    char prevExpStr[100];
    prevExpStr[0] = '\0';
    isIdent ? strcpy_s(ident1,100,buffer) : strcpy_s(ident1,100,"\0");
    while (sym[0] == '*' || sym[0] == '/')
    {
        strcpy_s(prevExpStr,100,expString);
        struct Instruction* prevInstr = GetInstr();
        char* operation = sym[0] == '*' ? "mul" : "div";
        strncat_s(prevExpStr,100,sym,strlen(sym));
        nextToken();
        Fac();
        strncat_s(prevExpStr,100,expString,strlen(expString));
        strcpy_s(expString,100,prevExpStr);
        isIdent ? strcpy_s(ident2,100,buffer) : strcpy_s(ident2,100,"\0");
        UpdateInstr(prevInstr,operation,expString);
        //AddInstr();
        if (strcmp(GetInstr()->instr,"const") != 0)
        {
            AddExpression_To_Rename(ident1,ident2,GetInstr());
            setFlags(0,0,0);
            strcpy_s(ident1,100,"\0");
        }
    }
}