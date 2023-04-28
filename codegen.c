//CS F363 : Compiler Construction
//Group No. 24
//1. Nachiket Kotalwar               (2020A7PS0024P)
//2.Parth Patel                      (2020A7PS0026P)
//3.Partha Sarathi Purkayastha       (2020A7PS0043P)
//4.Labeeb Ahsan                     (2020A7PS0045P)
//5.Anish Atul Kulkarni              (2020A7PS0975P)

#include "codegenDef.h"

// void codegen_dymarray(st_table *stable, quad *q, FILE *fp)
// {
//     char *op = q->op;
//     char *arg1 = q->arg1;
//
//     st_entry *st1 = getSymbolEntry(stable, arg1);
//
//     if (strcmp(op, "dymarray") == 0)
//     {
//         int size = st1->idType->width;                // size of array elements
//         char *array_name = st1->name;                 // name of the array variable
//         char *lower_bound = st1->info.array.lb->name; // name of lower bound variable
//         char *upper_bound = st1->info.array.ub->name; // name of upper bound variable
//         int lb_offsetAct = st1->info.array.lb->offset;   // offset of lower bound variable in symbol table
//         int ub_offset = st1->info.array.ub->offset;   // offset of upper bound variable in symbol table
//         int array_offset = st1->info.array->offset;   // offset of array variable in symbol table
//
//         fprintf(fp, "MOV [RBP-%d], RSP\n", array_offset);
//
//         // load lower bound and upper bound into registers
//         fprintf(fp, "MOV RAX, [RBP-%d]\n", lb_offset);
//         fprintf(fp, "MOV RCX, [RBP-%d]\n", ub_offset);
//
//         // calculate array size (upper - lower + 1)
//         fprintf(fp, "SUB RCX, RAX\n");
//         fprintf(fp, "ADD RCX, 1\n");
//         fprintf(fp, "MOV RBX, %d\n", size);
//
//         // allocate space on the stack for the array
//         // Here, RCX times sub RBX from RSP
//         fprintf(fp, "CMP RCX, 0\n");
//         // fprintf(fp, "JLE ERROR_1\n");
//         fprintf(fp, "JE LOOP_END\n"); // BKL CHNAGE KARIYO
//         fprintf(fp, "LOOP_START:\n");
//         fprintf(fp, "SUB RSP, RBX\n");
//         fprintf(fp, "DEC RCX\n");
//         fprintf(fp, "CMP RCX, 0\n");
//         fprintf(fp, "JNE LOOP_START\n");
//         fprintf(fp, "JE LOOP_END\n");
//
//         // error handling
//         // fprintf(fp, "ERROR_1:\n");
//         // fprintf(fp, "MOV RAX, 1\n");
//         // fprintf(fp, "MOV RDI, 1\n");
//         // fprintf(fp, "MOV RSI, ERROR_MSG\n");
//         // fprintf(fp, "MOV RDX, 20\n");
//         // fprintf(fp, "SYSCALL\n");
//         // fprintf(fp, "MOV RAX, 60\n");
//         // fprintf(fp, "SYSCALL \n");
//
//         fprintf(fp, "LOOP_END:\n");
//     }
// }
//
// void codegen_statarray(st_table *stable, quad *q, FILE *fp)
// {
//     char *op = q->op;
//     char *arg1 = q->arg1;
//
//     st_entry *st1 = getSymbolEntry(stable, arg1);
//
//     if (strcmp(op, "statarray") == 0)
//     {
//         int size = st1->info.array.size; // size of array
//         int array_offset = st1->offset;  // offset of array variable in symbol table
//
//         fprintf(fp, "MOV [RBP-%d], RSP\n", array_offset);
//
//         // calculate array size (upper - lower + 1)
//         fprintf(fp, "MOV RCX, %d\n", size);
//
//         // allocate space on the stack for the array
//         // Here, RCX times sub RBX from RSP
//         fprintf(fp, "CMP RCX, 0\n");
//         // fprintf(fp, "JLE ERROR_1\n");
//         fprintf(fp, "JE LOOP_END\n"); // BKL CHNAGE KARIYO
//         fprintf(fp, "SUB RSP, RCX\n");
//
//         // error handling
//         // fprintf(fp, "ERROR_1:\n");
//         // fprintf(fp, "MOV RAX, 1\n");
//         // fprintf(fp, "MOV RDI, 1\n");
//         // fprintf(fp, "MOV RSI, ERROR_MSG\n");
//         // fprintf(fp, "MOV RDX, 20\n");
//         // fprintf(fp, "SYSCALL\n");
//         // fprintf(fp, "MOV RAX, 60\n");
//         // fprintf(fp, "SYSCALL \n");
//
//         fprintf(fp, "LOOP_END:\n");
//     }
// }
//
// void codegen_array_access(st_table *stable, quad *q, FILE *fp)
// {
//     char *op = q->op;
//     char *arg1 = q->arg1;
//     char *arg2 = q->arg2;
//     char *result = q->result;
//
//     st_entry *st1 = getSymbolEntry(stable, arg1);
//     st_entry *st2 = getSymbolEntry(stable, arg2);
//     st_entry *st3 = getSymbolEntry(stable, result);
//
//     int size = st1->idType->width;                // size of array elements
//     char *array_name = st1->name;                 // name of the array variable
//     char *lower_bound = st1->info.array.lb->name; // name of lower bound variable
//     char *upper_bound = st1->info.array.ub->name; // name of upper bound variable
//     int lb_offset = st1->info.array.lb->offset;   // offset of lower bound variable in symbol table
//     int ub_offset = st1->info.array.ub->offset;   // offset of upper bound variable in symbol table
//     int array_offset = st1->info.array->offset;   // offset of array variable in symbol table
//
//     fprintf(fp, "MOV RCX, [RBP-%d]\n", st2->offset);
//     fprintf(fp, "MOV RAX,  [RBP-%d]\n", st1->info.array.lb->offset)
//         fprintf(fp, "CMP RCX, RAX\n");
//     // fprintf(fp,"JL ERROR_LO\n");
//     fprintf(fp, "MOV RAX,  [RBP-%d]\n", st1->info.array.lb->offset)
//         fprintf(fp, "CMP RCX, RAX\n");
//     // fprintf(fp,"JG ERROR_HI\n");
//     fprintf(fp, "MOV RBX, [RBP-%d]\n", st1->info.array.address->offset);
//     fprintf(fp, "MOV RAX,  [RBP-%d]\n", st1->info.array.lb->offset);
//     fprintf(fp, "push RBX\n");
//     fprintf(fp, "MOV RBX,0\n ");
//     fprintf(fp, " SUB RBX,RAX\n");
//     fprintf(fp, "MOV RAX,RBX\n ");
//     fprintf(fp, "pop RBX\n");
//     fprintf(fp, "ADD RCX,RAX\n");
//     fprintf(fp, "MOV RAX, [RBX+RCX*%d]\n", size);
//     fprintf(fp, "MOV [RBP-%d], RAX\n", st3->offset);
//     fprintf(fp, "JMP END_LOOP:\n");
//     // fprintf(fp, "ERROR_LO:\n");
//     // fprintf(d=fp,"ERROR_HI:\n");
//     fprintf(fp, "END_LOOP:\n");
// }

