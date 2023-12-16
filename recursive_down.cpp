#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "diff_project/diff.h"
#include "recursive_down.h"
#include "frontend.h"

#define TYPE_OF_ELEM parsed_program->tokens[token_num].type
#define VALUE_OF_ELEM parsed_program->tokens[token_num].number
#define IS_ELEM(type, value) TYPE_OF_ELEM == type && VALUE_OF_ELEM == value

#define PRINT_CUR_SIT fprintf(plog,"   ip = %d, type: %d, value: %lg\n\n\n", token_num, TYPE_OF_ELEM, VALUE_OF_ELEM)

static diff_tree_element * get_subexpression(token_array * parsed_program);

static diff_tree_element * get_number(token_array * parsed_program) {
    fprintf(plog, "in get_number");
    PRINT_CUR_SIT; 
    if (TYPE_OF_ELEM != value_t) {
        fprintf(plog, "!!!!!!!!!! not a number in getnaumber");
        PRINT_CUR_SIT;
    }
    int value = VALUE_OF_ELEM;
    token_num++;
    return NUMBER_NODE(value);
}

static diff_tree_element * get_variable(token_array * parsed_program) {
    if (TYPE_OF_ELEM == variable_t) {
        token_num++;
        return node_ctor(0, variable_t, NULL, NULL, NULL);
    } else {
        fprintf(plog, "in get_variable, call get_number:");
        PRINT_CUR_SIT; 
        return get_number(parsed_program);
    }
}

static diff_tree_element * get_long_op(token_array * parsed_program) {
    if (TYPE_OF_ELEM == operator_t && VALUE_OF_ELEM == OP_SIN) {
        fprintf(plog, "in get_long_op, call get_subexpression for sin:");
        PRINT_CUR_SIT; 
        token_num++; // chech bracket
        token_num++;
        PRINT_CUR_SIT; 
        diff_tree_element * sin = SIN(get_subexpression(parsed_program));
        token_num++;// chech bracket
        PRINT_CUR_SIT; 
        return sin;
    } else if (TYPE_OF_ELEM == operator_t && VALUE_OF_ELEM == OP_COS) {
        fprintf(plog, "in get_long_op, call get_subexpression for cos:");
        PRINT_CUR_SIT; 
        token_num++; // chech bracket
        token_num++;
        diff_tree_element * cos = COS(get_subexpression(parsed_program));
        token_num++;// che
        return cos;
    } else {
        fprintf(plog, "in get_long_op, call get_variable:");
        PRINT_CUR_SIT; 
        return get_variable(parsed_program);
    }
}

static diff_tree_element * get_bracket(token_array * parsed_program) {
    if (TYPE_OF_ELEM == syntax_t && VALUE_OF_ELEM == OP_ROUND_O) {
        token_num++;
        fprintf(plog, "in get_bracket, call get_subexpression:");
        PRINT_CUR_SIT; 
        diff_tree_element * value = get_subexpression(parsed_program);
        if (TYPE_OF_ELEM == syntax_t && VALUE_OF_ELEM == OP_ROUND_C) {
            token_num++;
        } else {
            fprintf(plog,"bracket trouble\n");
            PRINT_CUR_SIT;
        }
        return value;
    } else {
        fprintf(plog, "in get_bracket, call get_long_op:");
        PRINT_CUR_SIT; 
        return get_long_op(parsed_program);
    }
}

static diff_tree_element * get_pow(token_array * parsed_program) {
    fprintf(plog, "in get_pow, call get_bracket:");
    PRINT_CUR_SIT;  
    diff_tree_element * value = get_bracket(parsed_program);
    while (TYPE_OF_ELEM == operator_t && VALUE_OF_ELEM == OP_POW) {
        token_num++;
        fprintf(plog, "in get_pow, call get_bracket:");
        PRINT_CUR_SIT; 
        diff_tree_element * value2 = get_bracket(parsed_program);
        value =  POW(value, value2);
    }
    return value;
}

