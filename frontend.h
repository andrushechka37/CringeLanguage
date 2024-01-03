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
    char name[OP_NAME_LEN];
};

const int VARIABLE_COUNT = 20;

struct variables_info {
    variables table[VARIABLE_COUNT]; 
    int size = 0;
};

#define cur_char program[ip]
#define create_token(type, value, name) set_token(type, value, &(parsed_program[size]), name)

// dermo
#define create_right_token(func, name)        \
    if (func >= OP_FIG_C) {                   \
        create_token(syntax_t, func, name);   \
    } else {                                  \
        create_token(operator_t, func, name); \
    }  

#define IS_ELEM(element, type_of_node, value_of_node) (element->type == type_of_node && element->value.operator_info.op_number == value_of_node)

#define NULL_ELEM            \
    if (element == NULL) {   \
        return;              \
    }

int is_func_name(char name[]);
element_info * parse_str_lexically(size_t len);

void print_complex_expression(diff_tree_element * element);
void print_node(diff_tree_element * element);
int read_program(char file[] = "program.txt");

void print_to_file_c_program(diff_tree_element * element);