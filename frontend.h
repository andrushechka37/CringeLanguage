#pragma once
#include "diff_project/tree.h"

struct element_info {
    types_of_node type;
    double number;
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
#define create_token(type, value) set_token(type, value, &(parsed_program[size]))

// dermo
#define create_right_token(func)        \
    if (func >= OP_FIG_C) {             \
        create_token(syntax_t, func);   \
    } else {                            \
        create_token(operator_t, func); \
    }  

int is_func_name(char name[]);
