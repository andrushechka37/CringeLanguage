#include <stdlib.h>
#include <stdio.h>
#include "diff_project/diff.h"
#include "recursive_down.h"
#include "diff_project/deff_dump.h"
#include "frontend.h"
#include <ctype.h>

#include <math.h>
#include <string.h>

static int get_size_of_file(FILE * file);

static void set_token(types_of_node type, double value, element_info * elem);

static void get_long_name(int * ip, char * op);
static int get_number(int * ip);

token_array * parse_str_lexically(int len, token_array * parsed_program) {
    parsed_program->tokens = (element_info *) calloc(len, sizeof(element_info));

    int ip = 0;
    int size = 0;

    while (cur_char != '\0') {
        if (cur_char == ' ' || cur_char == '\n') {
            ip++;
            continue;
        }

        if (isdigit(cur_char) != 0) {

            create_token(value_t, get_number(&ip));  // experiment with \n

        } else if (isalpha(cur_char) != 0) {  // numbers are prohibited in names of funcs and variables

            char op[OP_NAME_LEN] = "";
            get_long_name(&ip, op);

            if (get_op_number_long_op(op) != -1) {
                create_token(operator_t, get_op_number_long_op(op));
            } else {
                // printf("peremenochka or x\n");
                // continue;
                create_token(variable_t, 66);
            }

        } else {
            int number = get_op_number_single_op(cur_char);

            if (number == -1) {
                printf("jknfg");
                ip++;
            } else {
                if (number >= OP_FIG_C) {    // dermo
                    create_token(syntax_t, number);
                } else {
                    create_token(operator_t, number);
                }
                ip++;
            }
        }
        printf("%d-type %lg-value\n", parsed_program->tokens[size].type, parsed_program->tokens[size].number);
        size++;
    }
    parsed_program->tokens[size].type = zero_t;
}


int main(void) {
    int len = read_program();
    token_array parsed_program = {};
    parse_str_lexically(len, &parsed_program);
    int i = 0;
    printf("--------------------------\n");
    while(i < 10) {
        printf("%d-type %lg-value\n", parsed_program.tokens[i].type, parsed_program.tokens[i].number);
        i++;
    }
    diff_tree_element * tree = get_expression(&parsed_program);
    set_parents(tree, tree);
    tree_visualize(tree);
    tree_dtor(&tree);
} 






#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static int get_size_of_file(FILE * file) {
    struct stat buff;
    fstat(fileno(file), &buff);
    return buff.st_size;
}

int read_program(char file[]) {
    FILE * pfile = fopen(file, "r");

    int len = get_size_of_file(pfile);
    program = (char *) calloc(len, sizeof(char));

    fread(program, sizeof(char), len, pfile);
    fclose(pfile);
    return len;
}

static void set_token(types_of_node type, double value, element_info * elem) {
    elem->number = value;
    elem->type = type;
}

static void get_long_name(int * ip, char * op) {
        int i = 0;
        while ('a' <= program[*ip] && program[*ip] <= 'z') { // opname len check 
            op[i] = program[*ip];                        // sscanf
            i++;    
            (*ip)++;
        }
}

static int get_number(int * ip) {    // sscanf
    int value = 0;
    while ('0' <= program[*ip] && program[*ip] <= '9') {
        value = value * 10 + program[*ip] - '0';
        (*ip)++;
    }
    return value;
}
























// static bool check_symbol(char symbol, FILE * pfile);
// static int set_type_and_value(double value, types_of_node type, diff_tree_element * element);
// static int get_op_arg_number(operations op);

// op_names_numbers_t op_names_numbers[OP_COUNT] = {
//         {OP_ADD,     "+",       2},
//         {OP_SUB,     "-",       2},
//         {OP_MUL,     "*",       2},
//         {OP_DIV,     "/",       2},
//         {OP_SQRT, "sqrt",       1},
//         {OP_SIN,   "sin",       1},
//         {OP_COS,   "cos",       1},
//         {OP_POW,     "^",       2},
//         {OP_FIG_C,   "}",       0},
//         {OP_FIG_O,   "{",       0},
//         {OP_ROUND_O, "(",       0},
//         {OP_ROUND_O, ")",       0},
// };



// int get_op_number_long_op(char name[]) { // do it faster with hashes instead of strcmp
//     int i = 0;
//     while (strcmp(op_names_numbers[i].name, name)) {
//         i++;
//         if (i > FUNCS_COUNT) {
//             return -1;
//         }
//     }
//     return op_names_numbers[i].number;
// }

// diff_tree_element * node_ctor(double value, types_of_node type, diff_tree_element * left,
//                               diff_tree_element * right, diff_tree_element * parent) {