void codegen_assign(quadruple *q, FILE *fp)
{

    if(q->arg1_type == NUM_QUAD)
    {   if(q->arg1_num<0){
        int val1=abs(q->arg1_num);
        fprintf(fp, "MOV RAX, %d\n", q->arg1_num);
        fprintf(fp, "push RBX\n");
        fprintf(fp, "MOV RBX,0\n ");
        fprintf(fp, " SUB RBX,RAX\n");
        fprintf(fp, "MOV RAX,RBX\n ");
        fprintf(fp, "pop RBX\n");
        fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);


    }else{
        fprintf(fp, "MOV RAX, %d\n", q->arg1_num);

    }
        fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);
    }
    else if(q->arg1_type == RNUM_QUAD)
    {
        fprintf(fp, "MOV RAX, %f\n", q->arg1_rnum);
        fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);
    }
    else if(q->arg1_type == BOOL_QUAD)
    {
        fprintf(fp, "MOV RAX, %d\n", q->arg1_bool);
        fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);
    }
    else if(q->arg1_type == ST_ENTRY_QUAD)
    {
        fprintf(fp, "MOV RAX, [RBP-%d]\n", q->arg1_st_entry->offsetAct);
        fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);
    }

}

void codegen_arithm_plus(quadruple* q,FILE* fp)
{
    if(q->arg1_type == NUM_QUAD && q->arg2_type == NUM_QUAD)
    {
        fprintf(fp, "MOV RAX, %d\n", q->arg1_num);
        fprintf(fp, "ADD RAX, %d\n", q->arg2_num);
        fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);
    }
    else if(q->arg1_type == NUM_QUAD && q->arg2_type == ST_ENTRY_QUAD)
    {
        fprintf(fp, "MOV RAX, %d\n", q->arg1_num);
        fprintf(fp, "ADD RAX, [RBP-%d]\n", q->arg2_st_entry->offsetAct);
        fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);

    }
    else if(q->arg1_type == ST_ENTRY_QUAD && q->arg2_type == NUM_QUAD)
    {
        fprintf(fp, "MOV RAX, [RBP-%d]\n", q->arg1_st_entry->offsetAct);
        fprintf(fp, "ADD RAX, %d\n", q->arg2_num);
        fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);

    }
    else if(q->arg1_type == ST_ENTRY_QUAD && q->arg2_type == ST_ENTRY_QUAD)
    {
        fprintf(fp, "MOV RAX, [RBP-%d]\n", q->arg1_st_entry->offsetAct);
        fprintf(fp, "ADD RAX, [RBP-%d]\n", q->arg2_st_entry->offsetAct);
        fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);

    }
}

