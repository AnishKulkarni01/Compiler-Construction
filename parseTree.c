//CS F363 : Compiler Construction
//Group No. 24
//1. Nachiket Kotalwar               (2020A7PS0024P)
//2.Parth Patel                      (2020A7PS0026P)
//3.Partha Sarathi Purkayastha       (2020A7PS0043P)
//4.Labeeb Ahsan                     (2020A7PS0045P)
//5.Anish Atul Kulkarni              (2020A7PS0975P)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parseTree.h"

// typedef struct T_NODE
// {
//     struct T_NODE *parent;
//     struct T_NODE *left_sibling;
//     struct T_NODE *right_sibling;
//     struct T_NODE **children;
//     int num_children;
//     int type;
//     char* value;
// } t_node;

// Function to create a new tree node

int parseTreeNodesCount = 0;

initCountNodes()
{
    parseTreeNodesCount = 0;
}

int getCountParseNodes()
{
    return parseTreeNodesCount;
}

t_node *create_node(int type, const char *value)
{
    parseTreeNodesCount++;
    t_node *new_node = malloc(sizeof(t_node));
    new_node->parent = NULL;
    new_node->left_sibling = NULL;
    new_node->right_sibling = NULL;
    new_node->num_children = 0;
    new_node->type = type;
    new_node->value = malloc(strlen(value) + 1);
    strcpy(new_node->value, value);
    new_node->children = NULL;
    new_node->lexeme = NULL;
    new_node->line_no = 0;
    new_node->grammarRuleIndex = -1;
    new_node->value_if_num = 0;
    new_node->value_if_rnum = 0.0;
    return new_node;
}

// Function to add a child node
void add_child(t_node *parent, t_node *child)
{
    if (parent == NULL)
        return;
    if (child == NULL)
        return;
    parent->num_children++;
    parent->children = realloc(parent->children, parent->num_children * sizeof(t_node *));
    parent->children[parent->num_children - 1] = child;
    child->parent = parent;
    if (parent->num_children > 1)
    {
        child->left_sibling = parent->children[parent->num_children - 2];
        parent->children[parent->num_children - 2]->right_sibling = child;
    }
}

// t_node* apply_rule(t_node* root, char* lhs, char** rhs, int len_rhs) {
//     if (root == NULL) {
//         return NULL;
//     }
//     if (strcmp(root->value, lhs) == 0 && root->num_children == 0) {
//         int i;
//         for (i = 0; i < len_rhs; i++) {
//             t_node* new_node = create_node(root->type, rhs[i]);
//             add_child(root, new_node);
//         }
//         return root->children[0];
//     }
//     int i;
//     for (i = 0; i < root->num_children; i++) {
//         t_node *res=apply_rule(root->children[i], lhs, rhs, len_rhs);
//         if(res!=NULL){
//             return res;
//         }
//     }
//     return NULL;
// }'
int isNonTerminal2(char *s)
{
    if ((strcmp(s, "AND") == 0) || (strcmp(s, "OR") == 0))
    {
        return 0;
    }
    else if (s[0] >= 'A' && s[0] <= 'Z')
    {
        return 1;
    }
    return 0;
}



