//CS F363 : Compiler Construction
//Group No. 24
//1. Nachiket Kotalwar               (2020A7PS0024P)
//2.Parth Patel                      (2020A7PS0026P)
//3.Partha Sarathi Purkayastha       (2020A7PS0043P)
//4.Labeeb Ahsan                     (2020A7PS0045P)
//5.Anish Atul Kulkarni              (2020A7PS0975P)

#include "intermediateCodegenDef.h"



void generate_Quadruples_Statements_List(ast_node* node,st_table* symbol_table,quadrupleList* ql,FILE* fp);
void generate_Quadruples_Statement(ast_node* node,st_table* symbol_table,quadrupleList* ql,FILE* fp);
st_entry* generate_Quadruples_Expression_Helper(ast_node* node,st_table* symbol_table,quadrupleList* ql,FILE* fp);

FILE* quad_fp;

int temp_count = 0;
int label_count = 0;

void initializeIntermediateCodeGen()
{
    label_count = 0;
    temp_count = 0;
}
st_entry* generate_temp(st_table* symbol_table)
{
    temp_count++;
    char* temp = (char*)malloc(sizeof(char)*10);
    sprintf(temp, "temp_%d", temp_count);
    
    st_entry* temp_entry = createSymbolEntry(temp, createIDType("integer", 0, 0, 0, -1, -1, NULL, NULL, 0, 0, NULL, 0, NULL, 0), symbol_table->scopeName, 8, symbol_table->offset, -1, -1, symbol_table->offsetAct);
    addSymbolEntry(symbol_table, temp_entry);

    return temp_entry;
}



quadrupleList* create_QuadrupleList()
{
    quadrupleList* ql = (quadrupleList*)malloc(sizeof(quadrupleList));
    ql->quad = NULL;
    ql->next = NULL;
    return ql;
}

void insert_at_end_quadrupleList(quadrupleList* ql,quadruple* q)
{
    quadrupleList* temp = ql;
    while(temp->next!=NULL)
    {
        temp = temp->next;
    }
    quadrupleList* new = (quadrupleList*)malloc(sizeof(quadrupleList));
    new->quad = q;
    new->next = NULL;
    temp->next = new;
}

void print_Quadruple(quadruple* quad)
{
    printf("%-20s\t",quad->op);
    //printf("Op printed\n");
    if(quad->arg1_type == ST_ENTRY_QUAD)
    {
        char* temp = (char*)malloc(sizeof(char)*20);
        sprintf(temp, "%s-%d", quad->arg1_st_entry->var_name, quad->arg1_st_entry->offsetAct);
        printf("%-30s\t",temp);
    }
    else if(quad->arg1_type == NUM_QUAD)
    {
        printf("%-30d\t",quad->arg1_num);
    }
    else if(quad->arg1_type == RNUM_QUAD)
    {
        printf("%-30f\t",quad->arg1_rnum);
    }
    else if(quad->arg1_type == BOOL_QUAD)
    {
        printf("%-30d\t",quad->arg1_bool);
    }
    else if(quad->arg1_type == LABEL_QUAD)
    {
        printf("%-30s\t",quad->arg1_label);
    }
    else
    {
        char* temp = (char*)malloc(sizeof(char)*20);
        sprintf(temp, "NULL");
        printf("%-30s\t",temp);
    }

    //printf("Arr1 printed\n");
    fflush(stdout);

    if(quad->arg2_type == ST_ENTRY_QUAD)
    {
        char *temp = (char *)malloc(sizeof(char) * 20);
        sprintf(temp, "%s-%d", quad->arg2_st_entry->var_name, quad->arg2_st_entry->offsetAct);
        printf("%-30s\t",temp);
    }
    else if(quad->arg2_type == NUM_QUAD)
    {
        printf("%-30d\t",quad->arg2_num);
    }
    else if(quad->arg2_type == RNUM_QUAD)
    {
        printf("%-30f\t",quad->arg2_rnum);
    }
    else if(quad->arg2_type == BOOL_QUAD)
    {
        printf("%-30d\t",quad->arg2_bool);
    }
    else if(quad->arg2_type == LABEL_QUAD)
    {
        printf("%-30s\t",quad->arg2_label);
    }
    else
    {
        char *temp = (char *)malloc(sizeof(char) * 20);
        sprintf(temp, "NULL");
        printf("%-30s\t",temp);
    }

    if(quad->result == NULL)
    {
        printf("%-30s\n","NULL");
    }
    else
    {
        char *temp = (char *)malloc(sizeof(char) * 20);
        sprintf(temp, "%s-%d", quad->result->var_name, quad->result->offsetAct);
        printf("%-30s\n",temp);
    }
}

void print_QuadrupleList(quadrupleList* ql)
{
    
    quadrupleList* temp = ql->next;
    while(temp!=NULL)
    {
        print_Quadruple(temp->quad);
        temp = temp->next;
    }
}

void print_Quadruple_in_FILE(quadruple *quad,st_table* symbol_table,quadrupleList* ql,FILE* fp)
{
    //printf("Print Quad in File\n");
    if(quad == NULL)
    {
        return;
    }

    insert_at_end_quadrupleList(ql,quad);
    
    // printf("%-20s\t",quad->op);
    // //printf("Op printed\n");
    // if(quad->arg1_type == ST_ENTRY_QUAD)
    // {
    //     printf("%s\t",quad->arg1_st_entry->var_name);
    // }
    // else if(quad->arg1_type == NUM_QUAD)
    // {
    //     printf("%d\t",quad->arg1_num);
    // }
    // else if(quad->arg1_type == RNUM_QUAD)
    // {
    //     printf("%f\t",quad->arg1_rnum);
    // }
    // else if(quad->arg1_type == BOOL_QUAD)
    // {
    //     printf("%d\t",quad->arg1_bool);
    // }
    // else if(quad->arg1_type == LABEL_QUAD)
    // {
    //     printf("%s\t",quad->arg1_label);
    // }
    // else
    // {
    //     printf("NULL\t");
    // }

    // //printf("Arr1 printed\n");
    // fflush(stdout);

    // if(quad->arg2_type == ST_ENTRY_QUAD)
    // {
    //     printf("%s\t",quad->arg2_st_entry->var_name);
    // }
    // else if(quad->arg2_type == NUM_QUAD)
    // {
    //     printf("%d\t",quad->arg2_num);
    // }
    // else if(quad->arg2_type == RNUM_QUAD)
    // {
    //     printf("%f\t",quad->arg2_rnum);
    // }
    // else if(quad->arg2_type == BOOL_QUAD)
    // {
    //     printf("%d\t",quad->arg2_bool);
    // }
    // else if(quad->arg2_type == LABEL_QUAD)
    // {
    //     printf("%s\t",quad->arg2_label);
    // }
    // else
    // {
    //     printf("NULL\t");
    // }

    // if(quad->result == NULL)
    // {
    //     printf("%s\n","NULL");
    // }
    // else
    // {
    //     printf("%s\n",quad->result->var_name);
    // }

}

// char* generate_Quadruples_Add(enum QUAD_ARG_TYPE arg1_type,st_entry* arg1_st_entry,int  ,char* arg2,st_table* symbol_table,quadrupleList* ql,FILE* fp)
// {
//     quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
//     quad->op = three_code_list[ADD_3CODE];
//     quad->arg1 = arg1;
//     quad->arg2 = arg2;
//     quad->result = generate_temp();
//     print_Quadruple_in_FILE(quad,quad_fp);
//     return quad->result;
// }

// st_entry* generate_Quadruples_Sub(char* arg1,st_entry* arg2,st_table* symbol_table,quadrupleList* ql,FILE* fp)
// {
//     quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
//     quad->op = three_code_list[SUB_3CODE];
//     // quad->arg1_type = 0;
//     quad->arg2_type = ST_ENTRY_QUAD;
//     quad->arg2_st_entry = arg2;
//     quad->result = generate_temp(symbol_table);
//
//     print_Quadruple_in_FILE(quad,symbol_table,ql,fp);
//
//     return quad->result;
// }
//
// char* generate_Quadruples_Mul(char* arg1,char* arg2,st_table* symbol_table,quadrupleList* ql,FILE* fp)
// {
//     quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
//     quad->op = three_code_list[MUL_3CODE];
//     quad->arg1 = arg1;
//     quad->arg2 = arg2;
//     quad->result = generate_temp();
//     print_Quadruple_in_FILE(quad,quad_fp);
//     return quad->result;
// }

// char* generate_Quadruples_Equal(char* arg1,char* arg2,st_table* symbol_table,quadrupleList* ql,FILE* fp)
// {
//     quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
//     quad->op = three_code_list[EQUAL_3CODE];
//     quad->arg1 = arg1;
//     quad->arg2 = arg2;
//     quad->result = generate_temp();
//     print_Quadruple_in_FILE(quad,quad_fp);
//     return quad->result;
// }

// char* generate_Quadruples_Less_Than(char* arg1,char* arg2,st_table* symbol_table,quadrupleList* ql,FILE* fp)
// {
//     quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
//     quad->op = three_code_list[LESS_THAN_3CODE];
//     quad->arg1 = arg1;
//     quad->arg2 = arg2;
//     quad->result = generate_temp();
//     print_Quadruple_in_FILE(quad,quad_fp);
//     return quad->result;
// }

// char* generate_Quadruples_Greater_Than(char* arg1,char* arg2,st_table* symbol_table,quadrupleList* ql,FILE* fp)
// {
//     quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
//     quad->op = three_code_list[GREATER_THAN_3CODE];
//     quad->arg1 = arg1;
//     quad->arg2 = arg2;
//     quad->result = generate_temp();
//     print_Quadruple_in_FILE(quad,quad_fp);
//     return quad->result;
// }

// char* generate_Quadruples_Less_Than_Equal(char* arg1,char* arg2,st_table* symbol_table,quadrupleList* ql,FILE* fp)
// {
//     quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
//     quad->op = three_code_list[LESS_THAN_EQUAL_3CODE];
//     quad->arg1 = arg1;
//     quad->arg2 = arg2;
//     quad->result = generate_temp();
//     print_Quadruple_in_FILE(quad,quad_fp);
//     return quad->result;
// }

// char* generate_Quadruples_Greater_Than_Equal(char* arg1,char* arg2,st_table* symbol_table,quadrupleList* ql,FILE* fp)
// {
//     quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
//     quad->op = three_code_list[GREATER_THAN_EQUAL_3CODE];
//     quad->arg1 = arg1;
//     quad->arg2 = arg2;
//     quad->result = generate_temp();
//     print_Quadruple_in_FILE(quad,quad_fp);
//     return quad->result;
// }

// char* generate_Quadruples_Not_Equal(char* arg1,char* arg2,st_table* symbol_table,quadrupleList* ql,FILE* fp)
// {
//     quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
//     quad->op = three_code_list[NOT_EQUAL_3CODE];
//     quad->arg1 = arg1;
//     quad->arg2 = arg2;
//     quad->result = generate_temp();
//     print_Quadruple_in_FILE(quad,quad_fp);
//     return quad->result;
// }

// char* generate_Quadruples_Jump(char* label,st_table* symbol_table,quadrupleList* ql,FILE* fp)
// {
//     quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
//     quad->op = three_code_list[JUMP_3CODE];
//     quad->arg1 = NULL;
//     quad->arg2 = NULL;
//     quad->result = label;
//     print_Quadruple_in_FILE(quad,quad_fp);
//     return quad->result;
// }

