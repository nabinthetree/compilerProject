



#include "funcBody.h"
#include "ioVar.h"
#include "lexer.h"
#include "varDecFunc.h"
#include "statSeq.h"

#include <assert.h>
#include <string.h>

void FuncBody() 
{

    while (sym[0] != '{')
    {
        nextToken();
        varDecl();
    }

    nextToken();
    if (sym[0] != '}')
    {
        statSeq();
    }
    nextToken();
    assert(strcmp(buffer, "}") == 0);

}