#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "diff_project/diff.h"
#include "recursive_down.h"
#include "frontend.h"

#define type_of_elem parsed_program->tokens[elem_num].type
#define value_of_elem parsed_program->tokens[elem_num].number

#define cur_sit fprintf(plog,"   ip = %d, type: %d, value: %lg\n\n\n", elem_num, type_of_elem, value_of_elem)

static diff_tree_element * get_subexpression(token_array * parsed_program);

static diff_tree_element * get_number(token_array * parsed_program) {
    fprintf(plog, "in get_number");
    cur_sit; 
    if (type_of_elem != value_t) {
        fprintf(plog, "!!!!!!!!!! not a number in getnaumber");
        cur_sit;
    }
    int value = value_of_elem;
    elem_num++;
    return NUMBER_NODE(value);
}

static diff_tree_element * get_variable(token_array * parsed_program) {
    if (type_of_elem == variable_t) {
        elem_num++;
        return node_ctor(0, variable_t, NULL, NULL, NULL);
    } else {
        fprintf(plog, "in get_variable, call get_number:");
        cur_sit; 
        return get_number(parsed_program);
    }
}

static diff_tree_element * get_long_op(token_array * parsed_program) {
    if (type_of_elem == operator_t && value_of_elem == OP_SIN) {
        fprintf(plog, "in get_long_op, call get_subexpression for sin:");
        cur_sit; 
        elem_num++; // chech bracket
        elem_num++;
        cur_sit; 
        diff_tree_element * sin = SIN(get_subexpression(parsed_program));
        elem_num++;// chech bracket
        cur_sit; 
        return sin;
    } else if (type_of_elem == operator_t && value_of_elem == OP_COS) {
        fprintf(plog, "in get_long_op, call get_subexpression for cos:");
        cur_sit; 
        elem_num++; // chech bracket
        elem_num++;
        diff_tree_element * cos = COS(get_subexpression(parsed_program));
        elem_num++;// che
        return cos;
    } else {
        fprintf(plog, "in get_long_op, call get_variable:");
        cur_sit; 
        return get_variable(parsed_program);
    }
}

static diff_tree_element * get_bracket(token_array * parsed_program) {
    if (type_of_elem == syntax_t && value_of_elem == BRACKET_OPEN) {
        elem_num++;
        fprintf(plog, "in get_bracket, call get_subexpression:");
        cur_sit; 
        diff_tree_element * value = get_subexpression(parsed_program);
        if (type_of_elem == syntax_t && value_of_elem == BRACKET_CLOSE) {
            elem_num++;
        } else {
            fprintf(plog,"bracket trouble\n");
            cur_sit;
        }
        return value;
    } else {
        fprintf(plog, "in get_bracket, call get_long_op:");
        cur_sit; 
        return get_long_op(parsed_program);
    }
}

static diff_tree_element * get_pow(token_array * parsed_program) {
    fprintf(plog, "in get_pow, call get_bracket:");
    cur_sit;  
    diff_tree_element * value = get_bracket(parsed_program);
    while (type_of_elem == operator_t && value_of_elem == OP_POW) {
        elem_num++;
        fprintf(plog, "in get_pow, call get_bracket:");
        cur_sit; 
        diff_tree_element * value2 = get_bracket(parsed_program);
        value =  POW(value, value2);
    }
    return value;
}

static diff_tree_element * get_mul_or_div(token_array * parsed_program) { 
    fprintf(plog, "in get_mul_or_div, call get_pow:");
    cur_sit;  
    diff_tree_element * value = get_pow(parsed_program);
    while (type_of_elem == operator_t && (value_of_elem == OP_MUL || value_of_elem == OP_DIV)) {
        int op = value_of_elem;
        elem_num++;
        fprintf(plog, "in get_mul_or_div, call get_pow:");
        cur_sit; 
        diff_tree_element * value2 = get_pow(parsed_program);
        switch (op)
        {
        case OP_MUL:
            value =  MUL(value, value2);
            break;
        case OP_DIV:
            value = DIV(value, value2);
            break;
        default:
            fprintf(plog, "not * or / in get_mul_or_div, op is <%c>", op);
            cur_sit;
            break;
        }
    }
    return value;
}

static diff_tree_element * get_subexpression(token_array * parsed_program) {
    fprintf(plog, "in get_subexpression, call get_mul_or_div:");
    cur_sit;
    diff_tree_element * value = get_mul_or_div(parsed_program);
    while (type_of_elem == operator_t && (value_of_elem == OP_ADD || value_of_elem == OP_SUB)) {
        int op = value_of_elem;
        elem_num++;
        fprintf(plog, "in get_subexpression, call get_mul_or_div:");
        cur_sit;
        diff_tree_element * value2 = get_mul_or_div(parsed_program);
        switch (op)
        {
        case OP_ADD:
            value = ADD(value, value2);
            break;
        case OP_SUB:
            value = SUB(value, value2);
            break;
        default:
            fprintf(plog, "not + or - in get_subexpression, op is %c", op);
            cur_sit;
            break;
        }
    }
    return value;
}

diff_tree_element * get_expression(token_array * parsed_program) {
    plog = fopen("log_down.txt", "w");
    fprintf(plog, "in get_expression, call get_subexpression:");
    cur_sit;
    diff_tree_element * value = get_subexpression(parsed_program);
    fclose(plog);
    if (type_of_elem == zero_t) {
        return value;
    } else {
        fprintf(plog, "end of program is wrong");
    }
}



















