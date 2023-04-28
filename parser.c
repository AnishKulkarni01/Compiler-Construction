// CS F363 : Compiler Construction
// Group No. 24
// 1. Nachiket Kotalwar               (2020A7PS0024P)
// 2.Parth Patel                      (2020A7PS0026P)
// 3.Partha Sarathi Purkayastha       (2020A7PS0043P)
// 4.Labeeb Ahsan                     (2020A7PS0045P)
// 5.Anish Atul Kulkarni              (2020A7PS0975P)

#include "parserDef.h"
#include "symbolTable.h"

#include "ast.h"

// stack start
typedef struct node
{
    char *data;
    struct node *next;
} node;

typedef struct stack
{
    node *head;
    int size;
} stack;

void push(stack *st, char *s)
{
    node *newNode = (node *)malloc(sizeof(node));
    newNode->data = s;
    newNode->next = st->head->next;
    st->head->next = newNode;
    st->size++;
}

void pop(stack *st)
{
    if (st->size == 0)
    {
        printf("nothing to pop\n");
    }
    else
    {
        node *temp = st->head->next;
        st->head->next = temp->next;
        // free(temp);
        st->size--;
    }
}

char *peek(stack *st)
{
    char *err = "nothing at top";
    if (st->size == 0)
    {
        return err;
    }
    return st->head->next->data;
}

void initStack(stack *st)
{
    st->head = (node *)malloc(sizeof(node));
    st->head->next = NULL;
    st->head->data = "head";
    st->size = 0;
}
// stack end

// stack start
typedef struct node2
{
    t_node *data;
    struct node2 *next;
} node2;

typedef struct stack2
{
    node2 *head;
    int size;
} stack2;

void push2(stack2 *st, t_node *s)
{
    node2 *newNode = (node2 *)malloc(sizeof(node2));
    newNode->data = s;
    newNode->next = st->head->next;
    st->head->next = newNode;
    st->size++;
}

void pop2(stack2 *st)
{
    if (st->size == 0)
    {
        printf("nothing to pop\n");
    }
    else
    {
        node2 *temp = st->head->next;
        st->head->next = temp->next;
        // free(temp);
        st->size--;
    }
}

t_node *peek2(stack2 *st)
{
    if (st->size == 0)
    {
        printf("nothing at top2");
        return NULL;
    }
    return st->head->next->data;
}

void initStack2(stack2 *st)
{
    st->head = (node2 *)malloc(sizeof(node2));
    st->head->next = NULL;
    st->head->data = NULL;
    st->size = 0;
}



void initializeParser()
{
    
    terminals = 0;
    nonTerminals = 0;

    terminalsMap = NULL;
    nonTerminalsMap = NULL;

    memoFirstSets = NULL;
    memoFollowSets = NULL;

    // buffer=(char*)malloc(sizeof(char)*BUF_SIZE);
    numRules = 0;
}

int isNonTerminal(char *s)
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

void clearBuffer(char *buffer)
{
    int i;
    for (i = 0; i < BUF_SIZE; i++)
    {
        buffer[i] = '\0';
    }
}

int readBuffer(FILE *fp, char *buffer)
{
    clearBuffer(buffer);
    int i = 0;
    char c;
    while (i < (BUF_SIZE - 1) && ((c = fgetc(fp)) != EOF))
    {
        buffer[i] = c;
        if (c == '.')
        {
            numRules++;
        }
        i++;
    }
    return i;
}

int countOccurences(char *s, char c)
{
    int count = 0;
    for (int i = 0; s[i]; i++)
    {
        if (s[i] == c)
        {
            count++;
        }
    }
    return count;
}

hash_table *getFirstSet(char *term, grammarRule *rules)
{
    if (memoFirstSets == NULL)
    {
        memoFirstSets = create_table();
    }

    if (get(memoFirstSets, term) != NULL)
    {
        return (hash_table *)get(memoFirstSets, term);
    }

    hash_table *ht = create_table();

    if (!isNonTerminal(term))
    {
        insert(ht, term, term);
        return ht;
    }

    for (int i = 0; i < numRules; i++)
    {
        if (strcmp(rules[i].lhs, term) != 0)
        {
            continue;
        }

        if (rules[i].rhsSize == 0)
        {
            insert(ht, "epsilon", "epsilon");
            continue;
        }

        int epsilonInAll = 1;
        for (int k = 0; k < rules[i].rhsSize; k++)
        {
            hash_table *fs = getFirstSet(rules[i].rhs[k], rules);

            for (int j = 0; j < TABLE_SIZE; j++)
            {
                if (fs->nodes[j] == NULL || strcmp(fs->nodes[j]->keys, "epsilon") == 0)
                    continue;
                insert(ht, fs->nodes[j]->keys, fs->nodes[j]->values);
            }

            if (get(fs, "epsilon") == NULL)
            {
                epsilonInAll = 0;
                break;
            }
        }

        if (epsilonInAll)
        {
            insert(ht, "epsilon", "epsilon");
        }
    }

    insert(memoFirstSets, term, ht);

    return ht;
}

hash_table *getFollowSet(char *term, grammarRule *rules)
{
    if (memoFollowSets == NULL)
    {
        memoFollowSets = create_table();
    }

    if (get(memoFollowSets, term) != NULL)
    {
        return (hash_table *)get(memoFollowSets, term);
    }

    hash_table *ht = create_table();

    // check if start symbol
    if (strcmp(term, rules[0].lhs) == 0)
    { // book rule 1
        insert(ht, "$", "$");
    }

    for (int i = 0; i < numRules; i++)
    {
        for (int j = 0; j < rules[i].rhsSize; j++)
        {
            if (strcmp(rules[i].rhs[j], term) != 0)
            {
                continue;
            }

            if (j == rules[i].rhsSize - 1)
            { // book rule 3 // LAST TERM IN RULE
                if (strcmp(rules[i].lhs, term) == 0)
                {
                    continue;
                }

                hash_table *fs = getFollowSet(rules[i].lhs, rules);
                for (int k = 0; k < TABLE_SIZE; k++)
                {
                    if (fs->nodes[k] == NULL)
                        continue;
                    insert(ht, fs->nodes[k]->keys, fs->nodes[k]->values);
                }
            }
            else
            { // book rule 2 and 3 // NOT LAST TERM IN RULE

                int epsilonInAll = 1;

                for (int k = j + 1; k < rules[i].rhsSize; k++)
                {
                    hash_table *fs = getFirstSet(rules[i].rhs[k], rules);

                    for (int l = 0; l < TABLE_SIZE; l++)
                    {
                        if (fs->nodes[l] == NULL || strcmp(fs->nodes[l]->keys, "epsilon") == 0)
                            continue;
                        insert(ht, fs->nodes[l]->keys, fs->nodes[l]->values);
                    }

                    if (get(fs, "epsilon") == NULL)
                    {
                        epsilonInAll = 0;
                        break;
                    }
                }

                if (epsilonInAll && strcmp(rules[i].lhs, term) != 0)
                {
                    hash_table *fs = getFollowSet(rules[i].lhs, rules);
                    for (int k = 0; k < TABLE_SIZE; k++)
                    {
                        if (fs->nodes[k] == NULL)
                            continue;
                        insert(ht, fs->nodes[k]->keys, fs->nodes[k]->values);
                    }
                }
            }
        }
    }

    insert(memoFollowSets, term, ht);

    return ht;
}

void computeFirstAndFollowSets(grammarRule *rules, hash_table *firstSets, hash_table *followSets)
{
    for (int i = 0; i < numRules; i++)
    {
        if (get(firstSets, rules[i].lhs) == NULL)
        {
            hash_table *firstSet = getFirstSet(rules[i].lhs, rules);
            insert(firstSets, rules[i].lhs, firstSet);
        }

        if (get(followSets, rules[i].lhs) == NULL)
        {
            hash_table *followSet = getFollowSet(rules[i].lhs, rules);
            insert(followSets, rules[i].lhs, followSet);
        }

        for (int j = 0; j < rules[i].rhsSize; j++)
        {
            if (get(firstSets, rules[i].rhs[j]) == NULL)
            {
                hash_table *firstSet = getFirstSet(rules[i].rhs[j], rules);
                insert(firstSets, rules[i].rhs[j], firstSet);
            }

            if (get(followSets, rules[i].rhs[j]) == NULL)
            {
                hash_table *followSet = getFollowSet(rules[i].rhs[j], rules);
                insert(followSets, rules[i].rhs[j], followSet);
            }
        }
    }
}