void codegen_print(quadruple *q, FILE *fp)
{
    if(q->arg1_type == ST_ENTRY_QUAD)
    {
        /*
        mov   rsi, [rbp-8]   ; "%x" takes a 32-bit unsigned int
        lea   rdi, format
        xor   rax, rax           ; AL=0  no FP args in XMM regs
        
        push rax
        push rbx
        push rcx
        push rsp
        
        
        call  printf
        
        pop rsp
        pop rcx
        pop rbx
        pop rax
        */
        if(q->arg1_type == ST_ENTRY_QUAD)
        {
            if(q->arg1_st_entry->idType->is_array == 1)
            {
                for(int i=q->arg1_st_entry->idType->leftIndex;i<=q->arg1_st_entry->idType->rightIndex;i++)
                {
                    fprintf(fp, "MOV RSI, [RBP-%d]\n", q->arg1_st_entry->offsetAct+24+i*8);
                    fprintf(fp, "MOV RDI, format\n");
                    fprintf(fp, "XOR RAX, RAX\n");
                    fprintf(fp, "PUSH RAX\n");
                    fprintf(fp, "PUSH RBX\n");
                    fprintf(fp, "PUSH RCX\n");
                    fprintf(fp, "PUSH RSP\n");
                    fprintf(fp, "CALL printf\n");
                    fprintf(fp, "POP RSP\n");
                    fprintf(fp, "POP RCX\n");
                    fprintf(fp, "POP RBX\n");
                    fprintf(fp, "POP RAX\n");
                }
            }
            else 
            {
                fprintf(fp, "MOV RSI, [RBP-%d]\n", q->arg1_st_entry->offsetAct);
                fprintf(fp, "MOV RDI, format\n");
                fprintf(fp, "XOR RAX, RAX\n");
                fprintf(fp, "PUSH RAX\n");
                fprintf(fp, "PUSH RBX\n");
                fprintf(fp, "PUSH RCX\n");
                fprintf(fp, "PUSH RSP\n");
                fprintf(fp, "CALL printf\n");
                fprintf(fp, "POP RSP\n");
                fprintf(fp, "POP RCX\n");
                fprintf(fp, "POP RBX\n");
                fprintf(fp, "POP RAX\n");
            }
        }

        

    }
}

void codegen_stat_array(quadruple* q,FILE *fp)
{
    int lower_bound = q->arg1_st_entry->idType->leftIndex;
    int upper_bound = q->arg1_st_entry->idType->rightIndex;
    
    int offset = q->arg1_st_entry->offsetAct;

    fprintf(fp, "MOV RAX, %d\n", lower_bound);
    fprintf(fp, "MOV RBX, %d\n", upper_bound);
    fprintf(fp, "MOV [RBP-%d], RAX\n", offset+8);
    fprintf(fp, "MOV [RBP-%d], RBX\n", offset+16);
}

