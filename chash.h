//CS F363 : Compiler Construction
//Group No. 24
//1. Nachiket Kotalwar               (2020A7PS0024P)
//2.Parth Patel                      (2020A7PS0026P)
//3.Partha Sarathi Purkayastha       (2020A7PS0043P)
//4.Labeeb Ahsan                     (2020A7PS0045P)
//5.Anish Atul Kulkarni              (2020A7PS0975P)

#ifndef CHASH_H
#define CHASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 1181
#define PRIME 61

typedef struct hash_node {
    void* keys;
    void* values;
} hash_node;

typedef struct hash_table {
    hash_node** nodes;
    int size;
} hash_table;

int hash_code(void* key);

int quadratic_probe(int index, int i);

hash_table* create_table();

void insert(hash_table* table, void* key, void* value);

void* get(hash_table* table, void* key);

void print_table(hash_table* table);

hash_table* initialize();

#endif