int **createParseTable(grammarRule *rules, hash_table *firstSets, hash_table *followSets)
{
    int **parseTable = (int **)malloc(nonTerminals * sizeof(int *));
    for (int i = 0; i < nonTerminals; i++)
    {
        parseTable[i] = (int *)malloc(terminals * sizeof(int));
    }

    for (int i = 0; i < nonTerminals; i++)
    {
        for (int j = 0; j < terminals; j++)
        {
            parseTable[i][j] = -1;
        }
    }

    for (int i = 0; i < numRules; i++)
    {
        int epsilonInAll = 1;

        for (int j = 0; j < rules[i].rhsSize; j++)
        {
            hash_table *firstSet = (hash_table *)get(firstSets, rules[i].rhs[j]);

            for (int k = 0; k < TABLE_SIZE; k++)
            {
                if (firstSet->nodes[k] != NULL)
                {

                    if (strcmp(firstSet->nodes[k]->keys, "epsilon") == 0)
                    {
                        continue;
                    }

                    int row = *(int *)get(nonTerminalsMap, rules[i].lhs);

                    int col = *(int *)get(terminalsMap, firstSet->nodes[k]->keys);

                    if (parseTable[row][col] != -1 && parseTable[row][col] != i)
                    {
                        printf("ERROR! Grammar is not LL1 due to rule %d\n", i + 1);
                    }

                    parseTable[row][col] = i;
                }
            }
            if (get(firstSet, "epsilon") == NULL)
            {
                epsilonInAll = 0;
                break;
            }
        }

        hash_table *followSet = (hash_table *)get(followSets, rules[i].lhs);

        if (epsilonInAll)
        {
            for (int k = 0; k < TABLE_SIZE; k++)
            {
                if (followSet->nodes[k] != NULL)
                {
                    int row = *(int *)get(nonTerminalsMap, rules[i].lhs);

                    int col = *(int *)get(terminalsMap, followSet->nodes[k]->keys);

                    if (parseTable[row][col] != -1 && parseTable[row][col] != i)
                    {
                        printf("ERROR! Grammar is not LL1 due to rule %d\n", i + 1);
                    }

                    parseTable[row][col] = i;
                }
            }
        }

        if (epsilonInAll && get(followSet, "$") != NULL)
        {
            int row = *(int *)get(nonTerminalsMap, rules[i].lhs);
            int col = *(int *)get(terminalsMap, "$");

            if (parseTable[row][col] != -1 && parseTable[row][col] != i)
            {
                printf("ERROR! Grammar is not LL1 due to rule %d\n", i + 1);
            }

            parseTable[row][col] = i;
        }
    }

    return parseTable;
}

grammarRule *parseInputSourceCode(const char *testCaseFile)
{
    FILE *fp = fopen(testCaseFile, "r");
    if (fp == NULL)
    {
        printf("Error opening file");
        exit(1);
    }
    readBuffer(fp, buffer);

    grammarRule *rules = (grammarRule *)malloc(sizeof(grammarRule) * numRules);

    char **tokens = (char **)malloc(sizeof(char *) * numRules);
    int rulesRead = 0;

    char *token = strtok(buffer, ".");
    while (token != NULL)
    {
        tokens[rulesRead] = token;
        token = strtok(NULL, ".");
        rulesRead++;
    }

    for (int i = 0; i < numRules; i++)
    {
        grammarRule rule;

        char tokenCopy[200];
        strcpy(tokenCopy, tokens[i]);

        char *token = strtok(tokens[i], " ");
        if (token[0] == '\n')
            token++;

        rule.lhs = token;

        token = strtok(NULL, " ");

        if (token == NULL)
        {
            rule.rhs = NULL;
            rule.rhsSize = 0;
            rules[i] = rule;
            continue;
        }

        rule.rhsSize = countOccurences(tokenCopy, ' ') - 1;
        rule.rhs = (char **)malloc(sizeof(char *) * rule.rhsSize);
        for (int j = 0; j < rule.rhsSize; j++)
        {
            if (token[0] == '\n')
                token++;
            rule.rhs[j] = token;
            token = strtok(NULL, " ");
        }
        rules[i] = rule;
    }

    return rules;
}

void constructVariablesMap(grammarRule *rules)
{
    hash_table *nonTerminalsSet = create_table();
    hash_table *terminalsSet = create_table();

    for (int i = 0; i < numRules; i++)
    {
        if (!isNonTerminal(rules[i].lhs))
        {
            if (get(terminalsSet, rules[i].lhs) == NULL)
            {
                insert(terminalsSet, rules[i].lhs, rules[i].lhs);
                terminals++;
            }
        }
        else
        {
            if (get(nonTerminalsSet, rules[i].lhs) == NULL)
            {
                insert(nonTerminalsSet, rules[i].lhs, rules[i].lhs);
                nonTerminals++;
            }
        }

        for (int j = 0; j < rules[i].rhsSize; j++)
        {
            if (!isNonTerminal(rules[i].rhs[j]))
            {
                if (get(terminalsSet, rules[i].rhs[j]) == NULL)
                {
                    insert(terminalsSet, rules[i].rhs[j], rules[i].rhs[j]);
                    terminals++;
                }
            }
            else
            {
                if (get(nonTerminalsSet, rules[i].rhs[j]) == NULL)
                {
                    insert(nonTerminalsSet, rules[i].rhs[j], rules[i].rhs[j]);
                    nonTerminals++;
                }
            }
        }
    }

    insert(terminalsSet, "$", "$"); // $ is added as it is in the parser table
    terminals++;

    terminalsMap = create_table();
    nonTerminalsMap = create_table();

    int terminalIndex = 0;
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (terminalsSet->nodes[i] != NULL)
        {
            int *terminalIndexPtr = (int *)malloc(sizeof(int));
            *terminalIndexPtr = terminalIndex;
            insert(terminalsMap, (char *)terminalsSet->nodes[i]->keys, terminalIndexPtr);
            terminalIndex++;
        }
    }

    int nonTerminalIndex = 0;
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (nonTerminalsSet->nodes[i] != NULL)
        {
            int *nonTerminalIndexPtr = (int *)malloc(sizeof(int));
            *nonTerminalIndexPtr = nonTerminalIndex;
            insert(nonTerminalsMap, nonTerminalsSet->nodes[i]->keys, nonTerminalIndexPtr);
            nonTerminalIndex++;
        }
    }
}

hash_table *constructSynchSets(hash_table *followSets)
{
    hash_table *synchSets = create_table();

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (nonTerminalsMap->nodes[i] != NULL)
        {
            hash_table *synchSet = create_table();

            hash_table *followSet = (hash_table *)get(followSets, nonTerminalsMap->nodes[i]->keys);

            for (int j = 0; j < TABLE_SIZE; j++)
            {
                if (followSet->nodes[j] != NULL)
                {
                    insert(synchSet, followSet->nodes[j]->keys, followSet->nodes[j]->keys);
                }
            }

            // insert(synchSet, "semicol", "semicol");

            if (strcmp(nonTerminalsMap->nodes[i]->keys, "Statement") == 0)
            {
                insert(synchSet, "semicol", "semicol");
            }

            if (strcmp(nonTerminalsMap->nodes[i]->keys, "Statements") == 0)
            {
                insert(synchSet, "semicol", "semicol");
                insert(synchSet, "end", "end");
            }

            // if (strcmp(nonTerminalsMap->nodes[i]->keys, "Expression") == 0)
            // {
            //     insert(synchSet, "semicol", "semicol");
            // }

            if ((strcmp(nonTerminalsMap->nodes[i]->keys, "Iostmt") == 0) || (strcmp(nonTerminalsMap->nodes[i]->keys, "Simplestmt") == 0) || (strcmp(nonTerminalsMap->nodes[i]->keys, "Declarestmt") == 0) || (strcmp(nonTerminalsMap->nodes[i]->keys, "Iterativestmt") == 0) || (strcmp(nonTerminalsMap->nodes[i]->keys, "Conditionalstmt") == 0))
            {
                insert(synchSet, "print", "print");
                insert(synchSet, "declare", "declare");
                insert(synchSet, "end", "end");
                insert(synchSet, "get_value", "get_value");
                insert(synchSet, "switch", "switch");
                insert(synchSet, "semicol", "semicol");
                insert(synchSet, "use", "use");
            }

            insert(synchSets, nonTerminalsMap->nodes[i]->keys, synchSet);
        }
    }

    return synchSets;
}
int generateParseTree(t_node *root, const char *parseTreeFile)
{
    FILE *fp = fopen("this.txt", "w");
    // printf("Tree be like:\n ");
    // printf("START => \n\n");

    inorder_traversal(root, fp);
    //preorder_traversal(root, fp,0,1);
    // printf("\nEND\n");

    fclose(fp);

    printf("Parse Tree generated successfully !! \n");
    return 0;
}

