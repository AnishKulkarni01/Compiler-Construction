//CS F363 : Compiler Construction
//Group No. 24
//1. Nachiket Kotalwar               (2020A7PS0024P)
//2.Parth Patel                      (2020A7PS0026P)
//3.Partha Sarathi Purkayastha       (2020A7PS0043P)
//4.Labeeb Ahsan                     (2020A7PS0045P)
//5.Anish Atul Kulkarni              (2020A7PS0975P)

#ifndef LEXERDEF_H
#define LEXERDEF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
# define TWIN_BUF_SIZE 20
#include "chash.h"
#include "enum.h"

typedef struct lexeme {
    char* lexeme;
    const char* token;
    int line_no;
    int value_if_num;
    float value_if_rnum;
} lexeme;

void initializeLexer(const char* sourceFile,int buffer_size);
lexeme* getNextToken();
lexeme* nextToken();

int bufInUse = 1;
int line_no = 1;
char* buf1;
char* buf2;

hash_table* ht;

FILE *fp1;

char c;
char *currentBuffer;
int s = 0;
int i = 0;
const char *token;
char *current;

int endPointer = -1;
int endPointerBuffer = 1;
int startPointer = 0;
int startPointerBuffer = 1;
int tokenized = 0;

int readNextChar = 1;
int endFlag = 0;
int bufferEnd = 1;

int notlast = 1;

#endif