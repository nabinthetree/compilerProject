
#ifndef LEXER_H
#define LEXER_H

extern int isIdent;
extern int isKeyWord;
extern int isReOp;

extern int branchIndex;
extern char* branchCond[6];
extern char dataType[50];
extern char expString[100];

void nextToken();


void parseNumber();
void parseIdent();
void parseEqual();
void parseNEqual();
void parseLessThan();
void parseGreaterThan();
void parseSingleTerminal();

void setBranchCond(int index);
void setFlags(int identFlag, int keywordFlag, int reOpFlag);

char* GetBranch();


#endif