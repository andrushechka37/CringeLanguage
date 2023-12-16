#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "diff_project/diff.h"
#include "recursive_down.h"
#include "frontend.h"

#define TYPE_OF_TOKEN parsed_program->tokens[token_num].type
#define VALUE_OF_ELEM parsed_program->tokens[token_num].number
#define IS_ELEM(type, value) TYPE_OF_TOKEN == type && VALUE_OF_ELEM == value

#define PRINT_CUR_SIT fprintf(plog,"   ip = %d, type: %d, value: %lg\n\n\n", token_num, TYPE_OF_TOKEN, VALUE_OF_ELEM)

#define PRINT_REPORT(text)   \
    fprintf(plog, text);     \
    PRINT_CUR_SIT;


static diff_tree_element * get_subexpression(token_array * parsed_program);

static diff_tree_element * get_number(token_array * parsed_program) {

    PRINT_REPORT("in get_number");
    if (TYPE_OF_TOKEN != value_t) {
        PRINT_REPORT("## not a number in get number")
    }

    int value = VALUE_OF_ELEM;
    token_num++;
    return NUMBER_NODE(value);
}

static diff_tree_element * get_variable(token_array * parsed_program) {
    if (TYPE_OF_TOKEN == variable_t) {
        token_num++;
        PRINT_REPORT("in get_variable, got number");
        return VAR;
    } else {
        PRINT_REPORT("in get_variable, call get_number:");
        return get_number(parsed_program);
    }
}
static diff_tree_element * get_long_op(token_array * parsed_program) {

    if (IS_ELEM(operator_t, OP_SIN)) {
        token_num++; // skip sin
        token_num++; // skip (

        PRINT_REPORT("in get_long_op, call get_subexpression for sin:");

        diff_tree_element * sin = SIN(get_subexpression(parsed_program));
        token_num++;// skip )
        PRINT_CUR_SIT; 
        return sin;
    } else if (TYPE_OF_TOKEN == operator_t && VALUE_OF_ELEM == OP_COS) {
        token_num++; // skip cos
        token_num++; // skip (

        PRINT_REPORT("in get_long_op, call get_subexpression for cos:");

        diff_tree_element * cos = COS(get_subexpression(parsed_program));
        token_num++; // skip )
        return cos;
    } else {
        PRINT_REPORT("in get_long_op, call get_variable:");
        return get_variable(parsed_program);
    }
}

static diff_tree_element * get_bracket(token_array * parsed_program) {
    if (IS_ELEM(syntax_t, OP_ROUND_O)) {
        token_num++; // skip (

        PRINT_REPORT("in get_bracket, call get_subexpression:");

        diff_tree_element * value = get_subexpression(parsed_program);

        if (IS_ELEM(syntax_t, OP_ROUND_C)) {
            token_num++; // skip )
        } else {
            PRINT_REPORT("bracket trouble with ) in get_bracket\n")
        }

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

        int op = VALUE_OF_ELEM;
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
        int op = VALUE_OF_ELEM;
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

diff_tree_element * get_expression(token_array * parsed_program) {
    PRINT_REPORT("in get expression, call get subexpression");

    diff_tree_element * value = get_subexpression(parsed_program);
    if (IS_ELEM(syntax_t, OP_LESS) || IS_ELEM(syntax_t, OP_MORE) || IS_ELEM(syntax_t, OP_EQUAL)) {
        diff_tree_element * right = NULL;
        switch ((operations)VALUE_OF_ELEM) {
        case OP_LESS:
            token_num++; // skip <
            fprintf(plog, "in get expression, call get subexpression for less");
            PRINT_CUR_SIT;
            right = get_subexpression(parsed_program);
            return LESS(value, right);
            break;
        case OP_MORE:
            token_num++; // skip >

            fprintf(plog, "in get expression, call get subexpression for more");
            PRINT_CUR_SIT;

            right = get_subexpression(parsed_program);
            return MORE(value, right);
            break;
        case OP_EQUAL:
            token_num++; // skip =
            fprintf(plog, "in get expression, call get subexpression for equal");
            PRINT_CUR_SIT;

            right = get_subexpression(parsed_program);
            return EQUAL(value, right);
            break;
        
        default:
            fprintf(plog, "# trouble in get expression");
            PRINT_CUR_SIT;
            break;
        }
    } else {
        return value;
    }

}



diff_tree_element * get_operator(token_array * parsed_program) {

    fprintf(plog, "in get_operator, call");

    if (IS_ELEM(syntax_t, OP_FIG_O)) {
        token_num++; // skip {
        if (IS_ELEM(syntax_t, OP_IF)) {

            token_num++; // skip if

            token_num++; // skip (

            fprintf(plog, "in get_operator, call get expression for if");
            PRINT_CUR_SIT;

            diff_tree_element * condition = get_expression(parsed_program);
            token_num++; // skip )

            fprintf(plog, "in get_operator, call get operator for if");
            PRINT_CUR_SIT;

            diff_tree_element * body = get_operator(parsed_program);
            token_num++; // skip }

            return IF(condition, body);
        } else if (IS_ELEM(syntax_t, OP_WHILE)) {
            token_num++; // skip if

            token_num++; // skip (

            fprintf(plog, "in get_operator, call get expression for while");
            PRINT_CUR_SIT;

            diff_tree_element * condition = get_expression(parsed_program);
            token_num++; // skip )

            fprintf(plog, "in get_operator, call get operator for while");
            PRINT_CUR_SIT;

            diff_tree_element * body = get_operator(parsed_program);
            token_num++; // skip }

            return WHILE(condition, body);
        } else {
            fprintf(plog, "in get_operator, call get expression for E");
            PRINT_CUR_SIT;

            diff_tree_element * body = get_expression(parsed_program);  // maybe while != }
            token_num++; // skip }
            return body;
        }
    } else {
        fprintf(plog, "in get_operator, call get expression");
        PRINT_CUR_SIT;

        return get_expression(parsed_program);
    }
}

diff_tree_element * get_program(token_array * parsed_program) {

    plog = fopen("log_down.md", "w");
    fprintf(plog, "1 in get_expression, call get_subexpression:");
    PRINT_CUR_SIT;

    diff_tree_element * value = get_operator(parsed_program);

    if (TYPE_OF_TOKEN == zero_t) {
        fclose(plog);
        return value;
    } else {
        fprintf(plog, "end of program is wrong");
        fclose(plog);
        return NULL;
    }
}
















