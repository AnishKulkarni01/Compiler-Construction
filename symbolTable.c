//CS F363 : Compiler Construction
//Group No. 24
//1. Nachiket Kotalwar               (2020A7PS0024P)
//2.Parth Patel                      (2020A7PS0026P)
//3.Partha Sarathi Purkayastha       (2020A7PS0043P)
//4.Labeeb Ahsan                     (2020A7PS0045P)
//5.Anish Atul Kulkarni              (2020A7PS0975P)

#include "symbolTable.h"


st_table* createSymbolTable(char* scopeName, st_table* parent, int lineNoStart, int lineNoEnd, char* moduleName){
    st_table* table = (st_table*)malloc(sizeof(st_table));
    table->table = create_table();
    table->scopeName = scopeName;
    table->parent = parent;
    table->offset = 0;
    table->lineNoStart = lineNoStart;
    table->lineNoEnd = lineNoEnd;
    table->moduleName = moduleName;
    table->inForLoop = 0;
    table->inWhileLoop = 0;
    table->forLoopVar = NULL;
    table->whileLoopVar1 = NULL;
    table->whileLoopVar2 = NULL;
    table->moduleOffset = NULL;

    if(strcmp(scopeName, "GLOBAL_SCOPE") == 0 || strcmp(scopeName, moduleName) == 0){
    }else{
        if(strcmp(parent->scopeName, parent->moduleName) == 0){
            table->moduleOffset = &parent->offset;
        }else{
            table->moduleOffset = parent->moduleOffset;
        }
    }
    
    if(parent){
        table->nestingLevel = parent->nestingLevel + 1;
    }else{
        table->nestingLevel = 0;
    }

    if(strcmp(scopeName, moduleName) != 0){
        table->offsetAct = parent->offsetAct;
        table->offset = parent->offset;
    }

    return table;
}

void addSymbolEntry(st_table* stable, st_entry* entry){
    insert(stable->table, entry->var_name, entry);
    stable->offset += entry->width;

    if(stable->moduleOffset != NULL){
        (*(stable->moduleOffset)) += entry->width;
    }
    if(entry->idType->is_array){
        if(entry->idType->is_static_array){
            stable->offsetAct += 24 + (entry->idType->rightIndex - entry->idType->leftIndex + 1) * 8;
        }else{
            stable->offsetAct += 24;
        }
    }else{
        stable->offsetAct += 8;
    }
}

st_entry* getSymbolEntry(st_table* stable, char* var_name){
    if(stable == NULL)
        return NULL;
    st_entry* curr = (st_entry*)get(stable->table, var_name);
    if(curr == NULL){
        return getSymbolEntry(stable->parent, var_name);
    }
    return curr;
}

st_entry* getSymbolEntryCurrent(st_table* stable, char* var_name){
    if(stable == NULL)
        return NULL;
    return (st_entry*)get(stable->table, var_name);
}

id_type* createIDType(char* type, int is_array, int is_static_array, int is_function, int leftIndex, int rightIndex, char* leftIndexName, char* rightIndexName, int isDynamicLeft, int isDynamicRight, id_type** inputParams, int numInputParams, id_type** outputParams, int numOutputParams){
    id_type* idType = (id_type*)malloc(sizeof(id_type));
    idType->type = type;
    idType->is_array = is_array;
    idType->is_static_array = is_static_array;
    idType->is_function = is_function;
    idType->leftIndex = leftIndex;
    idType->rightIndex = rightIndex;
    idType->leftIndexName = leftIndexName;
    idType->rightIndexName = rightIndexName;
    idType->isDynamicLeft = isDynamicLeft;
    idType->isDynamicRight = isDynamicRight;
    idType->inputParams = inputParams;
    idType->numInputParams = numInputParams;
    idType->outputParams = outputParams;
    idType->numOutputParams = numOutputParams;
    return idType;
}

st_entry* createSymbolEntry(char* var_name, id_type* type, char* scopeName, int width, int offset, int nestingLevel, int lineNo, int offsetAct){
    st_entry* entry = (st_entry*)malloc(sizeof(st_entry));
    entry->var_name = var_name;
    entry->scopeName = scopeName;
    entry->idType = type;
    entry->width = width;
    entry->offset = offset;
    entry->offsetAct = offsetAct;
    entry->nestingLevel = nestingLevel;
    entry->lineNo = lineNo;
    entry->assigned = 0;
    entry->didInFirstPass = 0;
    entry->isShadow = 0;
    entry->unusableFunc = 0;
    return entry;
}

