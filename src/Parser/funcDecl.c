



#include "funcDecl.h"
#include "formalParam.h"
#include "funcBody.h"

#include "lexer.h"
#include "ioVar.h"
#include "identFunc.h"
#include "numFunc.h"

#include <assert.h>
#include <string.h>


void FuncDecl()
{
    if (strcmp(buffer,"void") == 0)
    {
        nextToken();
    }
    assert(strcmp(buffer,"function") == 0);

    ident();
    FormalParam();
    nextToken();

    assert(strcmp(buffer, ";") == 0);

    FuncBody();
    nextToken();

    assert(strcmp(buffer,";") == 0);
}
