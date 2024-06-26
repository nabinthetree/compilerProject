

#include <stdio.h>
#include <stdlib.h>

#include "ioVar.h"
#include "ioFunc.h"

#include "errno.h"

FILE* ptr; 
char sym[2];
char buffer[100];
int numEndofFiles = 0;


void openProgram(const char* fileName)
{
    /* Open File for just reading */
    ptr = fopen(fileName, "r");
    if (ptr == NULL)
    {
        printf("error oppening file");
        exit(1);
    }

}

void closeProgram()
{
    fclose(ptr);
}

void nextChar()
{
   size_t numBytesRead = fread(sym,1,1,ptr);
   if (numBytesRead != 1)
   {
    sym[0] = '\0';
     if (numEndofFiles >= 1)
     {
        printf("End of File Reached");
        exit(1);
     }
     ++numEndofFiles;
    //exit(1);
   }
}

void nextNonSpace()
{
    

    while (sym[0] == ' ' || sym[0] == '\r' || sym[0] == '\n'
            || sym[0] == '\t')
    {
        nextChar();
    }
}

int isDigit()
{
    return sym[0] - '0' >= 0 && sym[0] - '0' <= 9;
}


int isChar()
{
    return ((sym[0] - 'A') >= 0 && (sym[0] - 'A') <= 25) ||
            ( (sym[0] - 'a') >= 0 && (sym[0] - 'a') <= 25);
}


void Error()
{
    printf("SyntaxError");
    exit(1);
}