//CS F363 : Compiler Construction
//Group No. 24
//1. Nachiket Kotalwar               (2020A7PS0024P)
//2.Parth Patel                      (2020A7PS0026P)
//3.Partha Sarathi Purkayastha       (2020A7PS0043P)
//4.Labeeb Ahsan                     (2020A7PS0045P)
//5.Anish Atul Kulkarni              (2020A7PS0975P)

#ifndef PARSER_H
#define PARSER_H

typedef struct s_gramamrRule
{
    char *lhs;
    char **rhs;
    int rhsSize;
} grammarRule;

void ll1Check();
void parserDriver(const char* testCaseFile,int buffer_size,const char* parseTreeFile);
void parserDriver_uptill_AST(const char *testCaseFile, int buffer_size, const char *parseTreeFile);
void parserDriver_uptill_SymbolTable(const char *testCaseFile, int buffer_size, const char *parseTreeFile);
void parserDriver_uptill_ActivationRecord(const char *testCaseFile, int buffer_size, const char *parseTreeFile);
void parserDriver_uptill_Arrays(const char *testCaseFile, int buffer_size, const char *parseTreeFile);
void parserDriver_uptill_CodeGen(const char *testCaseFile, int buffer_size, const char *parseTreeFile);
void parserDriver_Compression(const char *testCaseFile, int buffer_size, const char *parseTreeFile);

#endif