// char* generate_Quadruples_Jump_If_True(char* arg1,char* label,st_table* symbol_table,quadrupleList* ql,FILE* fp)
// {
//     quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
//     quad->op = three_code_list[JUMP_IF_TRUE_3CODE];
//     quad->arg1 = arg1;
//     quad->arg2 = NULL;
//     quad->result = label;
//     print_Quadruple_in_FILE(quad,quad_fp);
//     return quad->result;
// }

// char* generate_Quadruples_Jump_If_Not_True(char* arg1,char* label,st_table* symbol_table,quadrupleList* ql,FILE* fp)
// {
//     quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
//     quad->op = three_code_list[JUMP_IF_NOT_TRUE_3CODE];
//     quad->arg1 = arg1;
//     quad->arg2 = NULL;
//     quad->result = label;
//     print_Quadruple_in_FILE(quad,quad_fp);
//     return quad->result;
// }

st_entry* generate_Quadruples_Print_Array_Element_Access(ast_node *node,st_table* symbol_table,quadrupleList* ql,FILE* fp)
{
    if(node == NULL)
    {
        return NULL;
    }

    // quadruple *real_index_quad ;
    // quadruple *real_offset_quad ;
    //quadruple *quad = (quadruple *)malloc(sizeof(quadruple));

    // char* lower_bound = (char*)malloc(sizeof(char)*10);
    // //update lower_bound = "-10"
    // lower_bound[0] = '-';
    // lower_bound[1] = '1';
    // lower_bound[2] = '0';
    // lower_bound[3] = '\0';

    //printf("In array access\n");
    fflush(stdout);

    int arr_lower_bound;

    char* array_name = (char*)((ast_node*)(node->children[0]))->type;

    //printf("Array name is %s\n",array_name);

    st_entry* array_st_entry = getSymbolEntry(symbol_table,array_name);
    id_type *array_st_entry_type = array_st_entry->idType;



    st_entry* real_index_temp;

    printf("\n\nHere\n\n");
    fflush(stdout);

    if(strcmp(((ast_node*)(node->children[1]))->type,ast_list[BOUND_AST])==0)
    {
        if(strcmp(((ast_node*)((ast_node*)(node->children[1]))->children[0])->type,ast_list[MINUS_AST])==0)
        {
            //char* negated_index_temp = generate_Quadruples_Sub("0",(char*)((ast_node*)((ast_node*)(node->children[1]))->children[1])->type,fp);

            quadruple* quad = (quadruple *)malloc(sizeof(quadruple));

            quad->op = three_code_list[SUB_3CODE];
            quad->arg1_type = NUM_QUAD;
            quad->arg1_num = 0;
            // quad->arg2_type = ST_ENTRY_QUAD;
            // char* variable_name = ((ast_node*)((ast_node*)(node->children[1]))->children[1])->type;
            // quad->arg2_st_entry = getSymbolEntry(symbol_table,variable_name);
            
            ast_node* variable = (ast_node*)((ast_node*)(node->children[1]))->children[1];

            if(strcmp(variable->lexeme->token,token_list[NUM])==0)
            {
                //printf("In NUM\n");
                quad->arg2_type = NUM_QUAD;
                quad->arg2_num = variable->lexeme->value_if_num;
            }
            else if(strcmp(variable->lexeme->token,token_list[RNUM])==0)
            {
                //printf("In RNUM\n");
                quad->arg2_type = RNUM_QUAD;
                quad->arg2_rnum = variable->lexeme->value_if_rnum;
            }
            else
            {
                //printf("In ST_ENTRY\n");
                quad->arg2_type = ST_ENTRY_QUAD;
                char* variable_name = ((ast_node*)((ast_node*)(node->children[1]))->children[1])->type;
                //printf("Variable name is %s\n",variable_name);
                fflush(stdout);
                quad->arg2_st_entry = getSymbolEntry(symbol_table,variable_name);
            }
            
            quad->result = generate_temp(symbol_table);

            print_Quadruple_in_FILE(quad,symbol_table,ql,fp);

            //real_index_temp = generate_Quadruples_Sub(negated_index_temp,lower_bound,fp);

            quadruple* quad1 = (quadruple *)malloc(sizeof(quadruple));

            quad1->op = three_code_list[SUB_3CODE];
            quad1->arg1_type = ST_ENTRY_QUAD;
            quad1->arg1_st_entry = quad->result;

            if(array_st_entry_type->isDynamicLeft==1)
            {
                quad1->arg2_type = ST_ENTRY_QUAD;
                char* leftIndexString = array_st_entry_type->leftIndexName;
                char* left_bound_variable_name = (char*)malloc(sizeof(char)*strlen(leftIndexString));
                if(leftIndexString[0]=='-')
                {
                    int i=0;
                    for(i=1;i<strlen(leftIndexString);i++)
                    {
                        left_bound_variable_name[i-1] = leftIndexString[i];
                    }
                    left_bound_variable_name[i-1] = '\0';
                }

                st_entry* left_bound_variable_entry = getSymbolEntry(symbol_table,left_bound_variable_name);

                if(leftIndexString[0]=='-')
                {
                    quad->arg2_type = ST_ENTRY_QUAD;

                    quadruple* quad1 = (quadruple *)malloc(sizeof(quadruple));

                    quad1->op = three_code_list[SUB_3CODE];
                    quad1->arg1_type = NUM_QUAD;
                    quad1->arg1_num = 0;
                    quad1->arg2_type = ST_ENTRY_QUAD;
                    quad1->arg2_st_entry = left_bound_variable_entry;
                    quad1->result = generate_temp(symbol_table);

                    print_Quadruple_in_FILE(quad1,symbol_table,ql,fp);

                    quad->arg2_st_entry = quad1->result;
                }
                else
                {
                    quad->arg2_type = ST_ENTRY_QUAD;
                    quad->arg2_st_entry = left_bound_variable_entry;
                }

            }
            else
            {
                quad1->arg2_type = NUM_QUAD;
                quad1->arg2_num = array_st_entry_type->leftIndex;
            }
            quad1->result = generate_temp(symbol_table);

            print_Quadruple_in_FILE(quad1,symbol_table,ql,fp);

            real_index_temp = quad1->result;

        }
        else
        {
            //real_index_temp = generate_Quadruples_Sub((char*)((ast_node*)((ast_node*)(node->children[1]))->children[1])->type,lower_bound,fp);

            quadruple* quad = (quadruple *)malloc(sizeof(quadruple));

            quad->op = three_code_list[SUB_3CODE];

            ast_node* variable = (ast_node*)((ast_node*)(node->children[1]))->children[1];

            if(strcmp(variable->lexeme->token,token_list[NUM])==0)
            {
                //printf("In NUM\n");
                quad->arg1_type = NUM_QUAD;
                quad->arg1_num = variable->lexeme->value_if_num;
            }
            else if(strcmp(variable->lexeme->token,token_list[RNUM])==0)
            {
                //printf("In RNUM\n");
                quad->arg1_type = RNUM_QUAD;
                quad->arg1_rnum = variable->lexeme->value_if_rnum;
            }
            else
            {
                //printf("In ST_ENTRY\n");
                quad->arg1_type = ST_ENTRY_QUAD;
                char* variable_name = ((ast_node*)((ast_node*)(node->children[1]))->children[1])->type;
                //printf("Variable name is %s\n",variable_name);
                fflush(stdout);
                quad->arg1_st_entry = getSymbolEntry(symbol_table,variable_name);
            }

            // quad->arg1_type = ST_ENTRY_QUAD;
            // char* variable_name = ((ast_node*)((ast_node*)(node->children[1]))->children[1])->type;
            // quad->arg1_st_entry = getSymbolEntry(symbol_table,variable_name);

            if(array_st_entry_type->isDynamicLeft==1)
            {
                quad->arg2_type = ST_ENTRY_QUAD;
                char* leftIndexString = array_st_entry_type->leftIndexName;
                char* left_bound_variable_name = (char*)malloc(sizeof(char)*strlen(leftIndexString));
                if(leftIndexString[0]=='-')
                {
                    int i=0;
                    for(i=1;i<strlen(leftIndexString);i++)
                    {
                        left_bound_variable_name[i-1] = leftIndexString[i];
                    }
                    left_bound_variable_name[i-1] = '\0';
                }

                st_entry* left_bound_variable_entry = getSymbolEntry(symbol_table,left_bound_variable_name);

                if(leftIndexString[0]=='-')
                {
                    quad->arg2_type = ST_ENTRY_QUAD;

                    quadruple* quad1 = (quadruple *)malloc(sizeof(quadruple));

                    quad1->op = three_code_list[SUB_3CODE];
                    quad1->arg1_type = NUM_QUAD;
                    quad1->arg1_num = 0;
                    quad1->arg2_type = ST_ENTRY_QUAD;
                    quad1->arg2_st_entry = left_bound_variable_entry;
                    quad1->result = generate_temp(symbol_table);

                    print_Quadruple_in_FILE(quad1,symbol_table,ql,fp);

                    quad->arg2_st_entry = quad1->result;
                }
                else
                {
                    quad->arg2_type = ST_ENTRY_QUAD;
                    quad->arg2_st_entry = left_bound_variable_entry;
                }

            }
            else
            {
                quad->arg2_type = NUM_QUAD;
                quad->arg2_num = array_st_entry_type->leftIndex;
            }
            quad->result = generate_temp(symbol_table);

            real_index_temp = quad->result;

            print_Quadruple_in_FILE(quad,symbol_table,ql,fp);
        }
    }   
    else
    {
        //real_index_temp = generate_Quadruples_Sub((char*)((ast_node*)(node->children[1]))->type,lower_bound,fp);

        quadruple* quad = (quadruple *)malloc(sizeof(quadruple));

        quad->op = three_code_list[SUB_3CODE];
        //quad->arg1_type = ST_ENTRY_QUAD;
        ast_node* variable = (ast_node*)(node->children[1]);

        //printf("Variable lexeme is %s\n",variable->lexeme->lexeme);
        //printf("Variable token is %s\n",variable->lexeme->token);

        if(strcmp(variable->lexeme->token,token_list[NUM])==0)
        {
            //printf("In NUM\n");
            quad->arg1_type = NUM_QUAD;
            quad->arg1_num = variable->lexeme->value_if_num;
        }
        else if(strcmp(variable->lexeme->token,token_list[RNUM])==0)
        {
            //printf("In RNUM\n");
            quad->arg1_type = RNUM_QUAD;
            quad->arg1_rnum = variable->lexeme->value_if_rnum;
        }
        else
        {
            //printf("In ST_ENTRY\n");
            quad->arg1_type = ST_ENTRY_QUAD;
            char* variable_name = ((ast_node*)(node->children[1]))->type;
            //printf("Variable name is %s\n",variable_name);
            fflush(stdout);
            quad->arg1_st_entry = getSymbolEntry(symbol_table,variable_name);
        }

        // char* variable_name = ((ast_node*)(node->children[1]))->type;
        // printf("Variable name is %s\n",variable_name);
        // quad->arg1_st_entry = getSymbolEntry(symbol_table,variable_name);

        if(array_st_entry_type->isDynamicLeft==1)
        {
            quad->arg2_type = ST_ENTRY_QUAD;
            char* leftIndexString = array_st_entry_type->leftIndexName;
                char* left_bound_variable_name = (char*)malloc(sizeof(char)*strlen(leftIndexString));
                if(leftIndexString[0]=='-')
                {
                    int i=0;
                    for(i=1;i<strlen(leftIndexString);i++)
                    {
                        left_bound_variable_name[i-1] = leftIndexString[i];
                    }
                    left_bound_variable_name[i-1] = '\0';
                }

                st_entry* left_bound_variable_entry = getSymbolEntry(symbol_table,left_bound_variable_name);

                if(leftIndexString[0]=='-')
                {
                    quad->arg2_type = ST_ENTRY_QUAD;

                    quadruple* quad1 = (quadruple *)malloc(sizeof(quadruple));

                    quad1->op = three_code_list[SUB_3CODE];
                    quad1->arg1_type = NUM_QUAD;
                    quad1->arg1_num = 0;
                    quad1->arg2_type = ST_ENTRY_QUAD;
                    quad1->arg2_st_entry = left_bound_variable_entry;
                    quad1->result = generate_temp(symbol_table);

                    print_Quadruple_in_FILE(quad1,symbol_table,ql,fp);

                    quad->arg2_st_entry = quad1->result;
                }
                else
                {
                    quad->arg2_type = ST_ENTRY_QUAD;
                    quad->arg2_st_entry = left_bound_variable_entry;
                }
        }
        else
        {
            quad->arg2_type = NUM_QUAD;
            quad->arg2_num = array_st_entry_type->leftIndex;
        }
        quad->result = generate_temp(symbol_table);

        //printf("In array access2\n");
        //fflush(stdout);
        print_Quadruple_in_FILE(quad,symbol_table,ql,fp);

        //printf("In array access3\n");
        //fflush(stdout);

        real_index_temp = quad->result;

    }

    //char* real_offset_temp = generate_Quadruples_Mul(real_index_temp,"4",fp);

    quadruple* quad = (quadruple *)malloc(sizeof(quadruple));

    quad->op = three_code_list[MUL_3CODE];
    quad->arg1_type = ST_ENTRY_QUAD;
    quad->arg1_st_entry = real_index_temp;
    quad->arg2_type = NUM_QUAD;
    quad->arg2_num = getTypeWidth(array_st_entry_type->type);
    quad->result = generate_temp(symbol_table);

    print_Quadruple_in_FILE(quad,symbol_table,ql,fp);

        // quad->op = three_code_list[ARRAY_ACCESS_3CODE];
        // quad->arg1 = (char*)((ast_node*)(node->children[0]))->type;
        // quad->arg2 = real_offset_temp;
        // quad->result = generate_temp();
    quadruple* quad1 = (quadruple *)malloc(sizeof(quadruple));

    quad1->op = three_code_list[ARRAY_ACCESS_3CODE];
    quad1->arg1_type = ST_ENTRY_QUAD;
    quad1->arg1_st_entry = array_st_entry;
    quad1->arg2_type = ST_ENTRY_QUAD;
    quad1->arg2_st_entry = quad->result;
    quad1->result = generate_temp(symbol_table);    

    print_Quadruple_in_FILE(quad1,symbol_table,ql,fp);

    return quad1->result;

}

