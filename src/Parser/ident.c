


#include "identFunc.h"
#include "ioFunc.h"
#include "ioVar.h"
#include "lexer.h"
#include "ssaInstr.h"

void ident()
{
    nextToken();
    if(!isIdent)
    {
        Error();
    }
}