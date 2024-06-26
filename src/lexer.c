


#include "lexer.h"
#include "ioVar.h"
#include "ioFunc.h"
#include <string.h>


isIdent = 0;
isKeyWord = 0;
isReOp = 0;

int branchIndex;
char* branchCond[6] = {"bgt","bge","blt","ble","beq","bne"};
char dataType[50];
char expString[100];
// next token expects the current char
// pointed to by sym to not be a space
void nextToken()
{
    if (isDigit())
    {
        parseNumber();
    }
    else if (isChar())
    {
        parseIdent();
    }
    else if (sym[0] == '=')
    {
        parseEqual();
    }
    else if (sym[0] == '<')
    {
        parseLessThan();
    }
    else if (sym[0] == '>')
    {
        parseGreaterThan();
    }
    else if (sym[0] == '!')
    {
        parseNEqual();
    }
    else 
    {
        parseSingleTerminal();
    }
    nextNonSpace();
}


void parseNumber()
{
    unsigned int i = 0;
    while(isDigit())
    {
        buffer[i] = sym[0];
        nextChar();
        ++i;
    }
    buffer[i] = '\0';
    setFlags(0,0,0);
}


void parseIdent()
{
    unsigned int i = 0;
    while(isChar() || isDigit())
    {
        buffer[i] = sym[0];
        nextChar();
        ++i;
    }
    buffer[i] = '\0';
    setFlags(1,0,0); // come back to this to handle keywords
}

void parseSingleTerminal()
{
    buffer[0] = sym[0];
    buffer[1] = '\0';
    nextChar();
  
}

void parseEqual()
{
    nextChar();
    buffer[0] = '=';
    buffer[1] = sym[0];
    buffer[2] = '\0';
    nextChar();
    setFlags(0,0,!strcmp(buffer,"=="));
    setBranchCond(5);
}

void parseNEqual()
{
    nextChar();
    buffer[0] = '!';
    buffer[1] = sym[0];
    buffer[2] = '\0';
    nextChar();
    setFlags(0,0,!strcmp(buffer, "!="));
    setBranchCond(4);
}

void parseLessThan()
{
    buffer[0] = '<';
    buffer[1] = '\0';
    nextChar();
    if (sym[0] == '-' || sym[0] == '=')
    {
        buffer[1] = sym[0];
        buffer[2] = '\0';
        nextChar();
        setBranchCond(0);
    }
    else 
    {
        setBranchCond(1);
    }
    setFlags(0,0,!strcmp(buffer,"<") || !strcmp(buffer,"<="));
}

void parseGreaterThan()

{
    buffer[0] = '>';
    buffer[1] = '\0';
    nextChar();
    if (sym[0] == '=')
    {
        buffer[1] = sym[0];
        buffer[2] = '\0';
        nextChar();
        setBranchCond(2);
    }
    else 
    {
        setBranchCond(3);
    }
    setFlags(0,0,!strcmp(buffer,">") || !strcmp(buffer,">="));
}


void setFlags(int identFlag, int keywordFlag, int reOpFlag)
{
    isIdent = identFlag;
    isKeyWord = keywordFlag;
    isReOp = reOpFlag;
}

void setBranchCond(int index)
{
    branchIndex = index;
}

char* GetBranch() { return branchCond[branchIndex];}