//     diff_tree_element * element = (diff_tree_element *) calloc(1, sizeof(diff_tree_element));
//     element->type = type;
//     switch (type)
//     {
//     case value_t:
//         ELEM_DOUBLE = value;
//         break;
//     case operator_t:
//         ELEM_OP_NUM = (operations) value;
//         element->value.operator_info.arg_quantity = get_op_arg_number((operations) value);
//         break;
//     }
//     element->left = left;
//     element->right = right;
//     element->parent = parent;
//     return element;
    
// }



// const char * get_op_symbol(int op_num) { 
//     int i = 0;
//     while (op_num != op_names_numbers[i].number) i++;      // switch case
//     return op_names_numbers[i].name;
// }

// int get_op_number_single_op(char name) { // do it faster with hashes instead of strcmp
//     int i = 0;
//     while (name != op_names_numbers[i].name[0]) {
//         i++;
//         if (i > FUNCS_COUNT) {
//             return -1;
//         }
//     }
//     return op_names_numbers[i].number;
// }




// int tree_verify(diff_tree_element * element) {
//     if (element == NULL) {
//         return 1;
//     }
//     tree_verify(element->left);

//     switch (element->type) {
//     case value_t:
//         if (element->left != NULL || element->right != NULL) {   // if number node has left and right NULL children
//             printf("%p number does not have all nulls", element);
//             error_status = 1;
//         }
//         break;
//     case operator_t:
//         if (ELEM_OP_ARG == 1) { // if operator has one argument
//             if (element->left != NULL || element->right == NULL) {
//                 printf("%p op does not have all numbers 1", element);
//                 error_status = 1;
//             }
//         } else {
//             if (element->left == NULL || element->right == NULL) { // if operator has two arguments
//                 printf("%p op does not have all numbers", element);
//                 error_status = 1;
//             }
//         }
    
//     default:
//         break;
//     }

//     if (element->left != NULL && element->right != NULL) {   // if parents clild and child's parent are the same
//         if(element->left->parent != element || element->right->parent != element) {
//         printf("%p - left parent %p - right parent %p - elemen", element->left->parent, element->right->parent, element);
//         error_status = 1;
//         }
//     }
//     tree_verify(element->right);
//     if (error_status == 1) {
//         return 1;
//     }
//     return 0;
// }

// bool op_priority(double op1, double op2) {
//     if (((int)op1 & OP_PRIORITY_MASK) < ((int)op2 & OP_PRIORITY_MASK)) {
//         return 1;
//     }
//     return 0;
// }




// void tree_dtor(elem_ptr * root) {
//     if (*root == NULL) {     
//         return;
//     }

//     tree_dtor(&(*root)->left);

//     (*root)->parent = NULL;
//     set_type_and_value(0, (types_of_node)0, *root);

//     tree_dtor(&(*root)->right);

//     (*root)->right = NULL;
//     (*root)->left = NULL;
//     free(*root);
//     (*root) = NULL;
//     return;
// }

// double tree_eval(diff_tree_element * element, double x_value) {
//     if (element->type == value_t) {
//         return element->value.number;
//     }
//     if (element->type == variable_t) {
//         return x_value;
//     }

//     double right_value = 0;
//     double  left_value = 0;

//     if (ELEM_OP_ARG == 2) {
//         left_value = tree_eval(element->left, x_value);
//     }
//     right_value = tree_eval(element->right, x_value);

//     switch (ELEM_OP_NUM) {
//         case OP_ADD:
//             return left_value + right_value;
//         case OP_SUB:
//             return left_value - right_value;
//         case OP_MUL:
//             return left_value * right_value;
//         case OP_DIV:
//             if (right_value != 0) {
//                 return left_value / right_value;
//             } else {
//                 printf("divide on zero, error\n");
//                 return 0;
//             }
//         case OP_SIN:
//             return sin(right_value);
//         case OP_COS:
//             return cos(right_value);
//         case OP_POW:
//             return pow(left_value, right_value);
//         case OP_SQRT:
//             return sqrt(right_value);
//         default:
//             printf("there is no that operation, error %lf\n", element->value);
//             return 0;
//     }
// }

// static int get_op_arg_number(operations op) {
//     int i = 0;
//     while (op != op_names_numbers[i].number) i++;
//     return op_names_numbers[i].arg_quantity;
// }

// static int set_type_and_value(double value, types_of_node type, diff_tree_element * element) { // null ptr check
//     IS_NULL_PTR(element);
//     if (type == value_t) {
//         element->value.number = value;
//     } else if (type == operator_t) {
//         ELEM_OP_NUM = (operations)value;
//         ELEM_OP_ARG = get_op_arg_number((operations)value);

//     }
//     element->type = type;
//     return 0;
// }