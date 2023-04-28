//CS F363 : Compiler Construction
//Group No. 24
//1. Nachiket Kotalwar               (2020A7PS0024P)
//2.Parth Patel                      (2020A7PS0026P)
//3.Partha Sarathi Purkayastha       (2020A7PS0043P)
//4.Labeeb Ahsan                     (2020A7PS0045P)
//5.Anish Atul Kulkarni              (2020A7PS0975P)

#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "parseTree.h"
#include "symbolTable.h"

int getTypeWidth(char *type);
int typeToIntID(char *type);
void initializeFp(FILE * fp);
int resolveExpression(ast_node *root, st_table *parent);
void constructSymbolTable(ast_node *root, st_table *parent);
void constructSymbolTableDeclarations(ast_node *root, st_table *parent);
void initializeSemanticAnalyzer();
void printAllScopeTables(ast_node* root);
void printAllActivationRecords(ast_node* root);
void printAllScopeArrays(ast_node* root);

#endif