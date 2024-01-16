#include <stdlib.h>
#include <stdio.h>
#include "diff_project/diff.h"
#include "diff_project/tree.h"
#include "recursive_down.h"
#include "diff_project/deff_dump.h"
#include "frontend.h"
#include "backend.h"
#include <ctype.h>

#include <math.h>
#include <string.h>


// TODO: copypast of funcs of frontend, make file with common funcs
// TODO: i don't like build tree function

// in back = was in if and there is pop instead of push !!!!!!!!! while toooo
// метки пересекаются  от ифов и функций

// ОЧЕНЬ ВАЖНО ПРОВЕРИТЬ ПЕРЕСЕЧЕНИЕ МЕТОК

static bool check_symbol(char symbol, FILE * pfile);
static int put_name_to_table(char name[]);
static void set_type_value(diff_tree_element * element, double number, types_of_node type);

const int LABELS_QUANTITY = 10;
int labels_global[LABELS_QUANTITY] = {};

variables_info variables_table;


static int take_free_label(int labels[], int number_of_taken_label) {
    labels[number_of_taken_label] = 1;
}

static int get_free_label(int labels[]) {
    for (int i = 0; i < LABELS_QUANTITY; i++) {
        if (labels[i] == 0) {
            labels[i] = 1;
            return i;
        }
    }
    printf("out of range of labels!!!\n");
    return 999;
}

void print_single_command(diff_tree_element * element, FILE * pfile, diff_tree_element * funcs[], int labels[]) {

    if (element == NULL) {
        return;
    }

    if (ELEM_OP_ARG == 2) {

        print_single_command(element->left, pfile, funcs, labels);
        print_single_command(element->right, pfile, funcs, labels);

        switch (ELEM_OP_NUM) {

            case OP_ADD:
                fprintf(pfile, "add\n");
                break;

            case OP_SUB:
                fprintf(pfile, "sub\n");
                break;

            case OP_MUL:
                fprintf(pfile, "mul\n");
                break;
            
            case OP_DIV:
                fprintf(pfile, "div\n");
                break;
            
            default:
                printf("unknown arg - %d!!!!!!!\n", ELEM_OP_NUM);
                break;
        }

    } else if (ELEM_OP_ARG == 1) {

        print_single_command(element->right, pfile, funcs, labels);

        switch (ELEM_OP_NUM) {

            case OP_SIN:
                fprintf(pfile, "sin\n");
                break;

            case OP_COS:
                fprintf(pfile, "cos\n");
                break;

            case OP_SQRT:
                fprintf(pfile, "sqrt\n"); // is not working in recursive down, remove pow instead of sqrt
                break;
            
            default:
                printf("unknown arg - %d!!!!!!!\n", ELEM_OP_NUM);
                break;
            }

    } else {
        
        if (ELEM_TYPE == value_t) {

            fprintf(pfile, "push %d\n", (int)ELEM_DOUBLE);

        } else if (ELEM_TYPE == variable_t) {

            if (IS_ELEM(element->parent, syntax_t, OP_EQUAL) & !IS_ELEM(element->parent->parent, syntax_t, OP_IF)) {
                fprintf(pfile, "pop r%cx\n", (int)ELEM_DOUBLE + 'a');
            } else {
                fprintf(pfile, "push r%cx\n", (int)ELEM_DOUBLE + 'a'); // pop or push think
            }

        } else if (ELEM_TYPE == function_t) {

            if (element->right == NULL) {
                fprintf(pfile, "call :%lg\n", element->value.number);
            } else {
                funcs[(int)element->value.number] = element->right;
            }

        } else {

            if (ELEM_OP_NUM == OP_END) {

                print_single_command(element->left, pfile, funcs, labels);
                print_single_command(element->right, pfile, funcs, labels);

            } else if (ELEM_OP_NUM == OP_EQUAL) {

                print_single_command(element->right, pfile, funcs, labels);
                print_single_command(element->left, pfile, funcs, labels);
                
            } else if (ELEM_OP_NUM == OP_WHILE) {

                int begin = get_free_label(labels);
                int end = get_free_label(labels);
                fprintf(pfile, ":%d\n", begin);
                
                print_single_command(element->left, pfile, funcs, labels);

                switch (element->left->value.operator_info.op_number) {
                
                case OP_EQUAL:
                    fprintf(pfile, "jne :%d\n", end); // are changed to opposite commands,
                    break;                                     // because jump happens in opposite case

                case OP_MORE:
                    fprintf(pfile, "jbe :%d\n", end);
                    break;

                case OP_LESS:
                    fprintf(pfile, "jae :%d\n", end);
                    break;
                
                case OP_NEQUAL:
                    fprintf(pfile, "je :%d\n", end);
                    break;
                
                default:
                    printf("unknown arg - %d, 985698!!!!!!!\n", element->left->value.operator_info.op_number);
                    break;
                }

                print_single_command(element->right, pfile, funcs, labels);

                fprintf(pfile, "jmp :%d\n", begin);

                fprintf(pfile, ":%d\n", get_free_label(labels));
                
            } else if (ELEM_OP_NUM == OP_IF ){

                int end = get_free_label(labels);
                
                print_single_command(element->left->left, pfile, funcs, labels);
                print_single_command(element->left->right, pfile, funcs, labels);

                switch (element->left->value.operator_info.op_number) {
                
                case OP_EQUAL:
                    fprintf(pfile, "jne :%d\n", end); // are changed to opposite commands,
                    break;                                     // because jump happens in opposite case

                case OP_MORE:
                    fprintf(pfile, "jbe :%d\n", end);
                    break;

                case OP_LESS:
                    fprintf(pfile, "jae :%d\n", end);
                    break;

                case OP_NEQUAL:
                    fprintf(pfile, "je :%d\n", end);
                    break;
                
                default:
                    printf("unknown arg - %d, 985698!!!!!!!\n", element->left->value.operator_info.op_number);
                    break;
                }

                print_single_command(element->right, pfile, funcs, labels);

                fprintf(pfile, ":%d\n", end);
                
            } else if (ELEM_OP_NUM == OP_PRINT) {

                print_single_command(element->right, pfile, funcs, labels);
                fprintf(pfile, "out\n");

            } else if (ELEM_OP_NUM == OP_RET) {

                fprintf(pfile, "ret\n");

            } else {
                printf("dfkjvhjkdfhkjhdfkj");
            }
        }
    }
}

