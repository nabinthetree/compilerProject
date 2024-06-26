


#include "ioVar.h"
#include "ioFunc.h"

#include "identFunc.h"
#include "numFunc.h"
#include "expFunc.h"
#include "facFunc.h"
#include "funcCall.h"
#include "lexer.h"
#include "designator.h"
#include "ssaInstr.h"
#include "symTable.h"
#include "arrays.h"
#include <string.h>



void Fac()
{
    if (isDigit())
    {
        Num();
        strcpy_s(expString,100,buffer);
        return;
    }
    else if (sym[0] == '(')
    {
        char paren[100];
        paren[0] = '\0';
        strncat_s(paren,100,"(",1);
        nextToken();
        Exp();
        strncat_s(paren,100,expString,strlen(expString));
        strncat_s(paren,100,")",1);
        strcpy_s(expString,100,paren);
        nextToken();
        if (strcmp(buffer,")") != 0)
        {
            Error();
        }
        return;     
    }
    ident();
    char var[100];
    strcpy_s(var,100,buffer);
    if (strcmp(buffer, "call") == 0)
    {
        /* handle funcall function here */
        FuncCall();
        return;
    }
    /* designator function goes here */
    Designator();
    if (GetInstr() == NULL)
    {
        Set_Unitialized_Zero(var);
        SetInstr(Get_Last_Parsed_Instr());
    }
    if (isVarArray(var))
    {
        Create_Load_Instruction(var);
    }
    return;
}