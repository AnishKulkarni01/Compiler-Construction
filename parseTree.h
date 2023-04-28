//CS F363 : Compiler Construction
//Group No. 24
//1. Nachiket Kotalwar               (2020A7PS0024P)
//2.Parth Patel                      (2020A7PS0026P)
//3.Partha Sarathi Purkayastha       (2020A7PS0043P)
//4.Labeeb Ahsan                     (2020A7PS0045P)
//5.Anish Atul Kulkarni              (2020A7PS0975P)

#ifndef PARSETREE_H
#define PARSETREE_H
#include "enum.h"
#include "symbolTable.h"
#include "lexer.h"
typedef struct T_NODE
{
    struct T_NODE *parent;
    struct T_NODE *left_sibling;
    struct T_NODE *right_sibling;
    struct T_NODE **children;
    int num_children;
    int type;
    char* value;
    int line_no;
    char* lexeme;
    int grammarRuleIndex;
    int value_if_num;
    float value_if_rnum;
    lexeme* lex;
    void* node_syn;
    void* node_inh;
    void* list_syn;
    void* list_inh;
} t_node;


typedef struct AST_NODE
{
    char* type;
    int numChildren;
    void** children;
    st_table* symbolTable;

    int containsLexeme;
    lexeme* lexeme;

    int containsBounds;
    int startLine;
    int endLine;
} ast_node;

// Function to create a new tree node
t_node* create_node(int type,const char* value);

// Function to add a child node
void add_child(t_node* parent, t_node* child);


// int apply_rule(t_node* root, char* lhs, char** rhs, int len_rhs);



// Function to perform an inorder traversal- prints only the terminals/leaf nodes
void inorder_traversal(t_node* node,FILE* fp);
void preorder_traversal(t_node* node,FILE* fp,int nesting,int child);

t_node* initializeParseTree();

initCountNodes();

int getCountParseNodes();

#endif