void ll1Check()
{

    initializeParser();

    grammarRule *rules = parseInputSourceCode("grammar.txt");

    if (rules == NULL)
    {
        printf("Empty Grammer File !! \n");
        return;
    }

    constructVariablesMap(rules);

    hash_table *firstSets = create_table();
    hash_table *followSets = create_table();

    computeFirstAndFollowSets(rules, firstSets, followSets);

    int **parseTable = createParseTable(rules, firstSets, followSets);
}

FILE* fp;





void parserDriver(const char *testCaseFile, int buffer_size, const char *parseTreeFile)
{

    initializeParser();

    initializeLexer(testCaseFile, buffer_size);

    grammarRule *rules = parseInputSourceCode("grammar.txt");

    if (rules == NULL)
    {
        printf("Empty Grammer File !! \n");
        return;
    }

    constructVariablesMap(rules);

    hash_table *firstSets = create_table();
    hash_table *followSets = create_table();

    computeFirstAndFollowSets(rules, firstSets, followSets);

    int **parseTable = createParseTable(rules, firstSets, followSets);

    hash_table *synchSets = constructSynchSets(followSets);

    // struct StackNode* st = NULL;
    stack *st = (stack *)malloc(sizeof(stack));
    initStack(st);

    stack2 *st2 = (stack2 *)malloc(sizeof(stack2));
    initStack2(st2);

    stack2 *st3 = (stack2 *)malloc(sizeof(stack2));
    initStack2(st3);

    push(st, "$");
    push(st, rules[0].lhs);

    int treeNodeIndex = 0;
    push2(st2, create_node(treeNodeIndex++, "$"));

    t_node *root = create_node(treeNodeIndex++, rules[0].lhs);
    t_node *toPrint = root;
    push2(st2, root);

    char *dol = (char *)malloc(2 * sizeof(char));
    dol[0] = '$';
    dol[1] = '\0';
    lexeme *l = getNextToken();
    char inputToken[100];
    strcpy(inputToken, l->token);
    // printf("Input: --%s-- --%s--\n", inputToken,l->lexeme);
    int latestLineNo = l->line_no;
    int prevLineNo = l->line_no;

    int treeIndex = 0;

    t_node *currentNode = root;

    while (strcmp(peek(st), "$") != 0)
    {
        char *top = peek(st);
        t_node *top2 = peek2(st2);

        if (strcmp(top, inputToken) == 0)
        {
            pop(st);
            pop2(st2);
            currentNode = peek2(st2);

            if (l != NULL)
            {
                top2->line_no = l->line_no;
                top2->lexeme = l->lexeme;
                top2->value_if_num = l->value_if_num;
                top2->value_if_rnum = l->value_if_rnum;
                top2->lex = l; 
            }

            l = getNextToken();

            if (l == NULL)
                strcpy(inputToken, dol);
            else
                strcpy(inputToken, l->token);

            if (l != NULL)
            {
                prevLineNo = latestLineNo;
                latestLineNo = l->line_no;
            }
        }
        else if (!isNonTerminal(top))
        {
            printf("Parser Error - Line No: %d . Terminal on stack top - %s, does not match with next input - %s\n", prevLineNo, top, inputToken);

            // printf("Parser Synchronizing... Assumed %s\n", top);
            pop(st);
            pop2(st2);

            continue;
        }
        else
        {
            int row = *(int *)get(nonTerminalsMap, top);

            int col = *(int *)get(terminalsMap, inputToken);

            int ruleIndex = parseTable[row][col];

            if (ruleIndex == -1)
            {
                printf("Parser Error - Line No: %d . Did not find a Parse Table entry for the pair - %s, %s\n", prevLineNo, top, inputToken);

                hash_table *synchSet = (hash_table *)get(synchSets, top);

                while (get(synchSet, inputToken) == NULL)
                {

                    if (inputToken[0] == '$')
                    {
                        printf("Reached end of input.\n");
                        return;
                    }

                    // printf("Parser Synchronizing... Skipped %s\n", inputToken);
                    l = getNextToken();
                    if (l == NULL)
                        strcpy(inputToken, dol);
                    else
                        strcpy(inputToken, l->token);

                    if (l != NULL)
                    {
                        prevLineNo = latestLineNo;
                        latestLineNo = l->line_no;
                    }
                }

                pop(st);
                pop2(st2);
                continue;
            }

            pop(st);

            t_node *top2 = peek2(st2);
            pop2(st2);

            top2->grammarRuleIndex = ruleIndex;

            for (int i = rules[ruleIndex].rhsSize - 1; i >= 0; i--)
            {
                push(st, rules[ruleIndex].rhs[i]);
            }

            int pushed = 0;

            for (int i = 0; i < rules[ruleIndex].rhsSize; i++)
            {
                t_node *new_node = create_node(currentNode->type, rules[ruleIndex].rhs[i]);
                add_child(currentNode, new_node);
                push2(st3, new_node);
                pushed++;
            }

            while (pushed--)
            {
                t_node *popping = peek2(st3);
                pop2(st3);
                push2(st2, popping);
            }

            currentNode = peek2(st2);
        }
    }

    if (strcmp(inputToken, "$") == 0)
    {
        printf("Accepted\n");
    }
    else
    {
        printf("Parser Error - Line No: %d . Entire input not consumed\n", latestLineNo);
    }

    printf("Generating Parse Tree\n");
    generateParseTree(toPrint, NULL);
    printf("Finished Generating Parse Tree\n");

    // printf("Generating AST\n");
    // construct_AST(toPrint,rules);

    // printf("\n\nPrinting AST\n");
    // FILE *fp1 = fopen("ast.txt","w");
    // traverse_AST(toPrint->node_syn,fp1,0,1);
    // fclose(fp1);
    // printf("Finished Printing AST\n"); 

    

    // printf("\n\n----Generating Symbol Table----\nIMP: Module Related Errors can appear at the start, but line numbers are accurate.\n\n\n");
    // fp = fopen("symbolTable.txt", "w");

    // initializeFp(fp);
    // initializeSemanticAnalyzer();
    // constructSymbolTableDeclarations(toPrint->node_syn, NULL);
    // fflush(stdout);
    // constructSymbolTable(toPrint->node_syn, NULL);
    // fclose(fp);
    // printf("\n\n----Finished Generating Symbol Table----\n\n\n\n");

    // printf("\n\n----PRINTING SYMBOL TABLE----\n\n\n\n");
    // printf("VariableName     Scope(ModuleName)     Scope(LineNumbers)     TypeOfElement     IsArray     ArrayType    ArrayRange  Width   Offset    NestingLevel\n");
    // printAllScopeTables(toPrint->node_syn);

    // printf("\n\n----PRINTING Activation Records----\n\n\n\n");
    // printAllActivationRecords(toPrint->node_syn);

    // printf("\n\n----PRINTING ARRAYS---\n\n\n\n");
    // printAllScopeArrays(toPrint->node_syn);

    // printf("\n\n----Generating 3 Address Code----\n\n\n\n");
    // fp=fopen("3AC.txt","w");
    // quadrupleList* ql = create_QuadrupleList();
    // print_Quadruples(toPrint->node_syn,((ast_node*)(toPrint->node_syn))->symbolTable,ql,fp);
    // fclose(fp);
    // printf("\n\n----Finished Generating 3 Address Code----\n\n\n\n");

    // printf(" Printinf Quads From Quadruple List\n");
    // print_QuadrupleList(ql);
    // printf("Finished Printing Quads From Quadruple List\n");

    // printf("\n\n----Generating Final Code----\n\n\n\n");
    // fp=fopen("psp.asm","w");
    // codegen(ql,fp);
    // fclose(fp);
    // printf("\n\n----Finished Generating Final Code----\n\n\n\n");

    

}

