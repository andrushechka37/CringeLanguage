#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "diff_project/diff.h"
#include "recursive_down.h"
#include "frontend.h"

#define TYPE_OF_TOKEN parsed_program->tokens[token_num].type
#define VALUE_OF_TOKEN parsed_program->tokens[token_num].number
#define IS_ELEM(type, value) (TYPE_OF_TOKEN == type && VALUE_OF_TOKEN == value)
#define PRINT_CUR_SIT fprintf(plog,"   ip = %d, type: %d, value: %lg\n\n\n", token_num, TYPE_OF_TOKEN, VALUE_OF_TOKEN)

#define PRINT_REPORT(text)   \
    fprintf(plog, text);     \
    PRINT_CUR_SIT;

#define CHECK_BRACKET(name)                      \
    if (IS_ELEM(syntax_t, name)) {               \
        token_num++;                             \
    } else {                                     \
        fprintf(plog, "%s %d-line", __FUNCTION__, __LINE__);       \
        PRINT_REPORT("\n## error with bracket"); \
        token_num++;                             \
    }

static diff_tree_element * get_subexpression(token_array * parsed_program);

static diff_tree_element * get_number(token_array * parsed_program) {

    PRINT_REPORT("in get_number");
    if (TYPE_OF_TOKEN != value_t) {
        PRINT_REPORT("## not a number in get number")
    }

    int value = VALUE_OF_TOKEN;
    token_num++;
    return NUMBER_NODE(value);
}

static diff_tree_element * get_variable(token_array * parsed_program) {
    if (TYPE_OF_TOKEN == variable_t) {
        token_num++;
        PRINT_REPORT("in get_variable, got variable");
        return VAR;
    } else {
        PRINT_REPORT("in get_variable, call get_number:");
        return get_number(parsed_program);
    }
}

#define CREATE_LONG_OP_NODE(func)                                    \
    token_num++;                                                     \
                                                                     \
    CHECK_BRACKET(OP_ROUND_O);                                       \
    PRINT_REPORT("in get_long_op, call get_subexpression for sin:"); \
    diff_tree_element * val = func;                                  \
    CHECK_BRACKET(OP_ROUND_C);                                       \
                                                                     \
    PRINT_CUR_SIT;                                                   \
    return val;

static diff_tree_element * get_long_op(token_array * parsed_program) {
    if (IS_ELEM(operator_t, OP_SIN)) {
        CREATE_LONG_OP_NODE(SIN(get_subexpression(parsed_program)));
    } else if (IS_ELEM(operator_t, OP_COS)) {
        CREATE_LONG_OP_NODE(COS(get_subexpression(parsed_program)));
    } else {
        PRINT_REPORT("in get_long_op, call get_variable:");
        return get_variable(parsed_program);
    }
}

#undef CREATE_LONG_OP_NODE

static diff_tree_element * get_bracket(token_array * parsed_program) {
    if (IS_ELEM(syntax_t, OP_ROUND_O)) {
        CHECK_BRACKET(OP_ROUND_O)

        PRINT_REPORT("in get_bracket, call get_subexpression:");
        diff_tree_element * value = get_subexpression(parsed_program);

        CHECK_BRACKET(OP_ROUND_C);

        return value;
    } else {
        PRINT_REPORT("in get_bracket, call get_long_op:");
        return get_long_op(parsed_program);
    }
}

static diff_tree_element * get_pow(token_array * parsed_program) {

    PRINT_REPORT("in get_pow, call get_bracket:");

    diff_tree_element * value = get_bracket(parsed_program);

    while (IS_ELEM(operator_t, OP_POW)) {
        token_num++; // skip ^

        PRINT_REPORT("in get_pow, call get_bracket:");

        diff_tree_element * value2 = get_bracket(parsed_program);
        value = POW(value, value2);
    }
    return value;
}

static diff_tree_element * get_mul_or_div(token_array * parsed_program) { 
    PRINT_REPORT("in get_mul_or_div, call get_pow:");
 
    diff_tree_element * value = get_pow(parsed_program);
    while (IS_ELEM(operator_t, OP_MUL) || IS_ELEM(operator_t, OP_DIV)) {

        int op = VALUE_OF_TOKEN;
        token_num++; // skip op

        PRINT_REPORT("in get_mul_or_div, call get_pow:");

        diff_tree_element * value2 = get_pow(parsed_program);
        switch (op) {
        case OP_MUL:
            value =  MUL(value, value2);
            break;
        case OP_DIV:
            value = DIV(value, value2);
            break;
        default:
            PRINT_REPORT("## not * or / in get_mul_or_div");
            break;
        }
    }
    return value;
}

