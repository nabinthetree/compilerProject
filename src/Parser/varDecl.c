




#include "varDecFunc.h"
#include "identFunc.h"
#include "numFunc.h"
#include "ioFunc.h"
#include "ioVar.h"
#include "typeDeclFunc.h"
#include "lexer.h"
#include "hashTable.h"
#include "arrays.h"
#include "ssaInstr.h"
#include "symTable.h"
#include <string.h>
#include <assert.h>

void varDecl()
{
    typeDecl();
    ident();
    void (*parseDecl)(void*);
    parseDecl = strcmp("intVar",dataType) == 0 ? &Parse_Int_Decl : &arrayParse;
    (*parseDecl)(buffer);
    nextToken();
    while (strcmp(buffer, ",") == 0)
    {
        ident();
        (*parseDecl)(buffer);
        nextToken();
    }
    assert(strcmp(buffer,";") == 0);
    return;
}