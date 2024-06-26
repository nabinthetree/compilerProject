


#include <stdio.h>
#include <string.h>

#include "ioVar.h"

#include "ioFunc.h"
#include "compFunc.h"
#include "ssaInstr.h"
#include "dotGraph.h"
#include "ssaComSubExp.h"
#include "ssaPhi.h"
#include "ssaInstrRename.h"
#include "arrays.h"
#include "symTable.h"
#include "kills.h"

int main(int argc, char *argv[])
{
    /* Gets the program ready to parse the
    smpl program */
    
    openProgram(argv[1]);
    nextChar();
    nextNonSpace();

    Initialize_Kills();
    SSA_Initialize_Program();
    SSA_Renaming_Initialize();
    SSA_Phi_Initialize();
    Initialize_Arrays();
    SSA_ComSubExp_Initialize();
    Add_ComSubExp_Structure();

     /* adding the first procedure and block are for the constants */
    AddProced();
    AddBlock();
    GetBlock()->size = 0;
    /*------------------*/
    Initialize_Tables();
    Comp();
    Create_Graph(smplProgram);
    closeProgram();
   
    return 0;
}






/*
void printMenu()
{
    printf("hi my name is Navin Velazc\n");
    printf("I am student at UCI\n");
    printf("I like to do many activies");
}

*/
/*
void(* buf)();
    buf = &printMenu;
    (*buf)();
*/
/* this declares a pointer to a function
with no arguemnts and void return type of zero */