void generate_Quadruples_Print_Statement(ast_node *node,st_table* symbol_table,quadrupleList* ql,FILE* fp)
{
    //return;
    //printf("Quad Print Called\n");
    if(node == NULL)
    {
        return;
    }

    //printf("Passed NULL Check\n");

    quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
    quad->op = three_code_list[PRINT_3CODE];


    ast_node * child = (ast_node*)node->children[0];

    // if(strcmp(child->type,ast_list[ARRAY_ELEM_OR_ID_AST]) == 0)
    // {
    //     if(child->children[1] == NULL)
    //     {
    //         quad->arg1 = ((ast_node*)(child->children[0]))->type;
    //         quad->arg2 = NULL;
    //         quad->result = NULL;
    //     }
    //     else
    //     {
    //         char *temp = generate_Quadruples_Print_Array_Element_Access(child,fp);
    //         quad->arg1 = temp;
    //         quad->arg2 = NULL;
    //         quad->result = NULL;
    //     }

    // }
    // else if((strcmp(child->type,ast_list[TRUE_AST])==0) || (strcmp(child->type,ast_list[FALSE_AST])==0))
    // {
    //     quad->arg1 = child->type;
    //     quad->arg2 = NULL;
    //     quad->result = NULL;
    // }
    // else if(strcmp(child->type,ast_list[NUM_AST])==0)
    // {
    //     quad->arg1 = ((ast_node*)(child->children[0]))->type;
    //     quad->arg2 = NULL;
    //     quad->result = NULL;
    // }
    // else if(strcmp(child->type,ast_list[RNUM_AST])==0)
    // {
    //     quad->arg1 = ((ast_node*)(child->children[0]))->type;
    //     quad->arg2 = NULL;
    //     quad->result = NULL;
    // }
    if(strcmp(child->type,ast_list[ARRAY_ELEM_OR_ID_AST]) == 0)
    {
        if(child->children[1] == NULL)
        {
            quad->arg1_type = ST_ENTRY_QUAD;
            quad->arg1_st_entry = getSymbolEntry(symbol_table,((ast_node*)(child->children[0]))->type);
            quad->arg2_type = NULL_QUAD;
            quad->result = NULL;
        }
        else
        {
            //printf("Array Element Access\n");
            fflush(stdout);
            st_entry* temp = generate_Quadruples_Print_Array_Element_Access(child,symbol_table,ql,fp);
            quad->arg1_type = ST_ENTRY_QUAD;
            quad->arg1_st_entry = temp;
            quad->arg2_type = NULL_QUAD;
            quad->result = NULL;
        }

    }
    else if((strcmp(child->type,ast_list[TRUE_AST])==0) || (strcmp(child->type,ast_list[FALSE_AST])==0))
    {
        quad->arg1_type = BOOL_QUAD;
        quad->arg1_bool = (strcmp(child->type,ast_list[TRUE_AST])==0)?1:0;
        quad->arg2_type = NULL_QUAD;
        quad->result = NULL;
    }
    else if(strcmp(child->type,ast_list[NUM_AST])==0)
    {
        quad->arg1_type = NUM_QUAD;
        quad->arg1_num = (((ast_node*)(child->children[0]))->lexeme)->value_if_num;
        quad->arg2_type = NULL_QUAD;
        quad->result = NULL;
    }
    else if(strcmp(child->type,ast_list[RNUM_AST])==0)
    {
        quad->arg1_type = RNUM_QUAD;
        quad->arg1_num = (((ast_node*)(child->children[0]))->lexeme)->value_if_rnum;
        quad->arg2_type = NULL_QUAD;
        quad->result = NULL;
    }
    //printf("Quad Print Called\n");
    fflush(stdout);
    print_Quadruple_in_FILE(quad,symbol_table,ql,fp);
    //printf("Quad Print Successful\n");
    fflush(stdout);
}

void generate_Quadruples_Get_Value(ast_node * node,st_table* symbol_table,quadrupleList* ql,FILE* fp)
{
    if(node == NULL)
    {
        return;
    }

    // quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
    // quad->op = three_code_list[GET_VALUE_3CODE];
    // quad->arg1 = ((ast_node*)(node->children[0]))->type;
    // quad->arg2 = NULL;
    // quad->result = NULL;

    quadruple *quad = (quadruple *)malloc(sizeof(quadruple));

    quad->op = three_code_list[GET_VALUE_3CODE];
    quad->arg1_type = ST_ENTRY_QUAD;
    quad->arg1_st_entry = getSymbolEntry(symbol_table,((ast_node*)(node->children[0]))->type);
    quad->arg2_type = NULL_QUAD;
    quad->result = NULL;

    print_Quadruple_in_FILE(quad,symbol_table,ql,fp);
}

/*void generate_Quadruples_Module(ast_node * node,st_table* symbol_table,quadrupleList* ql,FILE* fp)
{
    if(node == NULL)
    {
        return;
    }

    quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
    quad->op = three_code_list[MODULE_3CODE];
    quad->arg1 = ((ast_node*)(node->children[0]))->type;
    quad->arg2 = NULL;
    quad->result = NULL;

    print_Quadruple_in_FILE(quad,fp);
}



void generate_Quadruples_Driver(ast_node * node,st_table* symbol_table,quadrupleList* ql,FILE* fp)
{
    if(node == NULL)
    {
        return;
    }

    quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
    quad->op = three_code_list[MODULE_3CODE];
    quad->arg1 = three_code_list[DRIVER_3CODE];
    quad->arg2 = NULL;
    quad->result = NULL;

    print_Quadruple_in_FILE(quad,fp);
}
*/

void generate_Quadruples_Module(ast_node * node,st_table* symbol_table,quadrupleList* ql,FILE* fp)
{
    if(node == NULL)
    {
        return;
    }

    quadruple *quad = (quadruple *)malloc(sizeof(quadruple));

    quad->op = three_code_list[MODULE_3CODE];
    quad->arg1_type = ST_ENTRY_QUAD;
    quad->arg1_st_entry = getSymbolEntry(symbol_table,((ast_node*)(node->children[0]))->type);
    quad->arg2_type = NULL_QUAD;
    quad->result = NULL;

    print_Quadruple_in_FILE(quad,symbol_table,ql,fp);
}

void generate_Quadruples_Driver(ast_node * node,st_table* symbol_table,quadrupleList* ql,FILE* fp)
{
    if(node == NULL)
    {
        return;
    }

    quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
    quad->op = three_code_list[MODULE_3CODE];
    quad->arg1_type = LABEL_QUAD;
    quad->arg1_label = three_code_list[DRIVER_3CODE];
    quad->arg2_type = NULL_QUAD;
    quad->result = NULL;

    print_Quadruple_in_FILE(quad,symbol_table,ql,fp);
}

void generate_Quadruples_GOTO(char* label,st_table* symbol_table,quadrupleList* ql,FILE* fp)
{
    if(label == NULL)
    {
        return;
    }

    quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
    quad->op = three_code_list[GOTO_3CODE];
    quad->arg1_type = LABEL_QUAD;
    quad->arg1_label = label;
    quad->arg2_type = NULL_QUAD;
    quad->result = NULL;

    print_Quadruple_in_FILE(quad,symbol_table,ql,fp);
}

char* generate_Label()
{
    label_count++;
    char* label = (char*)malloc(sizeof(char)*10);
    sprintf(label,"Label_%d",label_count);
    return label;
}

// void generate_Quadruples_Assignment(char* arg1,char* arg2,st_table* symbol_table,quadrupleList* ql,FILE* fp)
// {
//     quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
//     quad->op = three_code_list[ASSIGN_3CODE];
//     quad->arg1_st_entry = arg1;
//     quad->arg2_type = NULL_QUAD;
//     quad->result = arg2;
//
//     print_Quadruple_in_FILE(quad,fp);
// }
    
void generate_Quadruples_Label(char* label,st_table* symbol_table,quadrupleList* ql,FILE* fp)
{
    if(label == NULL)
    {
        return;
    }

    quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
    quad->op = three_code_list[LABEL_3CODE];
    quad->arg1_type = LABEL_QUAD;
    quad->arg1_label = label;
    quad->arg2_type = NULL_QUAD;
    quad->result = NULL;

    print_Quadruple_in_FILE(quad,symbol_table,ql,fp);
}

