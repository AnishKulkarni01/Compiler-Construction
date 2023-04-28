//CS F363 : Compiler Construction
//Group No. 24
//1. Nachiket Kotalwar               (2020A7PS0024P)
//2.Parth Patel                      (2020A7PS0026P)
//3.Partha Sarathi Purkayastha       (2020A7PS0043P)
//4.Labeeb Ahsan                     (2020A7PS0045P)
//5.Anish Atul Kulkarni              (2020A7PS0975P)

// for input 3 code

// Op A B R 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//to be added in driver
fprintf(fp,"section .text\n");
fprintf(fp,"global main\n");
fprintf(fp,"main:\n");
//

void codegen_arithm(st_table * stable, quadruple *q, FILE *fp)
{
    char *op = q->op;
    char *arg1 = q->arg1;
    char *arg2 = q->arg2;
    char *res = q->res;

    st_entry *st1 = getSymbolEntry(stable, arg1);
    st_entry *st2 = getSymbolEntry(stable, arg2);
    st_entry *st3 = getSymbolEntry(stable, res);

    if ((strcmp(st1->idType->type,"real")==0 && strcmp(st2->idType->type,"real")==0 ))
    {  
        switch(op)
        {
            case "+":
            {
                fprintf(fp,"MOV RAX, [RBP-%d]\n",st1->offset);
                fprintf(fp,"MOV RBX, [RBP-%d]\n",st2->offset);
                fprintf(fp,"ADD RAX, RBX\n");
                fprintf(fp,"MOV [RBP-%d], RAX\n",st3->offset);
            }
            break;
            case "-":
            {
                fprintf(fp,"MOV RAX, [RBP-%d]\n",st1->offset);
                fprintf(fp,"MOV RBX, [RBP-%d]\n",st2->offset);
                fprintf(fp,"SUB RAX, RBX\n");
                fprintf(fp,"MOV [RBP-%d], RAX\n",st3->offset);
            }
            break;
            case "*":
            {
                fprintf(fp,"MOV RAX, [RBP-%d]\n",st1->offset);
                fprintf(fp,"MOV RBX, [RBP-%d]\n",st2->offset);
                fprintf(fp,"MUL RAX, RBX\n");
                fprintf(fp,"MOV [RBP-%d], RAX\n",st3->offset);
            }
            break;
            case "/":
            {
                fprintf(fp,"MOV RAX, [RBP-%d]\n",st1->offset);
                fprintf(fp,"MOV RBX, [RBP-%d]\n",st2->offset);
                fprintf(fp,"DIV RAX, RBX\n");
                fprintf(fp,"MOV [RBP-%d], RAX\n",st3->offset);
            }
            break;            
            
        }

    }
    else if (strcmp(st1->idType->type,"integer")==0 && strcmp(st2->idType->type,"integer")==0)
    {
        
        switch(op)
        {
            case "+":
            {
                fprintf(fp,"MOV RAX, [RBP-%d]\n",st1->offset);
                fprintf(fp,"MOV RBX, [RBP-%d]\n",st2->offset);
                fprintf(fp,"ADD RAX, RBX\n");
                fprintf(fp,"MOV [RBP-%d], RAX\n",st3->offset);
            }
            break;
            case "-":
            {
                fprintf(fp,"MOV RAX, [RBP-%d]\n",st1->offset);
                fprintf(fp,"MOV RBX, [RBP-%d]\n",st2->offset);
                fprintf(fp,"SUB RAX, RBX\n");
                fprintf(fp,"MOV [RBP-%d], RAX\n",st3->offset);
            }
            break;
            case "*":
            {
                fprintf(fp,"MOV RAX, [RBP-%d]\n",st1->offset);
                fprintf(fp,"MOV RBX, [RBP-%d]\n",st2->offset);
                fprintf(fp,"MUL RAX, RBX\n");
                fprintf(fp,"MOV [RBP-%d], RAX\n",st3->offset);
            }
            break;
            
            
        }
    }
}

