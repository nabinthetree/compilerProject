


#include "reOp.h"
#include "lexer.h"
#include "ioFunc.h"
#include "ioVar.h"


void ReOp()
{

    nextToken();
    if(!isReOp)
    {
        printf("%s is not a relational operator",buffer);
        Error();
    }
    return;
}