void parserDriver_uptill_SymbolTable(const char *testCaseFile, int buffer_size, const char *parseTreeFile)
{

    initializeParser();

    initializeLexer(testCaseFile, buffer_size);

    grammarRule *rules = parseInputSourceCode("grammar.txt");

    if (rules == NULL)
    {
        printf("Empty Grammer File !! \n");
        return;
    }

    constructVariablesMap(rules);

    hash_table *firstSets = create_table();
    hash_table *followSets = create_table();

    computeFirstAndFollowSets(rules, firstSets, followSets);

    int **parseTable = createParseTable(rules, firstSets, followSets);

    hash_table *synchSets = constructSynchSets(followSets);

    // struct StackNode* st = NULL;
    stack *st = (stack *)malloc(sizeof(stack));
    initStack(st);

    stack2 *st2 = (stack2 *)malloc(sizeof(stack2));
    initStack2(st2);

    stack2 *st3 = (stack2 *)malloc(sizeof(stack2));
    initStack2(st3);

    push(st, "$");
    push(st, rules[0].lhs);

    int treeNodeIndex = 0;
    push2(st2, create_node(treeNodeIndex++, "$"));

    t_node *root = create_node(treeNodeIndex++, rules[0].lhs);
    t_node *toPrint = root;
    push2(st2, root);

    char *dol = (char *)malloc(2 * sizeof(char));
    dol[0] = '$';
    dol[1] = '\0';
    lexeme *l = getNextToken();
    char inputToken[100];
    strcpy(inputToken, l->token);
    // printf("Input: --%s-- --%s--\n", inputToken,l->lexeme);
    int latestLineNo = l->line_no;
    int prevLineNo = l->line_no;

    int treeIndex = 0;

    t_node *currentNode = root;

    while (strcmp(peek(st), "$") != 0)
    {
        char *top = peek(st);
        t_node *top2 = peek2(st2);

        if (strcmp(top, inputToken) == 0)
        {
            pop(st);
            pop2(st2);
            currentNode = peek2(st2);

            if (l != NULL)
            {
                top2->line_no = l->line_no;
                top2->lexeme = l->lexeme;
                top2->value_if_num = l->value_if_num;
                top2->value_if_rnum = l->value_if_rnum;
                top2->lex = l; 
            }

            l = getNextToken();

            if (l == NULL)
                strcpy(inputToken, dol);
            else
                strcpy(inputToken, l->token);

            if (l != NULL)
            {
                prevLineNo = latestLineNo;
                latestLineNo = l->line_no;
            }
        }
        else if (!isNonTerminal(top))
        {
            printf("Parser Error - Line No: %d . Terminal on stack top - %s, does not match with next input - %s\n", prevLineNo, top, inputToken);

            // printf("Parser Synchronizing... Assumed %s\n", top);
            pop(st);
            pop2(st2);

            continue;
        }
        else
        {
            int row = *(int *)get(nonTerminalsMap, top);

            int col = *(int *)get(terminalsMap, inputToken);

            int ruleIndex = parseTable[row][col];

            if (ruleIndex == -1)
            {
                printf("Parser Error - Line No: %d . Did not find a Parse Table entry for the pair - %s, %s\n", prevLineNo, top, inputToken);

                hash_table *synchSet = (hash_table *)get(synchSets, top);

                while (get(synchSet, inputToken) == NULL)
                {

                    if (inputToken[0] == '$')
                    {
                        printf("Reached end of input.\n");
                        return;
                    }

                    // printf("Parser Synchronizing... Skipped %s\n", inputToken);
                    l = getNextToken();
                    if (l == NULL)
                        strcpy(inputToken, dol);
                    else
                        strcpy(inputToken, l->token);

                    if (l != NULL)
                    {
                        prevLineNo = latestLineNo;
                        latestLineNo = l->line_no;
                    }
                }

                pop(st);
                pop2(st2);
                continue;
            }

            pop(st);

            t_node *top2 = peek2(st2);
            pop2(st2);

            top2->grammarRuleIndex = ruleIndex;

            for (int i = rules[ruleIndex].rhsSize - 1; i >= 0; i--)
            {
                push(st, rules[ruleIndex].rhs[i]);
            }

            int pushed = 0;

            for (int i = 0; i < rules[ruleIndex].rhsSize; i++)
            {
                t_node *new_node = create_node(currentNode->type, rules[ruleIndex].rhs[i]);
                add_child(currentNode, new_node);
                push2(st3, new_node);
                pushed++;
            }

            while (pushed--)
            {
                t_node *popping = peek2(st3);
                pop2(st3);
                push2(st2, popping);
            }

            currentNode = peek2(st2);
        }
    }

    if (strcmp(inputToken, "$") == 0)
    {
        printf("Accepted\n");
    }
    else
    {
        printf("Parser Error - Line No: %d . Entire input not consumed\n", latestLineNo);
    }

    // printf("Generating Parse Tree\n");
    // generateParseTree(toPrint, NULL);
    // printf("Finished Generating Parse Tree\n");

    //printf("Generating AST\n");
    construct_AST(toPrint,rules);

    // printf("\n\nPrinting AST\n");
    // FILE *fp1 = fopen("ast.txt","w");
    // traverse_AST(toPrint->node_syn,fp1,0,1);
    // fclose(fp1);
    // printf("Finished Printing AST\n"); 

    

    printf("\n\n----Generating Symbol Table----\nIMP: Module Related Errors can appear at the start, but line numbers are accurate.\n\n\n");
    fp = fopen("symbolTable.txt", "w");

    initializeFp(fp);
    initializeSemanticAnalyzer();
    constructSymbolTableDeclarations(toPrint->node_syn, NULL);
    fflush(stdout);
    constructSymbolTable(toPrint->node_syn, NULL);
    fclose(fp);
    printf("\n\n----Finished Generating Symbol Table----\n\n\n\n");

    printf("\n\n----PRINTING SYMBOL TABLE----\n\n\n\n");
printf("%-15s  %-20s  %-20s  %-25s  %-20s  %-12s  %-8s  %-8s  %-8s  %-8s\n","VariableName",     "Scope(ModuleName)",     "Scope(LineNumbers)",     "TypeOfElement",     "IsArray",     "ArrayType",    "ArrayRange",  "Width",   "Offset","NestingLevel");    printAllScopeTables(toPrint->node_syn);

    // printf("\n\n----PRINTING Activation Records----\n\n\n\n");
    // printAllActivationRecords(toPrint->node_syn);

    // printf("\n\n----PRINTING ARRAYS---\n\n\n\n");
    // printAllScopeArrays(toPrint->node_syn);

    // printf("\n\n----Generating 3 Address Code----\n\n\n\n");
    // fp=fopen("3AC.txt","w");
    // quadrupleList* ql = create_QuadrupleList();
    // print_Quadruples(toPrint->node_syn,((ast_node*)(toPrint->node_syn))->symbolTable,ql,fp);
    // fclose(fp);
    // printf("\n\n----Finished Generating 3 Address Code----\n\n\n\n");

    // printf(" Printinf Quads From Quadruple List\n");
    // print_QuadrupleList(ql);
    // printf("Finished Printing Quads From Quadruple List\n");

    // printf("\n\n----Generating Final Code----\n\n\n\n");
    // fp=fopen("psp.asm","w");
    // codegen(ql,fp);
    // fclose(fp);
    // printf("\n\n----Finished Generating Final Code----\n\n\n\n");

    

}