void codegen_array_access(quadruple* q,int index,FILE *fp)
{
    int lower_bound = q->arg1_st_entry->idType->leftIndex;
    int upper_bound = q->arg1_st_entry->idType->rightIndex;
    
    int offset = q->arg1_st_entry->offsetAct;

    char* label = (char*)malloc(sizeof(char)*10);
    char* label2 = (char*)malloc(sizeof(char)*10);

    sprintf(label, "OUT_OF_BOUND%d", index);
    sprintf(label2, "IN_BOUND%d", index);

    fprintf(fp, "MOV RAX, [RBP-%d]\n", offset+8);
    fprintf(fp, "MOV RBX, [RBP-%d]\n", offset+16);
    fprintf(fp, "MOV RCX, %d\n", q->arg2_num);
    fprintf(fp, "CMP RCX, RAX\n");
    fprintf(fp, "JL %s\n", label);
    fprintf(fp, "CMP RCX, RBX\n");
    fprintf(fp, "JG %s\n", label);

    
    fprintf(fp, "ADD RCX, 24\n");
    fprintf(fp, "MOV RBX, RBP\n");
    fprintf(fp, "SUB RBX, RCX\n");
    fprintf(fp, "MOV RAX, [RBX]\n");
    fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);

    fprintf(fp, "JMP %s\n", label2);
    fprintf(fp, "%s:\n", label);
    fprintf(fp, "JMP array_exit\n");
    fprintf(fp, "%s:\n", label2);
    
}

void codegen_get_value(quadruple* q,FILE* fp)
{
    if(q->arg1_type == ST_ENTRY_QUAD)
    {

        fprintf(fp, "MOV RSI, [RBP-%d]\n", q->arg1_st_entry->offsetAct);
        fprintf(fp, "MOV RDI, inputmsg\n");
        fprintf(fp, "XOR RAX, RAX\n");
        fprintf(fp, "PUSH RAX\n");
        fprintf(fp, "PUSH RBX\n");
        fprintf(fp, "PUSH RCX\n");
        fprintf(fp, "PUSH RSP\n");
        fprintf(fp, "CALL printf\n");
        fprintf(fp, "POP RSP\n");
        fprintf(fp, "POP RCX\n");
        fprintf(fp, "POP RBX\n");
        fprintf(fp, "POP RAX\n");

        if(q->arg1_st_entry->idType->is_array == 1)
        {
            for(int i=q->arg1_st_entry->idType->leftIndex;i<=q->arg1_st_entry->idType->rightIndex;i++)
            {
                fprintf(fp, "MOV RSI, var1\n");
                fprintf(fp, "MOV RDI, formatInp\n");
                fprintf(fp, "XOR RAX, RAX\n");
                fprintf(fp, "PUSH RAX\n");
                fprintf(fp, "PUSH RBX\n");
                fprintf(fp, "PUSH RCX\n");
                fprintf(fp, "PUSH RSP\n");
                fprintf(fp, "CALL scanf\n");
                fprintf(fp, "POP RSP\n");
                fprintf(fp, "POP RCX\n");
                fprintf(fp, "POP RBX\n");
                fprintf(fp, "POP RAX\n");
                fprintf(fp, "MOV RAX, [var1]\n");
                fprintf(fp, "MOV [RBP-%d], RAX\n", q->arg1_st_entry->offsetAct + 24 + (i)*8);
            }
        }
        else
        {
            fprintf(fp, "MOV RSI, var1\n");
            fprintf(fp, "MOV RDI, formatInp\n");
            fprintf(fp, "XOR RAX, RAX\n");
            fprintf(fp, "PUSH RAX\n");
            fprintf(fp, "PUSH RBX\n");
            fprintf(fp, "PUSH RCX\n");
            fprintf(fp, "PUSH RSP\n");
            fprintf(fp, "CALL scanf\n");
            fprintf(fp, "POP RSP\n");
            fprintf(fp, "POP RCX\n");
            fprintf(fp, "POP RBX\n");
            fprintf(fp, "POP RAX\n");
            fprintf(fp, "MOV RAX, [var1]\n");
            fprintf(fp, "MOV [RBP-%d], RAX\n", q->arg1_st_entry->offsetAct);
        }
        
    }
}

void codegen_arithm_mul(quadruple* q,FILE *fp)
{
    if(q->arg1_type == ST_ENTRY_QUAD && q->arg2_type == ST_ENTRY_QUAD)
    {
        //store one in eax and then call mul
        
        fprintf(fp, "MOV EAX, [RBP-%d]\n", q->arg1_st_entry->offsetAct);
        fprintf(fp, "MOV EBX, [RBP-%d]\n", q->arg2_st_entry->offsetAct);
        fprintf(fp, "MUL EBX\n");
        fprintf(fp, "MOV [RBP-%d], EAX\n", q->result->offsetAct);
        fprintf(fp, "MOV [RBP-%d], EDX\n", q->result->offsetAct-4);

    }
}

