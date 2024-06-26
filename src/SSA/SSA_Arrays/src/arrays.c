




#include "arrays.h"
#include "ssaComSubExp.h"
#include "ssaInstr.h"
#include "container.h"
#include "hashTable.h"
#include "valueInt.h"
#include "ioVar.h"
#include "symTable.h"
#include "ssaInstrRename.h"
#include "kills.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>


struct HashTable* arrays = NULL;
struct Container* curVarDimens = NULL;
struct Container* arrayIndices = NULL;
struct Instruction* varBaseOffset = NULL;
struct Instruction* addaInstr = NULL;
struct Container* indicesToRename = NULL;

void Initialize_Arrays()
{
    if ( arrays != NULL)
    {
        clear(arrays);
    }
    arrays = Create_HashTable(15,&isStrEqual,&strAssn,
                            &Direct_Assn,&hashFunction_String);
    curVarDimens = NULL;
    varBaseOffset = NULL;
    addaInstr = NULL;

    return;
}

void Add_Index(struct Instruction* index,char* var)
{
    if ( arrayIndices == NULL)
    {
        arrayIndices = Create_Container(15,&instrAssn);
        indicesToRename = Create_Container(15,&strAssn);
    }
    AddValue(arrayIndices,index);
    AddValue(indicesToRename,var);



}

void Add_Dimension(int dim)
{
    if (curVarDimens == NULL)
    {
        curVarDimens = Create_Container(15,&instrAssn);
    }
    /*
    char temp[100];
    strcpy(temp,buffer);
    strcpy(buffer,value);
    const_Instr_Parsed();
    */
    Parse_Literal(dim);
    AddValue(curVarDimens,Get_Last_Parsed_Instr());
}

void Clear_Dimensions()
{
    curVarDimens = NULL;
}
void Clear_Indices() { 
    if ( arrayIndices != NULL)
    {
        Clear(arrayIndices);
        Clear(indicesToRename);
    }
}

void Insert_Array(char* arrayName, struct Container* dimens,
                struct Instruction* baseOffSet)
{
    void** valueInfo = malloc(2 * sizeof(void*));
    valueInfo[0] = dimens;
    valueInfo[1] = baseOffSet;
    insert_KeyValue(arrays,arrayName,valueInfo);
}

int isVarArray(char* var) { return get_KeyValue(arrays,var) != NULL;}
struct Instruction* Get_Adda_Instr() { return addaInstr;}
struct Instruction* Get_BaseOffSet() { return varBaseOffset;}
struct Container* Get_Dimensions() { return curVarDimens;}
struct Container* Get_Indices() { return arrayIndices;}

void SetBaseOffSet(struct Instruction* offset) { varBaseOffset = offset;}



void Calculate_Array_Index(char* arrayName, struct Container** indices)
{
    assert(isVarArray(arrayName));
    assert(indices != NULL && indices[0]->size > 0);
    char* var;
    void** arrayInfo = get_KeyValue(arrays,arrayName);
    struct Container* dimens = arrayInfo[0];
    struct Instruction* offset = arrayInfo[1];
    struct Instruction* add = NULL;
    struct Instruction* const4 = NULL;
    struct Instruction* mul = NULL;
    unsigned int curDimIndex = 0;
    char indexCal[100];
    char literal[11];
    char* exp;
    strcpy_s(indexCal,100,"4*\0");
    Parse_Literal(4);
    const4 = Get_Last_Parsed_Instr();
    SetInstr(indices[0]->values[0]);
    var = indices[1]->values[0];
    if ( var == NULL || strcmp(var,"\0") == 0)
    {
        exp = Get_Exp_String(GetInstr());
        strncat_s(indexCal,100,exp,strlen(exp));
    }
    else 
    {
        strncat_s(indexCal,100,var,strlen(var));
    }
    /* come back to fix this, should not be null*/
    UpdateInstr(const4,"mul",indexCal);
    strncat_s(indexCal,100,"+",1);
    add = GetInstr();
    AddExpression_To_Rename(NULL,var,add);
    mul = const4;
    while (curDimIndex + 1 < indices[0]->size)
    {
        char factor[100];
        strcpy_s(factor,100,"4*\0");
        SetInstr(mul);
        mul = dimens->values[curDimIndex];
        _itoa_s(*(int*)(mul->op1),literal,11,10);
        strncat_s(factor,100,literal,11);
        UpdateInstr(mul,"mul","\0");
        mul = GetInstr();
        strncat_s(factor,100,"*\0",1);
        var = indices[1]->values[curDimIndex + 1];
        if ( var == NULL || strcmp(var,"\0") == 0)
        {
            exp = Get_Exp_String(GetInstr());
            strncat_s(factor,100,exp,strlen(exp));
        }
        else 
        {
            strncat_s(factor,100,var,strlen(var));
        }
        strncat_s(indexCal,100,factor,strlen(factor));
        UpdateInstr(indices[0]->values[curDimIndex + 1], "mul",factor);
        AddExpression_To_Rename(var,NULL,GetInstr());
        UpdateInstr(add,"add",indexCal);
        add = GetInstr();
        ++curDimIndex;
    }
    addaInstr = add;
}


void arrayParse(char* symbol)
{
    /* functions are responsible for the offset */
    /* maybe its not needed, maybe we can leave it as NULL */
    /*
    char temp[100];
    strcpy(temp,buffer);
    strcpy(buffer,"28");
    const_Instr_Parsed();
    strcpy(buffer,temp);
    */
    Parse_Literal(28);
    varBaseOffset = createInstruction("baseAddr",Get_Last_Parsed_Instr(),NULL);
    SetInstr(varBaseOffset);
    AddInstr();
    Insert_Array(symbol,curVarDimens,GetInstr());
}


void Create_Store_Instruction(char* symbol,struct Instruction* valToStore, struct Instruction* adda)
{
    
    void** arrayInfo = get_KeyValue(arrays,symbol);
    struct Instruction* offset = arrayInfo[1];
    SetInstr(offset);
    UpdateInstr(adda,"adda","\0");
    if (Get_Kill_Block() != NULL) { Add_Kill(GetInstr());}
    struct Instruction* store = createInstruction("store",GetInstr(),valToStore);
    SetInstr(store);
    AddInstr();

}


void Create_Load_Instruction(char* symbol)
{

    void** arrayInfo = get_KeyValue(arrays,symbol);
    struct Instruction* offset = arrayInfo[1];
    SetInstr(offset);
    UpdateInstr(addaInstr,"adda","\0");

    struct Instruction* adda = GetInstr();
    SetInstr(NULL);
    UpdateInstr(adda,"load","\0");
}