static diff_tree_element * get_subexpression(token_array * parsed_program) {
    PRINT_REPORT("in get_subexpression, call get_mul_or_div:");

    diff_tree_element * value = get_mul_or_div(parsed_program);
    while (IS_ELEM(operator_t, OP_ADD) || IS_ELEM(operator_t, OP_SUB)) {
        int op = VALUE_OF_TOKEN;
        token_num++; // skip op

        PRINT_REPORT("in get_subexpression, call get_mul_or_div:");

        diff_tree_element * value2 = get_mul_or_div(parsed_program);
        switch (op) {
        case OP_ADD:
            value = ADD(value, value2);
            break;
        case OP_SUB:
            value = SUB(value, value2);
            break;
        default:
            PRINT_REPORT("## not + or - in get_subexpression");
            break;
        }
    }
    return value;
}
#define CREATE_EXPRESSION_NODE(node)                                      \
    token_num++;                                                          \
    fprintf(plog, "in get expression, call get subexpression");           \
    PRINT_CUR_SIT;                                                        \
    right = get_subexpression(parsed_program);                            \
    CHECK_BRACKET(OP_END);                                                \
    return node;                                                          \
    break;    

diff_tree_element * get_expression(token_array * parsed_program) {
    PRINT_REPORT("in get expression, call get subexpression");

    diff_tree_element * value = get_subexpression(parsed_program); // is not ;
    if (!IS_ELEM(syntax_t, OP_END)) {
        diff_tree_element * right = NULL;
        switch ((operations)VALUE_OF_TOKEN) {
            case OP_LESS:
                CREATE_EXPRESSION_NODE(LESS(value, right));
            case OP_MORE:
                CREATE_EXPRESSION_NODE(MORE(value, right));
            case OP_EQUAL:
                CREATE_EXPRESSION_NODE(EQUAL(value, right));
            default:
                PRINT_REPORT("# trouble in get expression");
                break;
        }
    } else {
        CHECK_BRACKET(OP_END);
        return value;
    }

}

#undef CREATE_EXPRESSION_NODE

#define CREATE_OP_NODE(func)                                        \
    token_num++;                                                    \
    CHECK_BRACKET(OP_ROUND_O);                                      \
                                                                    \
    PRINT_REPORT("### in get_operator, call get expression for condition");           \
    diff_tree_element * condition = get_expression(parsed_program); \
    CHECK_BRACKET(OP_ROUND_C);                                      \
                                                                    \
    PRINT_REPORT("### in get_operator, call get operator for body");             \
    diff_tree_element * body = get_operator(parsed_program);        \
                                                                    \
    cur_node = func;

diff_tree_element * get_operator(token_array * parsed_program) {

    if (IS_ELEM(syntax_t, OP_FIG_O)) {
        PRINT_REPORT("call another get operator");
        CHECK_BRACKET(OP_FIG_O);
        diff_tree_element * value = get_operator(parsed_program);
        CHECK_BRACKET(OP_FIG_C);

        return value;
    } else {
        diff_tree_element * value = NULL;
        diff_tree_element * cur_node = NULL;
        diff_tree_element * prev_node = NULL;
        while (!IS_ELEM(syntax_t, OP_FIG_C)) {
            if (IS_ELEM(syntax_t, OP_IF)) { // value перезаписывается 

                CREATE_OP_NODE(IF(condition, body));

            } else if (IS_ELEM(syntax_t, OP_WHILE)) {

                CREATE_OP_NODE(WHILE(condition, body));

            } else {
                PRINT_REPORT("### int get operator call get expression")
                cur_node = get_expression(parsed_program);
            }
            prev_node = value;
            value = node_ctor(OP_END, syntax_t, cur_node, prev_node, NULL);
            fprintf(plog, "### %p is value ptr %p\n", value, value->left);
            PRINT_REPORT("blablka");
        }
        PRINT_REPORT("#### while ends");
        return value;
    }
}

#undef CREATE_OP_NODE

diff_tree_element * get_program(token_array * parsed_program) {

    plog = fopen("log_down.md", "w");
    IS_NULL_PTR(plog);
    PRINT_REPORT("in get program, call get operator:");

    diff_tree_element * value = get_operator(parsed_program);

    if (TYPE_OF_TOKEN == zero_t) {
        fclose(plog);
        return value;
    } else {
        PRINT_REPORT("## end of program is wrong")
        fclose(plog);
        return NULL;
    }
}
