int check_if_Default_In_Switch(ast_node * node)
{
    if(node->children[2]==NULL)
    {
        return 0;
    }
    return 1;
}

int get_Number_Of_Cases_In_Switch(ast_node * node)
{
    int count;

    count=((ast_node*)(node->children[1]))->numChildren;

    return count;
}




/*
void generate_Quadruples_If_Equal(char* arg1,char* arg2,char* label_if_true,st_table* symbol_table,quadrupleList* ql,FILE* fp)
{
    if(arg1 == NULL || arg2 == NULL || label_if_true == NULL)
    {
        return;
    }

    char* relational_result = generate_Quadruples_Equal(arg1,arg2,fp);

    generate_Quadruples_Jump_If_True(relational_result,label_if_true,fp);
    //generate_Quadruples_Jump_If_Not_True(relational_result,label_after_if,fp);

}

*/

void generate_Quadruples_Switch(ast_node * node,st_table* symbol_table,quadrupleList* ql,FILE* fp)
{
    if(node == NULL)
    {
        return;
    }

    // if(node->symbolTable!=NULL)
    // {
    //     printf
    // }
    int num_cases = get_Number_Of_Cases_In_Switch(node);
    int default_present = check_if_Default_In_Switch(node);

    char** case_labels = (char**)malloc(sizeof(char*)*num_cases);
    //char** case_if_labels = (char**)malloc(sizeof(char*)*num_cases);


    ast_node* switch_value = (ast_node*)(node->children[0]);
    ast_node* case_list = (ast_node*)(node->children[1]);
    ast_node* default_node = (ast_node*)(node->children[2]);
    char* switch_value_string = switch_value->type;

    st_entry* switch_value_entry = getSymbolEntry(symbol_table,switch_value_string);

    //quadruple *quad = (quadruple *)malloc(sizeof(quadruple));


    char* switch_next_label = generate_Label();
    char* test_value = generate_Label();

    generate_Quadruples_GOTO(test_value,symbol_table,ql,fp);

    for(int i=0;i<num_cases;i++)
    {
        ast_node* case_node = (ast_node*)(case_list->children[i]);
        ast_node* case_statements = (ast_node*)(case_node->children[1]);
        case_labels[i] = generate_Label();
        generate_Quadruples_Label(case_labels[i],symbol_table,ql,fp);
        printf("Statements in case -%d-\n",i);

        generate_Quadruples_Statements_List(case_statements,case_node->symbolTable,ql,fp);
        // ast_node* case_list = (ast_node*)(node->children[1]);
        // ast_node* case_node = (ast_node*)(case_list->children[i]);
        // ast_node* case_value = (ast_node*)(case_node->children[0]);
        // char* case_value_string = case_value->type;
        // generate_Quadruples_If_Equal(switch_value_string,case_value_string,case_labels[i],test_value,fp);

        generate_Quadruples_GOTO(switch_next_label,symbol_table,ql,fp);

    }
    char* default_label;

    if(default_node!=NULL)
    {
        default_label = generate_Label();
        generate_Quadruples_Label(default_label,symbol_table,ql,fp);
        ast_node* default_statements = (ast_node*)(default_node->children[0]);
        printf("Statements in default\n");
        generate_Quadruples_Statements_List(default_statements,default_node->symbolTable,ql,fp);

        generate_Quadruples_GOTO(switch_next_label,symbol_table,ql,fp);
    }

    generate_Quadruples_Label(test_value,symbol_table,ql,fp);

    //ast_node* case_list = (ast_node*)(node->children[1]);
    //
    // for(int i=0;i<num_cases;i++)
    // {
    //     case_if_labels[i] = generate_Label();
    // }

    for(int i=0;i<num_cases;i++)
    {

        
        ast_node* case_node = (ast_node*)(case_list->children[i]);
        ast_node* case_value = (ast_node*)(case_node->children[0]);
        char* case_value_type = case_value->lexeme->token;

        st_entry* case_value_entry;

        if(strcmp(case_value_type,token_list[NUM])==0)
        {
            quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
            quad->op = three_code_list[EQUAL_3CODE];
            quad->arg1_type = NUM_QUAD;
            quad->arg1_num = case_value->lexeme->value_if_num;
            quad->arg2_type = NULL_QUAD;
            quad->result = generate_temp(symbol_table);
            
            print_Quadruple_in_FILE(quad,symbol_table,ql,fp);

            case_value_entry = quad->result;

        }
        else if(strcmp(case_value_type,keyword_list[TRUE])==0)
        {
            quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
            quad->op = three_code_list[EQUAL_3CODE];
            quad->arg1_type = BOOL_QUAD;
            quad->arg1_bool = 1;
            quad->arg2_type = NULL_QUAD;
            quad->result = generate_temp(symbol_table);

            print_Quadruple_in_FILE(quad,symbol_table,ql,fp);

            case_value_entry = quad->result;
        }
        else if(strcmp(case_value_type,keyword_list[FALSE])==0)
        {
            quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
            quad->op = three_code_list[EQUAL_3CODE];
            quad->arg1_type = BOOL_QUAD;
            quad->arg1_bool = 0;
            quad->arg2_type = NULL_QUAD;
            quad->result = generate_temp(symbol_table);

            print_Quadruple_in_FILE(quad,symbol_table,ql,fp);

            case_value_entry = quad->result;
        }



        //generate_Quadruples_If_Equal(switch_value_string,case_value_string,case_labels[i],fp);

        quadruple *quad = (quadruple *)malloc(sizeof(quadruple));

        quad->op = three_code_list[EQUAL_3CODE];
        quad->arg1_type = ST_ENTRY_QUAD;
        quad->arg1_st_entry = switch_value_entry;
        quad->arg2_type = ST_ENTRY_QUAD;
        quad->arg2_st_entry = case_value_entry;
        quad->result = generate_temp(symbol_table);

        print_Quadruple_in_FILE(quad,symbol_table,ql,fp);

        quadruple *quad2 = (quadruple *)malloc(sizeof(quadruple));

        quad2->op = three_code_list[JUMP_IF_TRUE_3CODE];
        quad2->arg1_type = ST_ENTRY_QUAD;
        quad2->arg1_st_entry = quad->result;
        quad2->arg2_type = LABEL_QUAD;
        quad2->arg2_label = case_labels[i];
        quad2->result = NULL;

        print_Quadruple_in_FILE(quad2,symbol_table,ql,fp);

        //generate_Quadruples_Label(case_if_labels[i],fp);

    }
    if(default_node!=NULL)
    {
        generate_Quadruples_GOTO(default_label,symbol_table,ql,fp);
    }

    generate_Quadruples_Label(switch_next_label,symbol_table,ql,fp);

}



void generate_Quadruples_For(ast_node * node,st_table* symbol_table,quadrupleList* ql,FILE* fp)
{

    printf("For loop\n");
    fflush(stdout);

    if(node == NULL)
    {
        return;
    }

    // printf("For loop\n");
    // if(node->symbolTable!=NULL)
    // {
    //     printf("Symbol table present\n");
    // }else
    // {
    //     printf("Symbol table not present\n");
    // }
    // fflush(stdout);

    symbol_table = node->symbolTable;

    ast_node* for_value = (ast_node*)(node->children[0]);
    //char* for_value_string = for_value->type;
    printf("For value -%s-\n",for_value->type);
    st_entry* for_variable_entry = getSymbolEntry(symbol_table,for_value->type);
    if(for_variable_entry==NULL)
    {
        printf("Error: Variable not declared\n");
        //exit(1);
    }
    else
    {
        printf("For variable entry present\n");
    }

    ast_node* for_range = (ast_node*)(node->children[1]);

    ast_node* for_range_start = (ast_node*)(for_range->children[0]);

    ast_node* for_range_end = (ast_node*)(for_range->children[1]);

    //char* for_range_start_string;
    int for_range_start_type;
    int for_range_end_type;
    int for_range_start_int;
    int for_range_end_int;
    st_entry* for_range_start_entry;
    st_entry* for_range_end_entry;

    if(for_range_start->children[0]==NULL)
    {
        char* for_range_start_token = ((ast_node*)(for_range_start->children[1]))->lexeme->token;

        //printf("For range start token -%s-\n",for_range_start_token);

        if(strcmp(for_range_start_token,token_list[NUM])==0)
        {
            //printf("For range start is a number\n");
            for_range_start_type = 0;
            for_range_start_int = ((ast_node*)(for_range_start->children[1]))->lexeme->value_if_num;
        }
        else
        {
            for_range_start_entry = getSymbolEntry(symbol_table,((ast_node*)(for_range_start->children[1]))->type);
            for_range_start_type = 1;
        }
    }
    else
    {
        char* for_range_start_token = ((ast_node*)(for_range_start->children[1]))->lexeme->token;

        if(strcmp(for_range_start_token,token_list[NUM])==0)
        {
            for_range_start_type = 0;
            for_range_start_int = ((ast_node*)(for_range_start->children[1]))->lexeme->value_if_num;
        }
        else
        {
            for_range_start_entry = getSymbolEntry(symbol_table,((ast_node*)(for_range_start->children[1]))->type);
            for_range_start_type = 1;
        }


        if(strcmp(((ast_node*)(for_range_start->children[0]))->type,ast_list[MINUS_AST])==0)
        {
            //for_range_start_string = generate_Quadruples_Sub("0",for_range_start_string,fp);

            quadruple* quad = (quadruple*)malloc(sizeof(quadruple));

            quad->op = three_code_list[SUB_3CODE];
            quad->arg1_type = NUM_QUAD;
            quad->arg1_num = 0;
            if(for_range_start_type==0)
            {
                quad->arg2_type = NUM_QUAD;
                quad->arg2_num = for_range_start_int;
            }
            else
            {
                quad->arg2_type = ST_ENTRY_QUAD;
                quad->arg2_st_entry = for_range_start_entry;
            }
            quad->result = generate_temp(symbol_table);

            print_Quadruple_in_FILE(quad,symbol_table,ql,fp);

            //? IDHAR YE KARNA HAI??
            for_range_start_type = 1;
            for_range_start_entry = quad->result;

        }
    }

    if(for_range_end->children[0]==NULL)
    {
        //for_range_end_string = ((ast_node*)(for_range_end->children[1]))->type;
        char* for_range_end_token = ((ast_node*)(for_range_end->children[1]))->lexeme->token;

        if(strcmp(for_range_end_token,token_list[NUM])==0)
        {
            for_range_end_type = 0;
            for_range_end_int = ((ast_node*)(for_range_end->children[1]))->lexeme->value_if_num;
        }
        else
        {
            for_range_end_entry = getSymbolEntry(symbol_table,((ast_node*)(for_range_end->children[1]))->type);
            for_range_end_type = 1;
        }
    }
    else
    {
        //for_range_end_string = ((ast_node*)(for_range_end->children[1]))->type;

        char* for_range_end_token = ((ast_node*)(for_range_end->children[1]))->lexeme->token;

        if(strcmp(for_range_end_token,token_list[NUM])==0)
        {
            for_range_end_type = 0;
            for_range_end_int = ((ast_node*)(for_range_end->children[1]))->lexeme->value_if_num;
        }
        else
        {
            for_range_end_entry = getSymbolEntry(symbol_table,((ast_node*)(for_range_end->children[1]))->type);
            for_range_end_type = 1;
        }

        if(strcmp(((ast_node*)(for_range_end->children[0]))->type,ast_list[MINUS_AST])==0)
        {
            //for_range_end_string = generate_Quadruples_Sub("0",for_range_end_string,fp);

            quadruple* quad = (quadruple*)malloc(sizeof(quadruple));

            quad->op = three_code_list[SUB_3CODE];
            quad->arg1_type = NUM_QUAD;
            quad->arg1_num = 0;
            if(for_range_end_type==0)
            {
                quad->arg2_type = NUM_QUAD;
                quad->arg2_num = for_range_end_int;
            }
            else
            {
                quad->arg2_type = ST_ENTRY_QUAD;
                quad->arg2_st_entry = for_range_end_entry;
            }
            quad->result = generate_temp(symbol_table);

            print_Quadruple_in_FILE(quad,symbol_table,ql,fp);

            for_range_end_type = 1;
            for_range_end_entry = quad->result;
        }
    }

    //printf("For loop 2\n");
    fflush(stdout);

    char* for_start_label = generate_Label();

    char* for_condition_label = generate_Label();

    //generate_Quadruples_Assignment(for_value_string,for_range_start_string,fp);

    quadruple* quad = (quadruple*)malloc(sizeof(quadruple));

    quad->op = three_code_list[ASSIGN_3CODE];

    //printf("%d wvbk\n",for_range_start_type);
    if(for_range_start_type==0)
    {
        // printf("herein\n");
        quad->arg1_type = NUM_QUAD;
        quad->arg1_num = for_range_start_int;
    }
    else
    {
        //printf("herein\n");
        quad->arg1_type = ST_ENTRY_QUAD;
        quad->arg1_st_entry = for_range_start_entry;
    }
    // quad->arg1_type = NUM_QUAD;
    // quad->arg1_num = 0;
    quad->arg2_type = NULL_QUAD;
    quad->result = for_variable_entry;
    print_Quadruple_in_FILE(quad,symbol_table,ql,fp);

    //printf("For loop 3\n");
    fflush(stdout);

    generate_Quadruples_GOTO(for_condition_label,symbol_table, ql,fp);

    generate_Quadruples_Label(for_start_label,symbol_table, ql,fp);

    printf("Statements in for loop\n");

    generate_Quadruples_Statements_List(node->children[2],symbol_table, ql,fp);



    generate_Quadruples_Label(for_condition_label,symbol_table, ql,fp);


    //char* relational_result = generate_Quadruples_Less_Than_Equal(for_value_string,for_range_end_string,fp);

    printf("For_range_end_type = %d\n",for_range_end_type);
    printf("For_range_end_int = %d\n",for_range_end_int);
    fflush(stdout);

    quadruple* quad2 = (quadruple*)malloc(sizeof(quadruple));

    quad2->op = three_code_list[LESS_THAN_EQUAL_3CODE];
    quad2->arg1_type = ST_ENTRY_QUAD;
    quad2->arg1_st_entry = for_variable_entry;
    if(for_range_end_type==0)
    {
        printf("herein\n");
        quad2->arg2_type = NUM_QUAD;
        quad2->arg2_num = for_range_end_int;
    }
    else
    {
        quad2->arg2_type = ST_ENTRY_QUAD;
        quad2->arg2_st_entry = for_range_end_entry;
    }
    quad2->result = generate_temp(symbol_table);
    print_Quadruple_in_FILE(quad2,symbol_table,ql,fp);
    
    printf("For loop 4\n");
    fflush(stdout);
    //generate_Quadruples_Jump_If_True(relational_result,for_start_label,fp);

    quadruple* quad1 = (quadruple*)malloc(sizeof(quadruple));

    quad1->op = three_code_list[JUMP_IF_TRUE_3CODE];
    quad1->arg1_type = ST_ENTRY_QUAD;
    quad1->arg1_st_entry = quad->result;
    quad1->arg2_type = LABEL_QUAD;
    quad1->arg2_label = for_start_label;
    quad1->result = NULL;

    print_Quadruple_in_FILE(quad1,symbol_table,ql,fp);




    //generate_Quadruples_Label(for_end_label,fp);


}