void codegen_logical_or(quadruple* q,FILE* fp)
{
    fprintf(fp, "MOV RAX, [RBP-%d]\n", q->arg1_st_entry->offsetAct);
    fprintf(fp, "OR RAX, [RBP-%d]\n", q->arg2_st_entry->offsetAct);
    fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);

}

void codegen_logical_and(quadruple* q,FILE* fp)
{
    fprintf(fp, "MOV RAX, [RBP-%d]\n", q->arg1_st_entry->offsetAct);
    fprintf(fp, "AND RAX, [RBP-%d]\n", q->arg2_st_entry->offsetAct);
    fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);

}

void codegen_relational_lt(quadruple* q,int index,FILE* fp)
{
    char* if_lt_label = (char*)malloc(sizeof(char)*20);
    sprintf(if_lt_label, "if_lt_%d", index);
    char* if_not_lt_label = (char*)malloc(sizeof(char)*20);
    sprintf(if_not_lt_label, "if_not_lt_%d", index);
    char* exit_label = (char*)malloc(sizeof(char)*20);
    sprintf(exit_label, "exit_%d", index);
    fprintf(fp, "MOV RAX, [RBP-%d]\n", q->arg1_st_entry->offsetAct);
    fprintf(fp, "CMP RAX, [RBP-%d]\n", q->arg2_st_entry->offsetAct);
    fprintf(fp, "JL %s\n", if_lt_label);
    fprintf(fp, "JMP %s\n", if_not_lt_label);
    fprintf(fp, "%s:\n", if_lt_label);
    fprintf(fp, "MOV RAX, 1\n");
    fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);
    fprintf(fp, "JMP %s\n", exit_label);
    fprintf(fp, "%s:\n", if_not_lt_label);
    fprintf(fp, "MOV RAX, 0\n");
    fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);
    fprintf(fp, "%s:\n", exit_label);
}

void codegen_relational_le(quadruple* q,int index,FILE* fp)
{

    if(q->arg1_type == ST_ENTRY_QUAD && q->arg2_type == ST_ENTRY_QUAD)
    {char* if_lte_label = (char*)malloc(sizeof(char)*20);
    sprintf(if_lte_label, "if_lte_%d", index);
    char* if_not_lte_label = (char*)malloc(sizeof(char)*20);
    sprintf(if_not_lte_label, "if_not_lte_%d", index);
    char* exit_label = (char*)malloc(sizeof(char)*20);
    sprintf(exit_label, "exit_%d", index);
    fprintf(fp, "MOV RAX, [RBP-%d]\n", q->arg1_st_entry->offsetAct);
    fprintf(fp, "CMP RAX, [RBP-%d]\n", q->arg2_st_entry->offsetAct);
    fprintf(fp, "JLE %s\n", if_lte_label);
    fprintf(fp, "JMP %s\n", if_not_lte_label);
    fprintf(fp, "%s:\n", if_lte_label);
    fprintf(fp, "MOV RAX, 1\n");
    fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);
    fprintf(fp, "JMP %s\n", exit_label);
    fprintf(fp, "%s:\n", if_not_lte_label);
    fprintf(fp, "MOV RAX, 0\n");
    fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);
    fprintf(fp, "%s:\n", exit_label);
} else if(q->arg1_type == ST_ENTRY_QUAD && q->arg2_type == NUM_QUAD){
    char* if_lte_label = (char*)malloc(sizeof(char)*20);
    sprintf(if_lte_label, "if_lte_%d", index);
    char* if_not_lte_label = (char*)malloc(sizeof(char)*20);
    sprintf(if_not_lte_label, "if_not_lte_%d", index);
    char* exit_label = (char*)malloc(sizeof(char)*20);
    sprintf(exit_label, "exit_%d", index);
    fprintf(fp, "MOV RAX, [RBP-%d]\n", q->arg1_st_entry->offsetAct);
    fprintf(fp, "CMP RAX, %d\n", q->arg2_num);
    fprintf(fp, "JLE %s\n", if_lte_label);
    fprintf(fp, "JMP %s\n", if_not_lte_label);
    fprintf(fp, "%s:\n", if_lte_label);
    fprintf(fp, "MOV RAX, 1\n");
    fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);
    fprintf(fp, "JMP %s\n", exit_label);
    fprintf(fp, "%s:\n", if_not_lte_label);
    fprintf(fp, "MOV RAX, 0\n");
    fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);
    fprintf(fp, "%s:\n", exit_label);
} else if(q->arg1_type == NUM_QUAD  && q->arg2_type == ST_ENTRY_QUAD){
    char* if_lte_label = (char*)malloc(sizeof(char)*20);
    sprintf(if_lte_label, "if_lte_%d", index);
    char* if_not_lte_label = (char*)malloc(sizeof(char)*20);
    sprintf(if_not_lte_label, "if_not_lte_%d", index);
    char* exit_label = (char*)malloc(sizeof(char)*20);
    sprintf(exit_label, "exit_%d", index);
    fprintf(fp, "MOV RAX, %d\n", q->arg1_num);
    fprintf(fp, "CMP RAX, [RBP-%d]\n", q->arg2_st_entry->offsetAct);
    fprintf(fp, "JLE %s\n", if_lte_label);
    fprintf(fp, "JMP %s\n", if_not_lte_label);
    fprintf(fp, "%s:\n", if_lte_label);
    fprintf(fp, "MOV RAX, 1\n");
    fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);
    fprintf(fp, "JMP %s\n", exit_label);
    fprintf(fp, "%s:\n", if_not_lte_label);
    fprintf(fp, "MOV RAX, 0\n");
    fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);
    fprintf(fp, "%s:\n", exit_label);
}
}

