#pragma once
#include "diff_project/tree.h"

#define SET_RIGHT_TYPE_VALUE(func)                    \
    if (func >= OP_FIG_C) {                           \
        set_type_value(*element, func, syntax_class);     \
    } else {                                          \
        set_type_value(*element, func, operator_class);   \
    }

diff_tree_element * read_tree();
int build_tree(elem_ptr * element, FILE * in_file, elem_ptr * parent);