// void generate_Quadruples_Input_List_Parameter(st_entry* input_list_element,st_table* symbol_table,quadrupleList* ql,FILE* fp)
// {
//     quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
//
//     quad->op = three_code_list[INPUT_PARAM_3CODE];
//     quad->arg1_type = ST_ENTRY_QUAD;
//     quad->arg1_st_entry = input_list_element;
//     quad->arg2_type = NULL_QUAD;
//     quad->result = NULL;
//
//     print_Quadruple_in_FILE(quad,symbol_table,ql,fp);
//
// }

// void generate_Quadruples_Output_List_Parameter(st_entry* output_list_element,st_table* symbol_table,quadrupleList* ql,FILE* fp)
// {
//     quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
//
//     quad->op = three_code_list[OUTPUT_PARAM_3CODE];
//     quad->arg1_type = ST_ENTRY_QUAD;
//     quad->arg1_st_entry = output_list_element;
//     quad->arg2_type = NULL_QUAD;
//     quad->result = NULL;
//
//     print_Quadruple_in_FILE(quad,symbol_table,ql,fp);
//
// }

// void generate_Quadruples_Call_Module(st_entry* function_name,st_table* symbol_table,quadrupleList* ql,FILE* fp)
// {
//     quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
//
//     quad->op = three_code_list[MODULE_CALL_3CODE];
//     quad->arg1_type = ST_ENTRY_QUAD;
//     quad->arg1_st_entry = function_name;
//     quad->arg2_type = NULL_QUAD;
//     quad->result = NULL;
//
//     print_Quadruple_in_FILE(quad,symbol_table,ql,fp);
//
// }

void generate_Quadruples_Module_Reuse_Statement(ast_node * node,st_table* symbol_table,quadrupleList* ql,FILE* fp)
{
    if(node == NULL)
    {
        return;
    }

    ast_node* output_list = (ast_node*)(node->children[0]);
    ast_node* module_node = (ast_node*)(node->children[1]);
    ast_node* input_list = (ast_node*)(node->children[2]);

    int num_inputs = input_list->numChildren;

    st_entry** input_list_params = (st_entry**)malloc(sizeof(st_entry*)*num_inputs);

    for(int i=0;i<num_inputs;i++)
    {
        ast_node* parameter_variable = (ast_node*)(input_list->children[i]);

        ast_node* parameter_variable_value ;

        st_entry *before_sign_variable; 
        st_entry *after_sign_variable;
            
        st_entry *final_varible;

        if(parameter_variable->numChildren == 2)
        {
            parameter_variable_value = (ast_node*)(parameter_variable->children[1]);
        }
        else
        {
            parameter_variable_value = (ast_node*)(parameter_variable->children[0]);
        }

        if(strcmp(parameter_variable_value->type,ast_list[ARRAY_ELEM_OR_ID_AST]) == 0)
        {
            if(parameter_variable_value->children[1] == NULL)
            {
                //before_sign_variable = ((ast_node*)(parameter_variable_value->children[0]))->type;
                before_sign_variable = getSymbolEntry(symbol_table,((ast_node*)(parameter_variable_value->children[0]))->type);
            }
            else
            {
                st_entry *temp = generate_Quadruples_Print_Array_Element_Access(parameter_variable_value,symbol_table,ql,fp);
                before_sign_variable = temp;    
            }

        }
        else if((strcmp(parameter_variable_value->type,ast_list[TRUE_AST])==0) || (strcmp(parameter_variable_value->type,ast_list[FALSE_AST])==0))
        {

            // before_sign_variable = parameter_variable_value->type;
            quadruple *quad = (quadruple *)malloc(sizeof(quadruple));

            quad->op = three_code_list[ASSIGN_3CODE];
            quad->arg1_type = BOOL_QUAD;
            quad->arg1_bool = (strcmp(parameter_variable_value->type,ast_list[TRUE_AST])==0)?1:0;
            quad->arg2_type = NULL_QUAD;
            quad->result = generate_temp(symbol_table);

            print_Quadruple_in_FILE(quad,symbol_table,ql,fp);

            before_sign_variable = quad->result;
            
        }
        else if(strcmp(parameter_variable_value->type,ast_list[NUM_AST])==0)
        {
            //before_sign_variable = ((ast_node*)(parameter_variable_value->children[0]))->type;

            ast_node* num_node = (ast_node*)(parameter_variable_value->children[0]);

            quadruple *quad = (quadruple *)malloc(sizeof(quadruple));

            quad->op = three_code_list[ASSIGN_3CODE];

            quad->arg1_type = NUM_QUAD;
            quad->arg1_num = num_node->lexeme->value_if_num;
            quad->arg2_type = NULL_QUAD;
            quad->result = generate_temp(symbol_table);

            print_Quadruple_in_FILE(quad,symbol_table,ql,fp);

            before_sign_variable = quad->result;

            
        }
        else if(strcmp(parameter_variable_value->type,ast_list[RNUM_AST])==0)
        {
            ///before_sign_variable = ((ast_node*)(parameter_variable_value->children[0]))->type;
            
            ast_node* rnum_node = (ast_node*)(parameter_variable_value->children[0]);

            quadruple *quad = (quadruple *)malloc(sizeof(quadruple));

            quad->op = three_code_list[ASSIGN_3CODE];
            quad->arg1_type = RNUM_QUAD;
            quad->arg1_rnum = rnum_node->lexeme->value_if_rnum;
            quad->arg2_type = NULL_QUAD;
            quad->result = generate_temp(symbol_table);

            print_Quadruple_in_FILE(quad,symbol_table,ql,fp);

            before_sign_variable = quad->result;

        }

        if(parameter_variable->numChildren == 2 && strcmp(((ast_node*)(parameter_variable->children[0]))->type,ast_list[MINUS_AST])==0)
        {
            //after_sign_variable = generate_Quadruples_Sub("0",before_sign_variable,symbol_table,ql,fp);

            quadruple *quad = (quadruple *)malloc(sizeof(quadruple));

            quad->op = three_code_list[SUB_3CODE];
            quad->arg1_type = NUM_QUAD;
            quad->arg1_num = 0;
            quad->arg2_type = ST_ENTRY_QUAD;
            quad->arg2_st_entry = before_sign_variable;
            quad->result = generate_temp(symbol_table);

            print_Quadruple_in_FILE(quad,symbol_table,ql,fp);

            after_sign_variable = quad->result;

            final_varible = after_sign_variable;
        }
        else
        {
            final_varible = before_sign_variable;
        }

        input_list_params[i] = final_varible;
        //generate_Quadruples_Input_List_Parameter(final_varible,fp);

    }

    if(output_list == NULL)
    {

    }   
    else
    {
        int num_outputs = output_list->numChildren;

        for(int i=0;i<num_outputs;i++)
        {
            ast_node* output_value = (ast_node*)(output_list->children[i]);

            char* output_value_string = output_value->type;
            //generate_Quadruples_Output_List_Parameter(output_value_string,symbol_table,ql,fp);

            quadruple *quad = (quadruple *)malloc(sizeof(quadruple));

            quad->op = three_code_list[OUTPUT_PARAM_3CODE];
            quad->arg1_type = ST_ENTRY_QUAD;
            quad->arg1_st_entry = getSymbolEntry(symbol_table,output_value_string);
            quad->arg2_type = NULL_QUAD;
            quad->result = NULL;

            print_Quadruple_in_FILE(quad,symbol_table,ql,fp);

        }
    }

    for(int i=0;i<num_inputs;i++)
    {
        //generate_Quadruples_Input_List_Parameter(input_list_params[i],symbol_table,ql,fp);

        quadruple *quad = (quadruple *)malloc(sizeof(quadruple));

        quad->op = three_code_list[INPUT_PARAM_3CODE];
        quad->arg1_type = ST_ENTRY_QUAD;
        quad->arg1_st_entry = input_list_params[i];
        quad->arg2_type = NULL_QUAD;
        quad->result = NULL;

        print_Quadruple_in_FILE(quad,symbol_table,ql,fp);

    }

    //generate_Quadruples_Call_Module(module_node->type,symbol_table,ql,fp);

    quadruple *quad = (quadruple *)malloc(sizeof(quadruple));

    quad->op = three_code_list[MODULE_CALL_3CODE];
    quad->arg1_type = ST_ENTRY_QUAD;
    quad->arg1_st_entry = getSymbolEntry(symbol_table,module_node->type);
    quad->arg2_type = NULL_QUAD;
    quad->result = NULL;

    print_Quadruple_in_FILE(quad,symbol_table,ql,fp);

}

