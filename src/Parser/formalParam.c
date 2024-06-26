


#include "lexer.h"
#include "ioVar.h"
#include "identFunc.h"
#include "formalParam.h"

#include <string.h>
#include <assert.h>



void FormalParam()
{

    nextToken();

    assert(strcmp(buffer,"(") == 0);

    if (sym[0] != ')')
    {
        ident();
        while (sym[0] == ',')
        {
            nextToken();
            ident();
        }
    }
    nextToken();
    assert(strcmp(buffer,")") == 0);
}