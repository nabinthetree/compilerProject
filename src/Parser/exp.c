



#include "expFunc.h"
#include "ioFunc.h"
#include "termFunc.h"

#include "lexer.h"
#include "ioVar.h"
#include "ssaInstr.h"
#include "ssaInstrRename.h"
#include "instruction.h"
#include <string.h>
#include <stdio.h>

void Exp()
{
  Term();
  char ident1[100];
  char ident2[100];
  char prevExpStr[100];
  prevExpStr[0] = '\0';
  isIdent ? strcpy(ident1,buffer) : strcpy(ident1,"\0");
  while( sym[0] == '+' || sym[0] == '-')
  {
    strcpy_s(prevExpStr,100,expString);
    struct Instruction* prevInstr = GetInstr();
    char* operation = sym[0] == '+' ? "add" : "sub";
    strncat_s(prevExpStr,100,sym,strlen(sym));
    nextToken();
    Term();
    /* the new expression string is actually created
    * before its even created as an instruction node */
    strncat_s(prevExpStr,100,expString,strlen(expString));
    strcpy_s(expString,100,prevExpStr);
    isIdent ? strcpy(ident2,buffer) : strcpy(ident2,"\0");
    UpdateInstr(prevInstr,operation,expString);

    //AddInstr();
    if (strcmp(GetInstr()->instr,"const") != 0)
    {
      AddExpression_To_Rename(ident1,ident2, GetInstr());
      setFlags(0,0,0);
      strcpy(ident1,"\0");
    }
  }
  isIdent ? strcpy(buffer,ident1) : strcpy(buffer,buffer);
  return;
}



void ExpString()
{
  expString[0] = '\0';
  Exp();
  //printf("%s\n",expString);
}