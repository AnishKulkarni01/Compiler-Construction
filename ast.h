//CS F363 : Compiler Construction
//Group No. 24
//1. Nachiket Kotalwar               (2020A7PS0024P)
//2.Parth Patel                      (2020A7PS0026P)
//3.Partha Sarathi Purkayastha       (2020A7PS0043P)
//4.Labeeb Ahsan                     (2020A7PS0045P)
//5.Anish Atul Kulkarni              (2020A7PS0975P)

#ifndef AST_H
#define AST_H

#include "parseTree.h"
#include "parser.h"

typedef struct linked_list
{
    void *data;
    struct linked_list *next;
} llist;

llist *create_list();
llist* insert_at_end(llist *list, void *data);
void printLinkedList(llist *list);
ast_node *create_node_AST_from_list(char* type,llist *list1);
ast_node *create_node_AST(char *type,int containsLexeme,lexeme* lexeme, int numChildren, void *child1, void *child2, void *child3, void *child4, void *child5, void *child6, void *child7, void *child8, void *child9);
void traverse_AST_print(ast_node *node,int nested,int childNo);
void traverse_AST(ast_node *node,FILE* fp,int nested,int childNo);
ast_node *construct_AST(t_node *root, grammarRule *rules);
void initializeAST();
int getASTNodeCount();


#endif