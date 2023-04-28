// CS F363 : Compiler Construction
// Group No. 24
// 1. Nachiket Kotalwar               (2020A7PS0024P)
// 2.Parth Patel                      (2020A7PS0026P)
// 3.Partha Sarathi Purkayastha       (2020A7PS0043P)
// 4.Labeeb Ahsan                     (2020A7PS0045P)
// 5.Anish Atul Kulkarni              (2020A7PS0975P)

#include "parserDef.h"
#include "symbolTable.h"

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
// stack end

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
    FILE *fp = fopen(parseTreeFile, "w");
    // printf("Tree be like:\n ");
    // printf("START => \n\n");

    // inorder_traversal(root, fp);
    preorder_traversal(root, fp, 0, 1);
    // printf("\nEND\n");

    fclose(fp);

    printf("Parse Tree generated successfully !! \n");
    return 0;
}

// t_node *addNodeToParseTree(t_node *root, grammarRule *rules, int ruleNumber)
// {
//     t_node *current_node = apply_rule(root, rules[ruleNumber].lhs, rules[ruleNumber].rhs, rules[ruleNumber].rhsSize);
//     return current_node;
// }

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

typedef struct linked_list
{
    void *data;
    struct linked_list *next;
} llist;

llist *create_list()
{
    llist *list = (llist *)malloc(sizeof(llist));
    list->data = NULL;
    list->next = NULL;
    return list;
}

llist *insert_at_end(llist *list, void *data)
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

ast_node *create_node_AST_from_list(char *type, llist *list1)
{
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

ast_node *create_node_AST(char *type, int containsLexeme, lexeme *lexeme, int numChildren, void *child1, void *child2, void *child3, void *child4, void *child5, void *child6, void *child7, void *child8, void *child9)
{
    ast_node *node = (ast_node *)malloc(sizeof(ast_node));
    node->type = type;
    node->containsLexeme = containsLexeme;
    node->containsBounds = 0;
    if (containsLexeme)
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
        node->children[i - 1] = child;
    }
    return node;
}

void traverse_AST_print(ast_node *node, int nested, int childNo)
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
        printf("\t");
    }
    if (node->containsBounds)
        printf("%d. %s \t\t\t-child : %d- \t\t\t\t\t\t\t-start:%d- -end:%d-\n", childNo, node->type, node->numChildren, node->startLine, node->endLine);
    else
        printf("%d. %s \t\t\t-child : %d-\n", childNo, node->type, node->numChildren);

    for (int i = 0; i < node->numChildren; i++)
    {
        traverse_AST_print(node->children[i], nested + 1, i + 1);
    }
}

void traverse_AST(ast_node *node, FILE *fp, int nested, int childNo)
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

    printf("Here\n");
    if (node == NULL)
        return;

    for (int i = 0; i < nested; i++)
    {
        printf( "\t");
    }
    if (node->containsBounds)
        printf( "%d. %s \t\t\t-child : %d- \t\t\t\t\t\t\t-start:%d- -end:%d-\n", childNo, node->type, node->numChildren, node->startLine, node->endLine);
    else
        printf( "%d. %s \t\t\t-child : %d-\n", childNo, node->type, node->numChildren);

    for (int i = 0; i < node->numChildren; i++)
    {
        traverse_AST(node->children[i], fp, nested + 1, i + 1);
    }
}

void printLinkedList(llist *list)
{
    llist *temp = list;
    int i = 0;

    while (temp != NULL)
    {
        i++;
        printf("%s ", (char *)temp->data);
        temp = temp->next;
    }
    printf("I=%d\n", i);
}