void codegen_relational_gt(quadruple *q,int index,FILE* fp)
{
    if(q->arg1_type == ST_ENTRY_QUAD && q->arg2_type == ST_ENTRY_QUAD)
    {
        char* if_gt_label = (char*)malloc(sizeof(char)*20);
        sprintf(if_gt_label, "if_gt_%d", index);
        char* if_not_gt_label = (char*)malloc(sizeof(char)*20);
        sprintf(if_not_gt_label, "if_not_gt_%d", index);
        char* exit_label = (char*)malloc(sizeof(char)*20);
        sprintf(exit_label, "exit_%d", index);
        fprintf(fp, "MOV RAX, [RBP-%d]\n", q->arg1_st_entry->offsetAct);
        fprintf(fp, "MOV RBX, [RBP-%d]\n", q->arg2_st_entry->offsetAct);
        fprintf(fp, "CMP RAX, RBX\n");
        fprintf(fp, "JG %s\n", if_gt_label);
        fprintf(fp, "JMP %s\n", if_not_gt_label);
        fprintf(fp, "%s:\n", if_gt_label);
        fprintf(fp, "MOV RAX, 1\n");
        fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);
        fprintf(fp, "JMP %s\n", exit_label);
        fprintf(fp, "%s:\n", if_not_gt_label);
        fprintf(fp, "MOV RAX, 0\n");
        fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);
        fprintf(fp, "%s:\n", exit_label);
    }
    else if(q->arg1_type == ST_ENTRY_QUAD && q->arg2_type == NUM_QUAD)
    {
        char* if_gt_label = (char*)malloc(sizeof(char)*20);
        sprintf(if_gt_label, "if_gt_%d", index);
        char* if_not_gt_label = (char*)malloc(sizeof(char)*20);
        sprintf(if_not_gt_label, "if_not_gt_%d", index);
        char* exit_label = (char*)malloc(sizeof(char)*20);
        sprintf(exit_label, "exit_%d", index);
        fprintf(fp, "MOV RAX, [RBP-%d]\n", q->arg1_st_entry->offsetAct);
        fprintf(fp, "CMP RAX, %d\n", q->arg2_num);
        fprintf(fp, "JG %s\n", if_gt_label);
        fprintf(fp, "JMP %s\n", if_not_gt_label);
        fprintf(fp, "%s:\n", if_gt_label);
        fprintf(fp, "MOV RAX, 1\n");
        fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);
        fprintf(fp, "JMP %s\n", exit_label);
        fprintf(fp, "%s:\n", if_not_gt_label);
        fprintf(fp, "MOV RAX, 0\n");
        fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);
        fprintf(fp, "%s:\n", exit_label);
    }
     
}