// void generate_Quadruples_Operator_Print(char* arg1,char* arg2,char* operator,char*result,st_table* symbol_table,quadrupleList* ql,FILE* fp)
// {
//     quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
//     quad->op = operator;
//     quad->arg1 = arg1;
//     quad->arg2 = arg2;
//     quad->result = result;
//
//     print_Quadruple_in_FILE(quad,fp);
// }

st_entry* generate_Quadruples_Op1_Op2_Token(ast_node * node,st_table* symbol_table,quadrupleList* ql,FILE* fp)
{
    if(node == NULL)
    {
        return NULL;
    }



    if(node->containsLexeme == 1)
    {
        st_entry* temp_variable = generate_temp(symbol_table);
        char* token = node->lexeme->token;
        if(strcmp(token,token_list[NUM])==0)
        {
            quadruple* quad = (quadruple*)malloc(sizeof(quadruple));

            quad->op = three_code_list[ASSIGN_3CODE];
            quad->arg1_type = NUM_QUAD;
            quad->arg1_num = node->lexeme->value_if_num;
            quad->arg2_type = NULL_QUAD;
            quad->result = temp_variable;

            print_Quadruple_in_FILE(quad,symbol_table,ql,fp);
        }
        else if(strcmp(token,token_list[RNUM])==0)
        {
            quadruple* quad = (quadruple*)malloc(sizeof(quadruple));

            quad->op = three_code_list[ASSIGN_3CODE];
            quad->arg1_type = RNUM_QUAD;
            quad->arg1_rnum = node->lexeme->value_if_rnum;
            quad->arg2_type = NULL_QUAD;
            quad->result = temp_variable;

            print_Quadruple_in_FILE(quad,symbol_table,ql,fp);

        }
        else if(strcmp(token,keyword_list[TRUE])==0)
        {

            quadruple* quad = (quadruple*)malloc(sizeof(quadruple));

            quad->op = three_code_list[ASSIGN_3CODE];
            quad->arg1_type = BOOL_QUAD;
            quad->arg1_bool = 1;
            quad->arg2_type = NULL_QUAD;
            quad->result = temp_variable;

            print_Quadruple_in_FILE(quad,symbol_table,ql,fp);
        }
        else if(strcmp(token,keyword_list[FALSE])==0)
        {
            quadruple* quad = (quadruple*)malloc(sizeof(quadruple));

            quad->op = three_code_list[ASSIGN_3CODE];
            quad->arg1_type = BOOL_QUAD;
            quad->arg1_bool = 0;
            quad->arg2_type = NULL_QUAD;
            quad->result = temp_variable;

            print_Quadruple_in_FILE(quad,symbol_table,ql,fp);
        }
        else
        {
            char* id = node->type;

            st_entry* var =  getSymbolEntry(symbol_table,id);

            quadruple* quad = (quadruple*)malloc(sizeof(quadruple));

            quad->op = three_code_list[ASSIGN_3CODE];
            quad->arg1_type = ST_ENTRY_QUAD;
            quad->arg1_st_entry = var;
            quad->arg2_type = NULL_QUAD;
            quad->result = temp_variable;

            print_Quadruple_in_FILE(quad,symbol_table,ql,fp);

        }
        return temp_variable;   
    }
    else if(strcmp(node->type,ast_list[OPERATION_1_AST])==0 || strcmp(node->type,ast_list[OPERATION_2_AST])==0)
    {
        st_entry* op1 = generate_Quadruples_Op1_Op2_Token((ast_node*)(node->children[0]),symbol_table,ql,fp);
        st_entry* op2 = generate_Quadruples_Op1_Op2_Token((ast_node*)(node->children[2]),symbol_table,ql,fp);
        char* token = ((ast_node*)(node->children[1]))->type;

        //char* result = generate_temp(symbol_table);

        //generate_Quadruples_Operator_Print(op1,op2,token,result,fp);
        
        quadruple* quad = (quadruple*)malloc(sizeof(quadruple));

        quad->op = token;
        quad->arg1_type = ST_ENTRY_QUAD;
        quad->arg1_st_entry = op1;
        quad->arg2_type = ST_ENTRY_QUAD;
        quad->arg2_st_entry = op2;
        quad->result = generate_temp(symbol_table);

        return quad->result;
    }

    return NULL;

}

// char* generate_Quadruples_With_Array_And_Index(char* array_name,char* index_value,st_table* symbol_table,quadrupleList* ql,FILE* fp)
// {
//     char* final_elem_addr  = generate_temp();
//
//     char* lower_bound = (char*)malloc(sizeof(char)*10);
//     lower_bound[0] = '-';
//     lower_bound[1] = '1';
//     lower_bound[2] = '\0';
//
//     char* real_index = generate_Quadruples_Sub(index_value,lower_bound,fp);
//
//     char* real_offset = generate_Quadruples_Mul(real_index,"4",fp);
//
//     quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
//     quad->op = three_code_list[ARRAY_ACCESS_3CODE];
//     quad->arg1 = array_name;
//     quad->arg2 = real_offset;
//     quad->result = final_elem_addr;
//
//     print_Quadruple_in_FILE(quad,fp);
//
//     return final_elem_addr;
//
// }

st_entry* generate_Quadruples_Assign_LHS_Array_Element_Access(ast_node* node,st_table* symbol_table,quadrupleList* ql,FILE* fp)
{

    // traverse_AST_print(node,0,1);
    // fflush(stdout);

    if(node == NULL)
    {
        return NULL;
    }

    //printf("Called\n");

    char* array_name = ((ast_node*)(node->children[0]))->type;
    ast_node* index_node = (ast_node*)(node->children[1]);

    //printf("Called4\n");

    st_entry* index_value;

    // if(index_node == NULL)
    // {
    //     printf("Error in generate_Quadruples_Assign_LHS_Array_Element_Access\n");
    //     fflush(stdout);
    // }
    // strcmp(index_node->type,ast_list[ARRAY_INDEX_AST]);
    // printf("Called5\n");
    //
    // printf(" r= %d\n",strcmp(index_node->type,ast_list[ARRAY_INDEX_AST]));
    // printf("Called3\n");
    // fflush(stdout);

    if(strcmp(index_node->type,ast_list[ARRAY_INDEX_AST])==0)
    {
        // printf("Called1\n");
        // fflush(stdout);
        index_value = generate_Quadruples_Op1_Op2_Token(index_node->children[1],symbol_table,ql,fp);   
    }
    else
    {
        // printf("Called2\n");
        // fflush(stdout);
        index_value = generate_Quadruples_Op1_Op2_Token(index_node,symbol_table,ql,fp);
    }

    if(strcmp(index_node->type,ast_list[ARRAY_INDEX_AST])==0)
    {
        ast_node* array_index_sign= (ast_node*)(index_node->children[0]);
        if(strcmp(array_index_sign->type,ast_list[MINUS_AST])==0)
        {
            //index_value= generate_Quadruples_Sub("0",index_value,fp);

            quadruple *quad = (quadruple *)malloc(sizeof(quadruple));

            quad->op = three_code_list[SUB_3CODE];
            quad->arg1_type = NUM_QUAD;
            quad->arg1_num = 0;
            quad->arg2_type = ST_ENTRY_QUAD;
            quad->arg2_st_entry = index_value;
            quad->result = generate_temp(symbol_table);

            print_Quadruple_in_FILE(quad,symbol_table,ql,fp);

            index_value = quad->result;

        }
    }

    //return generate_Quadruples_With_Array_And_Index(array_name,index_value,fp);    

    st_entry* array_st_entry = getSymbolEntry(symbol_table,array_name);
    id_type *array_st_entry_type = array_st_entry->idType;

    st_entry* real_index_temp;

    quadruple *quad = (quadruple *)malloc(sizeof(quadruple));

    quad->op = three_code_list[SUB_3CODE];
    quad->arg1_type = ST_ENTRY_QUAD;
    quad->arg1_st_entry = index_value;

    if(array_st_entry_type->isDynamicLeft==1)
    {
        quad->arg2_type = ST_ENTRY_QUAD;
        //quad->arg2_st_entry = getSymbolEntry(symbol_table,array_st_entry_type->leftIndexName);
        char* leftIndexString = array_st_entry_type->leftIndexName;
        char* left_bound_variable_name = (char*)malloc(sizeof(char)*strlen(leftIndexString));
        if(leftIndexString[0]=='-')
        {
            int i=0;
            for(i=1;i<strlen(leftIndexString);i++)
            {
                left_bound_variable_name[i-1] = leftIndexString[i];
            }
            left_bound_variable_name[i-1] = '\0';
        }

        st_entry* left_bound_variable_entry = getSymbolEntry(symbol_table,left_bound_variable_name);

        if(leftIndexString[0]=='-')
        {
            quad->arg2_type = ST_ENTRY_QUAD;

            quadruple* quad1 = (quadruple *)malloc(sizeof(quadruple));

            quad1->op = three_code_list[SUB_3CODE];
            quad1->arg1_type = NUM_QUAD;
            quad1->arg1_num = 0;
            quad1->arg2_type = ST_ENTRY_QUAD;
            quad1->arg2_st_entry = left_bound_variable_entry;
            quad1->result = generate_temp(symbol_table);

            print_Quadruple_in_FILE(quad1,symbol_table,ql,fp);

            quad->arg2_st_entry = quad1->result;
        }
        else
        {
            quad->arg2_type = ST_ENTRY_QUAD;
            quad->arg2_st_entry = left_bound_variable_entry;
        }
    }
    else
    {
        quad->arg2_type = NUM_QUAD;
        quad->arg2_num = array_st_entry_type->leftIndex;
    }
    quad->result = generate_temp(symbol_table);

    print_Quadruple_in_FILE(quad,symbol_table,ql,fp);

    real_index_temp = quad->result;

    quadruple* quad2 = (quadruple *)malloc(sizeof(quadruple));

    quad2->op = three_code_list[MUL_3CODE];
    quad2->arg1_type = ST_ENTRY_QUAD;
    quad2->arg1_st_entry = real_index_temp;
    quad2->arg2_type = NUM_QUAD;
    quad2->arg2_num =getTypeWidth(array_st_entry_type->type);
    quad2->result = generate_temp(symbol_table);

    print_Quadruple_in_FILE(quad2,symbol_table,ql,fp);


    quadruple* quad1 = (quadruple *)malloc(sizeof(quadruple));

    quad1->op = three_code_list[ARRAY_ACCESS_3CODE];
    quad1->arg1_type = ST_ENTRY_QUAD;
    quad1->arg1_st_entry = array_st_entry;
    quad1->arg2_type = ST_ENTRY_QUAD;
    quad1->arg2_st_entry = quad2->result;
    quad1->result = generate_temp(symbol_table);    

    print_Quadruple_in_FILE(quad1,symbol_table,ql,fp);

    return quad1->result;

}