static diff_tree_element * get_mul_or_div(token_array * parsed_program) { 
    fprintf(plog, "in get_mul_or_div, call get_pow:");
    PRINT_CUR_SIT;  
    diff_tree_element * value = get_pow(parsed_program);
    while (TYPE_OF_ELEM == operator_t && (VALUE_OF_ELEM == OP_MUL || VALUE_OF_ELEM == OP_DIV)) {
        int op = VALUE_OF_ELEM;
        token_num++;
        fprintf(plog, "in get_mul_or_div, call get_pow:");
        PRINT_CUR_SIT; 
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
            PRINT_CUR_SIT;
            break;
        }
    }
    return value;
}

static diff_tree_element * get_subexpression(token_array * parsed_program) {
    fprintf(plog, "in get_subexpression, call get_mul_or_div:");
    PRINT_CUR_SIT;
    diff_tree_element * value = get_mul_or_div(parsed_program);
    while (TYPE_OF_ELEM == operator_t && (VALUE_OF_ELEM == OP_ADD || VALUE_OF_ELEM == OP_SUB)) {// cut type of token
        int op = VALUE_OF_ELEM;
        token_num++; // token !!!!!!!!!!!!!!!!!!!!!!!!!!!!1

        fprintf(plog, "in get_subexpression, call get_mul_or_div:");
        PRINT_CUR_SIT;// verb

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
            PRINT_CUR_SIT;
            break;
        }
    }
    return value;
}

diff_tree_element * get_expression(token_array * parsed_program) {
    plog = fopen("log_down.txt", "w");
    fprintf(plog, "in get_expression, call get_subexpression:");
    PRINT_CUR_SIT;
    diff_tree_element * value = get_subexpression(parsed_program);
    fclose(plog);
    if ((TYPE_OF_ELEM) == zero_t) {
        return value;
    } else {
        fprintf(plog, "end of program is wrong");
    }
}

// diff_tree_element * get_expression(token_array * parsed_program) {
//     diff_tree_element * value = get_subexpression(parsed_program);
//     if (IS_ELEM(syntax_t, OP_LESS) || IS_ELEM(syntax_t, OP_MORE) || IS_ELEM(syntax_t, OP_EQUAL)) {
//         diff_tree_element * right = NULL;
//         switch ((operations)VALUE_OF_ELEM) {
//         case OP_LESS:
//             token_num++; // skip <
//             right = get_subexpression(parsed_program);
//             return LESS(value, right);
//             break;
//         case OP_MORE:
//             token_num++; // skip >
//             right = get_subexpression(parsed_program);
//             return MORE(value, right);
//             break;
//         case OP_EQUAL:
//             token_num++; // skip =
//             right = get_subexpression(parsed_program);
//             return EQUAL(value, right);
//             break;
        
//         default:
//             ///////////////////////// errororrororoororor
//             break;
//         }
//     } else {
//         return value;
//     }

// }



// diff_tree_element * get_operator(token_array * parsed_program) {

//     fprintf(plog, "in get_operator, call");

//     if (IS_ELEM(syntax_t, OP_FIG_O)) {
//         token_num++; // skip {
//         if (IS_ELEM(syntax_t, OP_IF)) {

//             token_num++; // skip if

//             token_num++; // skip (
//             diff_tree_element * condition = get_expression(parsed_program);
//             token_num++; // skip )

//             diff_tree_element * body = get_operator(parsed_program);
//             token_num++; // skip }

//             return IF(condition, body);
//         } else if (IS_ELEM(syntax_t, OP_WHILE)) {
//             token_num++; // skip if

//             token_num++; // skip (
//             diff_tree_element * condition = get_expression(parsed_program);
//             token_num++; // skip )

//             diff_tree_element * body = get_operator(parsed_program);
//             token_num++; // skip }

//             return WHILE(condition, body);
//         } else {
//             diff_tree_element * body = get_expression(parsed_program);  // maybe while != }
//             token_num++; // skip }
//             return body;
//         }
//     } else {
//         return get_expression(parsed_program);
//     }
// }

// diff_tree_element * get_program(token_array * parsed_program) {

//     plog = fopen("log_down.txt", "w");
//     fprintf(plog, "in get_expression, call get_subexpression:");
//     PRINT_CUR_SIT;

//     diff_tree_element * value = get_operator(parsed_program);

//     if (TYPE_OF_ELEM == zero_t) {
//         fclose(plog);
//         return value;
//     } else {
//         fprintf(plog, "end of program is wrong");
//         fclose(plog);
//         return NULL;
//     }
// }
















