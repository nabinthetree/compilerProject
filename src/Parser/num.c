

#include <stdio.h>
#include <stdlib.h>

#include "ioVar.h"
#include "ioFunc.h"
#include "lexer.h"

#include "numFunc.h"
#include "ssaInstr.h"
#include "symTable.h"

void Num()
{
    nextToken();
    if (isIdent)
    {
        Error();
    }
    Parse_Literal(atoi(buffer));
    SetInstr(Get_Last_Parsed_Instr());
    //const_Instr_Parsed();
    
}