void printSymbolEntry(FILE* fp,st_entry* entry){
    fprintf(fp,"var_name: %-10s, assigned:  %-10d,  scopeName: %-10s,   width:           %-10d, offset:      %-10d, nestingLevel: %-10d, lineNo:     %-10d\n", entry->var_name, entry->assigned, entry->scopeName, entry->width, entry->offset, entry->nestingLevel, entry->lineNo);
    fprintf(fp,"Type:     %-10s, is_array:  %-10d, is_static_array: %-10d, is_function: %-10d, leftIndex:    %-10d, rightIndex: %-10d, leftIndexName: %-10s, rightIndexName: %-10s, isDynamicLeft: %-10d, isDynamicRight: %-10d\n", entry->idType->type, entry->idType->is_array, entry->idType->is_static_array, entry->idType->is_function, entry->idType->leftIndex, entry->idType->rightIndex, entry->idType->leftIndexName, entry->idType->rightIndexName, entry->idType->isDynamicLeft, entry->idType->isDynamicRight);
    if(entry->idType->is_function){
        fprintf(fp,"Input Params:\n");
        for(int i = 0; i < entry->idType->numInputParams; i++){
    fprintf(fp,"type:     %-10s, is_array:  %-10d, is_static_array: %-10d, is_function: %-10d, leftIndex:    %-10d, rightIndex: %-10d, leftIndexName: %-10s, rightIndexName: %-10s, isDynamicLeft: %-10d, isDynamicRight: %-10d \n", entry->idType->inputParams[i]->type, entry->idType->inputParams[i]->is_array, entry->idType->inputParams[i]->is_static_array, entry->idType->inputParams[i]->is_function, entry->idType->inputParams[i]->leftIndex, entry->idType->inputParams[i]->rightIndex, entry->idType->inputParams[i]->leftIndexName, entry->idType->inputParams[i]->rightIndexName, entry->idType->inputParams[i]->isDynamicLeft, entry->idType->inputParams[i]->isDynamicRight);
        }
        fprintf(fp,"Output Params:\n");
        for(int i = 0; i < entry->idType->numOutputParams; i++){
    fprintf(fp,"type:     %-10s, is_array:  %-10d, is_static_array: %-10d, is_function: %-10d, leftIndex:    %-10d, rightIndex: %-10d, leftIndexName: %-10s, rightIndexName: %-10s, isDynamicLeft: %-10d, isDynamicRight: %-10d \n", entry->idType->outputParams[i]->type, entry->idType->outputParams[i]->is_array, entry->idType->outputParams[i]->is_static_array, entry->idType->outputParams[i]->is_function, entry->idType->outputParams[i]->leftIndex, entry->idType->outputParams[i]->rightIndex, entry->idType->outputParams[i]->leftIndexName, entry->idType->outputParams[i]->rightIndexName, entry->idType->outputParams[i]->isDynamicLeft, entry->idType->outputParams[i]->isDynamicRight);
        }
    }
    fprintf(fp,"\n");
}

