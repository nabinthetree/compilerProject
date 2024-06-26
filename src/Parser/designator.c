

#include "arrays.h"
#include "designator.h"
#include "lexer.h"
#include "ioVar.h"
#include "ioFunc.h"
#include "expFunc.h"
#include "ssaInstr.h"
#include "symTable.h"
#include <string.h>
#include <assert.h>

/* the call before designator is always factor (correction, assign is can also be
* called right before designator, either assign or fac */
/* this means that ident is the last thing on the buffer */
void Designator()
{
    char var[100];
    strcpy(var,buffer);
    char arrayExp[100];
    arrayExp[0] = '\0';
    strcpy_s(arrayExp,100,var);
    if (sym[0] != '[')
    {
        strcpy(dataType,"intVar");
        Parse_Int_Desg(buffer);
        SetInstr(Get_Last_Parsed_Instr());
        strcpy_s(expString,100,arrayExp);
        //ident_Instr_Parsed();
        return;
    }
    strcpy(dataType,"intArray");
    struct Container* indiceInfo[2] = {Create_Container(5,&Direct_Assn),
                                        Create_Container(5,&strAssn)};
    char renameVar[100];                          
    //Clear_Indices();
    while(sym[0] == '[')
    {
        strncat_s(arrayExp,100,sym,strlen(sym));
        nextToken();
        Exp();
        strncat_s(arrayExp,100,expString,strlen(expString));
        isIdent ? strcpy(renameVar,buffer) : strcpy(renameVar,"\0");
        //Add_Index(GetInstr(),var);
        AddValue(indiceInfo[0],GetInstr());
        AddValue(indiceInfo[1],renameVar);
        strncat_s(arrayExp,100,sym,strlen(sym));
        nextToken();
        assert(strcmp(buffer,"]") == 0);
    }
    strcpy_s(expString,100,arrayExp);
    //Calculate_Array_Index(var,Get_Indices());
    Calculate_Array_Index(var,indiceInfo);
    free(indiceInfo[0]);
    free(indiceInfo[1]);
}