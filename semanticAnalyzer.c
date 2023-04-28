//CS F363 : Compiler Construction
//Group No. 24
//1. Nachiket Kotalwar               (2020A7PS0024P)
//2.Parth Patel                      (2020A7PS0026P)
//3.Partha Sarathi Purkayastha       (2020A7PS0043P)
//4.Labeeb Ahsan                     (2020A7PS0045P)
//5.Anish Atul Kulkarni              (2020A7PS0975P)

#include "semanticAnalyzerDef.h"
#include "symbolTable.h"

int getTypeWidth(char *type)
{
    if (strcmp(type, "integer") == 0)
    {
        return 2;
    }
    else if (strcmp(type, "real") == 0)
    {
        return 4;
    }
    else if (strcmp(type, "boolean") == 0)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

FILE *st_fp;

void initializeFp(FILE * fp)
{
    st_fp = fp;
}

int numSemanticErrors = 0;

int typeToIntID(char *type)
{
    if (strcmp(type, "integer") == 0)
    {
        return 1;
    }
    else if (strcmp(type, "real") == 0)
    {
        return 2;
    }
    else if (strcmp(type, "boolean") == 0)
    {
        return 3;
    }
    else
    {
        return -1;
    }
}

int idLineNumber = -1;
char* switchType;

void printAllScopeTables(ast_node* root){
    if(root == NULL){
        return;
    }
    if(root->symbolTable != NULL){
        printSymbolTable(root->symbolTable);
    }
    for(int i = 0; i < root->numChildren; i++){
        printAllScopeTables(root->children[i]);
    }
}

void printAllActivationRecords(ast_node* root){
    if(root == NULL){
        return;
    }
    if(root->symbolTable != NULL){
        printActivationRecord(root->symbolTable);
    }
    for(int i = 0; i < root->numChildren; i++){
        printAllActivationRecords(root->children[i]);
    }
}

void printAllScopeArrays(ast_node* root){
    if(root == NULL){
        return;
    }
    if(root->symbolTable != NULL){
        printSymbolTableArrays(root->symbolTable);
    }
    for(int i = 0; i < root->numChildren; i++){
        printAllScopeArrays(root->children[i]);
    }
}



void initializeSemanticAnalyzer()
{
    switchType = NULL;
    numSemanticErrors = 0;
    idLineNumber = -1; 
}

// returns -1 if type error, 1 if integer, 2 if real, 3 if boolean
int resolveExpression(ast_node *root, st_table *parent)
{
    int type = -1;

    if (strcmp(root->type, "UNARY_OP") == 0 || strcmp(root->type, "ARRAY_INDEX") == 0)
    {
        int type = resolveExpression(root->children[1], parent);
        if (type == -1)
            return -1;
        if (type != 1 && type != 2)
        {
            printf("Error : Unary operator can only be applied to integer or real! Line No. : %d\n", idLineNumber);
            numSemanticErrors++;
            return -1;
        }
        return type;
    }
    if (root->containsLexeme)
    {

        // printf("Resolve: Contains Lexeme\n");
        // fflush(stdout);

        idLineNumber = root->lexeme->line_no;

        if (strcmp(root->lexeme->token, token_list[NUM]) == 0)
        {
            return 1;
        }
        if (strcmp(root->lexeme->token, token_list[RNUM]) == 0)
        {
            return 2;
        }
        if (strcmp(root->lexeme->token, keyword_list[TRUE]) == 0 || strcmp(root->lexeme->token, keyword_list[FALSE]) == 0)
        {
            return 3;
        }
        if (strcmp(root->lexeme->token, token_list[ID]) == 0)
        {
            st_entry *entry = getSymbolEntry(parent, root->type);

            if (entry == NULL)
            {
                printf("Error : Variable %s not declared! Line No. : %d\n", root->lexeme->lexeme, idLineNumber);
                numSemanticErrors++;
                return -1;
            }

            if (entry->idType->is_array)
            {
                printf("Error : Array %s cannot be used in an expression! Line No. : %d\n", root->type, idLineNumber);
                numSemanticErrors++;
                return -1;
            }

            if (entry->idType->is_array)
            {
                printf("Error : Array %s cannot be used in an expression! Line No. : %d\n", root->type, idLineNumber);
                numSemanticErrors++;
                return -1;
            }
            return typeToIntID(entry->idType->type);
        }
        printf("Error : Invalid expression! Line No. : %d\n", idLineNumber);
        return -1;
    }
    if (strcmp("OPERATION_1", root->type) == 0 || strcmp("OPERATION_2", root->type) == 0 || strcmp("RELATIONAL_OP", root->type) == 0 || strcmp("LOGICAL_OP", root->type) == 0)
    {

        // printf("Resolve in OPS\n");
        // fflush(stdout);

        int type1 = resolveExpression(root->children[0], parent);
        int type2 = resolveExpression(root->children[2], parent);

        if (type1 == -1 || type2 == -1)
        {
            return -1;
        }

        ast_node *op = root->children[1];
        char *opType = op->type;

        if (type1 == 1 && type2 == 1)
        {
            if (strcmp(opType, "/") == 0)
            {
                return 2;
            }
            else if (strcmp(opType, "+") == 0 || strcmp(opType, "-") == 0 || strcmp(opType, "*") == 0)
            {
                return 1;
            }
            else if (strcmp(opType, ">") == 0 || strcmp(opType, "<") == 0 || strcmp(opType, ">=") == 0 || strcmp(opType, "<=") == 0 || strcmp(opType, "==") == 0 || strcmp(opType, "!=") == 0)
            {
                return 3;
            }
            else
            {
                // logical operators
                printf("Error : Cannot perform operation %s on integer! Line No. : %d\n", opType, idLineNumber);
                numSemanticErrors++;
                return -1;
            }
        }

        if (type1 == 2 && type2 == 2)
        {
            if (strcmp(opType, "/") == 0)
            {
                return 2;
            }
            else if (strcmp(opType, "+") == 0 || strcmp(opType, "-") == 0 || strcmp(opType, "*") == 0)
            {
                return 2;
            }
            else if (strcmp(opType, ">") == 0 || strcmp(opType, "<") == 0 || strcmp(opType, ">=") == 0 || strcmp(opType, "<=") == 0 || strcmp(opType, "==") == 0 || strcmp(opType, "!=") == 0)
            {
                return 3;
            }
            else
            {
                // logical operators
                printf("Error : Cannot perform operation %s on real! Line No. : %d\n", opType, idLineNumber);
                numSemanticErrors++;
                return -1;
            }
        }

        if (type1 == 1 && type2 == 2 || type1 == 2 && type2 == 1)
        {
            if (strcmp(opType, "/") == 0)
            {
                return 2;
            }
            else
            {
                printf("Error : Cannot perform operation %s on integer and real! Line No. : %d\n", opType, idLineNumber);
                numSemanticErrors++;
                return -1;
            }
        }

        if (type1 == 3 && type2 == 3)
        {
            if (strcmp(opType, "AND") == 0 || strcmp(opType, "OR") == 0)
            {
                return 3;
            }
            else
            {
                printf("Error : Cannot perform operation %s on booleans! Line No. : %d\n", opType, idLineNumber);
                numSemanticErrors++;
                return -1;
            }
        }

        printf("Error : Invalid expression! Line No. : %d\n", idLineNumber);
        numSemanticErrors++;
        return -1;
    }

    if (strcmp(root->type, "ARRAY_ELEM") == 0 || strcmp(root->type, "ARRAY") == 0)
    {
        ast_node *id = root->children[0];
        ast_node *index = root->children[1];

        idLineNumber = id->lexeme->line_no;

        st_entry *entry = getSymbolEntry(parent, id->type);
        if (entry == NULL)
        {
            printf("Error : Array Variable %s not declared! Line No. : %d\n", id->type, idLineNumber);
            numSemanticErrors++;
            return -1;
        }

        if (!entry->idType->is_array)
        {
            printf("Error : Variable %s is not an array! Line No. : %d\n", id->type, idLineNumber);
            numSemanticErrors++;
            return -1;
        }

        int indexType = resolveExpression(index, parent);
        if (indexType != 1)
        {
            printf("Error : Array index must be an integer! Line No. : %d\n", idLineNumber);
            numSemanticErrors++;
            return -1;
        }

        if (entry->idType->is_static_array && index->containsLexeme && strcmp(index->lexeme->token, token_list[NUM]) == 0)
        {
            int indexValue = index->lexeme->value_if_num;
            if (indexValue < entry->idType->leftIndex || indexValue > entry->idType->rightIndex)
            {
                printf("Error : Index %d out of bounds! Line No. : %d\n", indexValue, idLineNumber);
                numSemanticErrors++;
                return -1;
            }
        }

        if (entry->idType->is_static_array)
        {
            if (strcmp(index->type, "ARRAY_INDEX") == 0)
            {
                char *sign = ((ast_node *)index->children[0])->type;
                ast_node *num = (ast_node *)index->children[1];
                if (num->containsLexeme && strcmp(num->lexeme->token, token_list[NUM]) == 0)
                {
                    int indexValue = num->lexeme->value_if_num;
                    if (strcmp(sign, "-") == 0)
                    {
                        indexValue = -indexValue;
                    }
                    if (indexValue < entry->idType->leftIndex || indexValue > entry->idType->rightIndex)
                    {
                        printf("Error : Index %d out of bounds! Line No. : %d\n", indexValue, idLineNumber);
                        numSemanticErrors++;
                        return -1;
                    }
                }
            }
        }

        return typeToIntID(entry->idType->type);
    }
    return type;
}

void constructSymbolTable(ast_node *root, st_table *parent)
{
    ast_node *curr = root;
    if (curr != NULL)
    {
        //printf("Current node type: %s\n", curr->type);
        if (strcmp(curr->type, "PROGRAM") == 0)
        {
            for (int i = 0; i < curr->numChildren; i++)
            {
                constructSymbolTable(curr->children[i], curr->symbolTable);
            }
        }
        else if (strcmp(curr->type, "DRIVER") == 0)
        {
            curr->symbolTable = createSymbolTable("DRIVER", parent, curr->startLine, curr->endLine, "DRIVER");
            for (int i = 0; i < curr->numChildren; i++)
            {
                constructSymbolTable(curr->children[i], curr->symbolTable);
            }
        }
        else if (strcmp(curr->type, "DECLARE") == 0)
        {
            ast_node *acc_param_list = (ast_node *)(curr->children[1]);

            char *varType = ((ast_node *)(curr->children[0]))->type;
            int numParams = acc_param_list->numChildren;

            if (strcmp(varType, "RANGE_DATATYPE") == 0)
            {
                // isArray
                ast_node *typeNode = curr->children[0];
                char *arrayType = ((ast_node *)(typeNode->children[1]))->type;

                ast_node *rangeNode = (ast_node *)(typeNode->children[0]);

                ast_node *leftIndexNode = (ast_node *)(rangeNode->children[0]);
                ast_node *rightIndexNode = (ast_node *)(rangeNode->children[1]);

                int leftIndex = -1;
                int rightIndex = -1;
                int isDynamicLeft = 0;
                int isDynamicRight = 0;
                char *leftIndexName = NULL;
                char *rightIndexName = NULL;

                if (leftIndexNode->numChildren > 1)
                {
                    if (strlen(((ast_node *)(leftIndexNode->children[1]))->type) > 0 && ((ast_node *)(leftIndexNode->children[1]))->type[0] >= '0' && ((ast_node *)(leftIndexNode->children[1]))->type[0] <= '9')
                    {
                        leftIndex = atoi(((ast_node *)(leftIndexNode->children[1]))->type);
                        if (strcmp(((ast_node *)(leftIndexNode->children[0]))->type, "-") == 0)
                        {
                            leftIndex = -1 * leftIndex;
                        }
                    }
                    else
                    {
                        isDynamicLeft = 1;
                        leftIndexName = ((ast_node *)(leftIndexNode->children[1]))->type;
                        if (strcmp(((ast_node *)(leftIndexNode->children[0]))->type, "-") == 0)
                        {
                            char *temp = (char *)malloc(strlen(leftIndexName) + 1);
                            strcpy(temp + 1, leftIndexName);
                            temp[0] = '-';
                            leftIndexName = temp;
                        }
                    }
                }
                else
                {
                    if (strlen(leftIndexNode->type) > 0 && leftIndexNode->type[0] >= '0' && leftIndexNode->type[0] <= '9')
                    {
                        leftIndex = atoi(leftIndexNode->type);
                    }
                    else
                    {
                        isDynamicLeft = 1;
                        leftIndexName = leftIndexNode->type;
                    }
                }

                if (rightIndexNode->numChildren > 1)
                {
                    if (strlen(((ast_node *)(rightIndexNode->children[1]))->type) > 0 && ((ast_node *)(rightIndexNode->children[1]))->type[0] >= '0' && ((ast_node *)(rightIndexNode->children[1]))->type[0] <= '9')
                    {
                        rightIndex = atoi(((ast_node *)(rightIndexNode->children[1]))->type);
                        if (strcmp(((ast_node *)(rightIndexNode->children[0]))->type, "-") == 0)
                        {
                            rightIndex = -1 * rightIndex;
                        }
                    }
                    else
                    {
                        isDynamicRight = 1;
                        rightIndexName = ((ast_node *)(rightIndexNode->children[1]))->type;
                        if (strcmp(((ast_node *)(rightIndexNode->children[0]))->type, "-") == 0)
                        {
                            char *temp = (char *)malloc(strlen(rightIndexName) + 1);
                            strcpy(temp + 1, rightIndexName);
                            temp[0] = '-';
                            rightIndexName = temp;
                        }
                    }
                }
                else
                {
                    if (strlen(rightIndexNode->type) > 0 && rightIndexNode->type[0] >= '0' && rightIndexNode->type[0] <= '9')
                    {
                        rightIndex = atoi(rightIndexNode->type);
                    }
                    else
                    {
                        isDynamicRight = 1;
                        rightIndexName = rightIndexNode->type;
                    }
                }

                for (int i = 0; i < numParams; i++)
                {
                    ast_node *arr_elem_or_id = ((ast_node *)acc_param_list->children[i])->children[0];
                    ast_node *idNode = (ast_node *)arr_elem_or_id->children[0];

                    st_entry *existingEntry = getSymbolEntryCurrent(parent, idNode->type);

                    if (existingEntry != NULL && !existingEntry->isShadow)
                    {
                        printf("Error: Variable %s already declared in this scope! Line No. : %d\n", idNode->type, idNode->lexeme->line_no);
                        numSemanticErrors++;
                    }
                    else
                    {
                        int width = (isDynamicLeft || isDynamicRight) ? 1 : (1 + (rightIndex - leftIndex + 1) * getTypeWidth(arrayType));
                        id_type *idType = createIDType(arrayType, 1, !(isDynamicLeft || isDynamicRight), 0, leftIndex, rightIndex, leftIndexName, rightIndexName, isDynamicLeft, isDynamicRight, NULL, 0, NULL, 0);

                        if (existingEntry && existingEntry->isShadow)
                        {
                            existingEntry->isShadow = 0;
                            existingEntry->idType = idType;
                            existingEntry->width = width;
                            existingEntry->offset = parent->offset;
                            existingEntry->nestingLevel = parent->nestingLevel;
                            existingEntry->lineNo = idNode->lexeme->line_no;
                            addSymbolEntry(parent, createSymbolEntry("123", idType, parent->scopeName, width, -1, -1, -1, parent->offsetAct));
                            printSymbolEntry(st_fp, existingEntry);
                        }
                        else
                        {
                            st_entry *entry = createSymbolEntry(idNode->type, idType, parent->scopeName, width, parent->offset, parent->nestingLevel, idNode->lexeme->line_no, parent->offsetAct);
                            addSymbolEntry(parent, entry);
                            printSymbolEntry(st_fp, entry);
                        }
                    }
                }
            }
            else
            {
                // is not array
                for (int i = 0; i < numParams; i++)
                {
                    ast_node *arr_elem_or_id = ((ast_node *)acc_param_list->children[i])->children[0];
                    ast_node *idNode = (ast_node *)arr_elem_or_id->children[0];

                    st_entry *existingEntry = getSymbolEntryCurrent(parent, idNode->type);

                    if (existingEntry != NULL && !existingEntry->isShadow)
                    {
                        printf("Error : Variable %s already declared in this scope! Line No. : %d\n", idNode->type, idNode->lexeme->line_no);
                        numSemanticErrors++;
                    }
                    else
                    {
                        int width = getTypeWidth(varType);
                        int offset = -1;
                        id_type *idType = createIDType(varType, 0, 0, 0, -1, -1, NULL, NULL, 0, 0, NULL, 0, NULL, 0);

                        if (existingEntry && existingEntry->isShadow)
                        {
                            existingEntry->isShadow = 0;
                            existingEntry->idType = idType;
                            existingEntry->width = width;
                            existingEntry->offset = parent->offset;
                            existingEntry->nestingLevel = parent->nestingLevel;
                            existingEntry->lineNo = idNode->lexeme->line_no;
                            addSymbolEntry(parent, createSymbolEntry("123", idType, parent->scopeName, width, -1, -1, -1, parent->offsetAct));
                            printSymbolEntry(st_fp, existingEntry);
                        }
                        else
                        {
                            st_entry *entry = createSymbolEntry(idNode->type, idType, parent->scopeName, width, parent->offset, parent->nestingLevel, idNode->lexeme->line_no, parent->offsetAct);
                            addSymbolEntry(parent, entry);
                            printSymbolEntry(st_fp, entry);
                        }
                    }
                }
            }
        }
        else if (strcmp(curr->type, "MODULE") == 0)
        {
            char *functionName = ((ast_node *)(curr->children[0]))->type;

            if (getSymbolEntry(parent, functionName) != NULL)
            {
                st_entry *entry = getSymbolEntry(parent, functionName);
                if (entry->didInFirstPass)
                {
                    ast_node *statements = (ast_node *)(curr->children[3]);
                    for (int i = 0; i < statements->numChildren; i++)
                    {
                        constructSymbolTable(statements->children[i], curr->symbolTable);
                    }

                    ast_node *outputListt = (ast_node *)(curr->children[2]);
                    if(outputListt != NULL && (entry->unusableFunc == 2 || entry->unusableFunc == 0)){
                        if(entry->unusableFunc == 2){
                            entry->unusableFunc = 1;
                        }
                        for (int i = 0; i < outputListt->numChildren; i++)
                        {   
                            ast_node* outputID = outputListt->children[i];
                            ast_node* idNode = (ast_node *)(outputID->children[0]);
                            st_entry* entry = getSymbolEntry(curr->symbolTable, idNode->type);
                            if(!entry->assigned){
                                printf("Error : Output parameter %s not assigned a value! Line No. : %d\n", idNode->type, curr->symbolTable->lineNoEnd);
                                numSemanticErrors++;
                            }
                        }
                    }

                    return;
                }
                else
                {
                    printf("Error : Function %s already defined in this scope! Overloading not supported. Line No. : %d\n", functionName, ((ast_node *)(curr->children[0]))->lexeme->line_no);
                    numSemanticErrors++;
                    return;
                }
            }

            curr->symbolTable = createSymbolTable(functionName, parent, curr->startLine, curr->endLine, functionName);

            ast_node *inputList = (ast_node *)(curr->children[1]);
            int numInputParams = inputList->numChildren;

            id_type **inputParamTypes = (id_type **)malloc(sizeof(id_type *) * numInputParams);

            for (int i = 0; i < numInputParams; i++)
            {
                ast_node *inputParam = (ast_node *)(inputList->children[i]);
                ast_node *idNode = (ast_node *)(inputParam->children[0]);
                ast_node *typeNode = (ast_node *)(inputParam->children[1]);
                int width = 0;

                if (strcmp(typeNode->type, "RANGE_DATATYPE") == 0)
                {
                    char *arrayType = ((ast_node *)(typeNode->children[1]))->type;

                    ast_node *rangeNode = (ast_node *)(typeNode->children[0]);

                    ast_node *leftIndexNode = (ast_node *)(rangeNode->children[0]);
                    ast_node *rightIndexNode = (ast_node *)(rangeNode->children[1]);

                    int leftIndex = -1;
                    int rightIndex = -1;
                    int isDynamicLeft = 0;
                    int isDynamicRight = 0;
                    char *leftIndexName = NULL;
                    char *rightIndexName = NULL;

                    if (leftIndexNode->numChildren > 1)
                    {
                        if (strlen(((ast_node *)(leftIndexNode->children[1]))->type) > 0 && ((ast_node *)(leftIndexNode->children[1]))->type[0] >= '0' && ((ast_node *)(leftIndexNode->children[1]))->type[0] <= '9')
                        {
                            leftIndex = atoi(((ast_node *)(leftIndexNode->children[1]))->type);
                            if (strcmp(((ast_node *)(leftIndexNode->children[0]))->type, "-") == 0)
                            {
                                leftIndex = -1 * leftIndex;
                            }
                        }
                        else
                        {
                            isDynamicLeft = 1;
                            leftIndexName = ((ast_node *)(leftIndexNode->children[1]))->type;
                            if (strcmp(((ast_node *)(leftIndexNode->children[0]))->type, "-") == 0)
                            {
                                char *temp = (char *)malloc(strlen(leftIndexName) + 1);
                                strcpy(temp + 1, leftIndexName);
                                temp[0] = '-';
                                leftIndexName = temp;
                            }
                        }
                    }
                    else
                    {
                        if (strlen(leftIndexNode->type) > 0 && leftIndexNode->type[0] >= '0' && leftIndexNode->type[0] <= '9')
                        {
                            leftIndex = atoi(leftIndexNode->type);
                        }
                        else
                        {
                            isDynamicLeft = 1;
                            leftIndexName = leftIndexNode->type;
                        }
                    }

                    if (rightIndexNode->numChildren > 1)
                    {
                        if (strlen(((ast_node *)(rightIndexNode->children[1]))->type) > 0 && ((ast_node *)(rightIndexNode->children[1]))->type[0] >= '0' && ((ast_node *)(rightIndexNode->children[1]))->type[0] <= '9')
                        {
                            rightIndex = atoi(((ast_node *)(rightIndexNode->children[1]))->type);
                            if (strcmp(((ast_node *)(rightIndexNode->children[0]))->type, "-") == 0)
                            {
                                rightIndex = -1 * rightIndex;
                            }
                        }
                        else
                        {
                            isDynamicRight = 1;
                            rightIndexName = ((ast_node *)(rightIndexNode->children[1]))->type;
                            if (strcmp(((ast_node *)(rightIndexNode->children[0]))->type, "-") == 0)
                            {
                                char *temp = (char *)malloc(strlen(rightIndexName) + 1);
                                strcpy(temp + 1, rightIndexName);
                                temp[0] = '-';
                                rightIndexName = temp;
                            }
                        }
                    }
                    else
                    {
                        if (strlen(rightIndexNode->type) > 0 && rightIndexNode->type[0] >= '0' && rightIndexNode->type[0] <= '9')
                        {
                            rightIndex = atoi(rightIndexNode->type);
                        }
                        else
                        {
                            isDynamicRight = 1;
                            rightIndexName = rightIndexNode->type;
                        }
                    }

                    width = (isDynamicLeft || isDynamicRight) ? 1 : (1 + (rightIndex - leftIndex + 1) * getTypeWidth(arrayType));
                    id_type *idType = createIDType(arrayType, 1, !(isDynamicLeft || isDynamicRight), 0, leftIndex, rightIndex, leftIndexName, rightIndexName, isDynamicLeft, isDynamicRight, NULL, 0, NULL, 0);
                    inputParamTypes[i] = idType;
                }
                else
                {
                    // not array
                    id_type *type = createIDType(typeNode->type, 0, 0, 0, -1, -1, NULL, NULL, 0, 0, NULL, 0, NULL, 0);
                    inputParamTypes[i] = type;
                    width = getTypeWidth(type->type);
                }

                st_entry *entry = createSymbolEntry(idNode->type, inputParamTypes[i], curr->symbolTable->scopeName, width, curr->symbolTable->offset, curr->symbolTable->nestingLevel, idNode->lexeme->line_no, curr->symbolTable->offsetAct);
                entry->isShadow = 1;
                addSymbolEntry(curr->symbolTable, entry);
                printSymbolEntry(st_fp, entry);
            }

            ast_node *outputList = (ast_node *)(curr->children[2]);
            int numOutputParams = outputList->numChildren;

            id_type **outputParamTypes = (id_type **)malloc(sizeof(id_type *) * numOutputParams);

            for (int i = 0; i < numOutputParams; i++)
            {
                ast_node *outputParam = (ast_node *)(outputList->children[i]);
                ast_node *idNode = (ast_node *)(outputParam->children[0]);
                ast_node *typeNode = (ast_node *)(outputParam->children[1]);
                int width = 0;

                if (strcmp(typeNode->type, "RANGE_DATATYPE") == 0)
                {
                    char *arrayType = ((ast_node *)(typeNode->children[1]))->type;

                    ast_node *rangeNode = (ast_node *)(typeNode->children[0]);

                    ast_node *leftIndexNode = (ast_node *)(rangeNode->children[0]);
                    ast_node *rightIndexNode = (ast_node *)(rangeNode->children[1]);

                    int leftIndex = -1;
                    int rightIndex = -1;
                    int isDynamicLeft = 0;
                    int isDynamicRight = 0;
                    char *leftIndexName = NULL;
                    char *rightIndexName = NULL;

                    if (leftIndexNode->numChildren > 1)
                    {
                        if (strlen(((ast_node *)(leftIndexNode->children[1]))->type) > 0 && ((ast_node *)(leftIndexNode->children[1]))->type[0] >= '0' && ((ast_node *)(leftIndexNode->children[1]))->type[0] <= '9')
                        {
                            leftIndex = atoi(((ast_node *)(leftIndexNode->children[1]))->type);
                            if (strcmp(((ast_node *)(leftIndexNode->children[0]))->type, "-") == 0)
                            {
                                leftIndex = -1 * leftIndex;
                            }
                        }
                        else
                        {
                            isDynamicLeft = 1;
                            leftIndexName = ((ast_node *)(leftIndexNode->children[1]))->type;
                            if (strcmp(((ast_node *)(leftIndexNode->children[0]))->type, "-") == 0)
                            {
                                char *temp = (char *)malloc(strlen(leftIndexName) + 1);
                                strcpy(temp + 1, leftIndexName);
                                temp[0] = '-';
                                leftIndexName = temp;
                            }
                        }
                    }
                    else
                    {
                        if (strlen(leftIndexNode->type) > 0 && leftIndexNode->type[0] >= '0' && leftIndexNode->type[0] <= '9')
                        {
                            leftIndex = atoi(leftIndexNode->type);
                        }
                        else
                        {
                            isDynamicLeft = 1;
                            leftIndexName = leftIndexNode->type;
                        }
                    }

                    if (rightIndexNode->numChildren > 1)
                    {
                        if (strlen(((ast_node *)(rightIndexNode->children[1]))->type) > 0 && ((ast_node *)(rightIndexNode->children[1]))->type[0] >= '0' && ((ast_node *)(rightIndexNode->children[1]))->type[0] <= '9')
                        {
                            rightIndex = atoi(((ast_node *)(rightIndexNode->children[1]))->type);
                            if (strcmp(((ast_node *)(rightIndexNode->children[0]))->type, "-") == 0)
                            {
                                rightIndex = -1 * rightIndex;
                            }
                        }
                        else
                        {
                            isDynamicRight = 1;
                            rightIndexName = ((ast_node *)(rightIndexNode->children[1]))->type;
                            if (strcmp(((ast_node *)(rightIndexNode->children[0]))->type, "-") == 0)
                            {
                                char *temp = (char *)malloc(strlen(rightIndexName) + 1);
                                strcpy(temp + 1, rightIndexName);
                                temp[0] = '-';
                                rightIndexName = temp;
                            }
                        }
                    }
                    else
                    {
                        if (strlen(rightIndexNode->type) > 0 && rightIndexNode->type[0] >= '0' && rightIndexNode->type[0] <= '9')
                        {
                            rightIndex = atoi(rightIndexNode->type);
                        }
                        else
                        {
                            isDynamicRight = 1;
                            rightIndexName = rightIndexNode->type;
                        }
                    }

                    width = (isDynamicLeft || isDynamicRight) ? 1 : (1 + (rightIndex - leftIndex + 1) * getTypeWidth(arrayType));
                    id_type *idType = createIDType(arrayType, 1, !(isDynamicLeft || isDynamicRight), 0, leftIndex, rightIndex, leftIndexName, rightIndexName, isDynamicLeft, isDynamicRight, NULL, 0, NULL, 0);
                    outputParamTypes[i] = idType;
                }
                else
                {
                    // not array
                    id_type *type = createIDType(typeNode->type, 0, 0, 0, -1, -1, NULL, NULL, 0, 0, NULL, 0, NULL, 0);
                    outputParamTypes[i] = type;
                    width = getTypeWidth(type->type);
                }

                st_entry *entry = createSymbolEntry(idNode->type, outputParamTypes[i], curr->symbolTable->scopeName, width, curr->symbolTable->offset, curr->symbolTable->nestingLevel, idNode->lexeme->line_no, curr->symbolTable->offsetAct);
                addSymbolEntry(curr->symbolTable, entry);
                printSymbolEntry(st_fp, entry);
            }

            id_type *functionType = createIDType("module", 0, 0, 1, -1, -1, NULL, NULL, 0, 0, inputParamTypes, numInputParams, outputParamTypes, numOutputParams);

            st_entry *newEntry = NULL;
            //printf("New Module Defined\n");
            newEntry = createSymbolEntry(functionName, functionType, parent->scopeName, -1, -1, parent->nestingLevel, ((ast_node *)(curr->children[0]))->lexeme->line_no, -1);
            newEntry->assigned = 1;
            addSymbolEntry(parent, newEntry);
            printSymbolEntry(st_fp, newEntry);

            ast_node *statements = (ast_node *)(curr->children[3]);
            for (int i = 0; i < statements->numChildren; i++)
            {
                constructSymbolTable(statements->children[i], curr->symbolTable);
            }

            for (int i = 0; i < outputList->numChildren; i++)
            {
                ast_node* outputID = outputList->children[i];
                ast_node* idNode = (ast_node *)(outputID->children[0]);

                st_entry* entry = getSymbolEntry(curr->symbolTable, idNode->type);
                if(!entry->assigned){
                    printf("Error : Output parameter %s not assigned a value! Line No. : %d\n", idNode->type, curr->symbolTable->lineNoEnd);
                    numSemanticErrors++;
                }
            }
        }
        else if (strcmp(curr->type, "MODULE_LIST") == 0)
        {
            return; // pass
        }
        else if (strcmp(curr->type, "WHILE_LOOP") == 0)
        {
            curr->symbolTable = createSymbolTable("WHILE_LOOP", parent, curr->startLine, curr->endLine, parent->moduleName);
            
            ast_node *whileCondition = (ast_node *)(curr->children[0]);

            if (resolveExpression(whileCondition, curr->symbolTable) != 3)
            {
                printf("Error : While condition must be of boolean type! Line No. : %d\n", curr->startLine);
                numSemanticErrors++;
            }

            for (int i = 0; i < curr->numChildren; i++)
            {
                constructSymbolTable(curr->children[i], curr->symbolTable);
            }
        }
        else if (strcmp(curr->type, "FOR_LOOP") == 0)
        {
            char *loopid = ((ast_node *)(curr->children[0]))->type;
            curr->symbolTable = createSymbolTable("FOR_LOOP", parent, curr->startLine, curr->endLine, parent->moduleName);
            curr->symbolTable->inForLoop = 1;
            curr->symbolTable->forLoopVar = loopid;

            id_type *type = createIDType("integer", 0, 0, 0, -1, -1, NULL, NULL, 0, 0, NULL, 0, NULL, 0);
            st_entry *entry = createSymbolEntry(loopid, type, curr->symbolTable->scopeName, 2, curr->symbolTable->offset, curr->symbolTable->nestingLevel, ((ast_node *)(curr->children[0]))->lexeme->line_no, curr->symbolTable->offsetAct);
            entry->isForLoopVar = 1;
            addSymbolEntry(curr->symbolTable, entry);
            printSymbolEntry(st_fp, entry);

            for (int i = 0; i < curr->numChildren; i++)
            {
                constructSymbolTable(curr->children[i], curr->symbolTable);
            }
        }
        else if (strcmp(curr->type, "SWITCH") == 0)
        {
            ast_node *switchVariable = (ast_node *)(curr->children[0]);

            st_entry *switchVariableEntry = getSymbolEntry(parent, switchVariable->type);
            if (switchVariableEntry == NULL)
            {
                printf("Error : Variable %s is not declared! Line No. : %d\n", switchVariable->type, switchVariable->lexeme->line_no);
                numSemanticErrors++;
            }
            else if (strcmp(switchVariableEntry->idType->type, "real") == 0)
            {
                printf("Error : Variable %s is a real, cannot be used in a switch statement! Line No. : %d\n", switchVariable->type, switchVariable->lexeme->line_no);
                numSemanticErrors++;
            }
            else if (switchVariableEntry->idType->is_array)
            {
                printf("Error : Variable %s is an array, cannot be used in a switch statement! Line No. : %d\n", switchVariable->type, switchVariable->lexeme->line_no);
                numSemanticErrors++;
            }
            else
            {
                switchType = switchVariableEntry->idType->type;

                ast_node *defaultNode = (ast_node *)(curr->children[2]);
                if (strcmp(switchVariableEntry->idType->type, "integer") == 0 && defaultNode == NULL)
                {
                    printf("Error : Variable %s is of type integer, Switch must have a default case! Line No. : %d\n", switchVariable->type, switchVariable->lexeme->line_no);
                    numSemanticErrors++;
                }
                else if (strcmp(switchVariableEntry->idType->type, "boolean") == 0 && defaultNode != NULL)
                {
                    printf("Error : Variable %s is of type boolean, Switch cannot have a default case! Line No. : %d\n", switchVariable->type, switchVariable->lexeme->line_no);
                    numSemanticErrors++;
                }

                for (int i = 0; i < curr->numChildren; i++)
                {
                    constructSymbolTable(curr->children[i], parent);
                }
            }
        }
        else if (strcmp(curr->type, "CASE") == 0)
        {
            curr->symbolTable = createSymbolTable("CASE", parent, curr->startLine, curr->endLine, parent->moduleName);

            ast_node *caseValue = (ast_node *)(curr->children[0]);

            if (strcmp(caseValue->type, "true") == 0 || strcmp(caseValue->type, "false") == 0)
            {
                if (strcmp(switchType, "boolean") != 0)
                {
                    printf("Error : Case value %s, of type integer, cannot be used in a switch statement of type %s! Line No. : %d\n", caseValue->type, switchType, caseValue->lexeme->line_no);
                    numSemanticErrors++;
                }
            }
            else
            {
                if (strcmp(switchType, "integer") != 0)
                {
                    printf("Error : Case value %s , of type boolean, cannot be used in a switch statement of type %s! Line No. : %d\n", caseValue->type, switchType, caseValue->lexeme->line_no);
                    numSemanticErrors++;
                }
            }

            for (int i = 0; i < curr->numChildren; i++)
            {
                constructSymbolTable(curr->children[i], curr->symbolTable);
            }
        }
        else if (strcmp(curr->type, "DEFAULT") == 0)
        {
            curr->symbolTable = createSymbolTable("DEFAULT CASE", parent, curr->startLine, curr->endLine, parent->moduleName);
            for (int i = 0; i < curr->numChildren; i++)
            {
                constructSymbolTable(curr->children[i], curr->symbolTable);
            }
        }
        else if (strcmp(curr->type, "ASSIGN") == 0)
        {
            ast_node *lhs = (ast_node *)(curr->children[0]);
            ast_node *rhs = (ast_node *)(curr->children[1]);

            int both_array = 0;
            int errorFound = 0;

            if(lhs->containsLexeme && strcmp(lhs->lexeme->token, token_list[ID]) == 0){
                st_entry *lhsEntry = getSymbolEntry(parent, lhs->type);
                if(lhsEntry != NULL){
                    if(lhsEntry->isForLoopVar){
                        printf("Error : Variable %s is a for loop variable, cannot be assigned! Line No. : %d\n", lhs->type, lhs->lexeme->line_no);
                        numSemanticErrors++;
                        errorFound = 1;
                    }
                }
            }

            if (!errorFound && lhs->containsLexeme && rhs->containsLexeme && strcmp(lhs->lexeme->token, token_list[ID]) == 0 && strcmp(rhs->lexeme->token, token_list[ID]) == 0)
            {
                st_entry *lhsEntry = getSymbolEntry(parent, lhs->type);
                st_entry *rhsEntry = getSymbolEntry(parent, rhs->type);

                int entriesExist = 1;
                if (lhsEntry == NULL)
                {
                    printf("Error : Variable %s is not declared! Line No. : %d\n", lhs->type, lhs->lexeme->line_no);
                    numSemanticErrors++;
                    errorFound = 1;
                    entriesExist = 0;
                }
                if (rhsEntry == NULL)
                {
                    printf("Error : Variable %s is not declared! Line No. : %d\n", rhs->type, rhs->lexeme->line_no);
                    numSemanticErrors++;
                    errorFound = 1;
                    entriesExist = 0;
                }

                if (entriesExist && lhsEntry != NULL && rhsEntry != NULL)
                {
                    if (lhsEntry->idType->is_array || rhsEntry->idType->is_array)
                    {
                        both_array = 1;
                    }

                    if (lhsEntry->idType->is_array && rhsEntry->idType->is_array)
                    {
                        if (lhsEntry->idType->is_static_array && rhsEntry->idType->is_static_array)
                        {
                            if (strcmp(lhsEntry->idType->type, rhsEntry->idType->type) != 0)
                            {
                                printf("Error : Array %s and %s are not of same type! Line No. : %d\n", lhs->type, rhs->type, lhs->lexeme->line_no);
                                numSemanticErrors++;
                            }
                            else if (lhsEntry->width != rhsEntry->width)
                            {
                                printf("Error : Array %s and %s are not of same size! Line No. : %d\n", lhs->type, rhs->type,lhs->lexeme->line_no);
                                numSemanticErrors++;
                            }
                        }
                    }
                    else if (lhsEntry->idType->is_array && !rhsEntry->idType->is_array || !lhsEntry->idType->is_array && rhsEntry->idType->is_array)
                    {
                        printf("Error : Only one of the variables is an array! Line No. : %d\n",lhs->lexeme->line_no);
                        numSemanticErrors++;
                    }
                }
            }

            if(both_array && !errorFound){
                if(lhs->containsLexeme && strcmp(lhs->lexeme->token, token_list[ID]) == 0){
                    st_entry *lhsEntry = getSymbolEntry(parent, lhs->type);
                    if(lhsEntry != NULL){
                        lhsEntry->assigned = 1;
                    }
                }
            }


            if (!both_array && !errorFound)
            {
                //printf("HERE\n");

                int lhsType = resolveExpression(lhs, parent);
                int rhsType = resolveExpression(rhs, parent);

                // printf("LHS type : %d, RHS type : %d\n", lhsType, rhsType);

                int assignLineNumber = -1;
                if (strcmp(lhs->type, "ARRAY_ELEM") == 0 || strcmp(lhs->type, "ARRAY") == 0)
                {
                    assignLineNumber = ((ast_node *)(lhs->children[0]))->lexeme->line_no;
                }
                else
                {
                    assignLineNumber = lhs->lexeme->line_no;
                }

                if (lhsType != rhsType)
                {
                    printf("Error : Assignment statement type error! Line no. : %d\n", assignLineNumber);
                    numSemanticErrors++;
                }else{
                    if(lhs->containsLexeme && strcmp(lhs->lexeme->token, token_list[ID]) == 0){
                        st_entry *lhsEntry = getSymbolEntry(parent, lhs->type);
                        if(lhsEntry != NULL){
                            lhsEntry->assigned = 1;
                        }
                    }
                }
            }
        }
        else if (strcmp(curr->type, "REUSE_STMT") == 0)
        {
            ast_node *moduleName = (ast_node *)(curr->children[1]);
            st_entry *moduleEntry = getSymbolEntry(parent, moduleName->type);

            if (moduleEntry == NULL || !moduleEntry->assigned)
            {
                printf("Error : Module %s is not defined! Line No. : %d\n", moduleName->type, moduleName->lexeme->line_no);
                numSemanticErrors++;
            }
            else if (strcmp(moduleEntry->idType->type, "module") != 0)
            {
                printf("Error : %s is not a module! Line No. : %d\n", moduleName->type, moduleName->lexeme->line_no);
                numSemanticErrors++;
            }
            else if(strcmp(moduleName->type, parent->moduleName) == 0){
                printf("Error : Module %s cannot be called recursively! Line No. : %d\n", moduleName->type, moduleName->lexeme->line_no);
                numSemanticErrors++;
            }
            else
            {
                ast_node *paramsList = (ast_node *)(curr->children[2]);
                ast_node *outputList = (ast_node *)(curr->children[0]);
 
                int outputExists = 1;
                if (outputList == NULL)
                {
                    outputExists = 0;
                }

                if (paramsList->numChildren != moduleEntry->idType->numInputParams)
                {
                    // check if number of args correct
                    printf("Error : Number of parameters in module call do not match number of inputs! Line No. : %d\n", moduleName->lexeme->line_no);
                    numSemanticErrors++;
                }
                else
                {
                    // check if func call correct

                    for (int i = 0; i < paramsList->numChildren; i++)
                    {
                        ast_node *param = (ast_node *)(paramsList->children[i]);
                        ast_node *paramNode = (ast_node *)(param->children[0]);

                        id_type *definedType = moduleEntry->idType->inputParams[i];

                        if (strcmp(paramNode->type, "ARRAY_ELEM_OR_ID") == 0)
                        {
                            ast_node *idNode = (ast_node *)(paramNode->children[0]);
                            st_entry *idEntry = getSymbolEntry(parent, idNode->type);

                            if (idEntry == NULL)
                            {
                                printf("Error : Variable %s is not declared! Line No. : %d\n", idNode->type, idNode->lexeme->line_no);
                                numSemanticErrors++;
                            }
                            else
                            {
                                ast_node *arrIndex = (ast_node *)(paramNode->children[1]);
                                if (arrIndex != NULL)
                                {
                                    if (!idEntry->idType->is_array)
                                    {
                                        printf("Error : Variable %s is not an array! Line No. : %d\n", idNode->type, idNode->lexeme->line_no);
                                        numSemanticErrors++;
                                    }
                                    else
                                    {
                                        if (arrIndex->containsLexeme)
                                        {
                                            // NORMAL CASE
                                            if (strcmp(arrIndex->lexeme->token, token_list[ID]) == 0)
                                            {
                                                st_entry *indexEntry = getSymbolEntry(parent, arrIndex->type);
                                                if (indexEntry == NULL)
                                                {
                                                    printf("Error : Variable %s is not declared! Line No. : %d\n", arrIndex->type, arrIndex->lexeme->line_no);
                                                    numSemanticErrors++;
                                                }
                                                else if (strcmp("integer", indexEntry->idType->type) != 0)
                                                {
                                                    printf("Error : Array index must be an integer! Line No. : %d\n", arrIndex->lexeme->line_no);
                                                    numSemanticErrors++;
                                                }
                                            }
                                            else if (strcmp(arrIndex->lexeme->token, token_list[NUM]) == 0)
                                            {
                                                if (idEntry->idType->is_static_array)
                                                {
                                                    int index = atoi(arrIndex->lexeme->lexeme);
                                                    if (index < idEntry->idType->leftIndex || index > idEntry->idType->rightIndex)
                                                    {
                                                        printf("Error : Array index out of bounds! Line No. : %d\n", arrIndex->lexeme->line_no);
                                                        numSemanticErrors++;
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                printf("Error : Array index must be an integer! Line No. : %d\n", arrIndex->lexeme->line_no);
                                            }
                                        }
                                        else
                                        {
                                            // BOUND NODE
                                            ast_node *signNode = (ast_node *)(arrIndex->children[0]);
                                            int negative = 1;

                                            if (signNode != NULL)
                                            {
                                                if (strcmp(signNode->type, "-") == 0)
                                                {
                                                    negative = -1;
                                                }
                                            }

                                            ast_node *numNode = (ast_node *)(arrIndex->children[1]);

                                            if (strcmp(numNode->lexeme->token, token_list[ID]) == 0)
                                            {
                                                st_entry *indexEntry = getSymbolEntry(parent, numNode->type);
                                                if (indexEntry == NULL)
                                                {
                                                    printf("Error : Variable %s is not declared! Line No. : %d\n", numNode->type, numNode->lexeme->line_no);
                                                    numSemanticErrors++;
                                                }
                                                else if (strcmp("integer", indexEntry->idType->type) != 0)
                                                {
                                                    printf("Error : Array index must be an integer! Line No. : %d\n", numNode->lexeme->line_no);
                                                    numSemanticErrors++;
                                                }
                                            }
                                            else if (strcmp(numNode->lexeme->token, token_list[NUM]) == 0)
                                            {
                                                if (idEntry->idType->is_static_array)
                                                {
                                                    int index = atoi(numNode->lexeme->lexeme);
                                                    index = index * negative;
                                                    if (index < idEntry->idType->leftIndex || index > idEntry->idType->rightIndex)
                                                    {
                                                        printf("Error : Array index out of bounds! Line No. : %d\n", numNode->lexeme->line_no);
                                                        numSemanticErrors++;
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                printf("Error : Array index must be an integer! Line No. : %d\n", numNode->lexeme->line_no);
                                            }
                                        }

                                        if (strcmp(definedType->type, idEntry->idType->type) != 0)
                                        {
                                            printf("Error : Actual parameter of type %s, cannot be used in module call of type %s! Line No. : %d \n", idEntry->idType->type, definedType->type, idNode->lexeme->line_no);
                                            numSemanticErrors++;
                                        }
                                    }
                                }
                                else
                                {
                                    if (idEntry->idType->is_array && !definedType->is_array || !idEntry->idType->is_array && definedType->is_array)
                                    {
                                        printf("Error : Variable type error! One of them is an array, other is not! Line No. : %d\n", idNode->lexeme->line_no);
                                        numSemanticErrors++;
                                    }
                                    else if (strcmp(definedType->type, idEntry->idType->type) != 0)
                                    {
                                        printf("Error : Actual parameter of type %s, cannot be used in module call of type %s! Line No. : %d \n", idEntry->idType->type, definedType->type, idNode->lexeme->line_no);
                                        numSemanticErrors++;
                                    }
                                    if (definedType->is_static_array && idEntry->idType->is_static_array)
                                    {
                                        if (definedType->rightIndex - definedType->leftIndex != idEntry->idType->rightIndex - idEntry->idType->leftIndex)
                                        {
                                            printf("Error : Array size mismatch in function call! Line No. : %d\n", idNode->lexeme->line_no);
                                        }
                                    }
                                }
                            }
                        }
                        else if (strcmp(paramNode->type, "NUM") == 0)
                        {
                            if (definedType->is_array)
                            {
                                printf("Error : Actual parameter of type array, does not match with formal parameter of type %s! Line No. : %d \n", definedType->type, ((ast_node *)paramNode->children[0])->lexeme->line_no);
                                numSemanticErrors++;
                            }
                            else if (strcmp(definedType->type, "integer") != 0)
                            {
                                printf("Error :Actual parameter of type integer, does not match with formal parameter of type %s! Line No. : %d\n", definedType->type, ((ast_node *)paramNode->children[0])->lexeme->line_no);
                                numSemanticErrors++;
                            }
                        }
                        else if (strcmp(paramNode->type, "RNUM") == 0)
                        {
                            if (definedType->is_array)
                            {
                                printf("Error :Actual parameter of type array, does not match with formal parameter of type %s! Line No. : %d\n", definedType->type, ((ast_node *)paramNode->children[0])->lexeme->line_no);
                                numSemanticErrors++;
                            }
                            else if (strcmp(definedType->type, "real") != 0)
                            {
                                printf("Error :Actual parameter of type real, does not match with formal parameter of type %s! Line No. : %d\n", definedType->type, ((ast_node *)paramNode->children[0])->lexeme->line_no);
                                numSemanticErrors++;
                            }
                        }
                        else if (strcmp(paramNode->type, "true") == 0 || strcmp(paramNode->type, "false") == 0)
                        {
                            if (definedType->is_array)
                            {
                                printf("Error : Actual parameter %s of type array does not match with formal parameter of type %s! Line No. : %d\n", paramNode->type, definedType->type, paramNode->lexeme->line_no);
                                numSemanticErrors++;
                            }
                            else if (strcmp(definedType->type, "boolean") != 0)
                            {
                                printf("Error : Actual parameter %s  of type boolean, does not match with formal parameter of type %s! Line No. : %d\n", paramNode->type, definedType->type, paramNode->lexeme->line_no);
                                numSemanticErrors++;
                            }
                        }
                    }
                }

                if (outputExists && moduleEntry->idType->numOutputParams != outputList->numChildren)
                {
                    // check if number of outputs correct
                    printf("Error : Number of parameters in module call do not match number of outputs! Line No. : %d\n", moduleName->lexeme->line_no);
                    numSemanticErrors++;
                }
                else if (outputExists)
                {
                    for (int i = 0; i < outputList->numChildren; i++)
                    {
                        ast_node *output = (ast_node *)(outputList->children[i]);
                        id_type *definedType = moduleEntry->idType->outputParams[i];

                        if (output->containsLexeme && strcmp(output->lexeme->token, token_list[ID]) == 0)
                        {
                            
                            st_entry *idEntry = getSymbolEntry(parent, output->type);
                            if (idEntry == NULL)
                            {
                                printf("Error : Variable %s is not declared! Line No. : %d\n", output->type, output->lexeme->line_no);
                                numSemanticErrors++;
                            }
                            else
                            {
                                int errorFound = 0;
                                if (definedType->is_array && idEntry->idType->is_array || !definedType->is_array && !idEntry->idType->is_array)
                                {
                                    if (strcmp(definedType->type, idEntry->idType->type) != 0)
                                    {
                                        printf("Error : Output variable %d in module call does not match type of output! Line No. : %d\n", i + 1, output->lexeme->line_no);
                                        numSemanticErrors++;
                                        errorFound = 1;

                                    }
                                    else if (definedType->is_static_array && idEntry->idType->is_static_array)
                                    {
                                        if (definedType->rightIndex - definedType->leftIndex != idEntry->idType->rightIndex - idEntry->idType->leftIndex)
                                        {
                                            printf("Error : Array width mismatch! Line No. : %d", output->lexeme->line_no);
                                            numSemanticErrors++;
                                            errorFound = 1;
                                        }
                                    }
                                }
                                else
                                {
                                    printf("Error : Type mismatch, one of the variables is not an array! Line No. : %d\n", output->lexeme->line_no);
                                    numSemanticErrors++;
                                    errorFound = 1;
                                }

                                if (!errorFound)
                                {
                                    idEntry->assigned = 1;
                                }
                            }
                        }
                        else
                        {
                            printf("Error : Output variable %d in module call is not a variable! Line No. : %d\n", i + 1, output->lexeme->line_no);
                        }
                    }
                }
            }
        }
        else if (strcmp(curr->type, "GET_VALUE") == 0)
        {
            ast_node *idNode = (ast_node *)(curr->children[0]);
            if (idNode != NULL && idNode->containsLexeme)
            {
                if (strcmp(idNode->lexeme->token, token_list[ID]) == 0)
                {
                    st_entry *idEntry = getSymbolEntry(parent, idNode->type);
                    if (idEntry == NULL)
                    {
                        printf("Error : Variable %s is not declared! Line No. : %d\n", idNode->type, idNode->lexeme->line_no);
                        numSemanticErrors++;
                    }else if(idEntry->isForLoopVar){
                        printf("Error : Cannot modify for loop variable %s in get_value! Line No. : %d\n", idNode->type, idNode->lexeme->line_no);
                        numSemanticErrors++;
                    }else{
                        idEntry->assigned = 1;
                    }
                }
            }
            else
            {
                printf("Invalid get value statement!");
                numSemanticErrors++;
            }
        }
        else
        {
            for (int i = 0; i < curr->numChildren; i++)
            {
                constructSymbolTable(curr->children[i], parent);
            }
        }
    }
}

void constructSymbolTableDeclarations(ast_node *root, st_table *parent)
{
    ast_node *curr = root;

    if (curr != NULL)
    {
        // printf("Constructing symbol table for %s\n", curr->type);
        // fflush(stdout);
        if (strcmp(curr->type, "PROGRAM") == 0)
        {   traverse_AST(curr,NULL,0,1);// change line 3
            curr->symbolTable = createSymbolTable("GLOBAL_SCOPE", parent, -1, -1, "GLOBAL_SCOPE");

            for (int i = 0; i < curr->numChildren; i++)
            {
                constructSymbolTableDeclarations(curr->children[i], curr->symbolTable);
            }
        }
        else if (strcmp(curr->type, "MODULE_LIST") == 0)
        {
            int numDeclarations = curr->numChildren;
            for (int i = 0; i < numDeclarations; i++)
            {
                ast_node *declaration = (ast_node *)(curr->children[i]);
                id_type *type = createIDType("module", 0, 0, 1, -1, -1, NULL, NULL, 0, 0, NULL, 0, NULL, 0);
                st_entry *prev = getSymbolEntry(parent, declaration->type);
                if (prev != NULL)
                {
                    // CHECK THIS
                    printf("Error : Module %s already declared earlier! Line No. : %d\n", declaration->type, declaration->lexeme->line_no);
                    numSemanticErrors++;
                }
                else
                {
                    st_entry *decEntry = createSymbolEntry(declaration->type, type, parent->scopeName, -1, -1, parent->nestingLevel, declaration->lexeme->line_no, -1);
                    addSymbolEntry(parent, decEntry);
                    printSymbolEntry(st_fp, decEntry);
                    //printf("Added declaration %s to symbol table\n", declaration->type);
                }
            }
        }
        else if (strcmp(curr->type, "MODULE") == 0)
        {
            char *functionName = ((ast_node *)(curr->children[0]))->type;
            st_entry *newEntry = NULL;

            if (getSymbolEntry(parent, functionName) != NULL)
            {
                // should be declared before
                newEntry = getSymbolEntry(parent, functionName);
                //printf("Error : Module already declared earlier\n");

                if (newEntry->assigned == 1)
                {
                    printf("Error : Module %s already defined earlier, overloading not supported! Line No. : %d\n", functionName, ((ast_node *)(curr->children[0]))->lexeme->line_no);
                    numSemanticErrors++;
                    newEntry->unusableFunc = 2;
                    ((ast_node*)curr->children[3])->numChildren = 0;
                }
                else
                {
                    curr->symbolTable = createSymbolTable(functionName, parent, curr->startLine, curr->endLine, functionName);

                    ast_node *inputList = (ast_node *)(curr->children[1]);
                    int numInputParams = inputList->numChildren;

                    id_type **inputParamTypes = (id_type **)malloc(sizeof(id_type *) * numInputParams);

                    for (int i = 0; i < numInputParams; i++)
                    {
                        ast_node *inputParam = (ast_node *)(inputList->children[i]);
                        ast_node *idNode = (ast_node *)(inputParam->children[0]);
                        ast_node *typeNode = (ast_node *)(inputParam->children[1]);
                        int width = 0;

                        if (strcmp(typeNode->type, "RANGE_DATATYPE") == 0)
                        {
                            char *arrayType = ((ast_node *)(typeNode->children[1]))->type;

                            ast_node *rangeNode = (ast_node *)(typeNode->children[0]);

                            ast_node *leftIndexNode = (ast_node *)(rangeNode->children[0]);
                            ast_node *rightIndexNode = (ast_node *)(rangeNode->children[1]);

                            int leftIndex = -1;
                            int rightIndex = -1;
                            int isDynamicLeft = 0;
                            int isDynamicRight = 0;
                            char *leftIndexName = NULL;
                            char *rightIndexName = NULL;

                            if (leftIndexNode->numChildren > 1)
                            {
                                if (strlen(((ast_node *)(leftIndexNode->children[1]))->type) > 0 && ((ast_node *)(leftIndexNode->children[1]))->type[0] >= '0' && ((ast_node *)(leftIndexNode->children[1]))->type[0] <= '9')
                                {
                                    leftIndex = atoi(((ast_node *)(leftIndexNode->children[1]))->type);
                                    if (strcmp(((ast_node *)(leftIndexNode->children[0]))->type, "-") == 0)
                                    {
                                        leftIndex = -1 * leftIndex;
                                    }
                                }
                                else
                                {
                                    isDynamicLeft = 1;
                                    leftIndexName = ((ast_node *)(leftIndexNode->children[1]))->type;
                                    if (strcmp(((ast_node *)(leftIndexNode->children[0]))->type, "-") == 0)
                                    {
                                        char *temp = (char *)malloc(strlen(leftIndexName) + 1);
                                        strcpy(temp + 1, leftIndexName);
                                        temp[0] = '-';
                                        leftIndexName = temp;
                                    }
                                }
                            }
                            else
                            {
                                if (strlen(leftIndexNode->type) > 0 && leftIndexNode->type[0] >= '0' && leftIndexNode->type[0] <= '9')
                                {
                                    leftIndex = atoi(leftIndexNode->type);
                                }
                                else
                                {
                                    isDynamicLeft = 1;
                                    leftIndexName = leftIndexNode->type;
                                }
                            }

                            if (rightIndexNode->numChildren > 1)
                            {
                                if (strlen(((ast_node *)(rightIndexNode->children[1]))->type) > 0 && ((ast_node *)(rightIndexNode->children[1]))->type[0] >= '0' && ((ast_node *)(rightIndexNode->children[1]))->type[0] <= '9')
                                {
                                    rightIndex = atoi(((ast_node *)(rightIndexNode->children[1]))->type);
                                    if (strcmp(((ast_node *)(rightIndexNode->children[0]))->type, "-") == 0)
                                    {
                                        rightIndex = -1 * rightIndex;
                                    }
                                }
                                else
                                {
                                    isDynamicRight = 1;
                                    rightIndexName = ((ast_node *)(rightIndexNode->children[1]))->type;
                                    if (strcmp(((ast_node *)(rightIndexNode->children[0]))->type, "-") == 0)
                                    {
                                        char *temp = (char *)malloc(strlen(rightIndexName) + 1);
                                        strcpy(temp + 1, rightIndexName);
                                        temp[0] = '-';
                                        rightIndexName = temp;
                                    }
                                }
                            }
                            else
                            {
                                if (strlen(rightIndexNode->type) > 0 && rightIndexNode->type[0] >= '0' && rightIndexNode->type[0] <= '9')
                                {
                                    rightIndex = atoi(rightIndexNode->type);
                                }
                                else
                                {
                                    isDynamicRight = 1;
                                    rightIndexName = rightIndexNode->type;
                                }
                            }

                            width = (isDynamicLeft || isDynamicRight) ? 1 : (1 + (rightIndex - leftIndex + 1) * getTypeWidth(arrayType));
                            id_type *idType = createIDType(arrayType, 1, !(isDynamicLeft || isDynamicRight), 0, leftIndex, rightIndex, leftIndexName, rightIndexName, isDynamicLeft, isDynamicRight, NULL, 0, NULL, 0);
                            inputParamTypes[i] = idType;
                        }
                        else
                        {
                            // not array
                            id_type *type = createIDType(typeNode->type, 0, 0, 0, -1, -1, NULL, NULL, 0, 0, NULL, 0, NULL, 0);
                            inputParamTypes[i] = type;
                            width = getTypeWidth(type->type);
                        }

                        st_entry *entry = createSymbolEntry(idNode->type, inputParamTypes[i], curr->symbolTable->scopeName, width, curr->symbolTable->offset, curr->symbolTable->nestingLevel, idNode->lexeme->line_no, curr->symbolTable->offsetAct);
                        entry->isShadow = 1;
                        addSymbolEntry(curr->symbolTable, entry);
                        printSymbolEntry(st_fp, entry);
                    }

                    ast_node *outputList = (ast_node *)(curr->children[2]);
                    int numOutputParams = outputList->numChildren;

                    id_type **outputParamTypes = (id_type **)malloc(sizeof(id_type *) * numOutputParams);

                    for (int i = 0; i < numOutputParams; i++)
                    {
                        ast_node *outputParam = (ast_node *)(outputList->children[i]);
                        ast_node *idNode = (ast_node *)(outputParam->children[0]);
                        ast_node *typeNode = (ast_node *)(outputParam->children[1]);
                        int width = 0;

                        if (strcmp(typeNode->type, "RANGE_DATATYPE") == 0)
                        {
                            char *arrayType = ((ast_node *)(typeNode->children[1]))->type;

                            ast_node *rangeNode = (ast_node *)(typeNode->children[0]);

                            ast_node *leftIndexNode = (ast_node *)(rangeNode->children[0]);
                            ast_node *rightIndexNode = (ast_node *)(rangeNode->children[1]);

                            int leftIndex = -1;
                            int rightIndex = -1;
                            int isDynamicLeft = 0;
                            int isDynamicRight = 0;
                            char *leftIndexName = NULL;
                            char *rightIndexName = NULL;

                            if (leftIndexNode->numChildren > 1)
                            {
                                if (strlen(((ast_node *)(leftIndexNode->children[1]))->type) > 0 && ((ast_node *)(leftIndexNode->children[1]))->type[0] >= '0' && ((ast_node *)(leftIndexNode->children[1]))->type[0] <= '9')
                                {
                                    leftIndex = atoi(((ast_node *)(leftIndexNode->children[1]))->type);
                                    if (strcmp(((ast_node *)(leftIndexNode->children[0]))->type, "-") == 0)
                                    {
                                        leftIndex = -1 * leftIndex;
                                    }
                                }
                                else
                                {
                                    isDynamicLeft = 1;
                                    leftIndexName = ((ast_node *)(leftIndexNode->children[1]))->type;
                                    if (strcmp(((ast_node *)(leftIndexNode->children[0]))->type, "-") == 0)
                                    {
                                        char *temp = (char *)malloc(strlen(leftIndexName) + 1);
                                        strcpy(temp + 1, leftIndexName);
                                        temp[0] = '-';
                                        leftIndexName = temp;
                                    }
                                }
                            }
                            else
                            {
                                if (strlen(leftIndexNode->type) > 0 && leftIndexNode->type[0] >= '0' && leftIndexNode->type[0] <= '9')
                                {
                                    leftIndex = atoi(leftIndexNode->type);
                                }
                                else
                                {
                                    isDynamicLeft = 1;
                                    leftIndexName = leftIndexNode->type;
                                }
                            }

                            if (rightIndexNode->numChildren > 1)
                            {
                                if (strlen(((ast_node *)(rightIndexNode->children[1]))->type) > 0 && ((ast_node *)(rightIndexNode->children[1]))->type[0] >= '0' && ((ast_node *)(rightIndexNode->children[1]))->type[0] <= '9')
                                {
                                    rightIndex = atoi(((ast_node *)(rightIndexNode->children[1]))->type);
                                    if (strcmp(((ast_node *)(rightIndexNode->children[0]))->type, "-") == 0)
                                    {
                                        rightIndex = -1 * rightIndex;
                                    }
                                }
                                else
                                {
                                    isDynamicRight = 1;
                                    rightIndexName = ((ast_node *)(rightIndexNode->children[1]))->type;
                                    if (strcmp(((ast_node *)(rightIndexNode->children[0]))->type, "-") == 0)
                                    {
                                        char *temp = (char *)malloc(strlen(rightIndexName) + 1);
                                        strcpy(temp + 1, rightIndexName);
                                        temp[0] = '-';
                                        rightIndexName = temp;
                                    }
                                }
                            }
                            else
                            {
                                if (strlen(rightIndexNode->type) > 0 && rightIndexNode->type[0] >= '0' && rightIndexNode->type[0] <= '9')
                                {
                                    rightIndex = atoi(rightIndexNode->type);
                                }
                                else
                                {
                                    isDynamicRight = 1;
                                    rightIndexName = rightIndexNode->type;
                                }
                            }

                            width = (isDynamicLeft || isDynamicRight) ? 1 : (1 + (rightIndex - leftIndex + 1) * getTypeWidth(arrayType));
                            id_type *idType = createIDType(arrayType, 1, !(isDynamicLeft || isDynamicRight), 0, leftIndex, rightIndex, leftIndexName, rightIndexName, isDynamicLeft, isDynamicRight, NULL, 0, NULL, 0);
                            outputParamTypes[i] = idType;
                        }
                        else
                        {
                            // not array
                            id_type *type = createIDType(typeNode->type, 0, 0, 0, -1, -1, NULL, NULL, 0, 0, NULL, 0, NULL, 0);
                            outputParamTypes[i] = type;
                            width = getTypeWidth(type->type);
                        }

                        st_entry *entry = createSymbolEntry(idNode->type, outputParamTypes[i], curr->symbolTable->scopeName, width, curr->symbolTable->offset, curr->symbolTable->nestingLevel, idNode->lexeme->line_no, curr->symbolTable->offsetAct);
                        addSymbolEntry(curr->symbolTable, entry);
                        printSymbolEntry(st_fp, entry);
                    }

                    id_type *functionType = createIDType("module", 0, 0, 1, -1, -1, NULL, NULL, 0, 0, inputParamTypes, numInputParams, outputParamTypes, numOutputParams);

                    newEntry->idType = functionType;
                    //printf("Error : Module %s already declared earlier\n", functionName);
                    newEntry->assigned = 1;
                    newEntry->didInFirstPass = 1;
                    addSymbolEntry(parent, newEntry);
                    printSymbolEntry(st_fp, newEntry);
                }
            }
        }
        else
        {
            for (int i = 0; i < curr->numChildren; i++)
            {
                constructSymbolTableDeclarations(curr->children[i], parent);
            }
        }
    }
}