void parserDriver_uptill_AST(const char *testCaseFile, int buffer_size, const char *parseTreeFile)
{

    initializeParser();

    initializeLexer(testCaseFile, buffer_size);

    grammarRule *rules = parseInputSourceCode("grammar.txt");

    if (rules == NULL)
    {
        printf("Empty Grammer File !! \n");
        return;
    }

    constructVariablesMap(rules);

    hash_table *firstSets = create_table();
    hash_table *followSets = create_table();

    computeFirstAndFollowSets(rules, firstSets, followSets);

    int **parseTable = createParseTable(rules, firstSets, followSets);

    hash_table *synchSets = constructSynchSets(followSets);

    // struct StackNode* st = NULL;
    stack *st = (stack *)malloc(sizeof(stack));
    initStack(st);

    stack2 *st2 = (stack2 *)malloc(sizeof(stack2));
    initStack2(st2);

    stack2 *st3 = (stack2 *)malloc(sizeof(stack2));
    initStack2(st3);

    push(st, "$");
    push(st, rules[0].lhs);

    int treeNodeIndex = 0;
    push2(st2, create_node(treeNodeIndex++, "$"));

    t_node *root = create_node(treeNodeIndex++, rules[0].lhs);
    t_node *toPrint = root;
    push2(st2, root);

    char *dol = (char *)malloc(2 * sizeof(char));
    dol[0] = '$';
    dol[1] = '\0';
    lexeme *l = getNextToken();
    char inputToken[100];
    strcpy(inputToken, l->token);
    // printf("Input: --%s-- --%s--\n", inputToken,l->lexeme);
    int latestLineNo = l->line_no;
    int prevLineNo = l->line_no;

    int treeIndex = 0;

    t_node *currentNode = root;

    while (strcmp(peek(st), "$") != 0)
    {
        char *top = peek(st);
        t_node *top2 = peek2(st2);

        if (strcmp(top, inputToken) == 0)
        {
            pop(st);
            pop2(st2);
            currentNode = peek2(st2);

            if (l != NULL)
            {
                top2->line_no = l->line_no;
                top2->lexeme = l->lexeme;
                top2->value_if_num = l->value_if_num;
                top2->value_if_rnum = l->value_if_rnum;
                top2->lex = l; 
            }

            l = getNextToken();

            if (l == NULL)
                strcpy(inputToken, dol);
            else
                strcpy(inputToken, l->token);

            if (l != NULL)
            {
                prevLineNo = latestLineNo;
                latestLineNo = l->line_no;
            }
        }
        else if (!isNonTerminal(top))
        {
            printf("Parser Error - Line No: %d . Terminal on stack top - %s, does not match with next input - %s\n", prevLineNo, top, inputToken);

            // printf("Parser Synchronizing... Assumed %s\n", top);
            pop(st);
            pop2(st2);

            continue;
        }
        else
        {
            int row = *(int *)get(nonTerminalsMap, top);

            int col = *(int *)get(terminalsMap, inputToken);

            int ruleIndex = parseTable[row][col];

            if (ruleIndex == -1)
            {
                printf("Parser Error - Line No: %d . Did not find a Parse Table entry for the pair - %s, %s\n", prevLineNo, top, inputToken);

                hash_table *synchSet = (hash_table *)get(synchSets, top);

                while (get(synchSet, inputToken) == NULL)
                {

                    if (inputToken[0] == '$')
                    {
                        printf("Reached end of input.\n");
                        return;
                    }

                    // printf("Parser Synchronizing... Skipped %s\n", inputToken);
                    l = getNextToken();
                    if (l == NULL)
                        strcpy(inputToken, dol);
                    else
                        strcpy(inputToken, l->token);

                    if (l != NULL)
                    {
                        prevLineNo = latestLineNo;
                        latestLineNo = l->line_no;
                    }
                }

                pop(st);
                pop2(st2);
                continue;
            }

            pop(st);

            t_node *top2 = peek2(st2);
            pop2(st2);

            top2->grammarRuleIndex = ruleIndex;

            for (int i = rules[ruleIndex].rhsSize - 1; i >= 0; i--)
            {
                push(st, rules[ruleIndex].rhs[i]);
            }

            int pushed = 0;

            for (int i = 0; i < rules[ruleIndex].rhsSize; i++)
            {
                t_node *new_node = create_node(currentNode->type, rules[ruleIndex].rhs[i]);
                add_child(currentNode, new_node);
                push2(st3, new_node);
                pushed++;
            }

            while (pushed--)
            {
                t_node *popping = peek2(st3);
                pop2(st3);
                push2(st2, popping);
            }

            currentNode = peek2(st2);
        }
    }

    if (strcmp(inputToken, "$") == 0)
    {
        printf("Accepted\n");
    }
    else
    {
        printf("Parser Error - Line No: %d . Entire input not consumed\n", latestLineNo);
    }

    // printf("Generating Parse Tree\n");
    // generateParseTree(toPrint, NULL);
    // printf("Finished Generating Parse Tree\n");

    printf("Generating AST\n");
    construct_AST(toPrint,rules);

    printf("\n\nPrinting AST\n");
    FILE *fp1 = fopen("ast.txt","w");
    traverse_AST(toPrint->node_syn,fp1,0,1);
    fclose(fp1);
    printf("Finished Printing AST\n"); 

    

    // printf("\n\n----Generating Symbol Table----\nIMP: Module Related Errors can appear at the start, but line numbers are accurate.\n\n\n");
    // fp = fopen("symbolTable.txt", "w");

    // initializeFp(fp);
    // initializeSemanticAnalyzer();
    // constructSymbolTableDeclarations(toPrint->node_syn, NULL);
    // fflush(stdout);
    // constructSymbolTable(toPrint->node_syn, NULL);
    // fclose(fp);
    // printf("\n\n----Finished Generating Symbol Table----\n\n\n\n");

    // printf("\n\n----PRINTING SYMBOL TABLE----\n\n\n\n");
    // printf("VariableName     Scope(ModuleName)     Scope(LineNumbers)     TypeOfElement     IsArray     ArrayType    ArrayRange  Width   Offset    NestingLevel\n");
    // printAllScopeTables(toPrint->node_syn);

    // printf("\n\n----PRINTING Activation Records----\n\n\n\n");
    // printAllActivationRecords(toPrint->node_syn);

    // printf("\n\n----PRINTING ARRAYS---\n\n\n\n");
    // printAllScopeArrays(toPrint->node_syn);

    // printf("\n\n----Generating 3 Address Code----\n\n\n\n");
    // fp=fopen("3AC.txt","w");
    // quadrupleList* ql = create_QuadrupleList();
    // print_Quadruples(toPrint->node_syn,((ast_node*)(toPrint->node_syn))->symbolTable,ql,fp);
    // fclose(fp);
    // printf("\n\n----Finished Generating 3 Address Code----\n\n\n\n");

    // printf(" Printinf Quads From Quadruple List\n");
    // print_QuadrupleList(ql);
    // printf("Finished Printing Quads From Quadruple List\n");

    // printf("\n\n----Generating Final Code----\n\n\n\n");
    // fp=fopen("psp.asm","w");
    // codegen(ql,fp);
    // fclose(fp);
    // printf("\n\n----Finished Generating Final Code----\n\n\n\n");

    

}


void parserDriver_uptill_ActivationRecord(const char *testCaseFile, int buffer_size, const char *parseTreeFile)
{

    initializeParser();

    initializeLexer(testCaseFile, buffer_size);

    grammarRule *rules = parseInputSourceCode("grammar.txt");

    if (rules == NULL)
    {
        printf("Empty Grammer File !! \n");
        return;
    }

    constructVariablesMap(rules);

    hash_table *firstSets = create_table();
    hash_table *followSets = create_table();

    computeFirstAndFollowSets(rules, firstSets, followSets);

    int **parseTable = createParseTable(rules, firstSets, followSets);

    hash_table *synchSets = constructSynchSets(followSets);

    // struct StackNode* st = NULL;
    stack *st = (stack *)malloc(sizeof(stack));
    initStack(st);

    stack2 *st2 = (stack2 *)malloc(sizeof(stack2));
    initStack2(st2);

    stack2 *st3 = (stack2 *)malloc(sizeof(stack2));
    initStack2(st3);

    push(st, "$");
    push(st, rules[0].lhs);

    int treeNodeIndex = 0;
    push2(st2, create_node(treeNodeIndex++, "$"));

    t_node *root = create_node(treeNodeIndex++, rules[0].lhs);
    t_node *toPrint = root;
    push2(st2, root);

    char *dol = (char *)malloc(2 * sizeof(char));
    dol[0] = '$';
    dol[1] = '\0';
    lexeme *l = getNextToken();
    char inputToken[100];
    strcpy(inputToken, l->token);
    // printf("Input: --%s-- --%s--\n", inputToken,l->lexeme);
    int latestLineNo = l->line_no;
    int prevLineNo = l->line_no;

    int treeIndex = 0;

    t_node *currentNode = root;

    while (strcmp(peek(st), "$") != 0)
    {
        char *top = peek(st);
        t_node *top2 = peek2(st2);

        if (strcmp(top, inputToken) == 0)
        {
            pop(st);
            pop2(st2);
            currentNode = peek2(st2);

            if (l != NULL)
            {
                top2->line_no = l->line_no;
                top2->lexeme = l->lexeme;
                top2->value_if_num = l->value_if_num;
                top2->value_if_rnum = l->value_if_rnum;
                top2->lex = l; 
            }

            l = getNextToken();

            if (l == NULL)
                strcpy(inputToken, dol);
            else
                strcpy(inputToken, l->token);

            if (l != NULL)
            {
                prevLineNo = latestLineNo;
                latestLineNo = l->line_no;
            }
        }
        else if (!isNonTerminal(top))
        {
            printf("Parser Error - Line No: %d . Terminal on stack top - %s, does not match with next input - %s\n", prevLineNo, top, inputToken);

            // printf("Parser Synchronizing... Assumed %s\n", top);
            pop(st);
            pop2(st2);

            continue;
        }
        else
        {
            int row = *(int *)get(nonTerminalsMap, top);

            int col = *(int *)get(terminalsMap, inputToken);

            int ruleIndex = parseTable[row][col];

            if (ruleIndex == -1)
            {
                printf("Parser Error - Line No: %d . Did not find a Parse Table entry for the pair - %s, %s\n", prevLineNo, top, inputToken);

                hash_table *synchSet = (hash_table *)get(synchSets, top);

                while (get(synchSet, inputToken) == NULL)
                {

                    if (inputToken[0] == '$')
                    {
                        printf("Reached end of input.\n");
                        return;
                    }

                    // printf("Parser Synchronizing... Skipped %s\n", inputToken);
                    l = getNextToken();
                    if (l == NULL)
                        strcpy(inputToken, dol);
                    else
                        strcpy(inputToken, l->token);

                    if (l != NULL)
                    {
                        prevLineNo = latestLineNo;
                        latestLineNo = l->line_no;
                    }
                }

                pop(st);
                pop2(st2);
                continue;
            }

            pop(st);

            t_node *top2 = peek2(st2);
            pop2(st2);

            top2->grammarRuleIndex = ruleIndex;

            for (int i = rules[ruleIndex].rhsSize - 1; i >= 0; i--)
            {
                push(st, rules[ruleIndex].rhs[i]);
            }

            int pushed = 0;

            for (int i = 0; i < rules[ruleIndex].rhsSize; i++)
            {
                t_node *new_node = create_node(currentNode->type, rules[ruleIndex].rhs[i]);
                add_child(currentNode, new_node);
                push2(st3, new_node);
                pushed++;
            }

            while (pushed--)
            {
                t_node *popping = peek2(st3);
                pop2(st3);
                push2(st2, popping);
            }

            currentNode = peek2(st2);
        }
    }

    if (strcmp(inputToken, "$") == 0)
    {
        printf("Accepted\n");
    }
    else
    {
        printf("Parser Error - Line No: %d . Entire input not consumed\n", latestLineNo);
    }

    // printf("Generating Parse Tree\n");
    // generateParseTree(toPrint, NULL);
    // printf("Finished Generating Parse Tree\n");

    //printf("Generating AST\n");
    construct_AST(toPrint,rules);

    // printf("\n\nPrinting AST\n");
    // FILE *fp1 = fopen("ast.txt","w");
    // traverse_AST(toPrint->node_syn,fp1,0,1);
    // fclose(fp1);
    // printf("Finished Printing AST\n"); 

    

    printf("\n\n----Generating Symbol Table----\nIMP: Module Related Errors can appear at the start, but line numbers are accurate.\n\n\n");
    fp = fopen("symbolTable.txt", "w");

    initializeFp(fp);
    initializeSemanticAnalyzer();
    constructSymbolTableDeclarations(toPrint->node_syn, NULL);
    fflush(stdout);
    constructSymbolTable(toPrint->node_syn, NULL);
    fclose(fp);
    printf("\n\n----Finished Generating Symbol Table----\n\n\n\n");

    // printf("\n\n----PRINTING SYMBOL TABLE----\n\n\n\n");
    // printf("VariableName     Scope(ModuleName)     Scope(LineNumbers)     TypeOfElement     IsArray     ArrayType    ArrayRange  Width   Offset    NestingLevel\n");
    // printAllScopeTables(toPrint->node_syn);

    printf("\n\n----PRINTING Activation Records----\n\n\n\n");
    printAllActivationRecords(toPrint->node_syn);

    // printf("\n\n----PRINTING ARRAYS---\n\n\n\n");
    // printAllScopeArrays(toPrint->node_syn);

    // printf("\n\n----Generating 3 Address Code----\n\n\n\n");
    // fp=fopen("3AC.txt","w");
    // quadrupleList* ql = create_QuadrupleList();
    // print_Quadruples(toPrint->node_syn,((ast_node*)(toPrint->node_syn))->symbolTable,ql,fp);
    // fclose(fp);
    // printf("\n\n----Finished Generating 3 Address Code----\n\n\n\n");

    // printf(" Printinf Quads From Quadruple List\n");
    // print_QuadrupleList(ql);
    // printf("Finished Printing Quads From Quadruple List\n");

    // printf("\n\n----Generating Final Code----\n\n\n\n");
    // fp=fopen("psp.asm","w");
    // codegen(ql,fp);
    // fclose(fp);
    // printf("\n\n----Finished Generating Final Code----\n\n\n\n");

    

}