void printSymbolTable(st_table* stable){
    for(int i = 0; i < TABLE_SIZE; i++){
        if(stable->table->nodes[i] != NULL){
            st_entry* entry = (st_entry*)stable->table->nodes[i]->values;

            if(entry == NULL || strcmp(entry->var_name, "123") == 0){
                continue;
            }
            
            char* array_range = (char*)malloc(sizeof(char)*100);
            if(entry->idType->is_array){
                if(entry->idType->is_static_array){
                    sprintf(array_range, "[%d..%d]", entry->idType->leftIndex, entry->idType->rightIndex);
                }else if(entry->idType->isDynamicLeft && entry->idType->isDynamicRight){
                    sprintf(array_range, "[%s..%s]", entry->idType->leftIndexName, entry->idType->rightIndexName);
                }else if(entry->idType->isDynamicLeft){
                    sprintf(array_range, "[%s..%d]", entry->idType->leftIndexName, entry->idType->rightIndex);
                }else if(entry->idType->isDynamicRight){
                    sprintf(array_range, "[%d..%s]", entry->idType->leftIndex, entry->idType->rightIndexName);
                }
            }
            else{
                sprintf(array_range, "**");
            }

            char* scope_line_nos = (char*)malloc(sizeof(char)*100);
            if(stable->lineNoStart != -1){
                sprintf(scope_line_nos, "[%d..%d]", stable->lineNoStart, stable->lineNoEnd);
            }else{
                sprintf(scope_line_nos, "All Lines");
            }

            char* widthStr = (char*)malloc(sizeof(char)*100);
            if(entry->width != -1){
                sprintf(widthStr, "%d", entry->width);
            }else{
                sprintf(widthStr, "**");
            }

            char* offsetStr = (char*)malloc(sizeof(char)*100);
            if(entry->offset != -1){
                sprintf(offsetStr, "%d", entry->offset);
            }else{
                sprintf(offsetStr, "**");
            }

            char* arrayTypeString = (char*)malloc(sizeof(char)*100);
            if(entry->idType->is_array){
                if(entry->idType->is_static_array){
                    sprintf(arrayTypeString, "Static");
                }else{
                    sprintf(arrayTypeString, "Dynamic");
                }
            }else{
                sprintf(arrayTypeString, "**");
            }
            

            printf("%-15s  %-20s  %-20s  %-25s  %-20d  %-15s  %-8s  %-8s  %-8s  %-8d\n", entry->var_name, stable->moduleName, scope_line_nos, entry->idType->type, entry->idType->is_array, arrayTypeString, array_range, widthStr, offsetStr, entry->nestingLevel);
        }
    }
    //printf("\n");
    fflush(stdout);
}

void printSymbolTableArrays(st_table* stable){
    for(int i = 0; i < TABLE_SIZE; i++){
        if(stable->table->nodes[i] != NULL){
            st_entry* entry = (st_entry*)stable->table->nodes[i]->values;

            if(entry == NULL || strcmp(entry->var_name, "123") == 0){
                continue;
            }

            if(!entry->idType->is_array){
                continue;
            }
            
            char* array_range = (char*)malloc(sizeof(char)*100);
            if(entry->idType->is_static_array){
                sprintf(array_range, "[%d..%d]", entry->idType->leftIndex, entry->idType->rightIndex);
            }else if(entry->idType->isDynamicLeft && entry->idType->isDynamicRight){
                sprintf(array_range, "[%s..%s]", entry->idType->leftIndexName, entry->idType->rightIndexName);
            }else if(entry->idType->isDynamicLeft){
                sprintf(array_range, "[%s..%d]", entry->idType->leftIndexName, entry->idType->rightIndex);
            }else if(entry->idType->isDynamicRight){
                sprintf(array_range, "[%d..%s]", entry->idType->leftIndex, entry->idType->rightIndexName);
            }

            char* scope_line_nos = (char*)malloc(sizeof(char)*100);
            if(stable->lineNoStart != -1){
                sprintf(scope_line_nos, "[%d..%d]", stable->lineNoStart, stable->lineNoEnd);
            }else{
                sprintf(scope_line_nos, "All Lines");
            }

            char* arrayTypeString = (char*)malloc(sizeof(char)*100);
            if(entry->idType->is_static_array){
                sprintf(arrayTypeString, "Static");
            }else{
                sprintf(arrayTypeString, "Dynamic");
            }


            printf("scope_name(module_name): %s, Scope Lines: %s, Name: %s, ArrayType: %s, ArrayRange: %s, ElementType: %s\n", stable->moduleName, scope_line_nos, entry->var_name, arrayTypeString, array_range, entry->idType->type);
        }
    }
    printf("\n");
    fflush(stdout);
}

void printActivationRecord(st_table* stable){
    if(strcmp(stable->moduleName, stable->scopeName) == 0 && strcmp(stable->scopeName, "GLOBAL_SCOPE") != 0){
        printf("Function Name: %s, Activation Record Size: %d\n", stable->moduleName, stable->offset);
    }
    fflush(stdout);
}