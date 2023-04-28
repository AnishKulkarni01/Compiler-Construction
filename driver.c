//CS F363 : Compiler Construction
//Group No. 24
//1. Nachiket Kotalwar               (2020A7PS0024P)
//2.Parth Patel                      (2020A7PS0026P)
//3.Partha Sarathi Purkayastha       (2020A7PS0043P)
//4.Labeeb Ahsan                     (2020A7PS0045P)
//5.Anish Atul Kulkarni              (2020A7PS0975P)

#include "parser.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[])
{
    //checking for correct number of arguments
    if(argc < 3)
    {
        printf("Usage: ./compiler <testcase_file> <code_asm_file\n");
        return 0;
    }
    // getLexemes("st1.txt",30);

    //parserDriver("t10.txt",40,"p.txt");

    //argv[3] is a number convert it to a int
    // int bufSize = atoi(argv[3]);

    int x=1;
    
    // if(bufSize == 0)
    // {
    //     printf("Error in converting buffer size to int\n");
    //     return 0;
    // }

    clock_t start_time, end_time;

    double total_CPU_time, total_CPU_time_in_seconds;

    // while(x!=0)
    // {
    //     printf("\n0 - Exit\n1 - Remove Comments\n2 - Printing Tokens\n3 - Parsing and syntactic correctness\n4 - Print time\n5 - Check if grammar is LL1(prints to console only in case of not ll1)\n");

    //     printf("Enter Choice: ");
    //     scanf("%d",&x);
    //     switch (x)
    //     {
    //     case 0:
    //         printf("\nExiting...\n");
    //         return 0;
    //     case 1://remove comments
    //         printf("\n\n.....................\n");
    //         removeComments(argv[1]);
    //         printf("\n.....................\n");
    //         break;
    //     case 2://printing token list
    //         printf("\n\n.....................\n");
    //         getLexemes(argv[1],bufSize);
    //         printf("\n.....................\n");
    //         break;
    //     case 3://parsing and printing parse tree
    //         printf("\n\n.....................\n");
    //         parserDriver(argv[1],bufSize,argv[2]);
    //         printf("\n.....................\n");
    //         break;
    //     case 4: //print time
    //         start_time = clock();

    //         printf("\n\n.....................\n");

    //         parserDriver(argv[1],bufSize,argv[2]);

    //         printf("\n.....................\n");

    //         end_time = clock();
    //         total_CPU_time = (double) (end_time - start_time);
    //         total_CPU_time_in_seconds = total_CPU_time/CLOCKS_PER_SEC;

    //         printf("Time Taken in ticks: %f || Time Taken in seconds: %f\n",total_CPU_time, total_CPU_time_in_seconds);

    //         printf("\n.....................\n");

    //         break;
    //     case 5://ll1 checker
    //         printf("\n\n.....................\n");
    //         ll1Check();
    //         printf(".....................\n");
    //         break;
    //     default:
    //         printf("Invalid Choice\n");
    //         break;
    //     }


    // }

    // getLexemes(argv[1]);
    // parserDriver(argv[1], argv[2],argv[3]);
    // return 0;


    // NEW DRIVER
    
    while(x!=0)
    {
        printf("\n0 - Exit\n1 - Lexer\n2 - Parser - syntactic correctness\n3 - AST \n4 - Memory allocated\n5 - Symbol table\n6 - Activation record size\n7 - Static and Dynamic Arrays\n8 - Error reporting and Total compile time\n9 - Code Generation\n");

        printf("Enter Choice: ");
        scanf("%d",&x);
        switch (x)
        {
        case 0:
            printf("\nExiting...\n");
            return 0;
        case 1://lexer
            printf("\n\n.....................\n");
            getLexemes(argv[1],30);
            printf("\n.....................\n");
            break;
        case 2://parser
            printf("\n\n.....................\n");
            parserDriver(argv[1],30,"p.txt");
            printf("\n.....................\n");
            break;
        case 3://ast
            printf("\n\n.....................\n");
            parserDriver_uptill_AST(argv[1],30,"p.txt"); 
            printf("\n.....................\n");
            break;
        case 4: //memory
            printf("\n\n.....................\n");
            parserDriver_Compression(argv[1],30,"p.txt");
            printf("\n.....................\n");
            break;
        case 5://st
            printf("\n\n.....................\n");
            
            parserDriver_uptill_SymbolTable(argv[1],30,"p.txt");

            printf(".....................\n");
            break;
        case 6://activation record size
            printf("\n\n.....................\n");
            parserDriver_uptill_ActivationRecord(argv[1],30,"p.txt");
            printf(".....................\n");
            break;
        case 7://static and dynamic arrays
            printf("\n\n.....................\n");
            parserDriver_uptill_Arrays(argv[1],30,"p.txt");
            printf(".....................\n");
            break;
        case 8://error reporting and total compile time
            printf("\n\n.....................\n");
            //Error reporting and total compile time
            //in ticks and seconds

            start_time = clock();

            printf("\n\n.....................\n");

            parserDriver_uptill_CodeGen(argv[1],20,argv[2]);

            printf("\n.....................\n");

            end_time = clock();
            total_CPU_time = (double) (end_time - start_time);
            total_CPU_time_in_seconds = total_CPU_time/CLOCKS_PER_SEC;

            printf("Time Taken in ticks: %f || Time Taken in seconds: %f\n",total_CPU_time, total_CPU_time_in_seconds);
            printf(".....................\n");
            break;
        case 9://code generation
            printf("\n\n.....................\n");
            parserDriver_uptill_CodeGen(argv[1],30,argv[2]);
            printf(".....................\n");
            break;        
        
        default:
            printf("Invalid Choice\n");
            break;
        }


    }
    
}