void parserDriver_uptill_Arrays(const char *testCaseFile, int buffer_size, const char *parseTreeFile)
{

    initializeParser();

    initializeLexer(testCaseFile, buffer_size);

    grammarRule *rules = parseInputSourceCode("grammar.txt");

    if (rules == NULL)
    {
        printf("Empty Grammer File !! \n");
        return;
    }

    constructVariablesMap(rules);

    hash_table *firstSets = create_table();
    hash_table *followSets = create_table();

    computeFirstAndFollowSets(rules, firstSets, followSets);

    int **parseTable = createParseTable(rules, firstSets, followSets);

    hash_table *synchSets = constructSynchSets(followSets);

    // struct StackNode* st = NULL;
    stack *st = (stack *)malloc(sizeof(stack));
    initStack(st);

    stack2 *st2 = (stack2 *)malloc(sizeof(stack2));
    initStack2(st2);

    stack2 *st3 = (stack2 *)malloc(sizeof(stack2));
    initStack2(st3);

    push(st, "$");
    push(st, rules[0].lhs);

    int treeNodeIndex = 0;
    push2(st2, create_node(treeNodeIndex++, "$"));

    t_node *root = create_node(treeNodeIndex++, rules[0].lhs);
    t_node *toPrint = root;
    push2(st2, root);

    char *dol = (char *)malloc(2 * sizeof(char));
    dol[0] = '$';
    dol[1] = '\0';
    lexeme *l = getNextToken();
    char inputToken[100];
    strcpy(inputToken, l->token);
    // printf("Input: --%s-- --%s--\n", inputToken,l->lexeme);
    int latestLineNo = l->line_no;
    int prevLineNo = l->line_no;

    int treeIndex = 0;

    t_node *currentNode = root;

    while (strcmp(peek(st), "$") != 0)
    {
        char *top = peek(st);
        t_node *top2 = peek2(st2);

        if (strcmp(top, inputToken) == 0)
        {
            pop(st);
            pop2(st2);
            currentNode = peek2(st2);

            if (l != NULL)
            {
                top2->line_no = l->line_no;
                top2->lexeme = l->lexeme;
                top2->value_if_num = l->value_if_num;
                top2->value_if_rnum = l->value_if_rnum;
                top2->lex = l; 
            }

            l = getNextToken();

            if (l == NULL)
                strcpy(inputToken, dol);
            else
                strcpy(inputToken, l->token);

            if (l != NULL)
            {
                prevLineNo = latestLineNo;
                latestLineNo = l->line_no;
            }
        }
        else if (!isNonTerminal(top))
        {
            printf("Parser Error - Line No: %d . Terminal on stack top - %s, does not match with next input - %s\n", prevLineNo, top, inputToken);

            // printf("Parser Synchronizing... Assumed %s\n", top);
            pop(st);
            pop2(st2);

            continue;
        }
        else
        {
            int row = *(int *)get(nonTerminalsMap, top);

            int col = *(int *)get(terminalsMap, inputToken);

            int ruleIndex = parseTable[row][col];

            if (ruleIndex == -1)
            {
                printf("Parser Error - Line No: %d . Did not find a Parse Table entry for the pair - %s, %s\n", prevLineNo, top, inputToken);

                hash_table *synchSet = (hash_table *)get(synchSets, top);

                while (get(synchSet, inputToken) == NULL)
                {

                    if (inputToken[0] == '$')
                    {
                        printf("Reached end of input.\n");
                        return;
                    }

                    // printf("Parser Synchronizing... Skipped %s\n", inputToken);
                    l = getNextToken();
                    if (l == NULL)
                        strcpy(inputToken, dol);
                    else
                        strcpy(inputToken, l->token);

                    if (l != NULL)
                    {
                        prevLineNo = latestLineNo;
                        latestLineNo = l->line_no;
                    }
                }

                pop(st);
                pop2(st2);
                continue;
            }

            pop(st);

            t_node *top2 = peek2(st2);
            pop2(st2);

            top2->grammarRuleIndex = ruleIndex;

            for (int i = rules[ruleIndex].rhsSize - 1; i >= 0; i--)
            {
                push(st, rules[ruleIndex].rhs[i]);
            }

            int pushed = 0;

            for (int i = 0; i < rules[ruleIndex].rhsSize; i++)
            {
                t_node *new_node = create_node(currentNode->type, rules[ruleIndex].rhs[i]);
                add_child(currentNode, new_node);
                push2(st3, new_node);
                pushed++;
            }

            while (pushed--)
            {
                t_node *popping = peek2(st3);
                pop2(st3);
                push2(st2, popping);
            }

            currentNode = peek2(st2);
        }
    }

    if (strcmp(inputToken, "$") == 0)
    {
        printf("Accepted\n");
    }
    else
    {
        printf("Parser Error - Line No: %d . Entire input not consumed\n", latestLineNo);
    }

    // printf("Generating Parse Tree\n");
    // generateParseTree(toPrint, NULL);
    // printf("Finished Generating Parse Tree\n");

    //printf("Generating AST\n");
    construct_AST(toPrint,rules);

    // printf("\n\nPrinting AST\n");
    // FILE *fp1 = fopen("ast.txt","w");
    // traverse_AST(toPrint->node_syn,fp1,0,1);
    // fclose(fp1);
    // printf("Finished Printing AST\n"); 

    

    printf("\n\n----Generating Symbol Table----\nIMP: Module Related Errors can appear at the start, but line numbers are accurate.\n\n\n");
    fp = fopen("symbolTable.txt", "w");

    initializeFp(fp);
    initializeSemanticAnalyzer();
    constructSymbolTableDeclarations(toPrint->node_syn, NULL);
    fflush(stdout);
    constructSymbolTable(toPrint->node_syn, NULL);
    fclose(fp);
    printf("\n\n----Finished Generating Symbol Table----\n\n\n\n");

    // printf("\n\n----PRINTING SYMBOL TABLE----\n\n\n\n");
    // printf("VariableName     Scope(ModuleName)     Scope(LineNumbers)     TypeOfElement     IsArray     ArrayType    ArrayRange  Width   Offset    NestingLevel\n");
    // printAllScopeTables(toPrint->node_syn);

    // printf("\n\n----PRINTING Activation Records----\n\n\n\n");
    // printAllActivationRecords(toPrint->node_syn);

    printf("\n\n----PRINTING ARRAYS---\n\n\n\n");
    printAllScopeArrays(toPrint->node_syn);

    // printf("\n\n----Generating 3 Address Code----\n\n\n\n");
    // fp=fopen("3AC.txt","w");
    // quadrupleList* ql = create_QuadrupleList();
    // print_Quadruples(toPrint->node_syn,((ast_node*)(toPrint->node_syn))->symbolTable,ql,fp);
    // fclose(fp);
    // printf("\n\n----Finished Generating 3 Address Code----\n\n\n\n");

    // printf(" Printinf Quads From Quadruple List\n");
    // print_QuadrupleList(ql);
    // printf("Finished Printing Quads From Quadruple List\n");

    // printf("\n\n----Generating Final Code----\n\n\n\n");
    // fp=fopen("psp.asm","w");
    // codegen(ql,fp);
    // fclose(fp);
    // printf("\n\n----Finished Generating Final Code----\n\n\n\n");

    

}


