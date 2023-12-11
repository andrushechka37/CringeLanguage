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


#define IF #if
#define WHILE #while


const char FIGURE_BRACKET_OPEN = '{';
const char FIGURE_BRACKET_CLOSE = '}';
const char BRACKET_OPEN = '(';
const char BRACKET_CLOSE = ')';