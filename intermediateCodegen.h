//CS F363 : Compiler Construction
//Group No. 24
//1. Nachiket Kotalwar               (2020A7PS0024P)
//2.Parth Patel                      (2020A7PS0026P)
//3.Partha Sarathi Purkayastha       (2020A7PS0043P)
//4.Labeeb Ahsan                     (2020A7PS0045P)
//5.Anish Atul Kulkarni              (2020A7PS0975P)

#ifndef INTERMEDIATECODEGEN_H
#define INTERMEDIATECODEGEN_H

#include "parseTree.h"
#include "parser.h"
#include "ast.h"
#include "symbolTable.h"
#include "semanticAnalyzer.h"


typedef struct quadruple
{
    char *op;

    enum QUAD_ARG_TYPE arg1_type;
    enum QUAD_ARG_TYPE arg2_type;
    // int arg1_is_st_entry;
    // int arg2_is_st_entry;
    // int arg1_is_num;
    // int arg2_is_num;
    // int arg1_is_rnum;
    // int arg2_is_rnum;
    // int arg1_is_bool;
    // int arg2_is_bool;
    // int arg1_is_label;
    // int arg2_is_label;

    st_entry* arg1_st_entry;
    st_entry* arg2_st_entry;
    int arg1_num;
    int arg2_num;
    float arg1_rnum;
    float arg2_rnum;
    int arg1_bool;
    int arg2_bool;
    char* arg1_label;
    char* arg2_label;

    st_entry* result;
} quadruple;

typedef struct quadrupleList
{
    quadruple* quad;
    struct quadrupleList* next;
}quadrupleList;

quadrupleList* create_QuadrupleList();
void print_Quadruples(ast_node* node,st_table* symbol_table,quadrupleList* ql,FILE* fp);
void print_QuadrupleList(quadrupleList* ql);
void initializeIntermediateCodeGen();


#endif