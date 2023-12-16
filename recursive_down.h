#pragma once 
#include "diff_project/tree.h"
#include "frontend.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++17-extensions"

inline int token_num = 0;
inline FILE * plog = NULL;

#pragma clang diagnostic pop
diff_tree_element * get_expression(token_array * parsed_program);


#define print_log(text, spec) fprintf(plog, text, spec)