// Function to perform an inorder traversal- prints only the terminals/leaf nodes
void inorder_traversal(t_node *node, FILE* fp)
{   
    // fp = fopen("output.txt", "w");
    if (node == NULL)
        return;
    //  printf("%s  \n", node->value);
    if (node->num_children > 0)
    {
        inorder_traversal(node->children[0],fp);
    }

    char *Par = (node->parent == NULL) ? strdup("--") : strdup(node->parent->value);

    // if (node->num_children > 0) {
    //     inorder_traversal(node->children[0]);
    // }
    char *Leaf=(!isNonTerminal2(node->value))?strdup("Leaf"):strdup("NonLeaf");
    char *str;
    asprintf(&str, "%d", /*(strcmp(Leaf, "NonLeaf") == 0)?0:*/(node->line_no));

    char *str2;
    asprintf(&str2, "%d", 225);

    char *lex = (/*strcmp(Leaf, "NonLeaf") == 0*/node->lexeme==NULL) ? strdup("--") : strdup(node->lexeme);


    if(strcmp(node->value,token_list[NUM])==0)
        printf("lexeme: %-20s CurrentNode Line Number: %-20s tokenName:%-20s parentNodeSymbol:%-20s isLeafNode:%-20s NodeSymbol:%-20s GrammerRuleNo: %-10d valueIfNum:%d ValueIfRnum:-- \n", lex, str, node->value, Par, Leaf, node->value,node->grammarRuleIndex,node->value_if_num);
    else if(strcmp(node->value,token_list[RNUM])==0)
        printf("lexeme: %-20s CurrentNode Line Number: %-20s tokenName:%-20s parentNodeSymbol:%-20s isLeafNode:%-20s NodeSymbol:%-20s GrammerRuleNo: %-10d valueIfNum:-- valueIfRnum:%f \n", lex, str, node->value, Par, Leaf, node->value,node->grammarRuleIndex,node->value_if_rnum);
    else
        printf("lexeme: %-20s CurrentNode Line Number: %-20s tokenName:%-20s parentNodeSymbol:%-20s isLeafNode:%-20s NodeSymbol:%-20s GrammerRuleNo: %-10d valueIfNum:-- valueIfRnum:-- \n", lex, str, node->value, Par, Leaf, node->value,node->grammarRuleIndex);
    int i;

    for (i = 1; i < node->num_children; i++)
    {
        inorder_traversal(node->children[i],fp);
    }
    // int i;
    // for (i = 1; i < node->num_children; i++) {
    //     inorder_traversal(node->children[i]);
    // }
}

void preorder_traversal(t_node *node, FILE* fp,int nesting,int child)
{   
    // fp = fopen("output.txt", "w");
    if (node == NULL)
        return;
    //  printf("%s  \n", node->value);
    // if (node->num_children > 0)
    // {
    //     preorder_traversal(node->children[0],fp);
    // }

    char *Par = (node->parent == NULL) ? strdup("--") : strdup(node->parent->value);

    // if (node->num_children > 0) {
    //     preorder_traversal(node->children[0]);
    // }
    char *Leaf=(!isNonTerminal2(node->value))?strdup("Leaf"):strdup("NonLeaf");
    char *str;
    asprintf(&str, "%d", /*(strcmp(Leaf, "NonLeaf") == 0)?0:*/(node->line_no));

    char *str2;
    asprintf(&str2, "%d", 225);

    char *lex = (/*strcmp(Leaf, "NonLeaf") == 0*/node->lexeme==NULL) ? strdup("--") : strdup(node->lexeme);

    for(int i=0;i<nesting;i++)
    fprintf(fp,"\t");

    fprintf(fp,"%d. TokenName: %-15s Lexeme: %-10s \n",child,node->value,lex);

    // if(strcmp(node->value,token_list[NUM])==0)
    //     fprintf(fp,"lexeme: %-20s CurrentNode Line Number: %-20s tokenName:%-20s parentNodeSymbol:%-20s isLeafNode:%-20s NodeSymbol:%-20s GrammerRuleNo: %-10d valueIfNum:%d ValueIfRnum:-- \n", lex, str, node->value, Par, Leaf, node->value,node->grammarRuleIndex,node->value_if_num);
    // else if(strcmp(node->value,token_list[RNUM])==0)
    //     fprintf(fp,"lexeme: %-20s CurrentNode Line Number: %-20s tokenName:%-20s parentNodeSymbol:%-20s isLeafNode:%-20s NodeSymbol:%-20s GrammerRuleNo: %-10d valueIfNum:-- valueIfRnum:%f \n", lex, str, node->value, Par, Leaf, node->value,node->grammarRuleIndex,node->value_if_rnum);
    // else
    //     fprintf(fp,"lexeme: %-20s CurrentNode Line Number: %-20s tokenName:%-20s parentNodeSymbol:%-20s isLeafNode:%-20s NodeSymbol:%-20s GrammerRuleNo: %-10d valueIfNum:-- valueIfRnum:-- \n", lex, str, node->value, Par, Leaf, node->value,node->grammarRuleIndex);
    int i;

    for (i = 0; i < node->num_children; i++)
    {
        preorder_traversal(node->children[i],fp,nesting+1,i+1);
    }
    // int i;
    // for (i = 1; i < node->num_children; i++) {
    //     inorder_traversal(node->children[i]);
    // }
}

t_node *initializeParseTree(const char *startSymbol)
{
    t_node *root = create_node(1, startSymbol);
    return root;
}