void codegen_relational_ge(quadruple *q,int index,FILE* fp)
{
    char* if_gte_label = (char*)malloc(sizeof(char)*20);
    sprintf(if_gte_label, "if_gte_%d", index);
    char* if_not_gte_label = (char*)malloc(sizeof(char)*20);
    sprintf(if_not_gte_label, "if_not_gte_%d", index);
    char* exit_label = (char*)malloc(sizeof(char)*20);
    sprintf(exit_label, "exit_%d", index);
    fprintf(fp, "MOV RAX, [RBP-%d]\n", q->arg1_st_entry->offsetAct);
    fprintf(fp, "CMP RAX, [RBP-%d]\n", q->arg2_st_entry->offsetAct);
    fprintf(fp, "JGE %s\n", if_gte_label);
    fprintf(fp, "JMP %s\n", if_not_gte_label);
    fprintf(fp, "%s:\n", if_gte_label);
    fprintf(fp, "MOV RAX, 1\n");
    fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);
    fprintf(fp, "JMP %s\n", exit_label);
    fprintf(fp, "%s:\n", if_not_gte_label);
    fprintf(fp, "MOV RAX, 0\n");
    fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);
    fprintf(fp, "%s:\n", exit_label);
}

void codegen_relational_eq(quadruple *q,int index,FILE* fp)
{   
    if(q->arg1_type==ST_ENTRY_QUAD && q->arg2_type==ST_ENTRY_QUAD)
    {
        char* if_eq_label = (char*)malloc(sizeof(char)*20);
        sprintf(if_eq_label, "if_eq_%d", index);
        char* if_not_eq_label = (char*)malloc(sizeof(char)*20);
        sprintf(if_not_eq_label, "if_not_eq_%d", index);
        char* exit_label = (char*)malloc(sizeof(char)*20);
        sprintf(exit_label, "exit_%d", index);
        fprintf(fp, "MOV RAX, [RBP-%d]\n", q->arg1_st_entry->offsetAct);
        fprintf(fp, "MOV RBX, [RBP-%d]\n", q->arg2_st_entry->offsetAct);
        fprintf(fp, "CMP RAX, RBX\n");
        fprintf(fp, "JE %s\n", if_eq_label);
        fprintf(fp, "JMP %s\n", if_not_eq_label);
        fprintf(fp, "%s:\n", if_eq_label);
        fprintf(fp, "MOV RAX, 1\n");
        fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);
        fprintf(fp, "JMP %s\n", exit_label);
        fprintf(fp, "%s:\n", if_not_eq_label);
        fprintf(fp, "MOV RAX, 0\n");
        fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);
        fprintf(fp, "%s:\n", exit_label);
    }
}

void codegen_arithm_minus(quadruple* q,FILE* fp)
{
    if(q->arg1_type == NUM_QUAD && q->arg2_type == NUM_QUAD)
    {
        fprintf(fp, "MOV RAX, %d\n", q->arg1_num);
        fprintf(fp, "SUB RAX, %d\n", q->arg2_num);
        fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);
    }
    else if(q->arg1_type == NUM_QUAD && q->arg2_type == ST_ENTRY_QUAD)
    {
        fprintf(fp, "MOV RAX, %d\n", q->arg1_num);
        fprintf(fp, "SUB RAX, [RBP-%d]\n", q->arg2_st_entry->offsetAct);
        fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);

    }
    else if(q->arg1_type == ST_ENTRY_QUAD && q->arg2_type == NUM_QUAD)
    {
        fprintf(fp, "MOV RAX, [RBP-%d]\n", q->arg1_st_entry->offsetAct);
        fprintf(fp, "SUB RAX, %d\n", q->arg2_num);
        fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);

    }
    else if(q->arg1_type == ST_ENTRY_QUAD && q->arg2_type == ST_ENTRY_QUAD)
    {
        fprintf(fp, "MOV RAX, [RBP-%d]\n", q->arg1_st_entry->offsetAct);
        fprintf(fp, "SUB RAX, [RBP-%d]\n", q->arg2_st_entry->offsetAct);
        fprintf(fp, "MOV [RBP-%d], RAX\n", q->result->offsetAct);

    }
}\

void codegen_goto(quadruple* q,FILE* fp)
{
    fprintf(fp, "JMP %s\n", q->arg1_label);
}

void codegen_label(quadruple* q,FILE* fp)
{
    fprintf(fp, "%s:\n", q->arg1_label);
}

