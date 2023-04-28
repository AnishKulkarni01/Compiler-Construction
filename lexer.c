//CS F363 : Compiler Construction
//Group No. 24
//1. Nachiket Kotalwar               (2020A7PS0024P)
//2.Parth Patel                      (2020A7PS0026P)
//3.Partha Sarathi Purkayastha       (2020A7PS0043P)
//4.Labeeb Ahsan                     (2020A7PS0045P)
//5.Anish Atul Kulkarni              (2020A7PS0975P)

#include "lexerDef.h"


//initialized the buffer to null
void clearBuf(char *buf)
{
    int i;
    for (i = 0; i < TWIN_BUF_SIZE; i++)
    {
        buf[i] = '\0';
    }
}

//reads the file and stores it in the buffer
int readChar(FILE *fp, char *buf)
{
    clearBuf(buf);
    int i = 0;
    char c;
    while (i < (TWIN_BUF_SIZE - 1) && ((c = fgetc(fp)) != EOF))
    {
        buf[i] = c;
        i++;
    }
    return i;
}

//function to remove comments and prints it to the command line
int removeComments(const char *sourceFile)
{
    FILE *fp1 = fopen(sourceFile, "r");
    //FILE *fp2 = fopen(outputFile, "w");

    if (fp1 == NULL)
    {
        printf("File not found\n");
        return 0;
    }
    char c;
    while ((c = fgetc(fp1)) != EOF)
    {
        //printf("%c here\n", c);
        if (c == '*')
        {
            c = fgetc(fp1);
            if (c == EOF)
            {
                //fputc('*', fp2);
                printf("%c", '*');
                break;
            }
            if (c == '*')
            {
                while ((c = fgetc(fp1)) != EOF)
                {
                    if (c == '*')
                    {
                        c = fgetc(fp1);
                        if (c == '*' || c == EOF)
                        {
                            break;
                        }
                    }
                }
            }
            else
            {
                //fputc('*', fp2);
                printf("%c", '*');
                //fputc(c, fp2);
                printf("%c", c);
                //printf("%c here1\n", c);
            }
        }
        else
        {
            //fputc(c, fp2);
            printf("%c", c);
            //printf("%c here2\n", c);
        }
    }
}

//function to get the next buffer
FILE *getStream(FILE *fp)
{
    if (bufInUse == 1)
    {
        int i = readChar(fp, buf1);
        bufInUse = 2;
        if (i < (TWIN_BUF_SIZE - 1))
        {
            return NULL;
        }
    }
    else
    {
        int i = readChar(fp, buf2);
        bufInUse = 1;
        if (i < (TWIN_BUF_SIZE - 1))
        {
            return NULL;
        }
    }
    return fp;
}

//code to reinitialize the lexer by reinitializing the buffers and file pointers
void initializeLexer(const char* sourceFile,int buffer_size)
{
    ht = initialize();
    fp1 = fopen(sourceFile, "r");

    if (fp1 == NULL)
    {
        printf("File not found\n");
    }

    bufInUse = 1;
    line_no = 1;
    buf1=(char*)malloc(buffer_size*sizeof(char));
    buf2=(char*)malloc(buffer_size*sizeof(char));

    s = 0;
    i = 0;

    endPointer = -1;
    endPointerBuffer = 1;
    startPointer = 0;
    startPointerBuffer = 1;
    tokenized = 0;

    readNextChar = 1;
    endFlag = 0;
    bufferEnd = 1;

    notlast = 1;
}

//function to print all the tokens
int getLexemes(const char* sourceFile,int buffer_size)
{

    initializeLexer(sourceFile, buffer_size);

    lexeme *l ;

    while(l=getNextToken()){
        printf("Line no: %4d    Lexeme: -%s-    Token: %-15s Value: %f\n", l->line_no, l->lexeme, l->token,(l->token == token_list[NUM] )?l->value_if_num:((l->token == token_list[RNUM])?l->value_if_rnum:0));
    }

    printf("lexical Analysis Complete\n");

    fclose(fp1);
}


