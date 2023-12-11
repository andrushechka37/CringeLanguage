#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "diff_project/diff.h"
#include "recursive_down.h"

static diff_tree_element * get_subexpression();

static diff_tree_element * get_number() {
    int value = 0;
    while ('0' <= program[ip] && program[ip] <= '9') {
        value = value * 10 + program[ip] - '0';
        ip++;
    }
    return NUMBER_NODE(value);
}

static diff_tree_element * get_variable() {
    if (program[ip] == 'x') {
        ip++;
        return node_ctor(0, variable_t, NULL, NULL, NULL);
    } else {
        return get_number();
    }
}
static diff_tree_element * get_long_op() {
    if(('a' <= program[ip] && program[ip] <= 'z' && program[ip] != 'x')) {
        char op[OP_NAME_LEN] = "";
        int i = 0;
        while ('a' <= program[ip] && program[ip] <= 'z') { // opname len check 
            op[i] = program[ip];                        // sscanf
            i++;    
            ip++;
        }
        if (strcmp(op, "sin") == 0) {
            ip++; // chech bracket
            diff_tree_element * sin = SIN(get_subexpression());
            ip++;// chech bracket
            return sin;
        } else if (strcmp(op, "cos") == 0) {
            ip++; // chech bracket
            diff_tree_element * cos = COS(get_subexpression());
            ip++;// che
            return cos;
        }
    } else {
        return get_variable();
    }
}

static diff_tree_element * get_bracket() {
    if (program[ip] == '(') {
        ip++;
        diff_tree_element * value = get_subexpression();
        if (program[ip] == ')') {
            ip++;
        } else {
            printf("bracket trouble\n");
        }
        return value;
    } else {
        return get_long_op();
    }
}

static diff_tree_element * get_pow() {
    diff_tree_element * value = get_bracket();
    while (program[ip] == '^') {
        char op = program[ip++];
        diff_tree_element * value2 = get_bracket();
        value =  POW(value, value2);
    }
    return value;
}

static diff_tree_element * get_mul_or_div() {   
    diff_tree_element * value = get_pow();
    while (program[ip] == '*' || program[ip] == '/') {
        char op = program[ip++];
        diff_tree_element * value2 = get_pow();
        switch (op)
        {
        case '*':
            value =  MUL(value, value2);
            break;
        case '/':
            value = DIV(value, value2);
            break;
        default:
            printf("errrrror\n");
            break;
        }
    }
    return value;
}

static diff_tree_element * get_subexpression() {
    diff_tree_element * value = get_mul_or_div();
    while (program[ip] == '+' || program[ip] == '-') {
        char op = program[ip++];
        diff_tree_element * value2 = get_mul_or_div();
        switch (op)
        {
        case '+':
            value = ADD(value, value2);
            break;
        case '-':
            value = SUB(value, value2);
            break;
        default:
            printf("errrrror");
            break;
        }
    }
    return value;
}

diff_tree_element * get_expression() {
    diff_tree_element * value = get_subexpression();
    if (program[ip] == '\0') {
        return value;
    } else {
        printf("syntax eroooooor");
    }
}



