void codegen_relational(st_table * stable, quadruple *q, FILE *fp)
{
    char *op = q->op;
    char *arg1 = q->arg1;
    char *arg2 = q->arg2;
    char *res = q->res;

    st_entry *st1 = getSymbolEntry(stable, arg1);
    st_entry *st2 = getSymbolEntry(stable, arg2);
    st_entry *st3 = getSymbolEntry(stable, res);

    if (strcmp(st1->idType->type,"integer")==0 && strcmp(st2->idType->type,"integer")==0 && strcmp(st3->idType->type,"boolean")==0 ||(strcmp(st1->idType->type,"real")==0 && strcmp(st2->idType->type,"real")==0 && strcmp(st3->idType->type,"boolean")==0))
    {
        fprintf(fp,"section .data\n");
        fprintf(fp,"msg0 db 'false'\n");
        fprintf(fp,"msg1 db 'true'\n");
        
        
        switch(op)
        {
            case "lt":
            {
                fprintf(fp,"MOV RAX, [RBP-%d]\n",st1->offset);
                fprintf(fp,"MOV RBX, [RBP-%d]\n",st2->offset);
                fprintf(fp,"CMP RAX, RBX\n");
                fprintf(fp,"JL X1\n");
                fprintf(fp,"LEA SI, msg0\n");
                fprintf(fp,"MOV ECX, [SI]\n");
                fprintf(fp,"MOV [RBP-%d], ECX\n",st3->offset);
                fprintf(fp,"JMP EXIT\n");
                fprintf(fp,"X1: \n");
                fprintf(fp,"LEA SI, msg1\n");
                fprintf(fp,"MOV ECX, [SI]\n");
                fprintf(fp,"MOV [RBP-%d], ECX\n",st3->offset);
                fprintf(fp,"EXIT: \n");
            }
            break;
            case "gt":
            {
                fprintf(fp,"MOV RAX, [RBP-%d]\n",st1->offset);
                fprintf(fp,"MOV RBX, [RBP-%d]\n",st2->offset);
                fprintf(fp,"CMP RAX, RBX\n");   
                fprintf(fp,"JG X1\n");
                fprintf(fp,"LEA SI, msg0\n");
                fprintf(fp,"MOV ECX, [SI]\n");
                fprintf(fp,"MOV [RBP-%d], ECX\n",st3->offset);
                fprintf(fp,"JMP EXIT\n");
                fprintf(fp,"X1: \n");
                fprintf(fp,"LEA SI, msg1\n");
                fprintf(fp,"MOV ECX, [SI]\n");
                fprintf(fp,"MOV [RBP-%d], ECX\n",st3->offset);
                fprintf(fp,"EXIT: \n");
            }
            break;
            case "le":
            {
                fprintf(fp,"MOV RAX, [RBP-%d]\n",st1->offset);
                fprintf(fp,"MOV RBX, [RBP-%d]\n",st2->offset);
                fprintf(fp,"CMP RAX, RBX\n");   
                fprintf(fp,"JLE X1\n")
                fprintf(fp,"LEA SI, msg0\n");
                ;fprintf(fp,"MOV ECX, [SI]\n");
                fprintf(fp,"MOV [RBP-%d], ECX\n",st3->offset);
                fprintf(fp,"JMP EXIT\n");
                fprintf(fp,"X1: \n");
                fprintf(fp,"LEA SI, msg1\n");
                fprintf(fp,"MOV ECX, [SI]\n");
                fprintf(fp,"MOV [RBP-%d], ECX\n",st3->offset);
                fprintf(fp,"EXIT: \n");
            }
            break;
            case "ge":
            {
                fprintf(fp,"MOV RAX, [RBP-%d]\n",st1->offset);
                fprintf(fp,"MOV RBX, [RBP-%d]\n",st2->offset);
                fprintf(fp,"CMP RAX, RBX\n");   
                fprintf(fp,"JGE X1\n")
                fprintf(fp,"LEA SI, msg0\n");
                ;fprintf(fp,"MOV ECX, [SI]\n");
                fprintf(fp,"MOV [RBP-%d], ECX\n",st3->offset);
                fprintf(fp,"JMP EXIT\n");
                fprintf(fp,"X1: \n");
                fprintf(fp,"LEA SI, msg1\n");
                fprintf(fp,"MOV ECX, [SI]\n");
                fprintf(fp,"MOV [RBP-%d], ECX\n",st3->offset);
                fprintf(fp,"EXIT: \n");
            }
            break;
            case "eq":
            {
                fprintf(fp,"MOV RAX, [RBP-%d]\n",st1->offset);
                fprintf(fp,"MOV RBX, [RBP-%d]\n",st2->offset);
                fprintf(fp,"CMP RAX, RBX\n");   
                fprintf(fp,"JE X1\n");
                fprintf(fp,"LEA SI, msg0\n");
                fprintf(fp,"MOV ECX, [SI]\n");
                fprintf(fp,"MOV [RBP-%d], ECX\n",st3->offset);
                fprintf(fp,"JMP EXIT\n");
                fprintf(fp,"X1: \n");
                fprintf(fp,"LEA SI, msg1\n");
                fprintf(fp,"MOV ECX, [SI]\n");
                fprintf(fp,"MOV [RBP-%d], ECX\n",st3->offset);
                fprintf(fp,"EXIT: \n");
            }
            break;
            case "ne":
            {
                fprintf(fp,"MOV RAX, [RBP-%d]\n",st1->offset);
                fprintf(fp,"MOV RBX, [RBP-%d]\n",st2->offset);
                fprintf(fp,"CMP RAX, RBX\n");   
                fprintf(fp,"JNE X1\n")
                fprintf(fp,"LEA SI, msg0\n");
                ;fprintf(fp,"MOV ECX, [SI]\n");
                fprintf(fp,"MOV [RBP-%d], ECX\n",st3->offset);
                fprintf(fp,"JMP EXIT\n");
                fprintf(fp,"X1: \n");
                fprintf(fp,"LEA SI, msg1\n");
                fprintf(fp,"MOV ECX, [SI]\n");
                fprintf(fp,"MOV [RBP-%d], ECX\n",st3->offset);
                fprintf(fp,"EXIT: \n");
            }
            break;
            
        }
    }
}


