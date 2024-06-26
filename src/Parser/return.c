



#include "return.h"
#include "lexer.h"
#include "expFunc.h"
#include "ioVar.h"
#include "ioFunc.h"

#include <string.h>


void Return()
{
    
    nextToken();
    if (strcmp(buffer,"return") != 0)
    {
        Error();
    }
    if (sym[0] != '}' && sym[0] != ';')
    {
        ExpString();
    }
    return;
}