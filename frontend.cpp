#include <stdlib.h>
#include <stdio.h>
#include "diff_project/diff.h"
#include "recursive_down.h"
#include "diff_project/deff_dump.h"
#include "frontend.h"
#include <ctype.h>

#include <math.h>
#include <string.h>

FILE * pfile = NULL;

static int get_size_of_file(FILE * file);

static void set_token(types_of_node type, double value, element_info * elem, char name[]);

static void get_word(int * ip, char * op);
static int get_number(int * ip);
element_info * parse_str_lexically(size_t len);

// #define IS_ELEM(element, type_of_node, value_of_node) (element->type == type_of_node && ELEM_OP_NUM == value_of_node)

// #define NULL_ELEM            \
//     if (element == NULL) {   \
//         return;              \
//     }



// void print_var(diff_tree_element * element) {

//     fprintf(pfile, "xxxxxxx");

//     return;
// }

// void print_int(diff_tree_element * element) {

//     print_node(element->left);

//     fprintf(pfile, "%lg", ELEM_DOUBLE);

//     print_node(element->right);

//     return;
// }

// void print_complex_expression(diff_tree_element * element) {

//      NULL_ELEM;

//      if (IS_ELEM(element, syntax_t, OP_IF)) {

//      } else if (IS_ELEM(element, syntax_t, OP_IF)) {

//      } else {

//      }

// }
// void print_node(diff_tree_element * element) {

//     NULL_ELEM;

//     if (IS_ELEM(element, syntax_t, OP_END)) {

//         print_node(element->left);

//         if (!(IS_ELEM(element->left, syntax_t, OP_WHILE) || IS_ELEM(element->left, syntax_t, OP_IF))) {
//             fprintf(pfile, ";\n");
//         }

//         print_node(element->right);

//     } else if (ELEM_TYPE == value_t) {

//         print_int(element);

//     } else if (ELEM_TYPE == variable_t) {

//         print_var(element);

//     } else {
//         print_complex_expression(element);
//     }
// }

int main(void) {
    size_t len = read_program();
    token_array parsed_program = {};

    parsed_program.tokens = parse_str_lexically(len);

    printf("--------------------------\n");

    diff_tree_element * tree = get_program(&parsed_program);

    set_parents(tree, tree);
    tree_visualize(tree);
    tree_dtor(&tree);
    return 0;
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

static void set_token(types_of_node type, double value, element_info * elem, char name[]) {
    elem->number = value;
    elem->type = type;
    strcpy(elem->name, name);
}

static void get_word(int * ip, char * op) {
        int i = 0;
        while (('a' <= program[*ip] && program[*ip] <= 'z') || program[*ip] == '_') { // opname len check 
            op[i] = program[*ip];                        // sscanf
            i++;    
            (*ip)++;

            if (i > OP_NAME_LEN) {
                printf("name len overflow\n");
                return;
            }
        }
}

static int get_number(int * ip) {    // sscanf // %n
    int value = 0;
    while ('0' <= program[*ip] && program[*ip] <= '9') {
        value = value * 10 + program[*ip] - '0';
        (*ip)++;
    }
    return value;
}

      
element_info * parse_str_lexically(size_t len) {

    element_info * parsed_program = (element_info *) calloc(len, sizeof(element_info));
    IS_NULL_PTR(parsed_program);

    int ip = 0;
    int size = 0;
    
    while (cur_char != '\0') {
        if (isspace(cur_char) != 0) {

            ip++;
            continue;      // нужно ли единство стиля?

        } else if (isdigit(cur_char) != 0) {                   
                                 
            create_token(value_t, get_number(&ip), "0");        
                                                           
        } else if (isalpha(cur_char) != 0) { // non letters are restricted

            char op[OP_NAME_LEN] = "";
            get_word(&ip, op);
                
            if (is_func_name(op) != -1) {
                create_right_token(is_func_name(op), op);
            } else {
                create_token(variable_t, 7777, op);
            }


        } else {
            char op[2] = "";
            op[0] = cur_char;
            create_right_token(is_one_char_symbol(cur_char), op);
            ip++;
        }

        printf("%d   ,%d-type %lg-value,  %s- name\n", size, parsed_program[size].type, parsed_program[size].number, parsed_program[size].name);
        size++;
    }
    parsed_program[size].type = zero_t;
    return parsed_program;
}