void codegen_jmp_if_true(quadruple* q,FILE* fp)
{
    fprintf(fp, "MOV RAX, [RBP-%d]\n", q->arg1_st_entry->offsetAct);
    fprintf(fp, "CMP RAX, 1\n");
    fprintf(fp, "JE %s\n", q->arg2_label);
}

void codegen(quadrupleList *quadlist, FILE* fp)
{
    // FILE *fp = fopen("output.asm", "w");
    // if (fp == NULL)
    // {
    //     printf("Error opening file\n");
    //     exit(1);
    // }

    fprintf(fp,"default rel\n");
    fprintf(fp,"section .data\nformat db 'Output: %%d\', 10, 0\n");
    fprintf(fp,"formatInp db \"%%d\", 0\n");
    fprintf(fp,"inputmsg db 'Input: Enter an integer value',10\n");
    fprintf(fp,"section .bss\nmy_stack resb 1024\n");
    fprintf(fp,"var1 resd 1\n");
    fprintf(fp,"section .text\n");
    fprintf(fp,"global main\n");
    fprintf(fp,"extern printf\n");
    fprintf(fp,"extern scanf\n");
    fprintf(fp,"extern exit\n");
    fprintf(fp,"main:\n");
    fprintf(fp,"push RBP\n");
    fprintf(fp,"mov RBP, RSP\n");

    fprintf(fp,"sub RSP, 2048\n\n\n");

    // print all the global variables
    quadrupleList *qd = quadlist;
    qd = qd->next;
    int i = 0;
    while (qd != NULL)
    {
        i++;
        quadruple *quad = qd->quad;

        if(strcmp(quad->op,three_code_list[ASSIGN_3CODE])==0){
            codegen_assign( qd->quad, fp);
        }else if(strcmp(quad->op,"+")==0){
            codegen_arithm_plus( qd->quad, fp);
        }
        else if(strcmp(quad->op,three_code_list[PRINT_3CODE])==0)
        {
            codegen_print( qd->quad, fp);
        }
        else if(strcmp(quad->op,"*")==0 || strcmp(quad->op,"MUL")==0)
        {
            codegen_arithm_mul( qd->quad, fp);
        }
        else if(strcmp(quad->op,"-")==0 || strcmp(quad->op,"SUB")==0)
        {
            codegen_arithm_minus( qd->quad, fp);
        }
        else if(strcmp(quad->op,three_code_list[GET_VALUE_3CODE])==0)
        {
            codegen_get_value( qd->quad, fp);
        }
        else if(strcmp(quad->op,"OR")==0)
        {
            codegen_logical_or( qd->quad, fp);
        }
        else if(strcmp(quad->op,"<")==0)
        {
            codegen_relational_lt( qd->quad,i, fp);
        }
        else if(strcmp(quad->op,"<=")==0 || strcmp(quad->op,"LESS_THAN_EQUAL")==0)
        {
            codegen_relational_le( qd->quad,i, fp);
        }
        else if(strcmp(quad->op,">")==0)
        {
            codegen_relational_gt( qd->quad,i, fp);
        }
        else if(strcmp(quad->op,">=")==0)
        {
            codegen_relational_ge( qd->quad,i, fp);
        }
        else if(strcmp(quad->op,"==")==0)
        {
            codegen_relational_eq( qd->quad,i, fp);
        }
        else if(strcmp(quad->op,"AND")==0)
        {
            codegen_logical_and( qd->quad, fp);
        }
        else if(strcmp(quad->op,three_code_list[MODULE_3CODE])==0)
        {

        }
        else if(strcmp(quad->op,"STAT_ARRAY_DEC")==0)
        {
            codegen_stat_array( qd->quad, fp);
        }
        else if(strcmp(quad->op,three_code_list[GOTO_3CODE])==0)
        {
            codegen_goto( qd->quad, fp);
        }
        else if(strcmp(quad->op,three_code_list[LABEL_3CODE])==0){
            codegen_label( qd->quad, fp);
        }
    
        else if(strcmp(quad->op,three_code_list[JUMP_IF_TRUE_3CODE])==0){
            codegen_jmp_if_true( qd->quad, fp);
        }

       
        qd = qd->next;
        fprintf(fp,"\n\n");

    }
    

    fprintf(fp,"\n\nxor RAX,RAX\n");
    fprintf(fp,"xor RBX,RBX\n");
    fprintf(fp,"mov RSP,RBP\n");
    fprintf(fp,"pop RBP\n");
    fprintf(fp,"ret\n");



}