ast_node *construct_AST(t_node *root, grammarRule *rules)
{
    ast_node *node;
    grammarRule rule = rules[root->grammarRuleIndex];

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

        // printf("Here5");
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

        Moduledeclarations->node_syn = create_node_AST_from_list("MODULE_LIST", Moduledeclarations->list_syn);
        Othermodules1->node_syn = create_node_AST_from_list("OTHER_LIST1", Othermodules1->list_syn);
        Othermodules2->node_syn = create_node_AST_from_list("OTHER_LIST2", Othermodules2->list_syn);

        Program->node_syn = create_node_AST("PROGRAM", 0, NULL, 4, Moduledeclarations->node_syn, Othermodules1->node_syn, Drivermodule->node_syn, Othermodules2->node_syn, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 1: // Moduledeclarations Moduledeclaration Moduledeclarations1 .
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

        Moduledeclaration->node_syn = create_node_AST(id->lexeme, 1, id->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        ((ast_node *)Moduledeclaration->node_syn)->containsBounds = 1;
        ((ast_node *)Moduledeclaration->node_syn)->startLine = id->line_no;
        ((ast_node *)Moduledeclaration->node_syn)->endLine = id->line_no;
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
        // printf("DRIVER\n");
        fflush(stdout);
        t_node *Drivermodule = root;
        t_node *driver = root->children[1];
        t_node *Moduledef = root->children[4];
        // printf("here");
        construct_AST(Moduledef, rules);

        Drivermodule->node_syn = create_node_AST("DRIVER", 0, NULL, 1, Moduledef->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        ((ast_node *)(Drivermodule->node_syn))->containsBounds = 1;
        ((ast_node *)(Drivermodule->node_syn))->startLine = driver->line_no;
        ((ast_node *)(Drivermodule->node_syn))->endLine = ((ast_node *)(Moduledef->node_syn))->endLine;
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

        Input_plist->node_syn = create_node_AST_from_list("INPUT_LIST", Input_plist->list_syn);
        Module->node_syn = create_node_AST("MODULE", 0, NULL, 4, create_node_AST(id->lexeme, 1, id->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL), Input_plist->node_syn, Ret->node_syn, Moduledef->node_syn, NULL, NULL, NULL, NULL, NULL);
        ((ast_node *)(Module->node_syn))->containsBounds = 1;
        ((ast_node *)(Module->node_syn))->startLine = module->line_no;
        ((ast_node *)(Module->node_syn))->endLine = ((ast_node *)(Moduledef->node_syn))->endLine;
    }
    break;
    case 8: // Ret returns sqbo Output_plist sqbc semicol
    {
        t_node *Ret = root;
        t_node *Output_plist = root->children[2];

        Output_plist->list_inh = create_list();

        construct_AST(Output_plist, rules);

        Output_plist->node_syn = create_node_AST_from_list("OUTPUT_LIST", Output_plist->list_syn);
        // here no need to create new node
        // Ret->node_syn = create_node_AST("PARAMETERS_LIST", 1, Output_plist->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        Ret->node_syn = Output_plist->node_syn;
    }
    break;
    case 9: // Ret
    {
        t_node *Ret = root;

        Ret->node_syn = create_node_AST("OUTPUT_LIST", 0, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 10: // Input_plist id colon Datatype A' .
    {
        t_node *Input_plist = root;
        t_node *id = root->children[0];
        t_node *Datatype = root->children[2];
        t_node *A = root->children[3];

        construct_AST(Datatype, rules);

        A->list_inh = create_list();
        ast_node *data_id = create_node_AST("DATATYPE_ID", 0, NULL, 2, create_node_AST(id->lexeme, 1, id->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL), Datatype->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        A->list_inh = insert_at_end(A->list_inh, data_id);

        construct_AST(A, rules);

        Input_plist->list_syn = A->list_syn;
    }
    break;
    case 11: // A' comma id colon Datatype A'1
    {
        t_node *A = root;
        t_node *id = root->children[1];
        t_node *Datatype = root->children[3];
        t_node *A1 = root->children[4];

        construct_AST(Datatype, rules);

        ast_node *data_id = create_node_AST("DATATYPE_ID", 0, NULL, 2, create_node_AST(id->lexeme, 1, id->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL), Datatype->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        A1->list_inh = insert_at_end(A->list_inh, data_id);

        construct_AST(A1, rules);

        A->list_syn = A1->list_syn;
    }
    break;
    case 12: // A'
    {
        t_node *A = root;

        A->list_syn = A->list_inh;
    }
    break;
    case 13: // Output_plist id colon Datatype B'
    {
        t_node *Output_plist = root;
        t_node *id = root->children[0];
        t_node *Datatype = root->children[2];
        t_node *B = root->children[3];

        construct_AST(Datatype, rules);

        B->list_inh = create_list();
        ast_node *data_id = create_node_AST("DATATYPE_ID", 0, NULL, 2, create_node_AST(id->lexeme, 1, id->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL), Datatype->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        B->list_inh = insert_at_end(B->list_inh, data_id);

        construct_AST(B, rules);

        Output_plist->list_syn = B->list_syn;
    }
    break;
    case 14: // B' comma id colon Datatype B'1
    {
        t_node *B = root;
        t_node *id = root->children[1];
        t_node *Datatype = root->children[3];
        t_node *B1 = root->children[4];

        construct_AST(Datatype, rules);

        ast_node *data_id = create_node_AST("DATATYPE_ID", 0, NULL, 2, create_node_AST(id->lexeme, 1, id->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL), Datatype->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        B1->list_inh = insert_at_end(B->list_inh, data_id);

        construct_AST(B1, rules);

        B->list_syn = B1->list_syn;
    }
    break;
    case 15: // B'
    {
        t_node *B = root;

        B->list_syn = B->list_inh;
    }
    break;
    case 16: // Datatype integer
    {
        t_node *Datatype = root;
        t_node *integer = root->children[0];

        Datatype->node_syn = create_node_AST(integer->lexeme, 1, integer->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 17: // Datatype real
    {
        t_node *Datatype = root;
        t_node *real = root->children[0];

        Datatype->node_syn = create_node_AST(real->lexeme, 1, real->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 18: // Datatype boolean
    {
        t_node *Datatype = root;
        t_node *boolean = root->children[0];

        Datatype->node_syn = create_node_AST(boolean->lexeme, 1, boolean->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 19: // Datatype array sqbo Rangenew sqbc of Type
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

        Datatype->node_syn = create_node_AST("RANGE_DATATYPE", 0, NULL, 2, Rangenew->node_syn, Type->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 20: // Rangenew Indarray1 rangeop Indarray2 .
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

        Rangenew->node_syn = create_node_AST("ARRAY_RANGE", 0, NULL, 2, Indarray1->node_syn, Indarray2->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 21: // Indarray Sign Indexcoef .
    {
        t_node *Indarray = root;
        t_node *Sign = root->children[0];
        t_node *Indexcoef = root->children[1];

        // printf("In Indarray Sign Indexcoef\n");
        fflush(stdout);

        construct_AST(Sign, rules);
        construct_AST(Indexcoef, rules);

        Indarray->node_syn = create_node_AST("BOUND", 0, NULL, 2, Sign->node_syn, Indexcoef->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 22: // Indarray Indexcoef .
    {
        t_node *Indarray = root;
        t_node *Indexcoef = root->children[0];

        construct_AST(Indexcoef, rules);

        Indarray->node_syn = Indexcoef->node_syn;
    }
    break;
    case 23: // Indexcoef num .   //NUM LEXEME
    {
        t_node *Indexcoef = root;
        t_node *num = root->children[0];

        Indexcoef->node_syn = create_node_AST(num->lexeme, 1, num->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 24: // Indexcoef id .
    {
        t_node *Indexcoef = root;
        t_node *id = root->children[0];

        Indexcoef->node_syn = create_node_AST(id->lexeme, 1, id->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 25: // Sign plus .
    {
        t_node *Sign = root;
        t_node *plus = root->children[0];

        Sign->node_syn = create_node_AST(plus->lexeme, 1, plus->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 26: // Sign minus .
    {
        t_node *Sign = root;
        t_node *minus = root->children[0];

        Sign->node_syn = create_node_AST(minus->lexeme, 1, minus->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 27: // Type integer .
    {
        t_node *Type = root;
        t_node *integer = root->children[0];

        Type->node_syn = create_node_AST(integer->lexeme, 1, integer->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 28: // Type real .
    {
        t_node *Type = root;
        t_node *real = root->children[0];

        Type->node_syn = create_node_AST(real->lexeme, 1, real->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 29: // Type boolean .
    {
        t_node *Type = root;
        t_node *boolean = root->children[0];

        Type->node_syn = create_node_AST(boolean->lexeme, 1, boolean->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 30: // Moduledef start Statements end
    {
        // printf("\n");
        // printf("Moduledef start Statements end\n");
        fflush(stdout);
        t_node *Moduledef = root;
        t_node *start = root->children[0];
        t_node *Statements = root->children[1];
        t_node *end = root->children[2];

        Statements->list_inh = create_list();

        // printf("Statements\n");
        construct_AST(Statements, rules);
        // printf("Statements2\n");
        // printLinkedList(Statements->list_syn);

        Statements->node_syn = create_node_AST_from_list("STATEMENT_LIST", Statements->list_syn);
        // traverse_AST(Statements->node_syn);

        Moduledef->node_syn = Statements->node_syn;

        ((ast_node *)(Moduledef->node_syn))->startLine = start->line_no;
        ((ast_node *)(Moduledef->node_syn))->endLine = end->line_no;
        ((ast_node *)(Moduledef->node_syn))->containsBounds = 1;
    }
    break;
    case 31: // Statements Statement Statements
    {
        // printf("Statements Statement Statements\n");
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
    case 32: // Statements
    {
        // printf("Statements\n");
        fflush(stdout);
        t_node *Statements = root;

        Statements->list_syn = Statements->list_inh;
    }
    break;
    case 33: // Statement ioStmt
    {
        t_node *Statement = root;
        t_node *ioStmt = root->children[0];

        construct_AST(ioStmt, rules);

        Statement->node_syn = ioStmt->node_syn;
    }
    break;
    case 34: // Statement Simplestmt
    {
        // printf("Statement Simplestmt\n");
        fflush(stdout);
        t_node *Statement = root;
        t_node *Simplestmt = root->children[0];

        construct_AST(Simplestmt, rules);

        Statement->node_syn = Simplestmt->node_syn;
    }
    break;
    case 35: // Statement declareStmt
    {
        t_node *Statement = root;
        t_node *declareStmt = root->children[0];

        construct_AST(declareStmt, rules);

        Statement->node_syn = declareStmt->node_syn;
    }
    break;
    case 36: // Statement conditionalStmt
    {
        t_node *Statement = root;
        t_node *conditionalStmt = root->children[0];

        construct_AST(conditionalStmt, rules);

        Statement->node_syn = conditionalStmt->node_syn;
    }
    break;
    case 37: // Statement iterativeStmt
    {
        t_node *Statement = root;
        t_node *iterativeStmt = root->children[0];

        construct_AST(iterativeStmt, rules);

        Statement->node_syn = iterativeStmt->node_syn;
    }
    break;
    case 38: // Iostmt get_value bo id bc semicol
    {
        t_node *ioStmt = root;
        t_node *id = root->children[2];

        ioStmt->node_syn = create_node_AST("GET_VALUE", 0, NULL, 1, create_node_AST(id->lexeme, 1, id->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 39: // Iostmt print bo Printvar bc semicol
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

        ioStmt->node_syn = create_node_AST("PRINT", 0, NULL, 1, Printvar->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 40: // Varnew Printvar .
    {
        t_node *Varnew = root;
        t_node *Printvar = root->children[0];

        construct_AST(Printvar, rules);
        Varnew->node_syn = create_node_AST("PARAMETER_VARIABLE", 0, NULL, 1, Printvar->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 41: // Varnew Sign Printvar .
    {
        t_node *Varnew = root;
        t_node *Sign = root->children[0];
        t_node *Printvar = root->children[1];

        construct_AST(Sign, rules);
        construct_AST(Printvar, rules);
        Varnew->node_syn = create_node_AST("PARAMETER_VARIABLE", 0, NULL, 2, Sign->node_syn, Printvar->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
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

        Printvar->node_syn = create_node_AST("NUM", 0, NULL, 1, create_node_AST(num->lexeme, 1, num->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 44: // Printvar rnum .
    {
        t_node *Printvar = root;
        t_node *rnum = root->children[0];

        Printvar->node_syn = create_node_AST("RNUM", 0, NULL, 1, create_node_AST(rnum->lexeme, 1, rnum->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 45: // Printvar id J .
    {
        t_node *Printvar = root;
        t_node *id = root->children[0];
        t_node *J = root->children[1];

        // printf("PRINTVAR: %s\n", Printvar->lexeme);
        // printf("ID: %s\n", id->lexeme);
        // printf("J: %s\n", J->lexeme);
        // printf("Printing J children\n");
        for (int i = 0; i < J->num_children; i++)
        {
            // printf("J child %d: %s\n", i, J->children[i]->lexeme);
        }
        construct_AST(J, rules);

        // printf("CHECKINF INDARRAY\n");
        fflush(stdout);

        if (J->node_syn == NULL)
        {
            // printf("NULL\n");
            fflush(stdout);
        }
        else
        {
            // printf("NOT NULL\n");
            fflush(stdout);
        }

        Printvar->node_syn = create_node_AST("ARRAY_ELEM_OR_ID", 0, NULL, 2, create_node_AST(id->lexeme, 1, id->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL), J->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 46: // J' sqbo Indarray sqbc .
    {
        t_node *J = root;
        t_node *Indarray = root->children[1];

        construct_AST(Indarray, rules);

        // printf("CHECKING INDARRAY2222\n");
        if (Indarray->node_syn == NULL)
        {
            // printf("NULL\n");
            fflush(stdout);
        }
        else
        {
            // printf("NOT NULL\n");
            fflush(stdout);
        }

        J->node_syn = Indarray->node_syn;
    }
    break;
    case 47: // J' .
    {
        t_node *J = root;

        J->node_syn = NULL;
    }
    break;
    case 48: // Simplestmt Assignmentstmt .
    {
        t_node *simpleStmt = root;
        t_node *Assignmentstmt = root->children[0];

        construct_AST(Assignmentstmt, rules);

        simpleStmt->node_syn = Assignmentstmt->node_syn;
    }
    break;
    case 49: // Simplestmt Modulereusestmt .
    {
        // printf("Simplestmt Modulereusestmt\n");
        fflush(stdout);
        t_node *simpleStmt = root;
        t_node *Modulereusestmt = root->children[0];

        construct_AST(Modulereusestmt, rules);

        simpleStmt->node_syn = Modulereusestmt->node_syn;
    }
    break;
    case 50: // Assignmentstmt id Whichstmt
    {
        t_node *Assignmentstmt = root;
        t_node *id = root->children[0];
        t_node *Whichstmt = root->children[1];

        Whichstmt->node_inh = create_node_AST(id->lexeme, 1, id->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        construct_AST(Whichstmt, rules);

        Assignmentstmt->node_syn = Whichstmt->node_syn;
    }
    break;
    case 51: // Whichstmt Lvalueidstmt
    {
        t_node *Whichstmt = root;
        t_node *Lvalueidstmt = root->children[0];

        Lvalueidstmt->node_inh = Whichstmt->node_inh;

        construct_AST(Lvalueidstmt, rules);

        Whichstmt->node_syn = Lvalueidstmt->node_syn;
    }
    break;
    case 52: // Whichstmt Lvaluearrstmt
    {
        t_node *Whichstmt = root;
        t_node *Lvaluearrstmt = root->children[0];

        Lvaluearrstmt->node_inh = Whichstmt->node_inh;

        construct_AST(Lvaluearrstmt, rules);

        Whichstmt->node_syn = Lvaluearrstmt->node_syn;
    }
    break;
    case 53: // Lvalueidstmt assignop Expression semicol
    {
        t_node *Lvalueidstmt = root;
        t_node *Expression = root->children[1];

        ////printf("Expression -%s-",Expression->lexeme);
        // printf("here\n\n");
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

        Lvalueidstmt->node_syn = create_node_AST("ASSIGN", 0, NULL, 2, Lvalueidstmt->node_inh, Expression->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 54: // Lvaluearrstmt sqbo Elemindex sqbc assignop Expression semicol
    {
        t_node *Lvaluearrstmt = root;
        t_node *Elemindex = root->children[1];
        t_node *Expression = root->children[4];

        construct_AST(Elemindex, rules);
        construct_AST(Expression, rules);

        ast_node *array_elem = create_node_AST("ARRAY_ELEM", 0, NULL, 2, Lvaluearrstmt->node_inh, Elemindex->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        Lvaluearrstmt->node_syn = create_node_AST("ASSIGN", 0, NULL, 2, array_elem, Expression->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 55: // Elemindex Sign G'
    {
        t_node *Elemindex = root;
        t_node *Sign = root->children[0];
        t_node *G = root->children[1];

        construct_AST(Sign, rules);
        construct_AST(G, rules);

        Elemindex->node_syn = create_node_AST("ARRAY_INDEX", 0, NULL, 2, Sign->node_syn, G->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 56: // Elemindex Aexpr
    {
        t_node *Elemindex = root;
        t_node *Aexpr = root->children[0];

        construct_AST(Aexpr, rules);

        Elemindex->node_syn = Aexpr->node_syn;
    }
    break;
    case 57: // G' Indexcoef
    {
        t_node *G = root;
        t_node *Indexcoef = root->children[0];

        construct_AST(Indexcoef, rules);

        G->node_syn = Indexcoef->node_syn;
    }
    break;
    case 58: // G' bo Aexpr bc
    {
        t_node *G = root;
        t_node *Aexpr = root->children[1];

        construct_AST(Aexpr, rules);

        G->node_syn = Aexpr->node_syn;
    }
    break;
    case 59: // Modulereusestmt Optional use module id with parameters Actparalist semicol
    {
        // printf("Modulereusestmt Optional use module id with parameters Actparalist semicol\n");
        fflush(stdout);
        t_node *Modulereusestmt = root;
        t_node *Optional = root->children[0];
        t_node *id = root->children[3];
        t_node *Actparalist = root->children[6];

        construct_AST(Optional, rules);

        // printf("Here1\n");
        fflush(stdout);

        construct_AST(Actparalist, rules);

        // printf("Here\n");
        fflush(stdout);

        Actparalist->node_syn = create_node_AST_from_list("ACCPARA_LIST", Actparalist->list_syn);

        Modulereusestmt->node_syn = create_node_AST("REUSE_STMT", 0, NULL, 3, Optional->node_syn, create_node_AST(id->lexeme, 1, id->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL), Actparalist->node_syn, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 60: // Optional sqbo Idlist sqbc assignop .
    {
        // printf("Optional sqbo Idlist sqbc assignop\n");
        fflush(stdout);
        t_node *Optional = root;
        t_node *Idlist = root->children[1];

        // Idlist->list_inh = create_list();

        construct_AST(Idlist, rules);

        // printf("Here\n");

        // Idlist->node_syn = create_node_AST_from_list("ID_LIST", Idlist->list_syn);

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
        C->list_inh = insert_at_end(C->list_inh, create_node_AST(id->lexeme, 1, id->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL));

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

        C->list_inh = insert_at_end(C->list_inh, create_node_AST(id->lexeme, 1, id->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL));
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

        // N11->node_syn = create_node_AST_from_list("N11_LIST", N11->list_syn);

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

        Unaryop->node_syn = create_node_AST("UNARY_OP", 0, NULL, 2, Unaryop->node_inh, Arithmeticexpr->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 72: // Unaryop Var_idnum .
    {
        t_node *Unaryop = root;
        t_node *Var_idnum = root->children[0];

        construct_AST(Var_idnum, rules);

        Unaryop->node_syn = create_node_AST("UNARY_OP", 0, NULL, 2, Unaryop->node_inh, Var_idnum->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 73: // Var_idnum num .
    {
        t_node *Var_idnum = root;
        t_node *num = root->children[0];

        Var_idnum->node_syn = create_node_AST(num->lexeme, 1, num->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 74: // Var_idnum rnum .
    {
        t_node *Var_idnum = root;
        t_node *rnum = root->children[0];

        Var_idnum->node_syn = create_node_AST(rnum->lexeme, 1, rnum->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 75: // Var_idnum id .
    {
        t_node *Var_idnum = root;
        t_node *id = root->children[0];

        Var_idnum->node_syn = create_node_AST(id->lexeme, 1, id->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 76: // Op3 plus .
    {
        t_node *Op3 = root;
        t_node *plus = root->children[0];

        Op3->node_syn = create_node_AST(plus->lexeme, 1, plus->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 77: // Op3 minus .
    {
        t_node *Op3 = root;
        t_node *minus = root->children[0];

        Op3->node_syn = create_node_AST(minus->lexeme, 1, minus->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
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

        H->node_inh = create_node_AST("LOGICAL_OP", 0, NULL, 3, H->node_inh, Logicalop->node_syn, Genterm->node_syn, NULL, NULL, NULL, NULL, NULL, NULL);
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
        /// printf("Genterm Arithmeticexpr I'\n");
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

        I->node_syn = create_node_AST("RELATIONAL_OP", 0, NULL, 3, I->node_inh, Relationalop->node_syn, Arithmeticexpr->node_syn, NULL, NULL, NULL, NULL, NULL, NULL);
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

        // printf("Term = %s\n", ((ast_node*)(Term->node_syn))->type);

        D->node_inh = Term->node_syn;

        construct_AST(D, rules);

        // printf("D = %s\n", ((ast_node*)(D->node_syn))->type);
        // traverse_AST_print(D->node_syn,0,1);

        Arithmeticexpr->node_syn = D->node_syn;
    }
    break;
    case 85: // D' Op1 Term D'1 .
    {
        // rintf("D' Op1 Term D'1 .\n");
        fflush(stdout);
        t_node *D = root;
        t_node *Op1 = root->children[0];
        t_node *Term = root->children[1];
        t_node *D1 = root->children[2];

        construct_AST(Op1, rules);
        construct_AST(Term, rules);

        // printf("Op1 = %s\n", ((ast_node*)(Op1->node_syn))->type);
        // printf("Term = %s\n", ((ast_node*)(Term->node_syn))->type);

        D1->node_inh = create_node_AST("OPERATION_1", 0, NULL, 3, D->node_inh, Op1->node_syn, Term->node_syn, NULL, NULL, NULL, NULL, NULL, NULL);
        // D1->node_inh = D->node_inh;

        construct_AST(D1, rules);

        D->node_syn = D1->node_syn;

        // printf("Printing Inherited D->inh\n");
        // traverse_AST_print(D->node_inh,0,1);
        // printf("Printing Synthesized D->syn\n");
        // traverse_AST_print(D->node_syn,0,1);

        // traverse_AST_print(D->node_syn,0,1);
    }
    break;
    case 86: // D' .
    {
        // printf("D''' .\n");
        t_node *D = root;

        // printf("\t\tD''' D->inh\n");
        // traverse_AST_print(D->node_inh,2,1);
        // printf("\t\t\t\tPrinting Synthesized same as inh\t\t\n");

        D->node_syn = D->node_inh;
    }
    break;
    case 87: // Op1 plus .
    {
        t_node *Op1 = root;
        t_node *plus = root->children[0];

        Op1->node_syn = create_node_AST(plus->lexeme, 1, plus->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 88: // Op1 minus .
    {
        t_node *Op1 = root;
        t_node *minus = root->children[0];

        Op1->node_syn = create_node_AST(minus->lexeme, 1, minus->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
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
        // printf("E' Op2 Factor E'1\n");
        t_node *E = root;
        t_node *Op2 = root->children[0];
        t_node *Factor = root->children[1];
        t_node *E1 = root->children[2];

        construct_AST(Op2, rules);
        construct_AST(Factor, rules);

        // printf("Op2 = %s\n", ((ast_node*)(Op2->node_syn))->type);
        // printf("Factor = %s\n", ((ast_node*)(Factor->node_syn))->type);

        E->node_inh = create_node_AST("OPERATION_2", 0, NULL, 3, E->node_inh, Op2->node_syn, Factor->node_syn, NULL, NULL, NULL, NULL, NULL, NULL);
        E1->node_inh = E->node_inh;

        construct_AST(E1, rules);

        E->node_syn = E1->node_syn;
    }
    break;
    case 91: // E' .
    {

        // printf("E'''\n");
        t_node *E = root;

        E->node_syn = E->node_inh;
    }
    break;
    case 92: // Op2 mul .
    {
        t_node *Op2 = root;
        t_node *mul = root->children[0];

        Op2->node_syn = create_node_AST(mul->lexeme, 1, mul->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 93: // Op2 div .
    {
        t_node *Op2 = root;
        t_node *div = root->children[0];

        Op2->node_syn = create_node_AST(div->lexeme, 1, div->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
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

        Factor->node_syn = create_node_AST(num->lexeme, 1, num->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 97: // Factor rnum .
    {
        t_node *Factor = root;
        t_node *rnum = root->children[0];

        Factor->node_syn = create_node_AST(rnum->lexeme, 1, rnum->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 98: // cFactor id K' .
    {
        t_node *Factor = root;
        t_node *id = root->children[0];
        t_node *K = root->children[1];

        K->node_inh = create_node_AST(id->lexeme, 1, id->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        construct_AST(K, rules);

        Factor->node_syn = K->node_syn;
    }
    break;
    case 99: // K' sqbo Elemindex sqbc .
    {
        t_node *K = root;
        t_node *Elemindex = root->children[1];

        construct_AST(Elemindex, rules);

        K->node_syn = create_node_AST("ARRAY", 0, NULL, 2, K->node_inh, Elemindex->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 100: // K' .
    {
        t_node *K = root;

        // printf("most inside\n");
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

        N4->node_inh = create_node_AST("OPERATION_1", 0, NULL, 3, N4->node_inh, Op1->node_syn, Aterm->node_syn, NULL, NULL, NULL, NULL, NULL, NULL);
        N41->node_inh = N4->node_inh;

        construct_AST(N41, rules);

        N4->node_syn = N41->node_syn;
    }
    break;
    case 103: // N4 .
    {
        // printf("N4 -> .\n");
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

        N5->node_inh = create_node_AST("OPERATION_2", 0, NULL, 3, N5->node_inh, Op2->node_syn, Afactor->node_syn, NULL, NULL, NULL, NULL, NULL, NULL);
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
    case 107: // Afactor id .
    {
        t_node *Afactor = root;
        t_node *id = root->children[0];

        Afactor->node_syn = create_node_AST(id->lexeme, 1, id->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 108: // Afactor num .
    {
        t_node *Afactor = root;
        t_node *num = root->children[0];

        Afactor->node_syn = create_node_AST(num->lexeme, 1, num->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 109: // Afactor Bool .
    {
        t_node *Afactor = root;
        t_node *Bool = root->children[0];
        construct_AST(Bool, rules);
        Afactor->node_syn = Bool->node_syn;
    }
    break;
    case 110: // Afactor bo Aexpr bc .
    {
        t_node *Afactor = root;
        t_node *Aexpr = root->children[1];
        construct_AST(Aexpr, rules);
        Afactor->node_syn = Aexpr->node_syn;
    }
    break;
    case 111: // Bool true .
    {
        t_node *Bool = root;
        t_node *true = root->children[0];

        Bool->node_syn = create_node_AST(true->lexeme, 1, true->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 112: // Bool false .
    {
        t_node *Bool = root;
        t_node *false = root->children[0];

        Bool->node_syn = create_node_AST(false->lexeme, 1, false->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 113: // Logicalop AND .
    {
        t_node *Logicalop = root;
        t_node *AND = root->children[0];

        Logicalop->node_syn = create_node_AST(AND->lexeme, 1, AND->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 114: // Logicalop OR .
    {
        t_node *Logicalop = root;
        t_node *OR = root->children[0];

        Logicalop->node_syn = create_node_AST(OR->lexeme, 1, OR->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 115: // Relationalop lt .
    {
        t_node *Relationalop = root;
        t_node *lt = root->children[0];

        Relationalop->node_syn = create_node_AST(lt->lexeme, 1, lt->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 116: // Relationalop gt .
    {
        t_node *Relationalop = root;
        t_node *gt = root->children[0];

        Relationalop->node_syn = create_node_AST(gt->lexeme, 1, gt->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 117: // Relationalop le .
    {
        t_node *Relationalop = root;
        t_node *le = root->children[0];

        Relationalop->node_syn = create_node_AST(le->lexeme, 1, le->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 118: // Relationalop ge .
    {
        t_node *Relationalop = root;
        t_node *ge = root->children[0];

        Relationalop->node_syn = create_node_AST(ge->lexeme, 1, ge->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 119: // Relationalop eq .
    {
        t_node *Relationalop = root;
        t_node *eq = root->children[0];

        Relationalop->node_syn = create_node_AST(eq->lexeme, 1, eq->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 120: // Relationalop ne .
    {
        t_node *Relationalop = root;
        t_node *ne = root->children[0];

        Relationalop->node_syn = create_node_AST(ne->lexeme, 1, ne->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
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
        Declarestmt->node_syn = create_node_AST("DECLARE", 0, NULL, 2, Datatype->node_syn, Actparalist->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
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
        Conditionalstmt->node_syn = create_node_AST("SWITCH", 0, NULL, 3, create_node_AST(id->lexeme, 1, id->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL), Casestmt->node_syn, Default->node_syn, NULL, NULL, NULL, NULL, NULL, NULL);

        ((ast_node *)(Conditionalstmt->node_syn))->containsBounds = 1;
        ((ast_node *)(Conditionalstmt->node_syn))->startLine = SWITCH->line_no;
        ((ast_node *)(Conditionalstmt->node_syn))->endLine = END->line_no;
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
        ast_node *case_node = create_node_AST("CASE", 0, NULL, 2, Value->node_syn, Statements->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        Casestmt->list_inh = insert_at_end(Casestmt->list_inh, case_node);
        F->list_inh = Casestmt->list_inh;

        construct_AST(F, rules);

        Casestmt->list_syn = F->list_syn;

        ((ast_node *)(case_node))->containsBounds = 1;
        ((ast_node *)(case_node))->startLine = CASE->line_no;
        ((ast_node *)(case_node))->endLine = SEMICOL->line_no;
    }
    break;
    case 124: // F' case Value colon Statements break semicol F'1
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

        ast_node *case_node = create_node_AST("CASE", 0, NULL, 2, Value->node_syn, Statements->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        F->list_inh = insert_at_end(F->list_inh, case_node);
        F1->list_inh = F->list_inh;

        construct_AST(F1, rules);

        F->list_syn = F1->list_syn;

        ((ast_node *)(case_node))->containsBounds = 1;
        ((ast_node *)(case_node))->startLine = CASE1->line_no;
        ((ast_node *)(case_node))->endLine = SEMICOL1->line_no;
    }
    break;
    case 125: // F'
    {
        t_node *F = root;

        F->list_syn = F->list_inh;
    }
    break;
    case 126: // Value num .
    {
        t_node *Value = root;
        t_node *num = root->children[0];

        Value->node_syn = create_node_AST(num->lexeme, 1, num->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 127: // Value true .
    {
        t_node *Value = root;
        t_node *true = root->children[0];

        Value->node_syn = create_node_AST(true->lexeme, 1, true->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 128: // Value false .
    {
        t_node *Value = root;
        t_node *false = root->children[0];

        Value->node_syn = create_node_AST(false->lexeme, 1, false->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 129: // Default default colon Statements break semicol .
    {
        t_node *Default = root;
        t_node *DEFAULT = root->children[0];
        t_node *Statements = root->children[2];
        t_node *SEMICOL2 = root->children[4];

        Statements->list_inh = create_list();

        construct_AST(Statements, rules);

        Statements->node_syn = create_node_AST_from_list("STATEMENT_LIST", Statements->list_syn);

        Default->node_syn = create_node_AST("DEFAULT", 0, NULL, 1, Statements->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        ((ast_node *)(Default->node_syn))->containsBounds = 1;
        ((ast_node *)(Default->node_syn))->startLine = DEFAULT->line_no;
        ((ast_node *)(Default->node_syn))->endLine = SEMICOL2->line_no;
    }
    break;
    case 130: // Default .
    {
        t_node *Default = root;

        Default->node_syn = NULL;
    }
    break;
    case 131: // Iterativestmt for bo id in Range bc start Statements end .
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

        Iterativestmt->node_syn = create_node_AST("FOR_LOOP", 0, NULL, 3, create_node_AST(id->lexeme, 1, id->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL), Range->node_syn, Statements->node_syn, NULL, NULL, NULL, NULL, NULL, NULL);

        ((ast_node *)(Iterativestmt->node_syn))->containsBounds = 1;
        ((ast_node *)(Iterativestmt->node_syn))->startLine = FOR->line_no;
        ((ast_node *)(Iterativestmt->node_syn))->endLine = END->line_no;
    }
    break;
    case 132: // Range Indloop1 rangeop Indloop2 .
    {
        t_node *Range = root;
        t_node *Indloop1 = root->children[0];
        t_node *Indloop2 = root->children[2];

        construct_AST(Indloop1, rules);
        construct_AST(Indloop2, rules);

        Range->node_syn = create_node_AST("LOOP_RANGE", 0, NULL, 2, Indloop1->node_syn, Indloop2->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 133: // Indloop Signloop Indcoefloop .
    {
        t_node *Indloop = root;
        t_node *Signloop = root->children[0];
        t_node *Indcoefloop = root->children[1];

        construct_AST(Signloop, rules);
        construct_AST(Indcoefloop, rules);

        Indloop->node_syn = create_node_AST("RANGE_LIMIT", 0, NULL, 2, Signloop->node_syn, Indcoefloop->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 134: // Indcoefloop num .
    {
        t_node *Indcoefloop = root;
        t_node *num = root->children[0];

        Indcoefloop->node_syn = create_node_AST(num->lexeme, 1, num->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 135: // Signloop plus .
    {
        t_node *Signloop = root;
        t_node *plus = root->children[0];

        Signloop->node_syn = create_node_AST(plus->lexeme, 1, plus->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 136: // Signloop minus .
    {
        t_node *Signloop = root;
        t_node *minus = root->children[0];

        Signloop->node_syn = create_node_AST(minus->lexeme, 1, minus->lex, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    break;
    case 137: // Signloop .
    {
        t_node *Signloop = root;

        Signloop->node_syn = NULL;
    }
    break;
    case 138: // Iterativestmt while bo Aorbexpr bc start Statements end .
    {         // 0   1   2       3   4       5       6
        t_node *Iterativestmt = root;
        t_node *WHILE = root->children[0];
        t_node *Aorbexpr = root->children[2];
        t_node *Statements = root->children[5];
        t_node *end = root->children[6];

        Statements->list_inh = create_list();

        construct_AST(Aorbexpr, rules);
        construct_AST(Statements, rules);

        Statements->node_syn = create_node_AST_from_list("STATEMENT_LIST", Statements->list_syn);

        Iterativestmt->node_syn = create_node_AST("WHILE_LOOP", 0, NULL, 2, Aorbexpr->node_syn, Statements->node_syn, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        ((ast_node *)(Iterativestmt->node_syn))->containsBounds = 1;
        ((ast_node *)(Iterativestmt->node_syn))->startLine = WHILE->line_no;
        ((ast_node *)(Iterativestmt->node_syn))->endLine = end->line_no;
    }
    break;
    default:
        break;
    }

    return node;
}

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

FILE *fp;

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
            else if (strcmp(opType, ">") == 0|| strcmp(opType, "<") == 0|| strcmp(opType, ">=") == 0 || strcmp(opType, "<=") == 0 || strcmp(opType, "==") == 0 || strcmp(opType, "!=") == 0)
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
                printf(" Error : Index %d out of bounds! Line No. : %d\n", indexValue, idLineNumber);
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

                    st_entry* existingEntry = getSymbolEntryCurrent(parent, idNode->type);

                    if (existingEntry != NULL && !existingEntry->isShadow)
                    {
                        printf("Error: Variable %s already declared in this scope! Line No. : %d\n", idNode->type, idNode->lexeme->line_no);
                        numSemanticErrors++;
                    }
                    else
                    {
                        int width = (isDynamicLeft || isDynamicRight) ? 1 : (1 + (rightIndex - leftIndex + 1) * getTypeWidth(arrayType));
                        id_type *idType = createIDType(arrayType, 1, !(isDynamicLeft || isDynamicRight), 0, leftIndex, rightIndex, leftIndexName, rightIndexName, isDynamicLeft, isDynamicRight, NULL, 0, NULL, 0);

                        if(existingEntry && existingEntry->isShadow){
                            addSymbolEntry(parent, createSymbolEntry("123", idType, parent->scopeName, width, -1, -1, -1, parent->offsetAct));
                            existingEntry->isShadow = 0;
                            existingEntry->idType = idType;
                            existingEntry->width = width;
                            existingEntry->offset = parent->offset;
                            existingEntry->nestingLevel = parent->nestingLevel;
                            existingEntry->lineNo = idNode->lexeme->line_no;
                            printSymbolEntry(fp, existingEntry);
                        }else{
                            st_entry *entry = createSymbolEntry(idNode->type, idType, parent->scopeName, width, parent->offset, parent->nestingLevel, idNode->lexeme->line_no, parent->offsetAct);
                            addSymbolEntry(parent, entry);
                            printSymbolEntry(fp, entry);
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

                    st_entry* existingEntry = getSymbolEntryCurrent(parent, idNode->type);

                    if(existingEntry != NULL && !existingEntry->isShadow)
                    {
                        printf("Error : Variable %s already declared in this scope! Line No. : %d\n", idNode->type, idNode->lexeme->line_no);
                        numSemanticErrors++;
                    }
                    else
                    {
                        int width = getTypeWidth(varType);
                        int offset = -1;
                        id_type *idType = createIDType(varType, 0, 0, 0, -1, -1, NULL, NULL, 0, 0, NULL, 0, NULL, 0);

                        if(existingEntry && existingEntry->isShadow){
                            addSymbolEntry(parent, createSymbolEntry("123", idType, parent->scopeName, width, -1, -1, -1, parent->offsetAct));
                            existingEntry->isShadow = 0;
                            existingEntry->idType = idType;
                            existingEntry->width = width;
                            existingEntry->offset = parent->offset;
                            existingEntry->nestingLevel = parent->nestingLevel;
                            existingEntry->lineNo = idNode->lexeme->line_no;
                            printSymbolEntry(fp, existingEntry);
                        }else{
                            st_entry *entry = createSymbolEntry(idNode->type, idType, parent->scopeName, width, parent->offset, parent->nestingLevel, idNode->lexeme->line_no, parent->offsetAct);
                            addSymbolEntry(parent, entry);
                            printSymbolEntry(fp, entry);
                        }
                    }
                }
            }
        }
        else if (strcmp(curr->type, "MODULE") == 0)
        {
            char *functionName = ((ast_node *)(curr->children[0]))->type;

            if(getSymbolEntry(parent, functionName) != NULL){
                st_entry *entry = getSymbolEntry(parent, functionName);
                if(entry->didInFirstPass){
                    ast_node *statements = (ast_node *)(curr->children[3]);
                    for (int i = 0; i < statements->numChildren; i++)
                    {
                        constructSymbolTable(statements->children[i], curr->symbolTable);
                    }
                    return;
                }else{
                    printf("Error : Function %s already declared in this scope! Line No. : %d\n",  functionName,((ast_node *)(curr->children[0]))->lexeme->line_no);
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
                printSymbolEntry(fp, entry);
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
                printSymbolEntry(fp, entry);
            }

            id_type *functionType = createIDType("module", 0, 0, 1, -1, -1, NULL, NULL, 0, 0, inputParamTypes, numInputParams, outputParamTypes, numOutputParams);

            st_entry *newEntry = NULL;
            printf("New Module Defined\n");
            newEntry = createSymbolEntry(functionName, functionType, parent->scopeName, -1, -1, parent->nestingLevel, ((ast_node *)(curr->children[0]))->lexeme->line_no, -1);
            newEntry->assigned = 1;
            addSymbolEntry(parent, newEntry);
            printSymbolEntry(fp, newEntry);

            ast_node *statements = (ast_node *)(curr->children[3]);
            for (int i = 0; i < statements->numChildren; i++)
            {
                constructSymbolTable(statements->children[i], curr->symbolTable);
            }
        }
        else if (strcmp(curr->type, "MODULE_LIST") == 0)
        {
            return; //pass
        }
        else if (strcmp(curr->type, "WHILE_LOOP") == 0)
        {
            curr->symbolTable = createSymbolTable("WHILE_LOOP", parent, curr->startLine, curr->endLine, parent->moduleName);
            ast_node* whileCondition = (ast_node *)(curr->children[0]);

            if(resolveExpression(whileCondition, curr->symbolTable) != 3){
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
            curr->symbolTable = createSymbolTable("FOR_LOOP", parent, curr->startLine, curr->endLine, parent->moduleName);
            char* loopid = ((ast_node *)(curr->children[0]))->type;

            id_type *type = createIDType("integer", 0, 0, 0, -1, -1, NULL, NULL, 0, 0, NULL, 0, NULL, 0);
            st_entry *entry = createSymbolEntry(loopid, type, curr->symbolTable->scopeName, 2, curr->symbolTable->offset, curr->symbolTable->nestingLevel, ((ast_node *)(curr->children[0]))->lexeme->line_no, curr->symbolTable->offsetAct);
            addSymbolEntry(curr->symbolTable, entry);
            printSymbolEntry(fp, entry);

            for (int i = 0; i < curr->numChildren; i++)
            {
                constructSymbolTable(curr->children[i], curr->symbolTable);
            }
        }
        else if (strcmp(curr->type, "SWITCH") == 0)
        {
            ast_node* switchVariable = (ast_node*)(curr->children[0]);
      
            st_entry* switchVariableEntry = getSymbolEntry(parent, switchVariable->type);
            if (switchVariableEntry == NULL){
                printf("Error : Variable %s is not declared! Line No. : %d\n", switchVariable->type, switchVariable->lexeme->line_no);
                numSemanticErrors++;
            }else if(strcmp(switchVariableEntry->idType->type, "real") == 0){
                printf("Error : Variable %s is a real, cannot be used in a switch statement! Line No. : %d\n", switchVariable->type, switchVariable->lexeme->line_no);
                numSemanticErrors++;
            }else if(switchVariableEntry->idType->is_array){
                printf("Error : Variable %s is an array, cannot be used in a switch statement! Line No. : %d\n", switchVariable->type, switchVariable->lexeme->line_no);
                numSemanticErrors++;
            }else{
                switchType = switchVariableEntry->idType->type;

                ast_node* defaultNode = (ast_node*)(curr->children[2]);
                if(strcmp(switchVariableEntry->idType->type, "integer") == 0 && defaultNode == NULL){
                    printf("Error : Variable %s is of type integer, Switch must have a default case! Line No. : %d\n", switchVariable->type, switchVariable->lexeme->line_no);
                    numSemanticErrors++;
                }else if(strcmp(switchVariableEntry->idType->type, "boolean") == 0 && defaultNode != NULL){
                    printf("Error : Variable %s is of type boolean, Switch cannot have a default case! Line No. : %d\n", switchVariable->type, switchVariable->lexeme->line_no);
                    numSemanticErrors++;
                }
                
                for (int i = 0; i < curr->numChildren; i++)
                {
                    constructSymbolTable(curr->children[i], parent);
                }
            }           
        }else if(strcmp(curr->type, "CASE") == 0){
            curr->symbolTable = createSymbolTable("CASE", parent, curr->startLine, curr->endLine, parent->moduleName);

            ast_node* caseValue = (ast_node*)(curr->children[0]);
            
            if(strcmp(caseValue->type, "true") == 0 || strcmp(caseValue->type, "false") == 0){
                if(strcmp(switchType, "boolean") != 0){
                    printf("Error : Case value %s, of type integer, cannot be used in a switch statement of type %s! Line No. : %d\n", caseValue->type, switchType, caseValue->lexeme->line_no);
                    numSemanticErrors++;
                }
            }else{
                if(strcmp(switchType, "integer") != 0){
                    printf("Error : Case value %s , of type boolean, cannot be used in a switch statement of type %s! Line No. : %d\n", caseValue->type, switchType, caseValue->lexeme->line_no);
                    numSemanticErrors++;
                }
            }

            for (int i = 0; i < curr->numChildren; i++)
            {
                constructSymbolTable(curr->children[i], curr->symbolTable);
            }
        }else if(strcmp(curr->type, "DEFAULT") == 0){
            curr->symbolTable = createSymbolTable("DEFAULT CASE", parent, curr->startLine, curr->endLine, parent->moduleName);
            for (int i = 0; i < curr->numChildren; i++)
            {
                constructSymbolTable(curr->children[i], curr->symbolTable);
            }
        }else if (strcmp(curr->type, "ASSIGN") == 0){
            ast_node *lhs = (ast_node *)(curr->children[0]);
            ast_node *rhs = (ast_node *)(curr->children[1]);

            if (lhs->containsLexeme && rhs->containsLexeme)
            {
                st_entry *lhsEntry = getSymbolEntry(parent, lhs->type);
                st_entry *rhsEntry = getSymbolEntry(parent, rhs->type);

                int entriesExist = 1;
                if (lhsEntry == NULL)
                {
                    fprintf(fp, "Error : Variable %s is not declared!\n", lhs->type);
                    numSemanticErrors++;
                    entriesExist = 0;
                }
                if (rhsEntry == NULL)
                {
                    fprintf(fp, "Error : Variable %s is not declared!\n", rhs->type);
                    numSemanticErrors++;
                    entriesExist = 0;
                }

                if (entriesExist && lhsEntry != NULL && rhsEntry != NULL)
                {
                    if (lhsEntry->idType->is_array && rhsEntry->idType->is_array)
                    {
                        if (lhsEntry->idType->is_static_array && rhsEntry->idType->is_static_array)
                        {
                            if (strcmp(lhsEntry->idType->type, rhsEntry->idType->type) != 0)
                            {
                                fprintf(fp, "Error : Array %s and %s are not of same type!\n", lhs->type, rhs->type);
                                numSemanticErrors++;
                            }
                            else if (lhsEntry->width != rhsEntry->width)
                            {
                                fprintf(fp, "Error : Array %s and %s are not of same size!\n", lhs->type, rhs->type);
                                numSemanticErrors++;
                            }
                        }
                    }
                }
            }

            int lhsType = resolveExpression(lhs, parent);
            int rhsType = resolveExpression(rhs, parent);

            //printf("LHS type : %d, RHS type : %d\n", lhsType, rhsType);

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
            }
        }
        else if(strcmp(curr->type, "REUSE_STMT") == 0){
            ast_node* moduleName = (ast_node*)(curr->children[1]);
            st_entry* moduleEntry = getSymbolEntry(parent, moduleName->type);
            if(moduleEntry == NULL){
                printf("Error : Module %s is not declared! Line No. : %d\n", moduleName->type, moduleName->lexeme->line_no);
                numSemanticErrors++;
            }else if(strcmp(moduleEntry->idType->type, "module") != 0){
                printf("Error : %s is not a module! Line No. : %d\n", moduleName->type, moduleName->lexeme->line_no);
                numSemanticErrors++;
            }else{
                ast_node* paramsList = (ast_node*)(curr->children[2]);
                ast_node* outputList = (ast_node*)(curr->children[0]);

                int outputExists = 1;
                if(outputList == NULL){
                    outputExists = 0;
                }

                if(paramsList->numChildren != moduleEntry->idType->numInputParams){
                    //check if number of args correct
                    printf("Error : Number of parameters in module call do not match number of inputs! Line No. : %d\n", moduleName->lexeme->line_no);
                    numSemanticErrors++;
                }else{
                    //check if func call correct

                    for(int i = 0; i < paramsList->numChildren; i++){
                        ast_node* param = (ast_node*)(paramsList->children[i]);
                        ast_node* paramNode = (ast_node*)(param->children[0]);

                        id_type* definedType = moduleEntry->idType->inputParams[i];
                        
                        if(strcmp(paramNode->type, "ARRAY_ELEM_OR_ID") == 0){
                            ast_node* idNode = (ast_node*)(paramNode->children[0]);
                            st_entry* idEntry = getSymbolEntry(parent, idNode->type);

                            if(idEntry == NULL){
                                printf("Error : Variable %s is not declared! Line No. : %d\n", idNode->type, idNode->lexeme->line_no);
                                numSemanticErrors++;
                            }else{
                                ast_node* arrIndex = (ast_node*)(paramNode->children[1]);
                                if(arrIndex != NULL){
                                    if(!idEntry->idType->is_array){
                                        printf("Error : Variable %s is not an array! Line No. : %d\n", idNode->type, idNode->lexeme->line_no);
                                        numSemanticErrors++;
                                    }else{
                                        if(arrIndex->containsLexeme){
                                            //NORMAL CASE
                                            if(strcmp(arrIndex->lexeme->token, token_list[ID]) == 0){
                                                st_entry* indexEntry = getSymbolEntry(parent, arrIndex->type);
                                                if(indexEntry == NULL){
                                                    printf("Error : Variable %s is not declared! Line No. : %d\n", arrIndex->type, arrIndex->lexeme->line_no);
                                                    numSemanticErrors++;
                                                }else if(strcmp("integer", indexEntry->idType->type) != 0){
                                                    printf("Error : Array index must be an integer! Line No. : %d\n", arrIndex->lexeme->line_no);
                                                    numSemanticErrors++;
                                                }
                                            }else if(strcmp(arrIndex->lexeme->token, token_list[NUM]) == 0){
                                                if(idEntry->idType->is_static_array){
                                                    int index = atoi(arrIndex->lexeme->lexeme);
                                                    if(index < idEntry->idType->leftIndex || index > idEntry->idType->rightIndex){
                                                        printf("Error : Array index out of bounds! Line No. : %d\n", arrIndex->lexeme->line_no);
                                                        numSemanticErrors++;
                                                    }
                                                }
                                            }else{
                                                printf("Error : Array index must be an integer! Line No. : %d\n", arrIndex->lexeme->line_no);
                                            }
                                        }else{
                                            //BOUND NODE
                                            ast_node* signNode = (ast_node*)(arrIndex->children[0]);
                                            int negative = 1;

                                            if(signNode != NULL){
                                                if(strcmp(signNode->type, "-") == 0){
                                                    negative = -1;
                                                }
                                            }

                                            ast_node* numNode = (ast_node*)(arrIndex->children[1]);

                                            if(strcmp(numNode->lexeme->token, token_list[ID]) == 0){
                                                st_entry* indexEntry = getSymbolEntry(parent, numNode->type);
                                                if(indexEntry == NULL){
                                                    printf("Error : Variable %s is not declared! Line No. : %d\n", numNode->type, numNode->lexeme->line_no);
                                                    numSemanticErrors++;
                                                }else if(strcmp("integer", indexEntry->idType->type) != 0){
                                                    printf("Error : Array index must be an integer! Line No. : %d\n", numNode->lexeme->line_no);
                                                    numSemanticErrors++;
                                                }
                                            }else if(strcmp(numNode->lexeme->token, token_list[NUM]) == 0){
                                                if(idEntry->idType->is_static_array){
                                                    int index = atoi(numNode->lexeme->lexeme);
                                                    index = index*negative;
                                                    if(index < idEntry->idType->leftIndex || index > idEntry->idType->rightIndex){
                                                        printf("Error : Array index out of bounds! Line No. : %d\n", numNode->lexeme->line_no);
                                                        numSemanticErrors++;
                                                    }
                                                }
                                            }else{
                                                printf("Error : Array index must be an integer! Line No. : %d\n", numNode->lexeme->line_no);
                                            }
                                        }

                                        if(strcmp(definedType->type, idEntry->idType->type) != 0){
                                            printf("Error : Actual parameter of type %s, cannot be used in module call of type %s! Line No. : %d \n", idEntry->idType->type, definedType->type, idNode->lexeme->line_no);
                                            numSemanticErrors++;
                                        }
                                    }
                                }else{
                                    if(idEntry->idType->is_array && !definedType->is_array || !idEntry->idType->is_array && definedType->is_array){
                                        printf("Error : Variable type error! One of them is an array, other is not! Line No. : %d\n", idNode->lexeme->line_no);
                                        numSemanticErrors++;
                                    }else if(strcmp(definedType->type, idEntry->idType->type) != 0){
                                        printf("Error : Actual parameter of type %s, cannot be used in module call of type %s! Line No. : %d \n", idEntry->idType->type, definedType->type, idNode->lexeme->line_no);
                                        numSemanticErrors++;
                                    }
                                    if(definedType->is_static_array && idEntry->idType->is_static_array){
                                        if(definedType->rightIndex-definedType->leftIndex != idEntry->idType->rightIndex-idEntry->idType->leftIndex){
                                            printf("Error : Array size mismatch in function call! Line No. : %d\n", idNode->lexeme->line_no);
                                        }
                                    }
                                }
                            }
                        }else if(strcmp(paramNode->type, "NUM") == 0){
                            if(definedType->is_array){
                                printf("Error : Actual parameter of type array, does not match with formal parameter of type %s! Line No. : %d \n", definedType->type, ((ast_node*)paramNode->children[0])->lexeme->line_no);
                                numSemanticErrors++;
                            }else if(strcmp(definedType->type, "integer") != 0){
                                printf("Error :Actual parameter of type integer, does not match with formal parameter of type %s! Line No. : %d\n", definedType->type, ((ast_node*)paramNode->children[0])->lexeme->line_no);
                                numSemanticErrors++;
                            }
                        }else if(strcmp(paramNode->type, "RNUM") == 0){
                            if(definedType->is_array){
                                printf("Error :Actual parameter of type array, does not match with formal parameter of type %s! Line No. : %d\n", definedType->type, ((ast_node*)paramNode->children[0])->lexeme->line_no);
                                numSemanticErrors++;
                            }else if(strcmp(definedType->type, "real") != 0){
                                printf("Error :Actual parameter of type real, does not match with formal parameter of type %s! Line No. : %d\n", definedType->type, ((ast_node*)paramNode->children[0])->lexeme->line_no);
                                numSemanticErrors++;
                            }
                        }else if(strcmp(paramNode->type, "true") == 0 || strcmp(paramNode->type, "false") == 0){
                            if(definedType->is_array){
                                printf("Error : Actual parameter %s of type array does not match with formal parameter of type %s! Line No. : %d\n", paramNode->type, definedType->type, paramNode->lexeme->line_no);
                                numSemanticErrors++;
                            }else if(strcmp(definedType->type, "boolean") != 0){
                                printf("Error : Actual parameter %s  of type boolean, does not match with formal parameter of type %s! Line No. : %d\n", paramNode->type, definedType->type, paramNode->lexeme->line_no);
                                numSemanticErrors++;
                            }
                        }
                    }
                }

                if(outputExists && moduleEntry->idType->numOutputParams != outputList->numChildren){
                    //check if number of outputs correct
                    printf("Error : Number of parameters in module call do not match number of outputs! Line No. : \n", moduleName->lexeme->line_no);
                    numSemanticErrors++;
                }else if(outputExists){
                    for(int i = 0; i < outputList->numChildren; i++){
                        ast_node* output = (ast_node*)(outputList->children[i]);
                        id_type* definedType = moduleEntry->idType->outputParams[i];

                        if(output->containsLexeme && strcmp(output->lexeme->token, token_list[ID]) == 0){
                            st_entry* idEntry = getSymbolEntry(parent, output->type);
                            if(idEntry == NULL){
                                printf("Error : Variable %s is not declared! Line No. : %d\n", output->type, output->lexeme->line_no);
                                numSemanticErrors++;
                            }else{
                                if(definedType->is_array && idEntry->idType->is_array || !definedType->is_array && !idEntry->idType->is_array){
                                    if(strcmp(definedType->type, idEntry->idType->type) != 0){
                                        printf("Error : Output variable %d in module call does not match type of output! Line No. : %d\n", i+1, output->lexeme->line_no);
                                        numSemanticErrors++;
                                    }else if(definedType->is_static_array && idEntry->idType->is_static_array){
                                        if(definedType->rightIndex - definedType->leftIndex != idEntry->idType->rightIndex - idEntry->idType->leftIndex){
                                            printf("Error : Array width mismatch! Line No. : %d", output->lexeme->line_no);
                                        }
                                    }
                                }else{
                                    printf("Error : Type mismatch, one of the variables is not an array! Line No. : %d\n", output->lexeme->line_no);
                                }
                            }
                        }else{
                            printf("Error : Output variable %d in module call is not a variable! Line No. : %d\n", i+1, output->lexeme->line_no);
                        }
                    }
                }
            }
        }
        else if(strcmp(curr->type, "GET_VALUE") == 0){
            ast_node* idNode = (ast_node*)(curr->children[0]);
            if(idNode != NULL && idNode->containsLexeme){
                if(strcmp(idNode->lexeme->token, token_list[ID]) == 0){
                    st_entry* idEntry = getSymbolEntry(parent, idNode->type);
                    if(idEntry == NULL){
                        printf("Error : Variable %s is not declared! Line No. : %d\n", idNode->type, idNode->lexeme->line_no);
                        numSemanticErrors++;
                    }
                }
            }else{
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
        //printf("Constructing symbol table for %s\n", curr->type);
        //fflush(stdout);
        if (strcmp(curr->type, "PROGRAM") == 0)
        {
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
                    //CHECK THIS
                    printf("Error : Module %s already declared earlier! Line No. : %d\n", declaration->type, declaration->lexeme->line_no);
                    numSemanticErrors++;
                }
                else
                {
                    st_entry *decEntry = createSymbolEntry(declaration->type, type, parent->scopeName, -1, -1, parent->nestingLevel, declaration->lexeme->line_no, -1);
                    addSymbolEntry(parent, decEntry);
                    printSymbolEntry(fp, decEntry);
                    printf("Added declaration %s to symbol table\n", declaration->type);
                }
            }
        }
        else if (strcmp(curr->type, "MODULE") == 0)
        {
            char *functionName = ((ast_node *)(curr->children[0]))->type;
            st_entry *newEntry = NULL;

            if (getSymbolEntry(parent, functionName) != NULL)
            {
                //should be declared before
                newEntry = getSymbolEntry(parent, functionName);
                printf("Error : Module already declared earlier\n");

                if (newEntry->assigned == 1)
                {
                    printf("Error : Module %s already defined earlier! Line No. : %d\n", functionName, ((ast_node *)(curr->children[0]))->lexeme->line_no);
                    numSemanticErrors++;
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
                        printSymbolEntry(fp, entry);
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
                        printSymbolEntry(fp, entry);
                    }

                    id_type *functionType = createIDType("module", 0, 0, 1, -1, -1, NULL, NULL, 0, 0, inputParamTypes, numInputParams, outputParamTypes, numOutputParams);

                    newEntry->idType = functionType;
                    printf("Error : Module %s already declared earlier\n", functionName);
                    newEntry->assigned = 1;
                    newEntry->didInFirstPass = 1;
                    addSymbolEntry(parent, newEntry);
                    printSymbolEntry(fp, newEntry);
                }
            }
        }else{
            for (int i = 0; i < curr->numChildren; i++)
            {
                constructSymbolTableDeclarations(curr->children[i], parent);
            }
        }
    }
}

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
    generateParseTree(toPrint, parseTreeFile);

    printf("Generating AST\n");
    construct_AST(toPrint, rules);

    printf("\n\nPrinting AST\n");
    FILE *fp1 = fopen("ast.txt", "w");
    traverse_AST(toPrint->node_syn, fp1, 0, 1);
    fclose(fp1);
    printf("Finished Printing AST\n");

    printf("\n\n----Generating Symbol Table----\n\n\n\n");
    fp = fopen("symbolTable.txt", "w");

    constructSymbolTableDeclarations(toPrint->node_syn, NULL);
    printf("Declarations done successfully!\n");
    fflush(stdout);
    constructSymbolTable(toPrint->node_syn, NULL);
    fclose(fp);
    printf("\n\n----Finished Generating Symbol Table----\n\n\n\n");
}