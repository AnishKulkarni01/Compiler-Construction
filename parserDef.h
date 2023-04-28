//CS F363 : Compiler Construction
//Group No. 24
//1. Nachiket Kotalwar               (2020A7PS0024P)
//2.Parth Patel                      (2020A7PS0026P)
//3.Partha Sarathi Purkayastha       (2020A7PS0043P)
//4.Labeeb Ahsan                     (2020A7PS0045P)
//5.Anish Atul Kulkarni              (2020A7PS0975P)

#ifndef PARSERDEF_H
#define PARSERDEF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "lexer.h"
#include "chash.h"
#include "parseTree.h"
#include "ast.h"

#include "parser.h"
#include "semanticAnalyzer.h"
#include "intermediateCodegen.h"
#include "codegen.h"

struct StackNode {
	char* data;
	struct StackNode* next;
};

int terminals = 0;
int nonTerminals = 0;

hash_table *terminalsMap;
hash_table *nonTerminalsMap;

hash_table *memoFirstSets;
hash_table *memoFollowSets;

// typedef struct s_gramamrRule
// {
//     char *lhs;
//     char **rhs;
//     int rhsSize;
// } grammarRule;


#define BUF_SIZE 100000
char buffer[BUF_SIZE];
int numRules = 0;

void parserDriver(const char* testCaseFile, int buffer_size,const char* parseTreeFile);

#endif