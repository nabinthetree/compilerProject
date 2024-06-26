



#include "lexer.h"
#include "stat.h"
#include "ioVar.h"
#include "ioFunc.h"



void statSeq()
{

    Statement();
    while(sym[0] == ';')
    {
        nextToken();
        if ( sym[0] != '}')
        {
            
            Statement();
        }
        else 
        {
            return;
        }
    }
    return;
}