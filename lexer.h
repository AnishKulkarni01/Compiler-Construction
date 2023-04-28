//CS F363 : Compiler Construction
//Group No. 24
//1. Nachiket Kotalwar               (2020A7PS0024P)
//2.Parth Patel                      (2020A7PS0026P)
//3.Partha Sarathi Purkayastha       (2020A7PS0043P)
//4.Labeeb Ahsan                     (2020A7PS0045P)
//5.Anish Atul Kulkarni              (2020A7PS0975P)

#ifndef LEXER_H
#define LEXER_H

typedef struct lexeme {
    char* lexeme;
    const char* token;
    int line_no;
    int value_if_num;
    float value_if_rnum;
} lexeme;

int removeComments(const char *sourceFile);
int getLexemes(const char* sourceFile,int buffer_size);
void initializeLexer(const char* sourceFile, int buffer_size);
lexeme* getNextToken();
lexeme* nextToken();

#endif