//helper fucntion to get the size of the lexeme across or in the same buffer
int sizeOfLexeme(int start, int startPointerBuffer, int end, int endPointerBuffer)
{
    if(startPointerBuffer==1 && endPointerBuffer==1){
        return (end-start+1);
    }
    else if(startPointerBuffer==2 && endPointerBuffer==2){
        return (end-start+1);
    }
    else if(startPointerBuffer==1 && endPointerBuffer==2){
        return (end-start+TWIN_BUF_SIZE);
    }
    else if(startPointerBuffer==2 && endPointerBuffer==1){
        return (end-start+TWIN_BUF_SIZE);
    }
}


//helper function to get the substring of the lexeme
char *substring(int start, int startPointerBuffer, int end, int endPointerBuffer)
{
    if (startPointerBuffer == 1 && endPointerBuffer == 1)
    {
        if (start > end)
        {
            char *test = (char *)malloc(sizeof(char));
            test[0] = '\0';
            return test;
        }
        char *sub = (char *)malloc(sizeof(char) * (end - start + 2));
        int i = 0;
        for (i = start; i <= end; i++)
        {
            sub[i - start] = buf1[i];
        }
        //sub[i] = '\0';
        sub[(end - start + 1)] = '\0';
        return sub;
    }
    else if (startPointerBuffer == 2 && endPointerBuffer == 2)
    {
        //printf("start: %d end: %d size:%d\n", start, end,(end-start+2));
        if (start > end)
        {
            char *test = (char *)malloc(sizeof(char));
            test[0] = '\0';
            return test;
        }
        char *sub = (char *)malloc(sizeof(char) * (end - start + 2));
        int i = 0;
        for (i = start; i <= end; i++)
        {
            sub[i - start] = buf2[i];
        }
        //sub[i] = '\0';
        sub[(end - start + 1)] = '\0';
        return sub;
    }
    else if (startPointerBuffer == 1 && endPointerBuffer == 2)
    {
        int size = TWIN_BUF_SIZE - start + end + 1;
        char *sub = (char *)malloc(sizeof(char) * (size));
        int i = 0;
        int j = 0;
        for (i = start; i < (TWIN_BUF_SIZE - 1); i++, j++)
        {
            sub[j] = buf1[i];
        }
        for (i = 0; i <= end; i++, j++)
        {
            sub[j] = buf2[i];
        }
        sub[j] = '\0';
        return sub;
    }
    else if (startPointerBuffer == 2 && endPointerBuffer == 1)
    {
        
        int size = TWIN_BUF_SIZE - start + end + 1;
        char *sub = (char *)malloc(sizeof(char) * (size));
        int i = 0;
        int j = 0;
        for (i = start; i < (TWIN_BUF_SIZE - 1); i++, j++)
        {
            sub[j] = buf2[i];
        }
        for (i = 0; i <= end; i++, j++)
        {
            sub[j] = buf1[i];
        }
        sub[j] = '\0';
        return sub;
    }
}

//function to get the next token and ignoring the comments, newlines, blanks, tabs and errors
lexeme* getNextToken()
{
    //printf("getNextToken startPointer: %d startPointerBuffer: %d endPointer: %d endPointerBuffer: %d\n",startPointer, startPointerBuffer,endPointer, endPointerBuffer);
    lexeme* l;
    l=nextToken();
    while(l!=NULL && (strcmp(l->token,token_list[COMMENTMARK])==0 || strcmp(l->token,token_list[END_OF_FILE])==0 || strcmp(l->token,token_list[NEWLINE])==0 || strcmp(l->token,token_list[ERROR])==0 || strcmp(l->token,token_list[BLANK])==0 || strcmp(l->token,token_list[TAB])==0 || strcmp(l->token,token_list[ID_LENGTH_ERROR])==0)){
        if(l!=NULL)
        {
            if(strcmp(l->token,token_list[ERROR])==0){
                printf("Lexical Error :- Lexeme: %s Line No: %d\n", l->lexeme, l->line_no);
            }
            else if(strcmp(l->token,token_list[ID_LENGTH_ERROR])==0){
                printf("Lexical Error :- Lexeme Length Exceeds 20 - Lexeme: %s Line No: %d\n", l->lexeme, l->line_no);
            }
        }
        l=nextToken();
    }

    if(l!=NULL)
    {
        if(l->token == token_list[NUM])
            l->value_if_num=atoi(l->lexeme);
        else if (l->token == token_list[RNUM])
            l->value_if_rnum=atof(l->lexeme);
        else
            l->value_if_num=-1;  
    }

    return l;
}

