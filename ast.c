//CS F363 : Compiler Construction
//Group No. 24
//1. Nachiket Kotalwar               (2020A7PS0024P)
//2.Parth Patel                      (2020A7PS0026P)
//3.Partha Sarathi Purkayastha       (2020A7PS0043P)
//4.Labeeb Ahsan                     (2020A7PS0045P)
//5.Anish Atul Kulkarni              (2020A7PS0975P)

#include "astDef.h"

int count = 0;

void initialize_AST()
{
    count = 0;
}

int getASTNodeCount()
{
    return count;
}

llist *create_list()
{
    llist *list = (llist *)malloc(sizeof(llist));
    list->data = NULL;
    list->next = NULL;
    return list;
}

llist* insert_at_end(llist *list, void *data)
{
    llist *temp = (llist *)malloc(sizeof(llist));
    temp->data = data;
    temp->next = NULL;
    if (list == NULL)
    {
        list = temp;
    }
    else
    {
        llist *temp1 = list;
        while (temp1->next != NULL)
        {
            temp1 = temp1->next;
        }
        temp1->next = temp;
    }
    return list;
}

void printLinkedList(llist *list)
{
    llist *temp = list;
    int i = 0;
    
    while (temp != NULL)
    {
        i++;
        printf("%s ", (char*)temp->data);
        temp = temp->next;
    }
    printf("I=%d\n",i);
}


ast_node *create_node_AST_from_list(char* type,llist *list1)
{
    count++;
    // if(list1==NULL)
    // {
    //     printf("Error in creating AST node --%s--\n",type);
    // }
    llist *list = list1->next;
    ast_node *node = (ast_node *)malloc(sizeof(ast_node));
    node->type = type;
    node->numChildren = 0;
    node->children = NULL;
    llist *temp = list;
    while (temp != NULL)
    {
        node->numChildren++;
        temp = temp->next;
    }
    node->children = (void **)malloc(sizeof(void *) * node->numChildren);
    temp = list;
    int i = 0;
    while (temp != NULL)
    {
        node->children[i] = temp->data;
        temp = temp->next;
        i++;
    }
    return node;
}

ast_node *create_node_AST(char *type,int containsLexeme,lexeme* lexeme, int numChildren, void *child1, void *child2, void *child3, void *child4, void *child5, void *child6, void *child7, void *child8, void *child9)
{
    count++;
    ast_node *node = (ast_node *)malloc(sizeof(ast_node));
    node->type = type;
    node->containsLexeme = containsLexeme;
    node->containsBounds = 0;
    if(containsLexeme)
    {
        node->lexeme = lexeme;
        node->numChildren = 0;
        node->children = NULL;
        return node;
    }
    node->lexeme = NULL;
    node->numChildren = numChildren;
    node->children = (void **)malloc(sizeof(void *) * numChildren);
    void *child;
    for (int i = 1; i <= numChildren; i++)
    {
        switch (i)
        {
        case 1:
            child = child1;
            break;
        case 2:
            child = child2;
            break;
        case 3:
            child = child3;
            break;
        case 4:
            child = child4;
            break;
        case 5:
            child = child5;
            break;
        case 6:
            child = child6;
            break;
        case 7:
            child = child7;
            break;
        case 8:
            child = child8;
            break;
        case 9:
            child = child9;
            break;
        default:
            break;
        }
        node->children[i-1] = child;
    }
    return node;
}


void traverse_AST_print(ast_node *node,int nested,int childNo)
{   
    // if (node == NULL)
    //     return;
    // if (node->numChildren > 0)
    // {
    //     traverse_AST(node->children[0],fp);
    // }

    // fprintf(fp,"%s\n", node->type);
    // for (int i = 1; i < node->numChildren; i++)
    // {
        
    //     traverse_AST(node->children[i],fp);
    // }

    if (node == NULL)
        return;

    for (int i = 0; i < nested; i++)
    {
        printf( "\t");
    }
    if(node->containsBounds)
        printf("%d. %s \t\t\t-child : %d- \t\t\t\t\t\t\t-start:%d- -end:%d-\n",childNo, node->type,node->numChildren,node->startLine,node->endLine);
    else
        printf("%d. %s \t\t\t-child : %d-\n",childNo, node->type,node->numChildren);

    for (int i = 0; i < node->numChildren; i++)
    {
        traverse_AST_print(node->children[i], nested + 1,i+1);
    }
}

void traverse_AST(ast_node *node,FILE* fp,int nested,int childNo)
{
    
    // if (node == NULL)
    //     return;
    // if (node->numChildren > 0)
    // {
    //     traverse_AST(node->children[0],fp);
    // }

    // fprintf(fp,"%s\n", node->type);
    // for (int i = 1; i < node->numChildren; i++)
    // {
        
    //     traverse_AST(node->children[i],fp);
    // }

    if (node == NULL)
        return;
    node->symbolTable=NULL;  //change line 1
    for (int i = 0; i < nested; i++)
    {
        printf( "\t");
    }
    if(node->containsBounds)
        printf( "%d. %s \t\t\t-child : %d- \t\t\t\t\t\t\t-start:%d- -end:%d-\n",childNo, node->type,node->numChildren,node->startLine,node->endLine);
    else
        printf( "%d. %s \t\t\t-child : %d-\n",childNo, node->type,node->numChildren);

    for (int i = 0; i < node->numChildren; i++)
    {
        traverse_AST(node->children[i], fp, nested + 1,i+1);
    }
}