void codegen_logical(st_table * stable, quadruple *q, FILE *fp)
{
    char *op = q->op;
    char *arg1 = q->arg1;
    char *arg2 = q->arg2;
    char *res = q->res;

    st_entry *st1 = getSymbolEntry(stable, arg1);
    st_entry *st2 = getSymbolEntry(stable, arg2);
    st_entry *st3 = getSymbolEntry(stable, res);

    // boolean while input uses 1 for true and 0 for false, output needs to be printed as string
    if(st1->idType->type == "boolean" && st2->idType->type == "boolean")
    {
        fprintf(fp,"section .data\n");
        fprintf(fp,"msg0 db 'false'\n");
        fprintf(fp,"msg1 db 'true'\n");
        
        
        switch(op)
        {
            case "and":
            {
                fprintf(fp,"MOV RAX, [RBP-%d]\n",st1->offset);
                fprintf(fp,"MOV RBX, [RBP-%d]\n",st2->offset);
                fprintf(fp,"AND RAX, RBX\n");
                fprintf(fp,"JNZ X1\n");
                fprintf(fp,"LEA SI, msg0\n");
                fprintf(fp,"MOV ECX, [SI]\n");
                fprintf(fp,"MOV [RBP-%d], ECX\n",st3->offset);
                fprintf(fp,"JMP EXIT\n");
                fprintf(fp,"X1: \n");
                fprintf(fp,"LEA SI, msg1\n");
                fprintf(fp,"MOV ECX, [SI]\n");
                fprintf(fp,"MOV [RBP-%d], ECX\n",st3->offset);
                fprintf(fp,"EXIT: \n");
            }
            break;
            case "or":
            {
                fprintf(fp,"MOV RAX, [RBP-%d]\n",st1->offset);
                fprintf(fp,"MOV RBX, [RBP-%d]\n",st2->offset);
                fprintf(fp,"OR RAX, RBX\n");
                fprintf(fp,"JNZ X1\n");
                fprintf(fp,"LEA SI, msg0\n");
                fprintf(fp,"MOV ECX, [SI]\n");
                fprintf(fp,"MOV [RBP-%d], ECX\n",st3->offset);
                fprintf(fp,"JMP EXIT\n");
                fprintf(fp,"X1: \n");
                fprintf(fp,"LEA SI, msg1\n");
                fprintf(fp,"MOV ECX, [SI]\n");
                fprintf(fp,"MOV [RBP-%d], ECX\n",st3->offset);
                fprintf(fp,"EXIT: \n");
            }
            break;
            
        }
    }
}

void codegen_assign(st_table * stable, quadruple *q, FILE *fp)
{
    char *asop = q->op;
    char *arg1 = q->arg1;
    char *res = q->res;

    st_entry *st1 = getSymbolEntry(stable, arg1);
    st_entry *st3 = getSymbolEntry(stable, res);

    if(strcmp(st1->idType->type,"integer")==0 && strcmp(st3->idType->type,"integer")==0 ||(strcmp(st1->idType->type,"real")==0 && strcmp(st3->idType->type,"real")==0))
    {
        
        fprintf(fp,"MOV RAX, [RBP-%d]\n",st1->offset);
        fprintf(fp,"MOV [RBP-%d], RAX\n",st3->offset);
    }
}