void parserDriver_uptill_CodeGen(const char *testCaseFile, int buffer_size, const char *parseTreeFile)
{

    initializeParser();

    initializeLexer(testCaseFile, buffer_size);

    grammarRule *rules = parseInputSourceCode("grammar.txt");

    if (rules == NULL)
    {
        printf("Empty Grammer File !! \n");
        return;
    }

    constructVariablesMap(rules);

    hash_table *firstSets = create_table();
    hash_table *followSets = create_table();

    computeFirstAndFollowSets(rules, firstSets, followSets);

    int **parseTable = createParseTable(rules, firstSets, followSets);

    hash_table *synchSets = constructSynchSets(followSets);

    // struct StackNode* st = NULL;
    stack *st = (stack *)malloc(sizeof(stack));
    initStack(st);

    stack2 *st2 = (stack2 *)malloc(sizeof(stack2));
    initStack2(st2);

    stack2 *st3 = (stack2 *)malloc(sizeof(stack2));
    initStack2(st3);

    push(st, "$");
    push(st, rules[0].lhs);

    int treeNodeIndex = 0;
    push2(st2, create_node(treeNodeIndex++, "$"));

    t_node *root = create_node(treeNodeIndex++, rules[0].lhs);
    t_node *toPrint = root;
    push2(st2, root);

    char *dol = (char *)malloc(2 * sizeof(char));
    dol[0] = '$';
    dol[1] = '\0';
    lexeme *l = getNextToken();
    char inputToken[100];
    strcpy(inputToken, l->token);
    // printf("Input: --%s-- --%s--\n", inputToken,l->lexeme);
    int latestLineNo = l->line_no;
    int prevLineNo = l->line_no;

    int treeIndex = 0;

    t_node *currentNode = root;

    while (strcmp(peek(st), "$") != 0)
    {
        char *top = peek(st);
        t_node *top2 = peek2(st2);

        if (strcmp(top, inputToken) == 0)
        {
            pop(st);
            pop2(st2);
            currentNode = peek2(st2);

            if (l != NULL)
            {
                top2->line_no = l->line_no;
                top2->lexeme = l->lexeme;
                top2->value_if_num = l->value_if_num;
                top2->value_if_rnum = l->value_if_rnum;
                top2->lex = l; 
            }

            l = getNextToken();

            if (l == NULL)
                strcpy(inputToken, dol);
            else
                strcpy(inputToken, l->token);

            if (l != NULL)
            {
                prevLineNo = latestLineNo;
                latestLineNo = l->line_no;
            }
        }
        else if (!isNonTerminal(top))
        {
            printf("Parser Error - Line No: %d . Terminal on stack top - %s, does not match with next input - %s\n", prevLineNo, top, inputToken);

            // printf("Parser Synchronizing... Assumed %s\n", top);
            pop(st);
            pop2(st2);

            continue;
        }
        else
        {
            int row = *(int *)get(nonTerminalsMap, top);

            int col = *(int *)get(terminalsMap, inputToken);

            int ruleIndex = parseTable[row][col];

            if (ruleIndex == -1)
            {
                printf("Parser Error - Line No: %d . Did not find a Parse Table entry for the pair - %s, %s\n", prevLineNo, top, inputToken);

                hash_table *synchSet = (hash_table *)get(synchSets, top);

                while (get(synchSet, inputToken) == NULL)
                {

                    if (inputToken[0] == '$')
                    {
                        printf("Reached end of input.\n");
                        return;
                    }

                    // printf("Parser Synchronizing... Skipped %s\n", inputToken);
                    l = getNextToken();
                    if (l == NULL)
                        strcpy(inputToken, dol);
                    else
                        strcpy(inputToken, l->token);

                    if (l != NULL)
                    {
                        prevLineNo = latestLineNo;
                        latestLineNo = l->line_no;
                    }
                }

                pop(st);
                pop2(st2);
                continue;
            }

            pop(st);

            t_node *top2 = peek2(st2);
            pop2(st2);

            top2->grammarRuleIndex = ruleIndex;

            for (int i = rules[ruleIndex].rhsSize - 1; i >= 0; i--)
            {
                push(st, rules[ruleIndex].rhs[i]);
            }

            int pushed = 0;

            for (int i = 0; i < rules[ruleIndex].rhsSize; i++)
            {
                t_node *new_node = create_node(currentNode->type, rules[ruleIndex].rhs[i]);
                add_child(currentNode, new_node);
                push2(st3, new_node);
                pushed++;
            }

            while (pushed--)
            {
                t_node *popping = peek2(st3);
                pop2(st3);
                push2(st2, popping);
            }

            currentNode = peek2(st2);
        }
    }

    if (strcmp(inputToken, "$") == 0)
    {
        printf("Accepted\n");
    }
    else
    {
        printf("Parser Error - Line No: %d . Entire input not consumed\n", latestLineNo);
    }

    // printf("Generating Parse Tree\n");
    // generateParseTree(toPrint, NULL);
    // printf("Finished Generating Parse Tree\n");

    //printf("Generating AST\n");
    construct_AST(toPrint,rules);

    // printf("\n\nPrinting AST\n");
    // FILE *fp1 = fopen("ast.txt","w");
    // traverse_AST(toPrint->node_syn,fp1,0,1);
    // fclose(fp1);
    // printf("Finished Printing AST\n"); 

    

    printf("\n\n----Generating Symbol Table----\nIMP: Module Related Errors can appear at the start, but line numbers are accurate.\n\n\n");
    fp = fopen("symbolTable.txt", "w");

    initializeFp(fp);
    initializeSemanticAnalyzer();
    constructSymbolTableDeclarations(toPrint->node_syn, NULL);
    fflush(stdout);
    constructSymbolTable(toPrint->node_syn, NULL);
    fclose(fp);
    printf("\n\n----Finished Generating Symbol Table----\n\n\n\n");

    // printf("\n\n----PRINTING SYMBOL TABLE----\n\n\n\n");
    // printf("VariableName     Scope(ModuleName)     Scope(LineNumbers)     TypeOfElement     IsArray     ArrayType    ArrayRange  Width   Offset    NestingLevel\n");
    // printAllScopeTables(toPrint->node_syn);

    // printf("\n\n----PRINTING Activation Records----\n\n\n\n");
    // printAllActivationRecords(toPrint->node_syn);

    // printf("\n\n----PRINTING ARRAYS---\n\n\n\n");
    // printAllScopeArrays(toPrint->node_syn);

    if(buffer_size == 20)
    {
        return;
    }

    printf("\n\n----Generating 3 Address Code----\n\n\n\n");
    fp=fopen("3AC.txt","w");
    initializeIntermediateCodeGen();
    quadrupleList* ql = create_QuadrupleList();
    print_Quadruples(toPrint->node_syn,((ast_node*)(toPrint->node_syn))->symbolTable,ql,fp);
    fclose(fp);
    printf("\n\n----Finished Generating 3 Address Code----\n\n\n\n");

    // printf(" Printinf Quads From Quadruple List\n");
    // print_QuadrupleList(ql);
    // printf("Finished Printing Quads From Quadruple List\n");

    printf("\n\n----Generating Final Code----\n\n\n\n");
    fp=fopen(parseTreeFile,"w");
    codegen(ql,fp);
    fclose(fp);
    printf("\n\n----Finished Generating Final Code----\n\n\n\n");

    

}



