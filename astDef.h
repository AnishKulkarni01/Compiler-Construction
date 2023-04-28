//CS F363 : Compiler Construction
//Group No. 24
//1. Nachiket Kotalwar               (2020A7PS0024P)
//2.Parth Patel                      (2020A7PS0026P)
//3.Partha Sarathi Purkayastha       (2020A7PS0043P)
//4.Labeeb Ahsan                     (2020A7PS0045P)
//5.Anish Atul Kulkarni              (2020A7PS0975P)

#ifndef ASTDEF_H
#define ASTDEF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parseTree.h"
#include "parser.h"

typedef struct linked_list
{
    void *data;
    struct linked_list *next;
} llist;

#endif