void codegen_unary(st_table * stable, quadruple *q, FILE *fp)
{
    char *op = q->op;
    char *arg1 = q->arg1;
    char *res = q->res;

    st_entry *st1 = getSymbolEntry(stable, arg1);
    st_entry *st3 = getSymbolEntry(stable, res);

    if(strcmp(st1->idType->type,"integer")==0 && strcmp(st3->idType->type,"integer")==0 || (strcmp(st1->idType->type,"real")==0 && strcmp(st3->idType->type,"real")==0))
        
        switch(op)
        {
            case "-":
            {
                fprintf(fp,"MOV RAX, [RBP-%d]\n",st1->offset);
                fprintf(fp,"MUL RAX,-1\n");
                fprintf(fp,"MOV [RBP-%d], RAX\n",st3->offset);
            }
            break;
            case "+":
            {
                fprintf(fp,"MOV RAX, [RBP-%d]\n",st1->offset);
                fprintf(fp,"MUL RAX,1\n");
                fprintf(fp,"MOV [RBP-%d], RAX\n",st3->offset);
            }
            break;
        }
}

void codegen_label(st_table * stable, quadruple *q, FILE *fp)
{
    char *arg1 = q->arg1;

    fprintf(fp,"%s:\n",arg1);
}

void codegen_dymarray(st_table *stable, quadruple *q, FILE *fp)
{
    char *op = q->op;
    char *arg1 = q->arg1;

    st_entry *st1 = getSymbolEntry(stable, arg1);

    if (strcmp(op, "dymarray") == 0)
    {
        int size = st1->idType->width;                // size of array elements
        char *array_name = st1->name;                 // name of the array variable
        char *lower_bound = st1->info.array.lb->name; // name of lower bound variable
        char *upper_bound = st1->info.array.ub->name; // name of upper bound variable
        int lb_offset = st1->info.array.lb->offset;   // offset of lower bound variable in symbol table
        int ub_offset = st1->info.array.ub->offset;   // offset of upper bound variable in symbol table
        int array_offset = st1->offset;               // offset of array variable in symbol table

        fprintf(fp, "MOV [RBP-%d], RSP\n", array_offset);

        // load lower bound and upper bound into registers
        fprintf(fp, "MOV RAX, [RBP-%d]\n", lb_offset);
        fprintf(fp, "MOV RCX, [RBP-%d]\n", ub_offset);

        // calculate array size (upper - lower + 1)
        fprintf(fp, "SUB RCX, RAX\n");
        fprintf(fp, "ADD RCX, 1\n");
        fprintf(fp, "MOV RBX, %d\n", size);


        // allocate space on the stack for the array
        // Here, RCX times sub RBX from RSP
        fprintf(fp,"CMP RCX, 0\n");
        fprintf(fp, "JE LOOP_END\n");
        fprintf(fp, "LOOP_START:\n");
        fprintf(fp,"SUB RSP, RBX\n");
        fprintf(fp, "DEC RCX\n");
        fprintf(fp, "CMP RCX, 0\n");
        fprintf(fp, "JNE LOOP_START\n");
        fprintf(fp, "JE LOOP_END\n");
        fprintf(fp, "LOOP_END:\n");

    }
}

void codegen_statarray(st_table *stable, quadruple *q, FILE *fp)
{
    char *op = q->op;
    char *arg1 = q->arg1;

    st_entry *st1 = getSymbolEntry(stable, arg1);

    if (strcmp(op, "statarray") == 0)
    {
        int size = st1->idType->width;                // size of array elements
        char *array_name = st1->name;                 // name of the array variable
        char *lower_bound = st1->info.array.lb->name; // name of lower bound variable
        char *upper_bound = st1->info.array.ub->name; // name of upper bound variable
        int lb_offset = st1->info.array.lb->offset;   // offset of lower bound variable in symbol table
        int ub_offset = st1->info.array.ub->offset;   // offset of upper bound variable in symbol table
        int array_offset = st1->offset;               // offset of array variable in symbol table

        fprintf(fp, "MOV [RBP-%d], RSP\n", array_offset);

        // load lower bound and upper bound into registers
        fprintf(fp, "MOV RAX, [RBP-%d]\n", lb_offset);
        fprintf(fp, "MOV RCX, [RBP-%d]\n", ub_offset);

        // calculate array size (upper - lower + 1)
        fprintf(fp, "SUB RCX, RAX\n");
        fprintf(fp, "ADD RCX, 1\n");
        fprintf(fp, "MOV RBX, %d\n", size);


        // allocate space on the stack for the array
        // Here, RCX times sub RBX from RSP
        fprintf(fp,"CMP RCX, 0\n");
        fprintf(fp, "JE LOOP_END\n"); 
        fprintf(fp, "LOOP_START:\n");
        fprintf(fp,"SUB RSP, RBX\n");
        fprintf(fp, "DEC RCX\n");
        fprintf(fp, "CMP RCX, 0\n");
        fprintf(fp, "JNE LOOP_START\n");
        fprintf(fp, "JE LOOP_END\n");
        fprintf(fp, "LOOP_END:\n");

    }
}







