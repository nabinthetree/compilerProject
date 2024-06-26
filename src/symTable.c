




#include <string.h>
#include "symTable.h"
#include "hashTable.h"
#include "valueInt.h"
#include "keyString.h"
#include "ssaInstr.h"
#include "instruction.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct HashTable* literals = NULL;
struct HashTable* integers = NULL;
struct Instruction* lastParsedInstr = NULL;


void Initialize_Tables()
{
    if ( literals != NULL)
    {
        clear(literals);
        clear(integers);
        return;
    }
    literals = Create_HashTable(32,&intComp,&intAssn,&Direct_Assn,&hashInt);
    integers = Create_HashTable(32,&isStrEqual,&strAssn,
                                &Direct_Assn,&hashFunction_String);
}

struct Instruction* Get_Last_Parsed_Instr()
{
    return lastParsedInstr;
}

void Parse_Literal(int val)
{
    if (getKeyIndex(literals,&val) == -1)
    {
        int* literal = malloc(sizeof(int));
        *literal = val;
        insert_KeyValue(literals,&val,createInstruction("const",literal,NULL));
        AddLiteral(get_KeyValue(literals,&val));
    }
    lastParsedInstr = get_KeyValue(literals,&val);
    return;
}

void Parse_Int_Decl(char*val)
{
    insert_KeyValue(integers,val,NULL);
    lastParsedInstr = NULL;
}

void Parse_Int_Desg(char* var)
{
    assert(getKeyIndex(integers,var) != -1);
    lastParsedInstr = get_KeyValue(integers,var);
}

void Update_Int_Table(char* var, struct Instruction* value)
{
    assert(getKeyIndex(integers,var) != -1);
    insert_KeyValue(integers,var, value);
    lastParsedInstr = value;
}


void Compute_Literal(int* op1, int* op2, char* operation)
{
    int result = 0;
    if (strcmp(operation,"mul") == 0)
    {
        result = (*op1) * (*op2);
    }
    else if (strcmp(operation,"sub") == 0)
    {
        result = (*op1) - (*op2);
    }
    else if (strcmp(operation,"div") == 0)
    {
        assert(*op2 != 0);
        result = (*op1) / (*op2);
    }
    else if (strcmp(operation,"add") == 0)
    {
        result = (*op1) + (*op2);

    }
    else 
    {
        lastParsedInstr = NULL;
        return;
    }
    Parse_Literal(result);
}

int Compute_Boolean(int* op1, int* op2, char* operation)
{
    
    if (strcmp(operation, "==") == 0)
    {
        return *op1 == *op2;
    }
    else if (strcmp(operation,"!=") == 0)
    {
        return *op1 != *op2;
    }
    else if (strcmp(operation,">") == 0)
    {
        return *op1 > *op2;
    }
    else if (strcmp(operation,">=") == 0)
    {
        return *op1 >= *op2;
    }
    else if (strcmp(operation, "<=") == 0)
    {
        return *op1 <= *op2;
    }
    else if (strcmp(operation, "<") == 0)
    {
        return *op1 < *op2;
    }
    return -1;
}

void Set_Unitialized_Zero(char* var)
{

    printf("Unitiazlied Variable %s, Intialilzed to zero",var);
    Parse_Literal(0);
    Update_Int_Table(var,Get_Last_Parsed_Instr());

}