ast_node *construct_AST(t_node *root, grammarRule *rules)
{
    ast_node *node;
    grammarRule rule = rules[root->grammarRuleIndex];

    FILE *quad_fp = fopen("quadruples.txt", "w");    

    // mr copilot try reading astrules.txt
    switch (root->grammarRuleIndex)
    {
    case 0: // Program Moduledeclarations Othermodules1 Drivermodule Othermodules2
    {
        t_node *Program = root;
        t_node *Moduledeclarations = root->children[0];
        t_node *Othermodules1 = root->children[1];
        t_node *Drivermodule = root->children[2];
        t_node *Othermodules2 = root->children[3];

        //printf("Here5");
        fflush(stdout);

        llist *head1 = create_list();
        llist *head2 = create_list();
        llist *head3 = create_list();
        
        Moduledeclarations->list_inh = head1;
        Othermodules1->list_inh = head2;
        Othermodules2->list_inh = head3;

        // //printf("Moduledeclarations 1-");
        // printLinkedList(Moduledeclarations->list_inh);

        construct_AST(Moduledeclarations, rules);
        construct_AST(Othermodules1, rules);
        construct_AST(Drivermodule, rules);
        construct_AST(Othermodules2, rules);

        // //printf("Moduledeclarations -");
        // printLinkedList(Moduledeclarations->list_syn);

        Moduledeclarations->node_syn = create_node_AST_from_list("MODULE_LIST",Moduledeclarations->list_syn);
        Othermodules1->node_syn = create_node_AST_from_list("OTHER_LIST1",Othermodules1->list_syn);
        Othermodules2->node_syn = create_node_AST_from_list("OTHER_LIST2",Othermodules2->list_syn);

        Program->node_syn = create_node_AST("PROGRAM",0,NULL, 4, Moduledeclarations->node_syn, Othermodules1->node_syn, Drivermodule->node_syn, Othermodules2->node_syn, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 1:// Moduledeclarations Moduledeclaration Moduledeclarations1 .
    {
        
        t_node *Moduledeclarations = root;
        t_node *Moduledeclaration = root->children[0];
        t_node *Moduledeclarations1 = root->children[1];

        construct_AST(Moduledeclaration, rules);

        Moduledeclarations->list_inh = insert_at_end(Moduledeclarations->list_inh, Moduledeclaration->node_syn);
        Moduledeclarations1->list_inh = Moduledeclarations->list_inh;
        
        construct_AST(Moduledeclarations1, rules);

        Moduledeclarations->list_syn = Moduledeclarations1->list_syn;
    }
    break;
    case 2: // Moduledeclarations
    {
        t_node *Moduledeclarations = root;

        Moduledeclarations->list_syn = Moduledeclarations->list_inh;
    }
    break;
    case 3: // Moduledeclaration declare module id semicol
    {
        t_node *Moduledeclaration = root;
        t_node *id = root->children[2];

        Moduledeclaration->node_syn = create_node_AST(id->lexeme,1,id->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);

        ((ast_node*)Moduledeclaration->node_syn)->containsBounds = 1;
        ((ast_node*)Moduledeclaration->node_syn)->startLine = id->line_no;
        ((ast_node*)Moduledeclaration->node_syn)->endLine = id->line_no;

    }
    break;
    case 4: // Othermodules Module Othermodules1
    {

        t_node *Othermodules = root;
        t_node *Module = root->children[0];
        t_node *Othermodules1 = root->children[1];

        construct_AST(Module, rules);
        
        Othermodules->list_inh = insert_at_end(Othermodules->list_inh, Module->node_syn);
        Othermodules1->list_inh = Othermodules->list_inh;

        construct_AST(Othermodules1, rules);

        Othermodules->list_syn = Othermodules1->list_syn;
    }
    break;
    case 5: // Othermodules
    {
        t_node *Othermodules = root;

        Othermodules->list_syn = Othermodules->list_inh;
    }
    break;
    case 6: // Drivermodule driverdef driver program driverenddef Moduledef
    {
        //printf("DRIVER\n");
        fflush(stdout);
        t_node *Drivermodule = root;
        t_node *driver = root->children[1];
        t_node *Moduledef = root->children[4];
        //printf("here");
        construct_AST(Moduledef, rules);

        Drivermodule->node_syn = create_node_AST("DRIVER",0,NULL, 1, Moduledef->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        ((ast_node*)(Drivermodule->node_syn))->containsBounds = 1;
        ((ast_node*)(Drivermodule->node_syn))->startLine = driver->line_no;
        ((ast_node*)(Drivermodule->node_syn))->endLine = ((ast_node*)(Moduledef->node_syn))->endLine;

        //generate_Quadruples_Driver(Drivermodule->node_syn,fp);
    }
    break;
    case 7: // Module def module id enddef takes input sqbo Input_plist sqbc semicol Ret Moduledef
    {
        t_node *Module = root;
        t_node *module = root->children[1];
        t_node *id = root->children[2];
        t_node *Input_plist = root->children[7];
        t_node *Ret = root->children[10];
        t_node *Moduledef = root->children[11];

        construct_AST(Input_plist, rules);
        construct_AST(Ret, rules);
        construct_AST(Moduledef, rules);

        Input_plist->node_syn = create_node_AST_from_list("INPUT_LIST",Input_plist->list_syn);
        Module->node_syn = create_node_AST("MODULE",0,NULL, 4, create_node_AST(id->lexeme,1,id->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL), Input_plist->node_syn, Ret->node_syn, Moduledef->node_syn, NULL, NULL, NULL, NULL, NULL);
        ((ast_node*)(Module->node_syn))->containsBounds = 1;
        ((ast_node*)(Module->node_syn))->startLine = module->line_no;
        ((ast_node*)(Module->node_syn))->endLine = ((ast_node*)(Moduledef->node_syn))->endLine;

        //generate_Quadruples_Module(Module->node_syn,fp);
    }
    break;
    case 8: // Ret returns sqbo Output_plist sqbc semicol
    {
        t_node *Ret = root;
        t_node *Output_plist = root->children[2];

        Output_plist->list_inh = create_list();

        construct_AST(Output_plist, rules);

        Output_plist->node_syn = create_node_AST_from_list("OUTPUT_LIST",Output_plist->list_syn);
        //here no need to create new node
        //Ret->node_syn = create_node_AST("PARAMETERS_LIST", 1, Output_plist->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        Ret->node_syn = Output_plist->node_syn;
    }
    break;
    case 9: // Ret
    {
        t_node *Ret = root;

        Ret->node_syn = create_node_AST("OUTPUT_LIST",0,NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 10://Input_plist id colon Datatype A' .
    {
        t_node *Input_plist = root;
        t_node *id = root->children[0];
        t_node *Datatype = root->children[2];
        t_node *A = root->children[3];

        construct_AST(Datatype, rules);

        A->list_inh = create_list();
        ast_node *data_id = create_node_AST("DATATYPE_ID",0,NULL, 2, create_node_AST(id->lexeme,1,id->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL), Datatype->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        A->list_inh = insert_at_end(A->list_inh, data_id);

        construct_AST(A, rules);

        Input_plist->list_syn = A->list_syn;
    }
    break;
    case 11://A' comma id colon Datatype A'1
    {
        t_node *A = root;
        t_node *id = root->children[1];
        t_node *Datatype = root->children[3];
        t_node *A1 = root->children[4];

        construct_AST(Datatype, rules);

        ast_node *data_id = create_node_AST("DATATYPE_ID",0,NULL, 2, create_node_AST(id->lexeme,1,id->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL), Datatype->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        A1->list_inh = insert_at_end(A->list_inh, data_id);

        construct_AST(A1, rules);

        A->list_syn = A1->list_syn;
    }
    break;
    case 12:// A'
    {
        t_node *A = root;

        A->list_syn = A->list_inh;
    }
    break;
    case 13://Output_plist id colon Datatype B'
    {
        t_node *Output_plist = root;
        t_node *id = root->children[0];
        t_node *Datatype = root->children[2];
        t_node *B = root->children[3];

        construct_AST(Datatype, rules);

        B->list_inh = create_list();
        ast_node *data_id = create_node_AST("DATATYPE_ID",0,NULL, 2, create_node_AST(id->lexeme,1,id->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL), Datatype->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        B->list_inh = insert_at_end(B->list_inh, data_id);

        construct_AST(B, rules);

        Output_plist->list_syn = B->list_syn;
    }
    break;
    case 14://B' comma id colon Datatype B'1
    {
        t_node *B = root;
        t_node *id = root->children[1];
        t_node *Datatype = root->children[3];
        t_node *B1 = root->children[4];

        construct_AST(Datatype, rules);

        ast_node *data_id = create_node_AST("DATATYPE_ID",0,NULL, 2, create_node_AST(id->lexeme,1,id->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL), Datatype->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        B1->list_inh = insert_at_end(B->list_inh, data_id);

        construct_AST(B1, rules);

        B->list_syn = B1->list_syn;
    }
    break;
    case 15://B'
    {
        t_node *B = root;

        B->list_syn = B->list_inh;
    }
    break;
    case 16://Datatype integer
    {
        t_node *Datatype = root;
        t_node *integer = root->children[0];

        Datatype->node_syn = create_node_AST(integer->lexeme,1,integer->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 17://Datatype real
    {
        t_node *Datatype = root;
        t_node *real = root->children[0];

        Datatype->node_syn = create_node_AST(real->lexeme,1,real->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 18://Datatype boolean
    {
        t_node *Datatype = root;
        t_node *boolean = root->children[0];

        Datatype->node_syn = create_node_AST(boolean->lexeme,1,boolean->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 19://Datatype array sqbo Rangenew sqbc of Type
    {
        // printf("In Datatype array\n");
        // fflush(stdout);
        t_node *Datatype = root;
        t_node *Rangenew = root->children[2];
        t_node *Type = root->children[5];

        construct_AST(Rangenew, rules);
        construct_AST(Type, rules);

        // if(Type->node_syn == NULL)
        //     printf("Type->node_syn is NULL\n");
        // else
        //     printf("Type->node_syn is not NULL\n");
        // fflush(stdout);
        // if(Rangenew->node_syn == NULL)
        //     printf("Rangenew->node_syn is NULL\n");
        // else
        //     printf("Rangenew->node_syn is not NULL\n");
        // fflush(stdout);

        Datatype->node_syn = create_node_AST("RANGE_DATATYPE",0,NULL, 2, Rangenew->node_syn, Type->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 20: //Rangenew Indarray1 rangeop Indarray2 .
    {
        t_node *Rangenew = root;
        t_node *Indarray1 = root->children[0];
        t_node *Indarray2 = root->children[2];

        construct_AST(Indarray1, rules);
        construct_AST(Indarray2, rules);

        // if(Indarray1->node_syn == NULL)
        //     printf("Indarray1->node_syn is NULL\n");
        // else
        //     printf("Indarray1->node_syn is not NULL\n");
        // fflush(stdout);

        // if(Indarray2->node_syn == NULL)
        //     printf("Indarray2->node_syn is NULL\n");
        // else
        //     printf("Indarray2->node_syn is not NULL\n");
        // fflush(stdout);

        Rangenew->node_syn = create_node_AST("ARRAY_RANGE",0,NULL, 2, Indarray1->node_syn,  Indarray2->node_syn, NULL, NULL, NULL, NULL, NULL,NULL,NULL);
    }
    break;
    case 21://Indarray Sign Indexcoef .
    {
        t_node *Indarray = root;
        t_node *Sign = root->children[0];
        t_node *Indexcoef = root->children[1];

        //printf("In Indarray Sign Indexcoef\n");
        fflush(stdout);

        construct_AST(Sign, rules);
        construct_AST(Indexcoef, rules);

        Indarray->node_syn = create_node_AST("BOUND",0,NULL, 2, Sign->node_syn, Indexcoef->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 22://Indarray Indexcoef .
    {
        t_node *Indarray = root;
        t_node *Indexcoef = root->children[0];

        construct_AST(Indexcoef, rules);

        Indarray->node_syn = Indexcoef->node_syn;
    }
    break;
    case 23://Indexcoef num .   //NUM LEXEME
    {
        t_node *Indexcoef = root;
        t_node *num = root->children[0];

        Indexcoef->node_syn = create_node_AST(num->lexeme,1,num->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);

    }
    break;
    case 24://Indexcoef id . 
    {
        t_node *Indexcoef = root;
        t_node *id = root->children[0];

        Indexcoef->node_syn = create_node_AST(id->lexeme,1,id->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 25://Sign plus . 
    {
        t_node *Sign = root;
        t_node *plus = root->children[0];

        Sign->node_syn = create_node_AST(plus->lexeme,1,plus->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 26://Sign minus . 
    {
        t_node *Sign = root;
        t_node *minus = root->children[0];

        Sign->node_syn = create_node_AST(minus->lexeme,1,minus->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 27://Type integer . 
    {
        t_node *Type = root;
        t_node *integer = root->children[0];

        Type->node_syn = create_node_AST(integer->lexeme,1,integer->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 28://Type real . 
    {
        t_node *Type = root;
        t_node *real = root->children[0];

        Type->node_syn = create_node_AST(real->lexeme,1,real->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 29://Type boolean . 
    {
        t_node *Type = root;
        t_node *boolean = root->children[0];

        Type->node_syn = create_node_AST(boolean->lexeme,1,boolean->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 30://Moduledef start Statements end 
    {
        //printf("\n");
        //printf("Moduledef start Statements end\n");
        fflush(stdout);
        t_node *Moduledef = root;
        t_node *start = root->children[0];
        t_node *Statements = root->children[1];
        t_node *end = root->children[2];

        Statements->list_inh = create_list();
        
        
        //printf("Statements\n");
        construct_AST(Statements, rules);
        //printf("Statements2\n");
        //printLinkedList(Statements->list_syn);

        Statements->node_syn = create_node_AST_from_list("STATEMENT_LIST", Statements->list_syn);
        // traverse_AST(Statements->node_syn);
        
        Moduledef->node_syn = Statements->node_syn;

        //generate_Quadruples_Statements_List(Statements->node_syn, fp);

        ((ast_node*)(Moduledef->node_syn))->startLine = start->line_no;
        ((ast_node*)(Moduledef->node_syn))->endLine = end->line_no;
        ((ast_node*)(Moduledef->node_syn))->containsBounds = 1;

        
    }
    break;
    case 31://Statements Statement Statements
    {
        //printf("Statements Statement Statements\n");
        fflush(stdout);
        t_node *Statements = root;
        t_node *Statement = root->children[0];
        t_node *Statements1 = root->children[1];

        construct_AST(Statement, rules);

        Statements->list_inh = insert_at_end(Statements->list_inh, Statement->node_syn);
        Statements1->list_inh = Statements->list_inh;

        construct_AST(Statements1, rules);

        Statements->list_syn = Statements1->list_syn;
    }
    break;
    case 32://Statements
    {
        //printf("Statements\n");
        fflush(stdout);
        t_node *Statements = root;

        Statements->list_syn = Statements->list_inh;
    }
    break;
    case 33://Statement ioStmt
    {
        t_node *Statement = root;
        t_node *ioStmt = root->children[0];

        construct_AST(ioStmt, rules);

        Statement->node_syn = ioStmt->node_syn;
    }
    break;
    case 34://Statement Simplestmt
    {
        //printf("Statement Simplestmt\n");
        fflush(stdout);
        t_node *Statement = root;
        t_node *Simplestmt = root->children[0];

        construct_AST(Simplestmt, rules);

        Statement->node_syn = Simplestmt->node_syn;
    }
    break;
    case 35://Statement declareStmt
    {
        t_node *Statement = root;
        t_node *declareStmt = root->children[0];

        construct_AST(declareStmt, rules);

        Statement->node_syn = declareStmt->node_syn;
    }
    break;
    case 36://Statement conditionalStmt
    {
        t_node *Statement = root;
        t_node *conditionalStmt = root->children[0];

        construct_AST(conditionalStmt, rules);

        Statement->node_syn = conditionalStmt->node_syn;
    }
    break;
    case 37://Statement iterativeStmt
    {
        t_node *Statement = root;
        t_node *iterativeStmt = root->children[0];

        construct_AST(iterativeStmt, rules);

        Statement->node_syn = iterativeStmt->node_syn;
    }
    break;
    case 38://Iostmt get_value bo id bc semicol 
    {
        t_node *ioStmt = root;
        t_node *id = root->children[2];

        ioStmt->node_syn = create_node_AST("GET_VALUE",0,NULL, 1, create_node_AST(id->lexeme,1,id->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        //generate_Quadruples_Get_Value(ioStmt->node_syn,fp);

    }
    break;
    case 39://Iostmt print bo Printvar bc semicol 
    {
        t_node *ioStmt = root;
        t_node *Printvar = root->children[2];

        construct_AST(Printvar, rules);

        // //printf("\n\n\n\n\nPRINT \n");
        // //printf("\nline no: %s", root->children[2]->value);
        // //printf("\nIoStmt: %s", ioStmt->lexeme);
        // //printf("\nprint: %s",root->children[0]->lexeme);
        // //printf("\nbo: %s",root->children[1]->lexeme);
        // //printf("\nid: %s",root->children[2]->lexeme);
        // //printf("\nbc: %s",root->children[3]->lexeme);
        // //printf("\nsemicol: %s",root->children[4]->lexeme);
        // //printf("\n\n\n\n\n");

        ioStmt->node_syn = create_node_AST("PRINT",0,NULL, 1, Printvar->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        //printf("In print\n");
        fflush(stdout);
        //generate_Quadruples_Print_Statement(ioStmt->node_syn, quad_fp);

    }
    break;
    case 40: // Varnew Printvar .
    {
        t_node *Varnew = root;
        t_node *Printvar = root->children[0];

        construct_AST(Printvar, rules);
        Varnew->node_syn = create_node_AST("PARAMETER_VARIABLE",0,NULL, 1, Printvar->node_syn,NULL ,NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 41: // Varnew Sign Printvar .
    {
        t_node *Varnew = root;
        t_node *Sign = root->children[0];
        t_node *Printvar = root->children[1];

        construct_AST(Sign, rules);
        construct_AST(Printvar, rules);
        Varnew->node_syn = create_node_AST("PARAMETER_VARIABLE",0,NULL, 2, Sign->node_syn, Printvar->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 42: // Printvar Bool .
    {
        t_node *Printvar = root;
        t_node *Bool = root->children[0];

        construct_AST(Bool, rules);
        Printvar->node_syn = Bool->node_syn;
    }
    break;
    case 43: // Printvar num .
    {
        t_node *Printvar = root;
        t_node *num = root->children[0];

        Printvar->node_syn = create_node_AST("NUM",0,NULL, 1, create_node_AST(num->lexeme,1,num->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 44: //Printvar rnum .
    {
        t_node *Printvar = root;
        t_node *rnum = root->children[0];

        Printvar->node_syn = create_node_AST("RNUM",0,NULL, 1, create_node_AST(rnum->lexeme,1,rnum->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 45: //Printvar id J .
    {
        t_node *Printvar = root;
        t_node *id = root->children[0];
        t_node *J = root->children[1];

        //printf("PRINTVAR: %s\n", Printvar->lexeme);
        //printf("ID: %s\n", id->lexeme);
        //printf("J: %s\n", J->lexeme);
        //printf("Printing J children\n");
        for(int i = 0; i < J->num_children; i++)
        {
            //printf("J child %d: %s\n", i, J->children[i]->lexeme);
        }
        construct_AST(J, rules);

        //printf("CHECKINF INDARRAY\n");
        fflush(stdout);

        if(J->node_syn == NULL)
        {
            //printf("NULL\n");
            fflush(stdout);
        }
        else
        {
            //printf("NOT NULL\n");
            fflush(stdout);
        }

        Printvar->node_syn = create_node_AST("ARRAY_ELEM_OR_ID",0,NULL, 2, create_node_AST(id->lexeme,1,id->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL), J->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 46: //J' sqbo Indarray sqbc .
    {
        t_node *J = root;
        t_node *Indarray = root->children[1];

        construct_AST(Indarray, rules);

        //printf("CHECKING INDARRAY2222\n");
        if(Indarray->node_syn == NULL)
        {
            //printf("NULL\n");
            fflush(stdout);
        }
        else
        {
            //printf("NOT NULL\n");
            fflush(stdout);
        }

        J->node_syn = Indarray->node_syn;
    }
    break;
    case 47: //J' .
    {
        t_node *J = root;

        J->node_syn = NULL;
    }
    break;
    case 48: //Simplestmt Assignmentstmt .
    {
        t_node *simpleStmt = root;
        t_node *Assignmentstmt = root->children[0];

        construct_AST(Assignmentstmt, rules);

        simpleStmt->node_syn = Assignmentstmt->node_syn;
    }
    break;
    case 49: //Simplestmt Modulereusestmt .
    {
        //printf("Simplestmt Modulereusestmt\n");
        fflush(stdout);
        t_node *simpleStmt = root;
        t_node *Modulereusestmt = root->children[0];

        construct_AST(Modulereusestmt, rules);

        simpleStmt->node_syn = Modulereusestmt->node_syn;
    }
    break;
    case 50://Assignmentstmt id Whichstmt
    {
        t_node *Assignmentstmt = root;
        t_node *id = root->children[0];
        t_node *Whichstmt = root->children[1];

        Whichstmt->node_inh = create_node_AST(id->lexeme,1,id->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);

        construct_AST(Whichstmt, rules);

        Assignmentstmt->node_syn = Whichstmt->node_syn;

        //generate_Quadruples_Assign_Statement(Assignmentstmt->node_syn,fp);

    }
    break;
    case 51://Whichstmt Lvalueidstmt
    {
        t_node *Whichstmt = root;
        t_node *Lvalueidstmt = root->children[0];

        Lvalueidstmt->node_inh = Whichstmt->node_inh;

        construct_AST(Lvalueidstmt, rules);

        Whichstmt->node_syn = Lvalueidstmt->node_syn;
    }
    break;
    case 52://Whichstmt Lvaluearrstmt
    {
        t_node *Whichstmt = root;
        t_node *Lvaluearrstmt = root->children[0];

        Lvaluearrstmt->node_inh = Whichstmt->node_inh;

        construct_AST(Lvaluearrstmt, rules);

        Whichstmt->node_syn = Lvaluearrstmt->node_syn;
    }
    break;
    case 53://Lvalueidstmt assignop Expression semicol
    {
        t_node *Lvalueidstmt = root;
        t_node *Expression = root->children[1];

        ////printf("Expression -%s-",Expression->lexeme);
        //printf("here\n\n");
        construct_AST(Expression, rules);
        // if(Expression->node_syn == NULL)
        // {
        //     printf("Expression is NULL\n");
        //     fflush(stdout);
        // }
        // else
        // {
        //     printf("Expression is NOT NULL\n");
        //     fflush(stdout);
        // }
        
        Lvalueidstmt->node_syn = create_node_AST("ASSIGN",0,NULL, 2, Lvalueidstmt->node_inh, Expression->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 54://Lvaluearrstmt sqbo Elemindex sqbc assignop Expression semicol
    {
        t_node *Lvaluearrstmt = root;
        t_node *Elemindex = root->children[1];
        t_node *Expression = root->children[4];

        construct_AST(Elemindex, rules);
        construct_AST(Expression, rules);

        
        ast_node* array_elem = create_node_AST("ARRAY_ELEM",0,NULL, 2, Lvaluearrstmt->node_inh, Elemindex->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        Lvaluearrstmt->node_syn = create_node_AST("ASSIGN",0,NULL, 2, array_elem, Expression->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 55://Elemindex Sign G'
    {
        t_node *Elemindex = root;
        t_node *Sign = root->children[0];
        t_node *G = root->children[1];

        construct_AST(Sign, rules);
        construct_AST(G, rules);

        Elemindex->node_syn = create_node_AST("ARRAY_INDEX",0,NULL, 2, Sign->node_syn, G->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 56://Elemindex Aexpr
    {
        t_node *Elemindex = root;
        t_node *Aexpr = root->children[0];

        construct_AST(Aexpr, rules);

        Elemindex->node_syn = Aexpr->node_syn;
    }
    break;
    case 57://G' Indexcoef
    {
        t_node *G = root;
        t_node *Indexcoef = root->children[0];

        construct_AST(Indexcoef, rules);

        G->node_syn = Indexcoef->node_syn;
    }
    break;
    case 58://G' bo Aexpr bc
    {
        t_node *G = root;
        t_node *Aexpr = root->children[1];

        construct_AST(Aexpr, rules);

        G->node_syn = Aexpr->node_syn;
    }
    break;
    case 59://Modulereusestmt Optional use module id with parameters Actparalist semicol
    {
        //printf("Modulereusestmt Optional use module id with parameters Actparalist semicol\n");
        fflush(stdout);
        t_node *Modulereusestmt = root;
        t_node *Optional = root->children[0];
        t_node *id = root->children[3];
        t_node *Actparalist = root->children[6];

        construct_AST(Optional, rules);

        //printf("Here1\n");
        fflush(stdout);

        construct_AST(Actparalist, rules);

        //printf("Here\n");
        fflush(stdout);

        Actparalist->node_syn = create_node_AST_from_list("ACCPARA_LIST", Actparalist->list_syn);

        Modulereusestmt->node_syn = create_node_AST("REUSE_STMT",0,NULL, 3, Optional->node_syn,create_node_AST(id->lexeme,1,id->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),  Actparalist->node_syn, NULL, NULL, NULL, NULL, NULL, NULL);
    
        //generate_Quadruples_Module_Reuse_Statement(Modulereusestmt->node_syn,fp);

    }
    break;
    case 60: // Optional sqbo Idlist sqbc assignop .
    {
        //printf("Optional sqbo Idlist sqbc assignop\n");
        fflush(stdout);
        t_node *Optional = root;
        t_node *Idlist = root->children[1];

        //Idlist->list_inh = create_list();

        construct_AST(Idlist, rules);

        //printf("Here\n");
        

        //Idlist->node_syn = create_node_AST_from_list("ID_LIST", Idlist->list_syn);

        Optional->node_syn = Idlist->node_syn;
        ////printf("")
    }
    break;
    case 61: // Optional .
    {
        t_node *Optional = root;

        Optional->node_syn = NULL;
    }
    break;
    case 62: // Idlist id C' .
    {
        t_node *Idlist = root;
        t_node *id = root->children[0];
        t_node *C = root->children[1];

        C->list_inh = create_list();
        C->list_inh = insert_at_end(C->list_inh, create_node_AST(id->lexeme,1,id->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL));

        construct_AST(C, rules);

        C->node_syn = create_node_AST_from_list("ID_LIST", C->list_syn);
        Idlist->node_syn = C->node_syn;
    }
    break;
    case 63: // C' comma id C'1 .
    {
        t_node *C = root;
        t_node *id = root->children[1];
        t_node *C1 = root->children[2];

        C->list_inh = insert_at_end(C->list_inh, create_node_AST(id->lexeme,1,id->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL));
        C1->list_inh = C->list_inh;

        construct_AST(C1, rules);

        C->list_syn = C1->list_syn;
    }
    break;
    case 64: // C' .
    {
        t_node *C = root;

        C->list_syn = C->list_inh;
    }
    break;
    case 65: // Actparalist Varnew N11 .
    {
        t_node *Actparalist = root;
        t_node *Varnew = root->children[0];
        t_node *N11 = root->children[1];

        construct_AST(Varnew, rules);

        N11->list_inh = create_list();
        N11->list_inh = insert_at_end(N11->list_inh, Varnew->node_syn);
        
        construct_AST(N11, rules);

        //N11->node_syn = create_node_AST_from_list("N11_LIST", N11->list_syn);

        Actparalist->list_syn = N11->list_syn;
    }
    break;
    case 66: // N11 comma Varnew N11a .
    {
        t_node *N11 = root;
        t_node *Varnew = root->children[1];
        t_node *N11a = root->children[2];

        construct_AST(Varnew, rules);

        N11->list_inh = insert_at_end(N11->list_inh, Varnew->node_syn);
        N11a->list_inh = N11->list_inh;

        construct_AST(N11a, rules);

        N11->list_syn = N11a->list_syn;
    }
    break;
    case 67: // N11 .
    {
        t_node *N11 = root;

        N11->list_syn = N11->list_inh;
    }
    break;
    case 68: // Expression Aorbexpr .
    {
        // printf("Expression Aorbexpr\n");
        // fflush(stdout);
        t_node *Expression = root;
        t_node *Aorbexpr = root->children[0];

        construct_AST(Aorbexpr, rules);

        // if(Aorbexpr->node_syn == NULL)
        // {
        //     printf("Aorbexpr->node_syn is NULL\n");
        //     fflush(stdout);
        // }
        // else
        // {
        //     printf("Aorbexpr->node_syn is not NULL\n");
        //     fflush(stdout);
        // }

        Expression->node_syn = Aorbexpr->node_syn;
    }
    break;
    case 69: // Expression Unaryexpr .
    {
        // printf("Expression Unaryexpr\n");
        // fflush(stdout);
        t_node *Expression = root;
        t_node *Unaryexpr = root->children[0];

        construct_AST(Unaryexpr, rules);

        
        Expression->node_syn = Unaryexpr->node_syn;
    }
    break;
    case 70: // Unaryexpr Op3 Unaryop .
    {
        t_node *Unaryexpr = root;
        t_node *Op3 = root->children[0];
        t_node *Unaryop = root->children[1];

        construct_AST(Op3, rules);
        
        Unaryop->node_inh = Op3->node_syn;
        
        construct_AST(Unaryop, rules);

        Unaryexpr->node_syn = Unaryop->node_syn;
    }
    break;
    case 71: // Unaryop bo Arithmeticexpr bc .
    {
        t_node *Unaryop = root;
        t_node *Arithmeticexpr = root->children[1];

        construct_AST(Arithmeticexpr, rules);

        Unaryop->node_syn = create_node_AST("UNARY_OP",0,NULL, 2, Unaryop->node_inh, Arithmeticexpr->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 72: // Unaryop Var_idnum .
    {
        t_node *Unaryop = root;
        t_node *Var_idnum = root->children[0];

        construct_AST(Var_idnum, rules);

        Unaryop->node_syn = create_node_AST("UNARY_OP",0,NULL, 2, Unaryop->node_inh, Var_idnum->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 73: // Var_idnum num .
    {
        t_node *Var_idnum = root;
        t_node *num = root->children[0];

        Var_idnum->node_syn = create_node_AST(num->lexeme,1,num->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 74: // Var_idnum rnum .
    {
        t_node *Var_idnum = root;
        t_node *rnum = root->children[0];

        Var_idnum->node_syn = create_node_AST(rnum->lexeme,1,rnum->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 75: // Var_idnum id .
    {
        t_node *Var_idnum = root;
        t_node *id = root->children[0];

        Var_idnum->node_syn = create_node_AST(id->lexeme,1,id->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 76: //Op3 plus .
    {
        t_node *Op3 = root;
        t_node *plus = root->children[0];

        Op3->node_syn = create_node_AST(plus->lexeme,1,plus->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 77: //Op3 minus .
    {
        t_node *Op3 = root;
        t_node *minus = root->children[0];

        Op3->node_syn = create_node_AST(minus->lexeme,1,minus->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 78: // Aorbexpr Genterm H' .
    {
       // printf("Aorbexpr Genterm H'\n");
        fflush(stdout);
        t_node *Aorbexpr = root;
        t_node *Genterm = root->children[0];
        t_node *H = root->children[1];

        construct_AST(Genterm, rules);

        // if(Genterm->node_syn == NULL)
        // {
        //     printf("Genterm->node_syn is NULL\n");
        //     fflush(stdout);
        // }
        // else
        // {
        //     printf("Genterm->node_syn is not NULL\n");
        //     fflush(stdout);
        // }

        H->node_inh = Genterm->node_syn;

        construct_AST(H, rules);

        // if(H->node_syn == NULL)
        // {
        //     printf("H->node_syn is NULL\n");
        //     fflush(stdout);
        // }
        // else
        // {
        //     printf("H->node_syn is not NULL\n");
        //     fflush(stdout);
        // }
        
        Aorbexpr->node_syn = H->node_syn;
    }
    break;
    case 79: // H' Logicalop Genterm H'1 .
    {
        t_node *H = root;
        t_node *Logicalop = root->children[0];
        t_node *Genterm = root->children[1];
        t_node *H1 = root->children[2];

        construct_AST(Logicalop, rules);
        construct_AST(Genterm, rules);

        H->node_inh = create_node_AST("LOGICAL_OP",0,NULL, 3, H->node_inh, Logicalop->node_syn, Genterm->node_syn, NULL, NULL, NULL, NULL, NULL, NULL);
        H1->node_inh = H->node_inh;

        construct_AST(H1, rules);

        H->node_syn = H1->node_syn;
    }
    break;
    case 80: // H' .
    {
        t_node *H = root;

        H->node_syn = H->node_inh;
    }
    break;
    case 81: // Genterm Arithmeticexpr I' .
    {
        ///printf("Genterm Arithmeticexpr I'\n");
        fflush(stdout);
        t_node *Genterm = root;
        t_node *Arithmeticexpr = root->children[0];
        t_node *I = root->children[1];

        construct_AST(Arithmeticexpr, rules);

        I->node_inh = Arithmeticexpr->node_syn;

        construct_AST(I, rules);

        Genterm->node_syn = I->node_syn;
    }
    break;
    case 82: // I' Relationalop Arithmeticexpr .
    {
        t_node *I = root;
        t_node *Relationalop = root->children[0];
        t_node *Arithmeticexpr = root->children[1];

        construct_AST(Relationalop, rules);
        construct_AST(Arithmeticexpr, rules);

        I->node_syn = create_node_AST("RELATIONAL_OP",0,NULL, 3, I->node_inh, Relationalop->node_syn, Arithmeticexpr->node_syn, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 83: // I' .
    {
        t_node *I = root;

        I->node_syn = I->node_inh;
    }
    break;
    case 84: // Arithmeticexpr Term D' .
    {
       // printf("Arithmeticexpr Term D' .\n");
        fflush(stdout);
        t_node *Arithmeticexpr = root;
        t_node *Term = root->children[0];
        t_node *D = root->children[1];

        construct_AST(Term, rules);

        //printf("Term = %s\n", ((ast_node*)(Term->node_syn))->type);

        D->node_inh = Term->node_syn;


        construct_AST(D, rules);

        //printf("D = %s\n", ((ast_node*)(D->node_syn))->type);
        //traverse_AST_print(D->node_syn,0,1);

        Arithmeticexpr->node_syn = D->node_syn;
    }
    break;
    case 85: // D' Op1 Term D'1 .
    {
        //rintf("D' Op1 Term D'1 .\n");
        fflush(stdout);
        t_node *D = root;
        t_node *Op1 = root->children[0];
        t_node *Term = root->children[1];
        t_node *D1 = root->children[2];

        construct_AST(Op1, rules);
        construct_AST(Term, rules);

        

        // printf("Op1 = %s\n", ((ast_node*)(Op1->node_syn))->type);
        // printf("Term = %s\n", ((ast_node*)(Term->node_syn))->type);
        

        D1->node_inh = create_node_AST("OPERATION_1",0,NULL, 3, D->node_inh, Op1->node_syn, Term->node_syn, NULL, NULL, NULL, NULL, NULL, NULL);
        //D1->node_inh = D->node_inh;

        construct_AST(D1, rules);

        D->node_syn = D1->node_syn;

        //printf("Printing Inherited D->inh\n");
        //traverse_AST_print(D->node_inh,0,1);
        //printf("Printing Synthesized D->syn\n");
        //traverse_AST_print(D->node_syn,0,1);

        

        //traverse_AST_print(D->node_syn,0,1);
    }
    break;
    case 86: // D' .
    {
        //printf("D''' .\n");
        t_node *D = root;

        //printf("\t\tD''' D->inh\n");
        //traverse_AST_print(D->node_inh,2,1);
        //printf("\t\t\t\tPrinting Synthesized same as inh\t\t\n");

        D->node_syn = D->node_inh;
    }
    break;
    case 87: // Op1 plus .
    {
        t_node *Op1 = root;
        t_node *plus = root->children[0];

        Op1->node_syn = create_node_AST(plus->lexeme,1,plus->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 88: // Op1 minus .
    {
        t_node *Op1 = root;
        t_node *minus = root->children[0];

        Op1->node_syn = create_node_AST(minus->lexeme,1,minus->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 89: // Term Factor E' .
    {
        // printf("Term Factor E'\n");
        t_node *Term = root;
        t_node *Factor = root->children[0];
        t_node *E = root->children[1];

        construct_AST(Factor, rules);

        // printf("Factor = %s\n", ((ast_node*)(Factor->node_syn))->type);

        E->node_inh = Factor->node_syn;

        construct_AST(E, rules);

        Term->node_syn = E->node_syn;
    }
    break;
    case 90: // E' Op2 Factor E'1 .
    {
        //printf("E' Op2 Factor E'1\n");
        t_node *E = root;
        t_node *Op2 = root->children[0];
        t_node *Factor = root->children[1];
        t_node *E1 = root->children[2];

        construct_AST(Op2, rules);
        construct_AST(Factor, rules);

        // printf("Op2 = %s\n", ((ast_node*)(Op2->node_syn))->type);
        // printf("Factor = %s\n", ((ast_node*)(Factor->node_syn))->type);

        E->node_inh = create_node_AST("OPERATION_2",0,NULL, 3, E->node_inh, Op2->node_syn, Factor->node_syn, NULL, NULL, NULL, NULL, NULL, NULL);
        E1->node_inh = E->node_inh;

        construct_AST(E1, rules);

        E->node_syn = E1->node_syn;
    }
    break;
    case 91: // E' .
    {

        //printf("E'''\n");
        t_node *E = root;

        E->node_syn = E->node_inh;
    }
    break;
    case 92: // Op2 mul .
    {
        t_node *Op2 = root;
        t_node *mul = root->children[0];

        Op2->node_syn = create_node_AST(mul->lexeme,1,mul->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 93: // Op2 div .
    {
        t_node *Op2 = root;
        t_node *div = root->children[0];

        Op2->node_syn = create_node_AST(div->lexeme,1,div->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 94: // Factor bo Aorbexpr bc .
    {
        t_node *Factor = root;
        t_node *Aorbexpr = root->children[1];

        construct_AST(Aorbexpr, rules);

        Factor->node_syn = Aorbexpr->node_syn;
    }
    break;
    case 95: // Factor Bool .
    {
        t_node *Factor = root;
        t_node *Bool = root->children[0];

        construct_AST(Bool, rules);

        Factor->node_syn = Bool->node_syn;
    }
    break;
    case 96: // Factor num .
    {
        t_node *Factor = root;
        t_node *num = root->children[0];

        Factor->node_syn = create_node_AST(num->lexeme,1,num->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 97: // Factor rnum .
    {
        t_node *Factor = root;
        t_node *rnum = root->children[0];

        Factor->node_syn = create_node_AST(rnum->lexeme,1,rnum->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 98: //cFactor id K' .
    {
        t_node *Factor = root;
        t_node *id = root->children[0];
        t_node *K = root->children[1];


        K->node_inh = create_node_AST(id->lexeme,1,id->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);

        construct_AST(K, rules);

        Factor->node_syn = K->node_syn;
    }
    break;
    case 99: // K' sqbo Elemindex sqbc .
    {
        t_node *K = root;
        t_node *Elemindex = root->children[1];
        
        construct_AST(Elemindex, rules);

        K->node_syn = create_node_AST("ARRAY",0,NULL, 2, K->node_inh, Elemindex->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 100: // K' .
    {
        t_node *K = root;

        //printf("most inside\n");
        K->node_syn = K->node_inh;
    }
    break;
    case 101: // Aexpr Aterm N4 .
    {
        t_node *Aexpr = root;
        t_node *Aterm = root->children[0];
        t_node *N4 = root->children[1];

        construct_AST(Aterm, rules);

        N4->node_inh = Aterm->node_syn;

        construct_AST(N4, rules);

        Aexpr->node_syn = N4->node_syn;
    }
    break;
    case 102: // N4 Op1 Aterm N41 .
    {
        t_node *N4 = root;
        t_node *Op1 = root->children[0];
        t_node *Aterm = root->children[1];
        t_node *N41 = root->children[2];

        construct_AST(Op1, rules);
        construct_AST(Aterm, rules);

        N4->node_inh = create_node_AST("OPERATION_1",0,NULL, 3, N4->node_inh, Op1->node_syn, Aterm->node_syn, NULL, NULL, NULL, NULL, NULL, NULL);
        N41->node_inh = N4->node_inh;

        construct_AST(N41, rules);

        N4->node_syn = N41->node_syn;
    }
    break;
    case 103: // N4 .
    {
        //printf("N4 -> .\n");
        t_node *N4 = root;

        N4->node_syn = N4->node_inh;
    }
    break;
    case 104: // Aterm Afactor N5 .
    {
        t_node *Aterm = root;
        t_node *Afactor = root->children[0];
        t_node *N5 = root->children[1];

        construct_AST(Afactor, rules);

        N5->node_inh = Afactor->node_syn;

        construct_AST(N5, rules);

        Aterm->node_syn = N5->node_syn;
    }
    break;
    case 105: // N5 Op2 Afactor N51 .
    {
        t_node *N5 = root;
        t_node *Op2 = root->children[0];
        t_node *Afactor = root->children[1];
        t_node *N51 = root->children[2];

        construct_AST(Op2, rules);
        construct_AST(Afactor, rules);

        N5->node_inh = create_node_AST("OPERATION_2",0,NULL, 3, N5->node_inh, Op2->node_syn, Afactor->node_syn, NULL, NULL, NULL, NULL, NULL, NULL);
        N51->node_inh = N5->node_inh;

        construct_AST(N51, rules);

        N5->node_syn = N51->node_syn;
    }
    break;
    case 106: // N5 .
    {
        t_node *N5 = root;

        N5->node_syn = N5->node_inh;
    }
    break;
        case 107://Afactor id .
    {
        t_node *Afactor = root;
        t_node *id = root->children[0];

        Afactor->node_syn = create_node_AST(id->lexeme,1,id->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
        case 108://Afactor num .
    {
        t_node *Afactor = root;
        t_node *num = root->children[0];
        
        Afactor->node_syn = create_node_AST(num->lexeme,1,num->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
        case 109://Afactor Bool .
    {
        t_node *Afactor = root;
        t_node *Bool = root->children[0];
        construct_AST(Bool, rules);
        Afactor->node_syn = Bool->node_syn ;
    }
    break;
    case 110://Afactor bo Aexpr bc .
    {
        t_node *Afactor = root;
        t_node *Aexpr = root->children[1];
        construct_AST(Aexpr, rules);
        Afactor->node_syn = Aexpr->node_syn;
    }
    break;
    case 111://Bool true .
    {
        t_node *Bool = root;
        t_node *true = root->children[0];

        Bool->node_syn = create_node_AST(true->lexeme,1,true->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 112://Bool false .
    {
        t_node *Bool = root;
        t_node *false = root->children[0];

        Bool->node_syn = create_node_AST(false->lexeme,1,false->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 113://Logicalop AND . 
    {
        t_node *Logicalop = root;
        t_node *AND = root->children[0];

        Logicalop->node_syn = create_node_AST(AND->lexeme,1,AND->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 114://Logicalop OR . 
    {
        t_node *Logicalop = root;
        t_node *OR = root->children[0];

        Logicalop->node_syn = create_node_AST(OR->lexeme,1,OR->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 115://Relationalop lt . 
    {
        t_node *Relationalop = root;
        t_node *lt = root->children[0];

        Relationalop->node_syn = create_node_AST(lt->lexeme,1,lt->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 116://Relationalop gt . 
    {
        t_node *Relationalop = root;
        t_node *gt = root->children[0];

        Relationalop->node_syn = create_node_AST(gt->lexeme,1,gt->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 117://Relationalop le . 
    {
        t_node *Relationalop = root;
        t_node *le = root->children[0];

        Relationalop->node_syn = create_node_AST(le->lexeme,1,le->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 118://Relationalop ge . 
    {
        t_node *Relationalop = root;
        t_node *ge = root->children[0];

        Relationalop->node_syn = create_node_AST(ge->lexeme,1,ge->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 119://Relationalop eq . 
    {
        t_node *Relationalop = root;
        t_node *eq = root->children[0];

        Relationalop->node_syn = create_node_AST(eq->lexeme,1,eq->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 120://Relationalop ne . 
    {
        t_node *Relationalop = root;
        t_node *ne = root->children[0];

        Relationalop->node_syn = create_node_AST(ne->lexeme,1,ne->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 121: // Declarestmt declare Actparalist colon Datatype semicol .
    {
        t_node *Declarestmt = root;
        t_node *Actparalist = root->children[1];
        t_node *Datatype = root->children[3];
       
        construct_AST(Actparalist, rules);
        construct_AST(Datatype, rules);

        Actparalist->node_syn = create_node_AST_from_list("ACCPARA_LIST", Actparalist->list_syn);
        Declarestmt->node_syn = create_node_AST("DECLARE",0,NULL, 2, Datatype->node_syn, Actparalist->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 122: // Conditionalstmt switch bo id bc start Casestmt Default end .
    {
        t_node *Conditionalstmt = root;
        t_node *SWITCH = root->children[0];
        t_node *id = root->children[2];
        t_node *Casestmt = root->children[5];
        t_node *Default = root->children[6];
        t_node *END = root->children[7];

        Casestmt->list_inh = create_list();

        construct_AST(Casestmt, rules);
        construct_AST(Default, rules);

        Casestmt->node_syn = create_node_AST_from_list("CASE_LIST", Casestmt->list_syn);
        Conditionalstmt->node_syn = create_node_AST("SWITCH",0,NULL, 3, create_node_AST(id->lexeme,1,id->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL), Casestmt->node_syn, Default->node_syn, NULL, NULL, NULL, NULL, NULL, NULL);
    
        ((ast_node*)(Conditionalstmt->node_syn))->containsBounds = 1;
        ((ast_node*)(Conditionalstmt->node_syn))->startLine = SWITCH->line_no;
        ((ast_node*)(Conditionalstmt->node_syn))->endLine = END->line_no;

        //generate_Quadruples_Switch(Conditionalstmt->node_syn,fp);

    }
    break;
    case 123: // Casestmt case Value colon Statements break semicol F' .
    {
        t_node *Casestmt = root;
        t_node *CASE = root->children[0];
        t_node *Value = root->children[1];
        t_node *Statements = root->children[3];
        t_node *SEMICOL = root->children[5];
        t_node *F = root->children[6];

        Statements->list_inh = create_list();

        construct_AST(Value, rules);
        construct_AST(Statements, rules);

        Statements->node_syn = create_node_AST_from_list("STATEMENT_LIST", Statements->list_syn);
        // F->node_inh = create_node_AST("CASE", 2, Value->node_syn, Statements->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        // Casestmt->list_inh = create_list(F->node_inh);
        //generate_Quadruples_Statements_List(Statements->node_syn,fp);
        ast_node *case_node = create_node_AST("CASE", 0, NULL, 2, Value->node_syn, Statements->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        Casestmt->list_inh=insert_at_end(Casestmt->list_inh, case_node);
        F->list_inh = Casestmt->list_inh;

        construct_AST(F, rules);

        Casestmt->list_syn = F->list_syn;

        ((ast_node*)(case_node))->containsBounds = 1;
        ((ast_node*)(case_node))->startLine = CASE->line_no;
        ((ast_node*)(case_node))->endLine = SEMICOL->line_no;
    }
    break;
    case 124: //F' case Value colon Statements break semicol F'1
    {
        t_node *F = root;
        t_node *CASE1 = root->children[0];
        t_node *Value = root->children[1];
        t_node *Statements = root->children[3];
        t_node *SEMICOL1 = root->children[5];
        t_node *F1 = root->children[6];

        Statements->list_inh = create_list();

        construct_AST(Value, rules);
        construct_AST(Statements, rules);

        Statements->node_syn = create_node_AST_from_list("STATEMENT_LIST", Statements->list_syn);

        //generate_Quadruples_Statements_List(Statements->node_syn,fp);

        ast_node *case_node = create_node_AST("CASE",0,NULL, 2, Value->node_syn, Statements->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        F->list_inh = insert_at_end(F->list_inh, case_node);
        F1->list_inh = F->list_inh;

        construct_AST(F1, rules);

        F->list_syn = F1->list_syn;

        ((ast_node*)(case_node))->containsBounds = 1;
        ((ast_node*)(case_node))->startLine = CASE1->line_no;
        ((ast_node*)(case_node))->endLine = SEMICOL1->line_no;
    }
    break;
    case 125: // F'
    {
        t_node *F = root;

        F->list_syn = F->list_inh;
    }
    break;
        case 126://Value num .
    {
        t_node *Value = root;
        t_node *num = root->children[0];
        
        Value->node_syn = create_node_AST(num->lexeme,1,num->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL); 
    }
    break;
    case 127://Value true .
    {
        t_node *Value = root;
        t_node *true = root->children[0];

        Value->node_syn = create_node_AST(true->lexeme,1,true->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 128://Value false .
    {
        t_node *Value = root;
        t_node *false = root->children[0];

        Value->node_syn = create_node_AST(false->lexeme,1,false->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 129://Default default colon Statements break semicol .
    {
        t_node *Default = root;
        t_node *DEFAULT = root->children[0];
        t_node *Statements = root->children[2];
        t_node *SEMICOL2 = root->children[4];

        Statements->list_inh = create_list();

        construct_AST(Statements, rules);

        Statements->node_syn = create_node_AST_from_list("STATEMENT_LIST", Statements->list_syn);

        //generate_Quadruples_Statements_List(Statements->node_syn,fp);

        Default->node_syn= create_node_AST("DEFAULT",0,NULL, 1, Statements->node_syn,NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        ((ast_node*)(Default->node_syn))->containsBounds = 1;
        ((ast_node*)(Default->node_syn))->startLine = DEFAULT->line_no;
        ((ast_node*)(Default->node_syn))->endLine = SEMICOL2->line_no;

    }
    break;
    case 130://Default .
    {
        t_node *Default = root;

        Default->node_syn = NULL;
    }
    break;
    case 131://Iterativestmt for bo id in Range bc start Statements end .
    {
        t_node *Iterativestmt = root;
        t_node *FOR = root->children[0];
        t_node *id = root->children[2];
        t_node *Range = root->children[4];
        t_node *Statements = root->children[7];
        t_node *END = root->children[8];

        Statements->list_inh = create_list();

        construct_AST(Range, rules);
        construct_AST(Statements, rules);

        Statements->node_syn = create_node_AST_from_list("STATEMENT_LIST", Statements->list_syn);

    

        Iterativestmt->node_syn= create_node_AST("FOR_LOOP",0,NULL, 3, create_node_AST(id->lexeme,1,id->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL), Range->node_syn, Statements->node_syn, NULL, NULL, NULL, NULL, NULL, NULL);

        ((ast_node*)(Iterativestmt->node_syn))->containsBounds = 1;
        ((ast_node*)(Iterativestmt->node_syn))->startLine = FOR->line_no;
        ((ast_node*)(Iterativestmt->node_syn))->endLine = END->line_no;

        //generate_Quadruples_For(Iterativestmt->node_syn,fp);

    }
    break;
    case 132://Range Indloop1 rangeop Indloop2 .
    {
        t_node *Range = root;
        t_node *Indloop1 = root->children[0];
        t_node *Indloop2 = root->children[2];

        construct_AST(Indloop1, rules);
        construct_AST(Indloop2, rules);

        Range->node_syn= create_node_AST("LOOP_RANGE",0,NULL, 2, Indloop1->node_syn, Indloop2->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 133://Indloop Signloop Indcoefloop .
    {
        t_node *Indloop = root;
        t_node *Signloop = root->children[0];
        t_node *Indcoefloop = root->children[1];

        construct_AST(Signloop, rules);
        construct_AST(Indcoefloop, rules);

        Indloop->node_syn= create_node_AST("RANGE_LIMIT",0,NULL, 2, Signloop->node_syn, Indcoefloop->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 134://Indcoefloop num .
    {
        t_node *Indcoefloop = root;
        t_node *num = root->children[0];
        
        Indcoefloop->node_syn = create_node_AST(num->lexeme,1,num->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 135://Signloop plus .
    {
        t_node *Signloop = root;
        t_node *plus = root->children[0];

        Signloop->node_syn = create_node_AST(plus->lexeme,1,plus->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 136://Signloop minus .
    {
        t_node *Signloop = root;
        t_node *minus = root->children[0];

        Signloop->node_syn = create_node_AST(minus->lexeme,1,minus->lex,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    break;
    case 137://Signloop .
    {
        t_node *Signloop = root;

        Signloop->node_syn = NULL;
    }
    break;
    case 138://Iterativestmt while bo Aorbexpr bc start Statements end .
    {                           //0   1   2       3   4       5       6  
        t_node *Iterativestmt = root;
        t_node *WHILE = root->children[0];
        t_node *Aorbexpr = root->children[2];
        t_node *Statements = root->children[5];
        t_node *end = root->children[6];

        Statements->list_inh = create_list();

        construct_AST(Aorbexpr, rules);
        construct_AST(Statements, rules);

        Statements->node_syn = create_node_AST_from_list("STATEMENT_LIST", Statements->list_syn);

        Iterativestmt->node_syn= create_node_AST("WHILE_LOOP",0,NULL, 2, Aorbexpr->node_syn, Statements->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    
        ((ast_node*)(Iterativestmt->node_syn))->containsBounds = 1;
        ((ast_node*)(Iterativestmt->node_syn))->startLine = WHILE->line_no;
        ((ast_node*)(Iterativestmt->node_syn))->endLine = end->line_no;

        //generate_Quadruples_While_Statement(Iterativestmt->node_syn,fp);

    }
    break;
    default:
        break;
    }

    return node;
}

