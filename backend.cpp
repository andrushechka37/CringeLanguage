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

static bool check_symbol(char symbol, FILE * pfile);
static int set_variable(char name[]);
variables_info variables_table;

void set_type_value(diff_tree_element * element, double number, types_of_node type) {

    if (type == variable_t || type == value_t) {
        element->value.number = number;
    } else {
        element->value.operator_info.op_number = (operations)number;
    }

    element->type = type;
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

            fscanf(in_file, "%[^_(]s", &op);

            //printf("%s, %d\n", op, is_func_name(op));

            if (is_func_name(op) != -1) {

                SET_RIGHT_TYPE_VALUE(is_func_name(op));

            } else {
                int num = set_variable(op);

                set_type_value(*element, num, variable_t);

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

int main(void) {
    variables_table.size = 0;
    diff_tree_element * tree = read_tree();

    tree_visualize(tree);

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

static int set_variable(char name[]) {

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





