





#include <string.h>
#include <assert.h>
#include "typeDeclFunc.h"
#include "identFunc.h"
#include "ioFunc.h"
#include "ioVar.h"
#include "lexer.h"
#include "numFunc.h"
#include "arrays.h"
#include <stdlib.h>
/* according the grammar, the first token in 
non terminal symbol has already been parsed */
/* "array" and "var" have already been parsed 
the following code starting parsing the [ for the the array */

void typeDecl()
{
    
    if (strcmp(buffer,"var") == 0)
    {
        strcpy(dataType,"intVar");
        return;
    }
    strcpy(dataType,"intArray");
    /*
    nextToken();
    assert(strcmp(buffer,"[") == 0);
    Num();
    nextToken();
    assert(strcmp(buffer,"]") == 0);
    */
    Clear_Dimensions();
    assert(sym[0] == '[');
    while(sym[0] == '[')
    {
        nextToken();
        Num();
        Add_Dimension(atoi(buffer));
        nextToken();
        assert(strcmp(buffer,"]") == 0);
    }
    return;
}