st_entry* generate_Quadruples_ArithmeticOp(ast_node * node,st_table* symbol_table,quadrupleList* ql,FILE* fp)
{
    if(node == NULL)
    {
        return NULL;
    }

    st_entry* op1 = generate_Quadruples_Expression_Helper((ast_node*)(node->children[0]),symbol_table,ql,fp);
    st_entry* op2 = generate_Quadruples_Expression_Helper((ast_node*)(node->children[2]),symbol_table,ql,fp);
    char* token = ((ast_node*)(node->children[1]))->type;

    quadruple *quad = (quadruple *)malloc(sizeof(quadruple));

    quad->op = token;
    quad->arg1_type = ST_ENTRY_QUAD;
    quad->arg1_st_entry = op1;
    quad->arg2_type = ST_ENTRY_QUAD;
    quad->arg2_st_entry = op2;
    quad->result = generate_temp(symbol_table);

    print_Quadruple_in_FILE(quad,symbol_table,ql,fp);

    //generate_Quadruples_Operator_Print(op1,op2,token,result,fp);
    return quad->result;


    // if(node->containsLexeme == 1)
    // {
    //     return node->type;
    // }
    // else if(strcmp(node->type,ast_list[ARRAY_AST])==0)
    // {
    //     return generate_Quadruples_Assign_LHS_Array_Element_Access(node,fp);
    // }
    // else if(strcmp(node->type,ast_list[OPERATION_1_AST])==0 || strcmp(node->type,ast_list[OPERATION_2_AST])==0)
    // {
    //     char* op1 = generate_Quadruples_ArithmeticOp((ast_node*)(node->children[0]),fp);
    //     char* op2 = generate_Quadruples_ArithmeticOp((ast_node*)(node->children[2]),fp);
    //     char* token = ((ast_node*)(node->children[1]))->type;
    //
    //     char* result = generate_temp();
    //
    //     generate_Quadruples_Operator_Print(op1,op2,token,result,fp);
    //     return result;
    // }

    return NULL;

}

st_entry* generate_Quadruples_RelationalOp(ast_node * node,st_table* symbol_table,quadrupleList* ql,FILE* fp)
{
    if(node == NULL)
    {
        return NULL;
    }

    st_entry* op1 = generate_Quadruples_Expression_Helper((ast_node*)(node->children[0]),symbol_table,ql,fp);
    st_entry* op2 = generate_Quadruples_Expression_Helper((ast_node*)(node->children[2]),symbol_table,ql,fp);
    char* token = ((ast_node*)(node->children[1]))->type;

    

    //generate_Quadruples_Operator_Print(op1,op2,token,result,fp);
    
    quadruple *quad = (quadruple *)malloc(sizeof(quadruple));

    quad->op = token;
    quad->arg1_type = ST_ENTRY_QUAD;
    quad->arg1_st_entry = op1;
    quad->arg2_type = ST_ENTRY_QUAD;
    quad->arg2_st_entry = op2;
    quad->result = generate_temp(symbol_table);

    print_Quadruple_in_FILE(quad,symbol_table,ql,fp);

    return quad->result;

    // if(strcmp(node->type,ast_list[RELATIONAL_OP_AST])==0)
    // {
    //     char* op1 = generate_Quadruples_RelationalOp((ast_node*)(node->children[0]),fp);
    //     char* op2 = generate_Quadruples_RelationalOp((ast_node*)(node->children[2]),fp);
    //     char* token = ((ast_node*)(node->children[1]))->type;
    //
    //     char* result = generate_temp();
    //
    //     generate_Quadruples_Operator_Print(op1,op2,token,result,fp);
    //     return result;
    // }
    // else
    // {
    //     return generate_Quadruples_ArithmeticOp(node,fp);
    // }

}

st_entry* generate_Quadruples_LogicalOp(ast_node * node,st_table* symbol_table,quadrupleList* ql,FILE* fp)
{
    if(node == NULL)
    {
        return NULL;
    }

    st_entry* op1 = generate_Quadruples_Expression_Helper((ast_node*)(node->children[0]),symbol_table,ql,fp);
    st_entry* op2 = generate_Quadruples_Expression_Helper((ast_node*)(node->children[2]),symbol_table,ql,fp);
    char* token = ((ast_node*)(node->children[1]))->type;


    //generate_Quadruples_Operator_Print(op1,op2,token,result,fp);

    quadruple* quad = (quadruple*)malloc(sizeof(quadruple));

    quad->op = token;
    quad->arg1_type = ST_ENTRY_QUAD;
    quad->arg1_st_entry = op1;
    quad->arg2_type = ST_ENTRY_QUAD;
    quad->arg2_st_entry = op2;
    quad->result = generate_temp(symbol_table);;

    print_Quadruple_in_FILE(quad,symbol_table,ql,fp);

    return quad->result;

    // if(strcmp(node->type,ast_list[LOGICAL_OP_AST])==0)
    // {
    //     char* op1 = generate_Quadruples_LogicalOp((ast_node*)(node->children[0]),fp);
    //     char* op2 = generate_Quadruples_LogicalOp((ast_node*)(node->children[2]),fp);
    //     char* token = ((ast_node*)(node->children[1]))->type;
    //
    //     char* result = generate_temp();
    //
    //       generate_Quadruples_Operator_Print(op1,op2,token,result,fp);
    //     return result;
    // }
    // else
    // {
    //     return generate_Quadruples_RelationalOp(node,fp);
    // }

}

st_entry* generate_Quadruples_Expression_Helper(ast_node * node,st_table* symbol_table,quadrupleList* ql,FILE* fp)
{
    if(node == NULL)
    {
        return NULL;
    }
    
    if(node->containsLexeme == 1)
    {
        st_entry* temp_variable = generate_temp(symbol_table);
        char* token = node->lexeme->token;
        if(strcmp(token,token_list[NUM])==0)
        {
            quadruple* quad = (quadruple*)malloc(sizeof(quadruple));

            quad->op = three_code_list[ASSIGN_3CODE];
            quad->arg1_type = NUM_QUAD;
            quad->arg1_num = node->lexeme->value_if_num;
            quad->arg2_type = NULL_QUAD;
            quad->result = temp_variable;

            print_Quadruple_in_FILE(quad,symbol_table,ql,fp);
        }
        else if(strcmp(token,token_list[RNUM])==0)
        {
            quadruple* quad = (quadruple*)malloc(sizeof(quadruple));

            quad->op = three_code_list[ASSIGN_3CODE];
            quad->arg1_type = RNUM_QUAD;
            quad->arg1_rnum = node->lexeme->value_if_rnum;
            quad->arg2_type = NULL_QUAD;
            quad->result = temp_variable;

            print_Quadruple_in_FILE(quad,symbol_table,ql,fp);

        }
        else if(strcmp(token,keyword_list[TRUE])==0)
        {

            quadruple* quad = (quadruple*)malloc(sizeof(quadruple));

            quad->op = three_code_list[ASSIGN_3CODE];
            quad->arg1_type = BOOL_QUAD;
            quad->arg1_bool = 1;
            quad->arg2_type = NULL_QUAD;
            quad->result = temp_variable;

            print_Quadruple_in_FILE(quad,symbol_table,ql,fp);
        }
        else if(strcmp(token,keyword_list[FALSE])==0)
        {
            quadruple* quad = (quadruple*)malloc(sizeof(quadruple));

            quad->op = three_code_list[ASSIGN_3CODE];
            quad->arg1_type = BOOL_QUAD;
            quad->arg1_bool = 0;
            quad->arg2_type = NULL_QUAD;
            quad->result = temp_variable;

            print_Quadruple_in_FILE(quad,symbol_table,ql,fp);
        }
        else
        {
            char* id = node->type;

            st_entry* var =  getSymbolEntry(symbol_table,id);

            quadruple* quad = (quadruple*)malloc(sizeof(quadruple));

            quad->op = three_code_list[ASSIGN_3CODE];
            quad->arg1_type = ST_ENTRY_QUAD;
            quad->arg1_st_entry = var;
            quad->arg2_type = NULL_QUAD;
            quad->result = temp_variable;

            print_Quadruple_in_FILE(quad,symbol_table,ql,fp);

        }
        return temp_variable;
    }
    else if(strcmp(node->type,ast_list[ARRAY_AST])==0)
    {
        //return generate_Quadruples_Assign_LHS_Array_Element_Access(node,symbol_table,ql,fp);
        return generate_Quadruples_Assign_LHS_Array_Element_Access(node,symbol_table,ql,fp);
    }
    if(strcmp(node->type,ast_list[LOGICAL_OP_AST])==0)
    {
        return generate_Quadruples_LogicalOp(node,symbol_table,ql,fp);
    }
    else if(strcmp(node->type,ast_list[RELATIONAL_OP_AST])==0)
    {
        return generate_Quadruples_RelationalOp(node,symbol_table,ql,fp);
    }
    else if(strcmp(node->type,ast_list[OPERATION_1_AST])==0 || strcmp(node->type,ast_list[OPERATION_2_AST])==0)
    {
        return generate_Quadruples_ArithmeticOp(node,symbol_table,ql,fp);
    }

}

st_entry* generate_Quadruples_Expression(ast_node * node,st_table* symbol_table,quadrupleList* ql,FILE* fp)
{
    // printf("Called\n");
    // printf("%s\n",node->type);
    // fflush(stdout);
    if(node == NULL)
    {
        return NULL;
    }
    else if(strcmp(node->type,ast_list[UNARY_OP_AST])==0)
    {
        // printf("Unary Op\n");
        // fflush(stdout);
        
        char* uniry_op = ((ast_node*)(node->children[0]))->type;
        char* exp = generate_Quadruples_Expression((ast_node*)(node->children[1]),symbol_table,ql,fp);
        if(strcmp(uniry_op,ast_list[MINUS_AST])==0)
        {
            char* result = generate_temp(symbol_table);

            quadruple* quad = (quadruple*)malloc(sizeof(quadruple));

            quad->op = uniry_op;
            quad->arg1_type = NUM_QUAD;
            quad->arg1_num = 0;
            quad->arg2_type = ST_ENTRY_QUAD;
            quad->arg2_st_entry = exp;
            quad->result = result;

            print_Quadruple_in_FILE(quad,symbol_table,ql,fp);
            //generate_Quadruples_Operator_Print("0",exp,uniry_op,result,fp);
            return result;
        }
        return exp;
    }
    else //if((strcmp(node->type,ast_list[LOGICAL_OP_AST]))==0)
    {
        char* exp = generate_Quadruples_Expression_Helper(node,symbol_table,ql,fp);
        return exp;
    }
    // else if((strcmp(node->type,ast_list[RELATIONAL_OP_AST]))==0)
    // {
    //     char* exp = generate_Quadruples_RelationalOp(node,fp);
    //     return exp;
    // }
    // else if((strcmp(node->type,ast_list[OPERATION_1_AST]))==0 || (strcmp(node->type,ast_list[OPERATION_2_AST]))==0)
    // {
    //     char* exp = generate_Quadruples_ArithmeticOp(node,fp);
    //     return exp;
    // }
    return NULL;
}