//function to get the next token
lexeme* nextToken()
{
    if(endFlag==1)
        return NULL;
    lexeme *l = (lexeme *)malloc(sizeof(lexeme));
    
    while (endFlag == 0)
    {
        if (bufferEnd == 1)
        {
            readNextChar = 1;
            i = 0;
            endPointer = -1;

            bufferEnd = 0;
            if (getStream(fp1) == NULL)
            {
                notlast = 0;
            }

            if (bufInUse == 1)
            {
                currentBuffer = buf2;
                if (startPointer == TWIN_BUF_SIZE - 1)
                {
                    startPointer = 0;
                    startPointerBuffer = 2;
                }
                endPointerBuffer = 2;
            }
            else
            {
                // printf("\nBuf1");
                currentBuffer = buf1;
                if (startPointer == TWIN_BUF_SIZE - 1)
                {
                    startPointer = 0;
                    startPointerBuffer = 1;
                }
                endPointerBuffer = 1;
            }
            //printf("\nBuffer --%s--", currentBuffer);
            //fflush(stdout);
        }
        // printf("\nLast --%d--", notlast);

        if (readNextChar == 1)
        {
            endPointer++;
            c = currentBuffer[i++];
            // printf("\nChar Read --%c--", c);
        }

        if (c == '\0')
        {
            bufferEnd = 1;
        }
        if (notlast == 0 && bufferEnd == 1)
        {
            //return NULL;
            endFlag = 1;
        }
        switch (s)
        {
            case (0):
            {
                readNextChar = 1;
                if(c=='\0'){
                    s=0;
                    readNextChar=1;
                    break;
                }
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_'))
                {
                    s = 1;
                }
                else if (c == '+')
                {
                    s = 2;
                }
                else if (c == '-')
                {
                    s = 3;
                }
                else if (c == '/')
                {
                    s = 4;
                }
                else if (c == '=')
                {
                    s = 5;
                }
                else if (c == ';')
                {
                    s = 7;
                }
                else if (c == '.')
                {
                    s = 8;
                }
                else if (c == ',')
                {
                    s = 10;
                }
                else if (c == '!')
                {
                    s = 11;
                }
                else if (c == '[')
                {
                    s = 13;
                }
                else if (c == ']')
                {
                    s = 14;
                }
                else if (c == '(')
                {
                    s = 15;
                }
                else if (c == ')')
                {
                    s = 16;
                }
                else if (c >= '0' && c <= '9')
                {
                    s = 17;
                }
                else if (c == '*')
                {
                    s = 23;
                }
                else if (c == ':')
                {
                    s = 27;
                }
                else if (c == '<')
                {
                    s = 29;
                }
                else if (c == '>')
                {
                    s = 33;
                }
                else if (c == '\n')
                {
                    s = 37;
                }
                else if (c == '\t')
                {
                    s = 38;
                }
                else if (c == ' ')
                {
                    s = 39;
                }
                else
                {
                    s = 40; // TRAP STATE
                    // printf("Invalid character %c", c);
                }
                break;
            }

            case (1):
            {
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '_'))
                {
                    s = 1;
                }
                else
                {
                    if (c == '\0' && notlast == 1)
                    {
                        s = 1;
                        readNextChar = 0;
                    }
                    else
                    {
                        
                        s = 0;
                        readNextChar = 0;
                        tokenized = 1;
                        //printf("\nStartPointer --%d-- StartBuffer --%d-- EndPointer --%d-- EndBuffer --%d--", startPointer, startPointerBuffer, endPointer, endPointerBuffer);
                        current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                        //printf("\nCurrent --%s--", current);
                        enum KEYWORD_ENUM* kw_id=(enum KEYWORD_ENUM*)get(ht, current);
                        int errorFlag=0;
                        if(sizeOfLexeme(startPointer, startPointerBuffer, endPointer, endPointerBuffer)>20)
                        {
                            errorFlag=1;
                            //printf("Error: Identifier %s at line %d is too long\n", current, line_no);
                        }
                        startPointer = endPointer;
                        startPointerBuffer = endPointerBuffer;
                        if(kw_id==NULL)
                            token = token_list[ID];
                        else
                            token = keyword_list[*kw_id];
                        if(errorFlag==1)
                            token = token_list[ID_LENGTH_ERROR];
                    }
                }
                break;
            }

            case (2):
            {
                // printf("\n Entering\n");
                //    printf("buf1: %s\n",buf1);
                //     printf("buf2: %s\n",buf2);
                //     printf("buffer: %s\n",currentBuffer);
                //     printf("startpointer: %d\n",startPointer);
                //     printf("startpointerbuffer: %d\n",startPointerBuffer);
                //     printf("endpointer: %d\n",endPointer);
                //     printf("endpointerbuffer: %d\n",endPointerBuffer);
                //     printf("\n\n\n\n");
                token = token_list[PLUS];
                s = 0;
                readNextChar = 0;
                tokenized = 1;
                current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                startPointer = endPointer;
                startPointerBuffer = endPointerBuffer;
                //    printf("buf1: %s\n",buf1);
                //     printf("buf2: %s\n",buf2);
                //     printf("buffer: %s\n",currentBuffer);
                //     printf("startpointer: %d\n",startPointer);
                //     printf("startpointerbuffer: %d\n",startPointerBuffer);
                //     printf("endpointer: %d\n",endPointer);
                //     printf("endpointerbuffer: %d\n",endPointerBuffer);
                
                break;
            }

            case (3):
            {
                token = token_list[MINUS];
                s = 0;
                readNextChar = 0;
                tokenized = 1;
                current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                startPointer = endPointer;
                startPointerBuffer = endPointerBuffer;

                
                break;
            }

            case (4):
            {
                token = token_list[DIV];
                s = 0;
                readNextChar = 0;
                tokenized = 1;
                current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                startPointer = endPointer;
                startPointerBuffer = endPointerBuffer;

                
                break;
            }

            case (5):
            {
                if (c == '=')
                {
                    s = 6;
                }
                else
                {
                    // ERROR
                    if (c == '\0' && notlast == 1)
                    {
                        s = 5;
                        readNextChar = 0;
                    }
                    else
                    {
                        s=40;
                        readNextChar = 0;
                        // token = token_list[ERROR];
                        // s = 0;
                        // readNextChar = 0;
                        // current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                        // startPointer = endPointer;
                        // startPointerBuffer = endPointerBuffer;
                    }
                }
                break;
            }

            case (6):
            {
                token = token_list[EQ];
                s = 0;
                readNextChar = 0;
                current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                startPointer = endPointer;
                startPointerBuffer = endPointerBuffer;
                break;
            }

            case (7):
            {
                token = token_list[SEMICOL];
                s = 0;
                readNextChar = 0;
                current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                startPointer = endPointer;
                startPointerBuffer = endPointerBuffer;
                 
                break;
            }

            case (8):
            {
                if (c == '.')
                {

                    s = 9;
                }
                else
                {
                    if (c == '\0' && notlast == 1)
                    {
                        s = 8;
                        readNextChar = 0;
                    }
                    else
                    {
                        // printf("\n\nERROR\n\n");
                        s=40;
                        readNextChar = 0;
                        // token = token_list[ERROR];
                        // s = 0;
                        // readNextChar = 0;
                        // current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                        // startPointer = endPointer;
                        // startPointerBuffer = endPointerBuffer;
                    }
                    // ERROR
                }
                break;
            }

            case (9):
            {
                token = token_list[RANGEOP];
                s = 0;
                readNextChar = 0;
                current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                startPointer = endPointer;
                startPointerBuffer = endPointerBuffer;
                break;
            }

            case (10):
            {
                token = token_list[COMMA];
                s = 0;
                readNextChar = 0;
                current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                startPointer = endPointer;
                startPointerBuffer = endPointerBuffer;
                break;
            }

            case (11):
            {
                if (c == '=')
                {
                    s = 12;
                }
                else
                {
                    // ERROR
                    if (c == '\0' && notlast == 1)
                    {
                        s = 11;
                        readNextChar = 0;
                    }
                    else
                    {
                        s=40;
                        readNextChar = 0;
                        // token = token_list[ERROR];
                        // s = 0;
                        // readNextChar = 0;
                        // current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                        // startPointer = endPointer;
                        // startPointerBuffer = endPointerBuffer;
                    }
                }
                break;
            }

            case (12):
            {
                token = token_list[NE];
                s = 0;
                readNextChar = 0;
                current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                startPointer = endPointer;
                startPointerBuffer = endPointerBuffer;
                break;
            }

            case (13):
            {
                token = token_list[SQBO];
                s = 0;
                readNextChar = 0;
                current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                startPointer = endPointer;
                startPointerBuffer = endPointerBuffer;
                break;
            }

            case (14):
            {
                token = token_list[SQBC];
                s = 0;
                readNextChar = 0;
                current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                startPointer = endPointer;
                startPointerBuffer = endPointerBuffer;
                break;
            }

            case (15):
            {
                token = token_list[BO];
                s = 0;
                readNextChar = 0;
                current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                startPointer = endPointer;
                startPointerBuffer = endPointerBuffer;
                break;
            }

            case (16):
            {
                token = token_list[BC];
                s = 0;
                readNextChar = 0;
                current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                startPointer = endPointer;
                startPointerBuffer = endPointerBuffer;
                break;
            }

            case (17):
            {
                if (c >= '0' && c <= '9')
                {
                    s = 17;
                }
                else if (c == '.')
                {
                    s = 18;
                }
                else
                {
                    if (c == '\0' && notlast == 1)
                    {
                        s = 17;
                        readNextChar = 0;
                    }
                    else
                    {
                        token = token_list[NUM];
                        s = 0;
                        readNextChar = 0;
                        current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                        startPointer = endPointer;
                        startPointerBuffer = endPointerBuffer;
                    }
                }
                break;
            }

            case (18):
            {
                if (c >= '0' && c <= '9')
                {
                    s = 19;
                }
                else if (c == '.')
                {
                    // RETRACT
                    //printf("retracting\n");
                    token = token_list[NUM];
                    s = 9;
                    tokenized = 1;
                    readNextChar = 1;
                    current = substring(startPointer, startPointerBuffer, endPointer - 2, endPointerBuffer);
                    if(endPointer>0){
                        startPointer = endPointer - 1;
                        startPointerBuffer = endPointerBuffer;
                    }else{
                        startPointer = TWIN_BUF_SIZE - 2;
                        startPointerBuffer = (endPointerBuffer == 1) ? 2 : 1;
                    }
                    
                 
                    // printf("buf1: %s\n",buf1);
                    // printf("buf2: %s\n",buf2);
                    // printf("buffer: %s\n",currentBuffer);
                    // printf("startpointer: %d\n",startPointer);
                    // printf("startpointerbuffer: %d\n",startPointerBuffer);
                    // printf("endpointer: %d\n",endPointer);
                    // printf("endpointerbuffer: %d\n",endPointerBuffer);
                }
                else
                {
                    // ERROR
                    if (c == '\0' && notlast == 1)
                    {
                        s = 18;
                        readNextChar = 0;
                    }
                    else
                    {
                        s=40;
                        readNextChar = 0;
                        // token = token_list[ERROR];
                        // s = 0;
                        // readNextChar = 0;
                        // current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                        // startPointer = endPointer;
                        // startPointerBuffer = endPointerBuffer;
                    }
                }
                break;
            }

            case (19):
            {
                if (c >= '0' && c <= '9')
                {
                    s = 19;
                }
                else if (c == 'E' || c == 'e')
                {
                    s = 20;
                }
                else
                {
                    if (c == '\0' && notlast == 1)
                    {
                        s = 19;
                        readNextChar = 0;
                    }
                    else
                    {
                        token = token_list[RNUM];
                        s = 0;
                        readNextChar = 0;
                        current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                        startPointer = endPointer;
                        startPointerBuffer = endPointerBuffer;
                    }
                }
                break;
            }

            case (20):
            {
                if (c == '+' || c == '-')
                {
                    s = 21;
                }
                else if (c >= '0' && c <= '9')
                {
                    s = 22;
                }
                else
                {
                    // ERROR
                    if (c == '\0' && notlast == 1)
                    {
                        s = 20;
                        readNextChar = 0;
                    }
                    else
                    {
                        s=40;
                        readNextChar = 0;
                        // token = token_list[ERROR];
                        // s = 0;
                        // readNextChar = 0;
                        // current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                        // startPointer = endPointer;
                        // startPointerBuffer = endPointerBuffer;
                    }
                }
                break;
            }

            case (21):
            {
                if (c >= '0' && c <= '9')
                {
                    s = 22;
                }
                else
                {
                    // ERROR
                    if (c == '\0' && notlast == 1)
                    {
                        s = 21;
                        readNextChar = 0;
                    }
                    else
                    {
                        s=40;
                        readNextChar = 0;
                        // token = token_list[ERROR];
                        // s = 0;
                        // readNextChar = 0;
                        // current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                        // startPointer = endPointer;
                        // startPointerBuffer = endPointerBuffer;
                    }
                }
                break;
            }

            case (22):
            {
                if (c >= '0' && c <= '9')
                {
                    s = 22;
                }
                else
                {
                    if (c == '\0' && notlast == 1)
                    {
                        s = 22;
                        readNextChar = 0;
                    }
                    else
                    {
                        token = token_list[RNUM];
                        s = 0;
                        readNextChar = 0;
                        current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                        startPointer = endPointer;
                        startPointerBuffer = endPointerBuffer;
                    }
                }
                break;
            }

            case (23):
            {
                //printf("\nReached here3");
                //fflush(stdout);
                if (c == '*')
                {
                    s = 24;
                }
                else
                {
                    if (c == '\0' && notlast == 1)
                    {
                        s = 23;
                        readNextChar = 0;
                    }
                    else
                    {
                        token = token_list[MUL];
                        s = 0;
                        readNextChar = 0;
                        current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                        startPointer = endPointer;
                        startPointerBuffer = endPointerBuffer;
                    }
                }
                break;
            }

            case (24):
            {
                //printf("\nReached here2");
                //fflush(stdout);
                if (c == '*')
                {
                    s = 25;
                }
                else if (c == '\n')
                {
                    s = 24;
                    line_no++;
                }
                else
                {
                    //s = 24;
                    if (c == '\0' && notlast == 1)
                    {
                        s = 24;
                        readNextChar = 0;
                    }
                    else if(c=='\0' && notlast == 0)
                    {
                        s=40;
                        readNextChar = 0;
                        // token = token_list[ERROR];
                        // s = 0;
                        // readNextChar = 0;
                        // current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                        // startPointer = endPointer;
                        // startPointerBuffer = endPointerBuffer;
                    }
                    else
                    {
                        s = 24;
                    }
                }
                break;
            }

            case (25):
            {
                //printf("\nReached here1");
                fflush(stdout);
                if (c == '*')
                {
                    s = 26;
                }
                else if (c == '\n')
                {
                    s = 24;
                    line_no++;
                }
                else
                {
                    //s = 24;
                    if (c == '\0' && notlast == 1)
                    {
                        s = 25;
                        readNextChar = 0;
                    }
                    else if(c=='\0')
                    {
                        s=40;
                        readNextChar = 0;
                        // token = token_list[ERROR];
                        // s = 0;
                        // readNextChar = 0;
                        // current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                        // startPointer = endPointer;
                        // startPointerBuffer = endPointerBuffer;
                    }
                    else
                    {
                        s = 24;
                    }
                }
                break;
            }

            case (26):
            {
                //printf("\nReached here");
                fflush(stdout);
                token = token_list[COMMENTMARK];
                readNextChar = 0;
                s = 0;
                //printf("\nStartPointer = %d, StartBuffer = %d, EndPointer = %d, EndBuffer = %d", startPointer, startPointerBuffer, endPointer, endPointerBuffer);
                current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                //printf("\nCurrent = %s", current);
                startPointer = endPointer;
                startPointerBuffer = endPointerBuffer;
                break;
            }

            case (27):
            {
                if (c == '=')
                {
                    s = 28;
                }
                else
                {
                    if (c == '\0' && notlast == 1)
                    {
                        s = 27;
                        readNextChar = 0;
                    }
                    else
                    {
                        token = token_list[COLON];
                        s = 0;
                        readNextChar = 0;
                        current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                        startPointer = endPointer;
                        startPointerBuffer = endPointerBuffer;
                    }
                }
                break;
            }

            case (28):
            {
                token = token_list[ASSIGNOP];
                s = 0;
                readNextChar = 0;
                current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                startPointer = endPointer;
                startPointerBuffer = endPointerBuffer;
                break;
            }

            case (29):
            {
                if (c == '=')
                {
                    s = 30;
                }
                else if (c == '<')
                {
                    s = 31;
                }
                else
                {
                    if (c == '\0' && notlast == 1)
                    {
                        s = 29;
                        readNextChar = 0;
                    }
                    else
                    {
                        token = token_list[LT];
                        s = 0;
                        readNextChar = 0;
                        current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                        startPointer = endPointer;
                        startPointerBuffer = endPointerBuffer;
                    }
                }
                break;
            }

            case (30):
            {
                token = token_list[LE];
                s = 0;
                readNextChar = 0;
                current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                startPointer = endPointer;
                startPointerBuffer = endPointerBuffer;
                break;
            }

            case (31):
            {
                if (c == '<')
                {
                    s = 32;
                }
                else
                {
                    if (c == '\0' && notlast == 1)
                    {
                        s = 31;
                        readNextChar = 0;
                    }
                    else
                    {
                        token = token_list[DEF];
                        s = 0;
                        readNextChar = 0;
                        current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                        startPointer = endPointer;
                        startPointerBuffer = endPointerBuffer;
                    }
                }
                break;
            }

            case (32):
            {
                token = token_list[DRIVERDEF];
                s = 0;
                readNextChar = 0;
                current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                startPointer = endPointer;
                startPointerBuffer = endPointerBuffer;
                break;
            }

            case (33):
            {
                if (c == '=')
                {
                    s = 34;
                }
                else if (c == '>')
                {
                    s = 35;
                }
                else
                {
                    if (c == '\0' && notlast == 1)
                    {
                        s = 33;
                        readNextChar = 0;
                    }
                    else
                    {
                        token = token_list[GT];
                        s = 0;
                        readNextChar = 0;
                        current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                        startPointer = endPointer;
                        startPointerBuffer = endPointerBuffer;
                    }
                }
                break;
            }

            case (34):
            {
                token = token_list[GE];
                s = 0;
                readNextChar = 0;
                current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                startPointer = endPointer;
                startPointerBuffer = endPointerBuffer;
                break;
            }

            case (35):
            {
                if (c == '>')
                {
                    s = 36;
                }
                else
                {
                    if (c == '\0' && notlast == 1)
                    {
                        s = 35;
                        readNextChar = 0;
                    }
                    else
                    {
                        token = token_list[ENDDEF];
                        s = 0;
                        readNextChar = 0;
                        current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                        startPointer = endPointer;
                        startPointerBuffer = endPointerBuffer;
                    }
                }
                break;
            }

            case (36):
            {
                token = token_list[DRIVERENDDEF];
                s = 0;
                readNextChar = 0;
                current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                startPointer = endPointer;
                startPointerBuffer = endPointerBuffer;
                break;
            }

            case (37):
            {
                token = token_list[NEWLINE];
                
                s = 0;
                readNextChar = 0;
                current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                startPointer = endPointer;
                startPointerBuffer = endPointerBuffer;
                line_no++;
                break;
            }

            case (38):
            {
                token = token_list[TAB];
                s = 0;
                readNextChar = 0;
                current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                startPointer = endPointer;
                startPointerBuffer = endPointerBuffer;
                break;
            }

            case (39):
            {
                token = token_list[BLANK];
                s = 0;
                readNextChar = 0;
                current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                startPointer = endPointer;
                startPointerBuffer = endPointerBuffer;

                l->lexeme = current;
                l->line_no = line_no;
                l->token = token;
                return l;
                break;
            }
            case (40):
            {
                token = token_list[ERROR];
                s = 0;
                readNextChar = 0;
                current = substring(startPointer, startPointerBuffer, endPointer - 1, endPointerBuffer);
                startPointer = endPointer;
                startPointerBuffer = endPointerBuffer;
            }
        }
        if(s==0 || tokenized==1){
            tokenized=0;
            l->lexeme = current;
            l->token = token;
            if(strcmp(token,token_list[NEWLINE])==0)
                l->line_no = line_no-1;
            else
                l->line_no = line_no;
            //printf("\nLexeme -%s",l->token);
            if(current == NULL)
            {
                l->lexeme = NULL;
                l->token = token_list[END_OF_FILE];
                l->line_no = -1;   
            }
            current=NULL;
            return l;
        }
    }
}