


#include "stat.h"
#include "ioVar.h"
#include "lexer.h"
#include "assign.h"
#include "funcCall.h"
#include "return.h"
#include "if.h"
#include "while.h"

void Statement()
{
    if (sym[0] == 'l')
    {
        Assign();
        return;
    }
    else if (sym[0] == 'i')
    {
        ifStatement();
        /* if statement handle it here */
        return;
    }
    else if (sym[0] == 'w')
    {
        /* while statement handle it here */
        whileStatement();
        return;
    }
    else if (sym[0] == 'r')
    {
        Return();
        return;
    }
    else if (sym[0] == 'c')
    {
        nextToken();
        FuncCall();
    }
    return;
}