void parserDriver_Compression(const char *testCaseFile, int buffer_size, const char *parseTreeFile)
{

    initializeParser();

    initializeLexer(testCaseFile, buffer_size);

    grammarRule *rules = parseInputSourceCode("grammar.txt");

    if (rules == NULL)
    {
        printf("Empty Grammer File !! \n");
        return;
    }

    constructVariablesMap(rules);

    hash_table *firstSets = create_table();
    hash_table *followSets = create_table();

    computeFirstAndFollowSets(rules, firstSets, followSets);

    int **parseTable = createParseTable(rules, firstSets, followSets);

    hash_table *synchSets = constructSynchSets(followSets);

    // struct StackNode* st = NULL;
    stack *st = (stack *)malloc(sizeof(stack));
    initStack(st);

    stack2 *st2 = (stack2 *)malloc(sizeof(stack2));
    initStack2(st2);

    stack2 *st3 = (stack2 *)malloc(sizeof(stack2));
    initStack2(st3);

    push(st, "$");
    push(st, rules[0].lhs);

    int treeNodeIndex = 0;
    push2(st2, create_node(treeNodeIndex++, "$"));

    t_node *root = create_node(treeNodeIndex++, rules[0].lhs);
    t_node *toPrint = root;
    push2(st2, root);

    char *dol = (char *)malloc(2 * sizeof(char));
    dol[0] = '$';
    dol[1] = '\0';
    lexeme *l = getNextToken();
    char inputToken[100];
    strcpy(inputToken, l->token);
    // printf("Input: --%s-- --%s--\n", inputToken,l->lexeme);
    int latestLineNo = l->line_no;
    int prevLineNo = l->line_no;

    int treeIndex = 0;

    t_node *currentNode = root;

    while (strcmp(peek(st), "$") != 0)
    {
        char *top = peek(st);
        t_node *top2 = peek2(st2);

        if (strcmp(top, inputToken) == 0)
        {
            pop(st);
            pop2(st2);
            currentNode = peek2(st2);

            if (l != NULL)
            {
                top2->line_no = l->line_no;
                top2->lexeme = l->lexeme;
                top2->value_if_num = l->value_if_num;
                top2->value_if_rnum = l->value_if_rnum;
                top2->lex = l; 
            }

            l = getNextToken();

            if (l == NULL)
                strcpy(inputToken, dol);
            else
                strcpy(inputToken, l->token);

            if (l != NULL)
            {
                prevLineNo = latestLineNo;
                latestLineNo = l->line_no;
            }
        }
        else if (!isNonTerminal(top))
        {
            printf("Parser Error - Line No: %d . Terminal on stack top - %s, does not match with next input - %s\n", prevLineNo, top, inputToken);

            // printf("Parser Synchronizing... Assumed %s\n", top);
            pop(st);
            pop2(st2);

            continue;
        }
        else
        {
            int row = *(int *)get(nonTerminalsMap, top);

            int col = *(int *)get(terminalsMap, inputToken);

            int ruleIndex = parseTable[row][col];

            if (ruleIndex == -1)
            {
                printf("Parser Error - Line No: %d . Did not find a Parse Table entry for the pair - %s, %s\n", prevLineNo, top, inputToken);

                hash_table *synchSet = (hash_table *)get(synchSets, top);

                while (get(synchSet, inputToken) == NULL)
                {

                    if (inputToken[0] == '$')
                    {
                        printf("Reached end of input.\n");
                        return;
                    }

                    // printf("Parser Synchronizing... Skipped %s\n", inputToken);
                    l = getNextToken();
                    if (l == NULL)
                        strcpy(inputToken, dol);
                    else
                        strcpy(inputToken, l->token);

                    if (l != NULL)
                    {
                        prevLineNo = latestLineNo;
                        latestLineNo = l->line_no;
                    }
                }

                pop(st);
                pop2(st2);
                continue;
            }

            pop(st);

            t_node *top2 = peek2(st2);
            pop2(st2);

            top2->grammarRuleIndex = ruleIndex;

            for (int i = rules[ruleIndex].rhsSize - 1; i >= 0; i--)
            {
                push(st, rules[ruleIndex].rhs[i]);
            }

            int pushed = 0;

            for (int i = 0; i < rules[ruleIndex].rhsSize; i++)
            {
                t_node *new_node = create_node(currentNode->type, rules[ruleIndex].rhs[i]);
                add_child(currentNode, new_node);
                push2(st3, new_node);
                pushed++;
            }

            while (pushed--)
            {
                t_node *popping = peek2(st3);
                pop2(st3);
                push2(st2, popping);
            }

            currentNode = peek2(st2);
        }
    }

    if (strcmp(inputToken, "$") == 0)
    {
        printf("Accepted\n");
    }
    else
    {
        printf("Parser Error - Line No: %d . Entire input not consumed\n", latestLineNo);
    }

    printf("Generating Parse Tree\n");
    
    initCountNodes();
    generateParseTree(toPrint, NULL);
    printf("Finished Generating Parse Tree\n");

    //initializeAST();
    printf("Generating AST\n");
    construct_AST(toPrint,rules);
    int ast_nodes = getASTNodeCount();

    int parseTreeNodesCount = 2.0f*ast_nodes;

    float parseTreeSize = parseTreeNodesCount*sizeof(t_node);
    float astSize = ast_nodes*sizeof(ast_node);

    printf("Parse Tree Number of Nodes: %d\n",parseTreeNodesCount);
    printf("Parse Tree memory allocated: %f\n",parseTreeSize);

    printf("AST Number of Nodes: %d\n",ast_nodes);
    printf("AST memory allocated: %f\n",astSize);

    float compression = ((parseTreeSize-astSize)/parseTreeSize)*100;
    printf("Compression Percentage: %f\n",compression);
    // printf("\n\nPrinting AST\n");
    // FILE *fp1 = fopen("ast.txt","w");
    // traverse_AST(toPrint->node_syn,fp1,0,1);
    // fclose(fp1);
    // printf("Finished Printing AST\n"); 

    

    // printf("\n\n----Generating Symbol Table----\nIMP: Module Related Errors can appear at the start, but line numbers are accurate.\n\n\n");
    // fp = fopen("symbolTable.txt", "w");

    // initializeFp(fp);
    // initializeSemanticAnalyzer();
    // constructSymbolTableDeclarations(toPrint->node_syn, NULL);
    // fflush(stdout);
    // constructSymbolTable(toPrint->node_syn, NULL);
    // fclose(fp);
    // printf("\n\n----Finished Generating Symbol Table----\n\n\n\n");

    // printf("\n\n----PRINTING SYMBOL TABLE----\n\n\n\n");
    // printf("VariableName     Scope(ModuleName)     Scope(LineNumbers)     TypeOfElement     IsArray     ArrayType    ArrayRange  Width   Offset    NestingLevel\n");
    // printAllScopeTables(toPrint->node_syn);

    // printf("\n\n----PRINTING Activation Records----\n\n\n\n");
    // printAllActivationRecords(toPrint->node_syn);

    // printf("\n\n----PRINTING ARRAYS---\n\n\n\n");
    // printAllScopeArrays(toPrint->node_syn);

    // printf("\n\n----Generating 3 Address Code----\n\n\n\n");
    // fp=fopen("3AC.txt","w");
    // quadrupleList* ql = create_QuadrupleList();
    // print_Quadruples(toPrint->node_syn,((ast_node*)(toPrint->node_syn))->symbolTable,ql,fp);
    // fclose(fp);
    // printf("\n\n----Finished Generating 3 Address Code----\n\n\n\n");

    // printf(" Printinf Quads From Quadruple List\n");
    // print_QuadrupleList(ql);
    // printf("Finished Printing Quads From Quadruple List\n");

    // printf("\n\n----Generating Final Code----\n\n\n\n");
    // fp=fopen("psp.asm","w");
    // codegen(ql,fp);
    // fclose(fp);
    // printf("\n\n----Finished Generating Final Code----\n\n\n\n");

    

}