void print_asm_code(diff_tree_element * element) {

    FILE * pfile = fopen("asm.txt", "w");
    if (pfile == NULL) {
        printf("null ptr pfile");
        return;
    }

    const int FUNCS_QUANTITY = 20;
    const int LABELS_QUANTITY = 10;

    diff_tree_element * funcs[FUNCS_QUANTITY] = {};            // magic const
    int * labels = labels_global;

    print_single_command(element, pfile, funcs, labels);

    fprintf(pfile, "hlt\n");

    for (int i = 0; i < FUNCS_QUANTITY; i++) {
        if (funcs[i] != NULL) {
            fprintf(pfile, "\n\n\n:%d\n", i);
            print_single_command(funcs[i], pfile, funcs, labels);
            fprintf(pfile, "ret\n\n\n\n");
        }
    }

    fclose(pfile);
    return;
}





int main(void) {
    variables_table.size = 0;
    diff_tree_element * tree = read_tree();

    tree_visualize(tree);
    print_asm_code(tree);

    return 0;
}

static bool check_symbol(char symbol, FILE * pfile) {

    bool is_found = 1;
    char check_char = getc(pfile);

    if (check_char != symbol) {                           
        ungetc(check_char, pfile);        
        is_found = 0;                                         
    } 

    check_char = getc(pfile); 

    if (check_char != '\n') {                            
        ungetc(check_char, pfile);          
    }
    
    return is_found;
}

#define VAR_NUM (variables_table.size)

// was taken from frontend, think about it, where to put it
static int put_name_to_table(char name[]) {

    int i = 0;
    while (i < variables_table.size) {

        if (strcmp(name, variables_table.table[i].name) == 0) {
            return i;
        }
        i++;
    }

    strcpy(variables_table.table[VAR_NUM].name, name); // not safe
    variables_table.table[VAR_NUM].value = VAR_NUM;

    VAR_NUM++;
    
    if (VAR_NUM > VARIABLE_COUNT) {
        printf("!!!!!!!!!!!!!!!!!!too much variables");
    }
    
    return VAR_NUM - 1;
}

diff_tree_element * read_tree() {

    FILE * in_file = fopen("in_program.txt", "r");
    if (in_file == NULL) {
        printf("null ptr");
        return NULL;
    }

    diff_tree_element * element = NULL;

    build_tree(&element, in_file, &element);

    fclose(in_file);
    return element;
}

#define LEFT &((*element)->left)
#define RIGHT &((*element)->right)

int build_tree(elem_ptr * element, FILE * in_file, elem_ptr * parent) {

    if (check_symbol('(', in_file) == 1) {

        *element = node_ctor(0, zero_t, NULL, NULL, *parent);

        build_tree(LEFT, in_file, element);

        double value = 0;                  // error obrabotka
        char op[OP_NAME_LEN] = {};
        char x = '0';

        if (fscanf(in_file, "%lf", &value) == 1) {

            set_type_value(*element, value, value_t);

        } else {

            if (check_symbol('$', in_file) == 1) {  // skip $ if it is
            
                fscanf(in_file, "%[^_(]s", &op);
                int num = put_name_to_table(op);
                set_type_value(*element, num, function_t);
                take_free_label(labels_global, num);

            } else {
                
                fscanf(in_file, "%[^_(]s", &op);

                if (is_func_name(op) != -1) {

                    SET_RIGHT_TYPE_VALUE(is_func_name(op));

                } else {

                    int num = put_name_to_table(op);

                    set_type_value(*element, num, variable_t);
                }
            }
        }

        build_tree(RIGHT, in_file, element);

        if (check_symbol(')', in_file) == 0) {      
            return 1; 
        }   

    } else if (check_symbol(NIL, in_file) == 1) {
        return 0;
    }
}

static void set_type_value(diff_tree_element * element, double number, types_of_node type) {

    if (type == variable_t || type == value_t || type == function_t) {
        element->value.number = number;
    } else {
        element->value.operator_info.op_number = (operations)number;
        element->value.operator_info.arg_quantity = get_op_arg_number(element->value.operator_info.op_number);
    }

    element->type = type;
}


