//CS F363 : Compiler Construction
//Group No. 24
//1. Nachiket Kotalwar               (2020A7PS0024P)
//2.Parth Patel                      (2020A7PS0026P)
//3.Partha Sarathi Purkayastha       (2020A7PS0043P)
//4.Labeeb Ahsan                     (2020A7PS0045P)
//5.Anish Atul Kulkarni              (2020A7PS0975P)

#include "chash.h"
#include "enum.h"

#define TABLE_SIZE 1181
#define PRIME 61

int hash_code(void* key) {
    int hash = 0;
    char* str = (char*) key;
    for (int i = 0; i < strlen(str); i++) {
        hash = ((hash * PRIME )%TABLE_SIZE+ str[i])%TABLE_SIZE;
    }
    return hash % TABLE_SIZE;
}

int quadratic_probe(int index, int i) {
    int c1=2,c2=3,c3=5;
    return (index + (c1*i*i + c2*i + c3)%TABLE_SIZE)  % TABLE_SIZE;
}

hash_table* create_table() {
    hash_table* table = (hash_table*)malloc(sizeof(hash_table));
    table->nodes = (hash_node**) calloc(TABLE_SIZE, sizeof(hash_node*));
    table->size = 0;
    return table;
}

void insert(hash_table* table, void* key, void* value) {
    hash_node* node = (hash_node*) malloc(sizeof(hash_node));
    node->keys = key;
    node->values = value;
    
    int index = hash_code(key);
    int i = 1;
    while (table->nodes[index] != NULL) {
        index = quadratic_probe(index, i);
        i++;
    }

    table->nodes[index] = node;
    table->size++;
}

void* get(hash_table* table, void* key) {
    int index = hash_code(key);
    int i = 1;
    while (table->nodes[index] != NULL) {
        if (strcmp((char*) table->nodes[index]->keys, (char*) key) == 0) {
            return table->nodes[index]->values;
        }
        index = quadratic_probe(index, i);
        i++;
    }
    return NULL;
}

void print_table(hash_table* table) {
    printf("Hash table:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (table->nodes[i] != NULL) {
             printf("[%d]: key=%s, value=%d\n", i, (char*) table->nodes[i]->keys, *(int*) table->nodes[i]->values);
        } 
    }
}

hash_table* initialize() {


    hash_table* ht = create_table();

    enum KEYWORD_ENUM* integer = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *integer = INTEGER;
    enum KEYWORD_ENUM* real = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *real = REAL;
    enum KEYWORD_ENUM* boolean = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *boolean = BOOLEAN;
    enum KEYWORD_ENUM* of = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *of = OF;
    enum KEYWORD_ENUM* array = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *array = ARRAY;
    enum KEYWORD_ENUM* start = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *start = START;
    enum KEYWORD_ENUM* end = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *end = END;
    enum KEYWORD_ENUM* declare = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *declare = DECLARE;
    enum KEYWORD_ENUM* module = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *module = MODULE;
    enum KEYWORD_ENUM* driver = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *driver = DRIVER;
    enum KEYWORD_ENUM* get_value = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *get_value = GET_VALUE;
    enum KEYWORD_ENUM* print = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *print = PRINT;
    enum KEYWORD_ENUM* use = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *use = USE;
    enum KEYWORD_ENUM* with = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *with = WITH;
    enum KEYWORD_ENUM* parameters = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *parameters = PARAMETERS;
    enum KEYWORD_ENUM* takes = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *takes = TAKES;
    enum KEYWORD_ENUM* input = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *input = INPUT;
    enum KEYWORD_ENUM* returns = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *returns = RETURNS;
    enum KEYWORD_ENUM* for_ = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *for_ = FOR;
    enum KEYWORD_ENUM* in_ = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *in_ = IN;
    enum KEYWORD_ENUM* switch_ = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *switch_ = SWITCH;
    enum KEYWORD_ENUM* case_ = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *case_ = CASE;
    enum KEYWORD_ENUM* break_ = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *break_ = BREAK;
    enum KEYWORD_ENUM* default_ = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *default_ = DEFAULT;
    enum KEYWORD_ENUM* while_ = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *while_ = WHILE;
    enum KEYWORD_ENUM* true_ = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *true_ = TRUE;
    enum KEYWORD_ENUM* false_ = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *false_ = FALSE;
    enum KEYWORD_ENUM* and_ = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *and_ = AND;
    enum KEYWORD_ENUM* or_ = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *or_ = OR;
    enum KEYWORD_ENUM* program_ = (enum KEYWORD_ENUM*) malloc(sizeof(enum KEYWORD_ENUM));
    *program_ = PROGRAM;

    insert(ht, "integer", integer);
    insert(ht, "real", real);
    insert(ht, "boolean", boolean);
    insert(ht, "of", of);
    insert(ht, "array", array);
    insert(ht, "start", start);
    insert(ht, "end", end);
    insert(ht, "declare", declare);
    insert(ht, "module", module);
    insert(ht, "driver", driver);
    insert(ht, "get_value", get_value);
    insert(ht, "print", print);
    insert(ht, "use", use);
    insert(ht, "with", with);
    insert(ht, "parameters", parameters);
    insert(ht, "takes", takes);
    insert(ht, "input", input);
    insert(ht, "returns", returns);
    insert(ht, "for", for_);
    insert(ht, "in", in_);
    insert(ht, "switch", switch_);
    insert(ht, "case", case_);
    insert(ht, "break", break_);
    insert(ht, "default", default_);
    insert(ht, "while", while_);
    insert(ht, "true", true_);
    insert(ht, "false", false_);
    insert(ht, "AND", and_);
    insert(ht, "OR", or_);
    insert(ht, "program", program_);

    // enum KEYWORD_ENUM* x1=(enum KEYWORD_ENUM*)get(ht, "integer");
    // enum KEYWORD_ENUM* x2=(enum KEYWORD_ENUM*)get(ht, "real");
    // enum KEYWORD_ENUM* x3=(enum KEYWORD_ENUM*)get(ht, "boolean");
    
    // enum KEYWORD_ENUM* x4=(enum KEYWORD_ENUM*)get(ht, "int");

    // printf("integer: %d\n", *x1);
    // printf("real: %d\n", *x2);
    // printf("and: %d\n", *x3);
    // if(x4!=NULL){
    //     printf("int: %d\n", *x4);
    // }else{
    //     printf("int: NULL\n");
    // }

    return ht;
}