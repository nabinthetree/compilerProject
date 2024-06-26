




#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ioVar.h"
#include "ioFunc.h"
#include "compFunc.h"
#include "lexer.h"
#include "varDecFunc.h"
#include "statSeq.h"
#include "funcDecl.h"

#include "ssaInstr.h"


// starting variable for the grammar
// every program of language smpl starts
// with a computation

void Comp()
{
    nextToken();
    if (strcmp(buffer, "main") != 0)
    {
        Error();
    }
    /* main function is being parsed for the smpl program => add a procedure, and block */
    AddProced();
    AddBlock();
    
    nextToken();
    while (strcmp(buffer, "{") != 0)
    {
        if (strcmp(buffer,"var") == 0 || strcmp(buffer,"array") == 0)
        {
           
            varDecl();
        }
        else if ( strcmp(buffer, "void") == 0 || 
                    strcmp(buffer,"function") == 0)
        {
            FuncDecl();
            /* call the function declaration here */
        }
        else 
        {
            Error();
        }
        nextToken();
    }
    statSeq();
    nextToken();
    if (strcmp(buffer, "}") != 0)
    {
        Error();
    }
    nextToken();
    if (strcmp(buffer, ".") != 0)
    {
        Error();
    }

    return;
}