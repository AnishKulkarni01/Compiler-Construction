//CS F363 : Compiler Construction
//Group No. 24
//1. Nachiket Kotalwar               (2020A7PS0024P)
//2.Parth Patel                      (2020A7PS0026P)
//3.Partha Sarathi Purkayastha       (2020A7PS0043P)
//4.Labeeb Ahsan                     (2020A7PS0045P)
//5.Anish Atul Kulkarni              (2020A7PS0975P)

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "chash.h"

typedef struct ID_TYPE{
    char* type;
    int is_array;
    int is_static_array;
    int is_function;
    int leftIndex;
    int rightIndex;
    char* leftIndexName;
    char* rightIndexName;
    int isDynamicLeft;
    int isDynamicRight;
    struct ID_TYPE** inputParams;
    int numInputParams;
    struct ID_TYPE** outputParams;
    int numOutputParams;
} id_type;

typedef struct ST_ENTRY{
    char *var_name;
    int isShadow;
    int isForLoopVar;
    char* scopeName;
    int width;
    int offset;
    int offsetAct;
    int nestingLevel;
    id_type* idType;
    int lineNo;
    int assigned;
    int didInFirstPass;
    int unusableFunc;
} st_entry;

typedef struct ST_TABLE{
    hash_table *table;
    char* scopeName;
    struct ST_TABLE* parent;
    int offset;
    int offsetAct;
    int nestingLevel;
    int lineNoStart;
    int lineNoEnd;
    char* moduleName;
    int inForLoop;
    int inWhileLoop;
    char* forLoopVar;
    char* whileLoopVar1;
    char* whileLoopVar2;
    int* moduleOffset;
} st_table;

st_table* createSymbolTable(char* scopeName, st_table* parent, int lineNoStart, int lineNoEnd, char* moduleName);
st_entry* createSymbolEntry(char* var_name, id_type* type, char* scopeName, int width, int offset, int nestingLevel, int lineNo, int offsetAct);
id_type* createIDType(char* type, int is_array, int is_static_array, int is_function, int leftIndex, int rightIndex, char* leftIndexName, char* rightIndexName, int isDynamicLeft, int isDynamicRight, id_type** inputParams, int numInputParams, id_type** outputParams, int numOutputParams);
void addSymbolEntry(st_table* stable, st_entry* entry);
st_entry* getSymbolEntry(st_table* stable, char* var_name);
st_entry* getSymbolEntryCurrent(st_table* stable, char* var_name);
void printSymbolEntry(FILE* fp,st_entry* entry);
void printSymbolTable(st_table* stable);
void printActivationRecord(st_table* stable);
void printSymbolTableArrays(st_table* stable);


#endif