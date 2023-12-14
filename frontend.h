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

#define IF #if
#define WHILE #while


const char FIGURE_BRACKET_OPEN = '{';
const char FIGURE_BRACKET_CLOSE = '}';
const char BRACKET_OPEN = '(';
const char BRACKET_CLOSE = ')';



int read_program(char file[] = "program.txt");

#define cur_char program[ip]
#define create_token(type, value) set_token(type, value, &(parsed_program->tokens[size]))

int get_op_number_long_op(char name[]);


