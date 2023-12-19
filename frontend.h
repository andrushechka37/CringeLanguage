#pragma once
#include "diff_project/tree.h"

struct element_info {
    types_of_node type;
    double number;
    char name[OP_NAME_LEN];
};
struct token_array {
    element_info * tokens;
    int size;
};

struct variables {
    int value;
    char * name;
};



int read_program(char file[] = "program.txt");

#define cur_char program[ip]
#define create_token(type, value, name) set_token(type, value, &(parsed_program[size]), name)

// dermo
#define create_right_token(func, name)        \
    if (func >= OP_FIG_C) {                   \
        create_token(syntax_t, func, name);   \
    } else {                                  \
        create_token(operator_t, func, name); \
    }  

int is_func_name(char name[]);