void generate_Quadruples_Assign_Statement(ast_node * node,st_table* symbol_table,quadrupleList* ql,FILE* fp)
{
    // traverse_AST_print(node,0,1);
    // //return;
    // fflush(stdout);
    if(node == NULL)
    {
        return;
    }

    ast_node* variable_node = (ast_node*)(node->children[0]);
    ast_node* expression_node = (ast_node*)(node->children[1]);

    
    st_entry* variable_entry;

    int expression_value_type;
    int expression_value_num;//0
    float expression_value_rnum;//1
    int expression_value_bool;//2
    st_entry* expression_value_st_entry;//3
    // write code for RHS

    //char* expression_value = generate_Quadruples_Expression(expression_node,fp);

    if(expression_node->containsLexeme == 1)
    {
        char* token = expression_node->lexeme->token;
        if(strcmp(token,token_list[NUM])==0)
        {
            expression_value_type = 0;
            expression_value_num = expression_node->lexeme->value_if_num;
        }
        else if(strcmp(token,token_list[RNUM])==0)
        {
            expression_value_type = 1;
            expression_value_rnum = expression_node->lexeme->value_if_rnum;

        }
        else if(strcmp(token,keyword_list[TRUE])==0)
        {
            expression_value_type = 2;
            expression_value_bool = 1;
        }
        else if(strcmp(token,keyword_list[FALSE])==0)
        {
            expression_value_type = 2;
            expression_value_bool = 0;
        }
        else
        {
            expression_value_type = 3;
            expression_value_st_entry = getSymbolEntry(symbol_table,expression_node->type);
        }
    }
    else
    {
        expression_value_type = 3;
        expression_value_st_entry = generate_Quadruples_Expression(expression_node,symbol_table,ql,fp);
    }

    //code below this complete

    if(variable_node->containsLexeme==1)
    {
        //case when directly lexeme
        char* variable_name = variable_node->type;
        variable_entry = getSymbolEntry(symbol_table,variable_name);
    }
    else
    {
        //case when array element
        variable_entry = generate_Quadruples_Assign_LHS_Array_Element_Access(variable_node,symbol_table,ql,fp);
    }   

    quadruple *quad = (quadruple *)malloc(sizeof(quadruple));
    quad->op = three_code_list[ASSIGN_3CODE];
    if(expression_value_type == 0)
    {
        quad->arg1_type = NUM_QUAD;
        quad->arg1_num = expression_value_num;
    }
    else if(expression_value_type == 1)
    {
        quad->arg1_type = RNUM_QUAD;
        quad->arg1_rnum = expression_value_rnum;
    }
    else if(expression_value_type == 2)
    {
        quad->arg1_type = BOOL_QUAD;
        quad->arg1_bool = expression_value_bool;
    }
    else
    {
        quad->arg1_type = ST_ENTRY_QUAD;
        quad->arg1_st_entry = expression_value_st_entry;
    }
    quad->arg2_type = NULL_QUAD;
    quad->result = variable_entry;

    print_Quadruple_in_FILE(quad,symbol_table,ql,fp);
    

}

void generate_Quadruples_While_Statement(ast_node * node,st_table* symbol_table,quadrupleList* ql,FILE* fp)
{
    if( node == NULL)
    {
        return;
    }

    symbol_table = node->symbolTable;

    char* while_begin_label = generate_Label();
    char* while_condition_label = generate_Label();
    
    generate_Quadruples_GOTO(while_condition_label,symbol_table,ql,fp);

    generate_Quadruples_Label(while_begin_label,symbol_table,ql,fp);

    generate_Quadruples_Statements_List(node->children[1],symbol_table,ql,fp);

    generate_Quadruples_Label(while_condition_label,symbol_table,ql,fp);

    st_entry* exp = generate_Quadruples_Expression(node->children[0],symbol_table,ql,fp);

    //generate_Quadruples_Jump_If_True(exp,while_begin_label,fp);

    quadruple *quad = (quadruple *)malloc(sizeof(quadruple));

    quad->op = three_code_list[JUMP_IF_TRUE_3CODE];
    quad->arg1_type = ST_ENTRY_QUAD;
    quad->arg1_st_entry = exp;
    quad->arg2_type = LABEL_QUAD;
    quad->arg2_label = while_begin_label;
    quad->result = NULL;

    //generate_Quadruples_Label(while_next,fp);

}

void generate_Quadruples_Statement(ast_node * node,st_table* symbol_table,quadrupleList* ql,FILE* fp)
{
    if(node == NULL)
    {
        return;
    }

    if(strcmp(node->type,ast_list[FOR_LOOP_STMT_AST])==0)
    {
        generate_Quadruples_For(node,symbol_table,ql,fp);
    }
    else if(strcmp(node->type,ast_list[WHILE_LOOP_STMT_AST])==0)
    {
        generate_Quadruples_While_Statement(node,symbol_table,ql,fp);
    }
    else if(strcmp(node->type,ast_list[SWITCH_STMT_AST])==0)
    {
        generate_Quadruples_Switch(node,symbol_table,ql,fp);   
    }
    else if(strcmp(node->type,ast_list[PRINT_STMT_AST])==0)
    {
        generate_Quadruples_Print_Statement(node,symbol_table,ql,fp);
    }
    else if(strcmp(node->type,ast_list[GET_VALUE_STMT_AST])==0)
    {
        generate_Quadruples_Get_Value(node,symbol_table,ql,fp);
    }
    else if(strcmp(node->type,ast_list[ASSIGN_STMT_AST])==0)
    {
        generate_Quadruples_Assign_Statement(node,symbol_table,ql,fp);
    }
}

void generate_Quadruples_Array_Declaration(st_entry* array,st_table* symbol_table,quadrupleList* ql,FILE* fp)
{
    if(array == NULL)
        return;

    quadruple* quad = (quadruple *)malloc(sizeof(quadruple));

    id_type* array_type = array->idType;


    if(array_type->is_static_array == 1)
        quad->op = three_code_list[STATIC_ARRAY_DECLARATION_3CODE];
    else
        quad->op = three_code_list[DYNAMIC_ARRAY_DECLARATION_3CODE];
    quad->arg1_type = ST_ENTRY_QUAD;
    quad->arg1_st_entry = array;
    quad->arg2_type = NULL_QUAD;
    quad->result = NULL;

    print_Quadruple_in_FILE(quad,symbol_table,ql,fp);

}

void generate_Quadruples_Array_Declaration_Statement(ast_node * node,st_table* symbol_table,quadrupleList* ql,FILE* fp)
{

    if(node == NULL)
    {
        return;
    }

    ast_node* accpara_list = node->children[1];

    int numParams = accpara_list->numChildren;

    for(int i=0;i<numParams;i++)
    {
        ast_node* param_variable = accpara_list->children[i];
        ast_node* array_elem_or_id = param_variable->children[0];
        ast_node* variable_id = array_elem_or_id->children[0];
        
        char* variable_id_string = variable_id->type;

        st_table* variable_entry = getSymbolEntry(symbol_table,variable_id_string);

        generate_Quadruples_Array_Declaration(variable_entry,symbol_table,ql,fp);

    }

}

void generate_Quadruples_Statements_List(ast_node * node,st_table* symbol_table,quadrupleList* ql,FILE* fp)
{
    //printf("in generate_Quadruples_Statements_List\n");
    if( node == NULL)
    {
        return;
    }

    int numChildren = node->numChildren;

    for(int i=0;i<numChildren;i++)
    {
        char* statement_type = ((ast_node*)(node->children[i]))->type;

        if(strcmp(statement_type,ast_list[FOR_LOOP_STMT_AST])==0)
        {
            generate_Quadruples_For(node->children[i],symbol_table,ql,fp);
        }
        else if(strcmp(statement_type,ast_list[WHILE_LOOP_STMT_AST])==0)
        {
            generate_Quadruples_While_Statement(node->children[i],symbol_table,ql,fp);
        }
        else if(strcmp(statement_type,ast_list[SWITCH_STMT_AST])==0)
        {
            generate_Quadruples_Switch(node->children[i],symbol_table,ql,fp);
        }
        else if(strcmp(statement_type,ast_list[PRINT_STMT_AST])==0)
        {
            generate_Quadruples_Print_Statement(node->children[i],symbol_table,ql,fp);
        }
        else if(strcmp(statement_type,ast_list[GET_VALUE_STMT_AST])==0)
        {
            generate_Quadruples_Get_Value(node->children[i],symbol_table,ql,fp);
        }
        else if(strcmp(statement_type,ast_list[ASSIGN_STMT_AST])==0)
        {
            generate_Quadruples_Assign_Statement(node->children[i],symbol_table,ql,fp);
        }
        else if(strcmp(statement_type,ast_list[MODULE_REUSE_STMT_AST])==0)
        {
            generate_Quadruples_Module_Reuse_Statement(node->children[i],symbol_table,ql,fp);
        }
        else if(strcmp(statement_type,ast_list[DECLARE_STMT_AST])==0)
        {
            ast_node* declare_type = ((ast_node*)((ast_node*)(node->children[i]))->children[0])->type;

            if(strcmp(declare_type,ast_list[RANGE_DATATYPE_AST])==0)
            {
                generate_Quadruples_Array_Declaration_Statement(node->children[i],symbol_table,ql,fp);
            }

        }
    }

}

void print_Quadruples(ast_node* node,st_table* symbol_table,quadrupleList* ql,FILE* fp)
{

    if (node == NULL)
        return;


    if(node->symbolTable != NULL)
        symbol_table = node->symbolTable;

    //getSymbolEntry(node->symbolTable. "a");

    if(strcmp(node->type,ast_list[DRIVER_AST])==0)
    {
        //printf("in driver\n");
        //fflush(stdout);
        generate_Quadruples_Driver(node,symbol_table,ql,fp);
        //printf("out driver\n");
        //fflush(stdout);
    }
    else if(strcmp(node->type,ast_list[MODULE_AST])==0)
    {
        // printf("in module\n");
        // fflush(stdout);
        generate_Quadruples_Module(node,symbol_table,ql,fp);
        // printf("out module\n");
        // fflush(stdout);
    }
    else if(strcmp(node->type,ast_list[STATEMENT_LIST_AST])==0)
    {
        generate_Quadruples_Statements_List(node,symbol_table,ql,fp);
    }

    for (int i = 0; i < node->numChildren; i++)
    {
        print_Quadruples(node->children[i],symbol_table,ql,fp);
    }

}
