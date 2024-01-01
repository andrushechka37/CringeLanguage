#pragma once 
#include "diff_project/tree.h"
#include "frontend.h"


diff_tree_element * get_program(token_array * parsed_program);

#define print_log(text, spec) fprintf(plog, text, spec)

#define TYPE_OF_TOKEN parsed_program->tokens[token_num].type
#define VALUE_OF_TOKEN parsed_program->tokens[token_num].number



#define PRINT_CUR_SIT fprintf(plog,"   ip = %d, type: %d, value: %lg\n\n\n", token_num, TYPE_OF_TOKEN, VALUE_OF_TOKEN)

#define IS_TOKEN(type, value) (TYPE_OF_TOKEN == type && VALUE_OF_TOKEN == value)

#define PRINT_REPORT(text)   \
    fprintf(plog, text);     \
    PRINT_CUR_SIT;

#define CHECK_BRACKET(name)                                        \
    if (IS_TOKEN(syntax_t, name)) {                                \
        token_num++;                                               \
    } else {                                                       \
        fprintf(plog, "%s %d-line", __FUNCTION__, __LINE__);       \
        PRINT_REPORT("\n## error with bracket");                   \
        token_num++;                                               \
    }
