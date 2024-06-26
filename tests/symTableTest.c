




#include "symTable.h"
#include "ssaInstr.h"
#include "instruction.h"
#include "valueInt.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void Test_Parse_Literal()
{
    int x = 0;
    int y = 15;

    Parse_Literal(0);
    struct Instruction* literal = Get_Last_Parsed_Instr();
    assert(intComp(&x,literal->op1));
    Parse_Literal(15);
    literal = Get_Last_Parsed_Instr();
    assert(intComp(&y,literal->op1));
    printf("Test Parse Literal Passed\n");

    return;
}

void Test_Parse_Int_Decl()
{
    Parse_Int_Decl("a");
    assert( NULL == Get_Last_Parsed_Instr());
    Parse_Int_Decl("b");
    assert(NULL == Get_Last_Parsed_Instr());

    printf("Test Parse Int Decl\n");
    return;
}

void Test_Parse_Int_Desg()
{
    Parse_Int_Decl("c");
    Parse_Int_Desg("c");

    assert(NULL == Get_Last_Parsed_Instr());
    Parse_Int_Decl("d");
    Parse_Int_Desg("d");
    printf("Test Parse Int Desg\n");
    return;
}

void Test_Update_Table()

{
    struct Instruction* read = createInstruction("read",NULL,NULL);
    struct Instruction* read2 = createInstruction("read",NULL,NULL);
    Update_Int_Table("a", read);
    assert(read == Get_Last_Parsed_Instr());
    Parse_Int_Desg("a");
    assert(read == Get_Last_Parsed_Instr());
    Update_Int_Table("b",read2);
    assert(read2 == Get_Last_Parsed_Instr());
    printf("Test Test Update Table");
}

void Test_Compute_Literal()
{
    Initialize_Tables();
    int x = 10;
    int y = 13;
    int result = x + y;
    Compute_Literal(&x,&y,"add");
    struct Instruction* r = Get_Last_Parsed_Instr();
    assert(intComp(&result,r->op1));
    result = y / x;
    Compute_Literal(&y,&x,"div");
    r = Get_Last_Parsed_Instr();
    assert(intComp(&result,r->op1));

    result = y - x;
    Compute_Literal(&y,&x,"sub");
    r = Get_Last_Parsed_Instr();
    assert(intComp(&result,r->op1));

    result = x * y;
    Compute_Literal(&x,&y, "mul");
    r = Get_Last_Parsed_Instr();
    assert(intComp(&result,r->op1));

    printf("Test Compute Literal Passed");
}

int main()


{
    SSA_Initialize_Program();
    AddProced();
    AddBlock();
    /*
    Initialize_Tables();
    Test_Parse_Literal();
    Test_Parse_Int_Decl();
    Test_Parse_Int_Desg();
    Test_Update_Table();
    */
    Test_Compute_Literal();

    return 0;
}