#pragma once
#include "diff_project/tree.h"

// TODO(DONE): What is an element? Rename.
struct token_info {
    types_of_node type;
    double number;
    char name[OP_NAME_LEN];
};

// TODO: Don't you want a generic array? 
struct tokens {
    token_info * tokens;
    int size;
};

struct variables {
    int value;
    char name[OP_NAME_LEN];
};

// TODO(DONE): Max variable count?
const int SYMBOL_TABLE_MAX_CAPACITY = 20;

struct variables_info {
    variables table[SYMBOL_TABLE_MAX_CAPACITY]; 
    int size = 0;
};

// TODO(DONE): Why is it in the header? "ip" -- "instruction pointer", not index


// TODO(DONE): It shouldn't be here. Also, don't be misleading, write macros from CAPITAL letter.
//       Same goes for previous macros and ones after that.

//#define CREATE_TOKEN(type, value, name) set_token(type, value, &(token_array[size]), name)

// dermo. TODO: OP_FIG_C??

// I you don't have enough letter, just ask, I've got plenty, take this:
// abcdefghijklmopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz
// ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ
// abcdfghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz
// ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ
// abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz
// ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ
// abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz
// ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ
// bcdefghijkmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz
// ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ
// abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz
// ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ
// abcdfghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz
// ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ
// abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz
// ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ
// acdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz
// ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ
// abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz
// ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ

// You can use them to create more explicit and understandable names! Take care.
// eblan


// TODO: Why is it a macro? It can be a function.------------------------------------------------------
#define create_right_token(func, name)        \
    if (func >= OP_FIG_C) {                   \
        CREATE_TOKEN(syntax_class, func, name);   \
    } else {                                  \
        CREATE_TOKEN(operator_class, func, name); \
    }  

#define NULL_ELEM            \
    if (element == NULL) {   \
        return;              \
    }


#define IS_ELEM(element, type_of_node, value_of_node) (element->type == type_of_node && \
                                                       element->value.operator_info.op_number == value_of_node)

int is_func_name(char name[]);
token_info * parse_str_lexically(size_t len);

void print_complex_expression(diff_tree_element * element);
void print_node(diff_tree_element * element);

void print_to_file_c_program(diff_tree_element * element);
