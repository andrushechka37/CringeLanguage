#pragma once
#include "diff_project/tree.h"

#define SET_RIGHT_TYPE_VALUE(func)                    \
    if (func >= OP_FIG_C) {                           \
        set_type_value(*element, func, syntax_t);     \
    } else {                                          \
        set_type_value(*element, func, operator_t);   \
    }