#include <stdlib.h>
#include <stdio.h>
#include "diff_project/diff.h"
#include "recursive_down.h"
#include "diff_project/deff_dump.h"
#include "frontend.h"
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <math.h>
#include <string.h>

FILE * pfile = NULL;
variables_info variables_table;

static int get_size_of_file(FILE * file);

static void set_token(types_of_node type, double value, element_info * elem, char name[]);

static void get_word(int * ip, char * op);
static int get_number(int * ip);

void print_inorder(diff_tree_element * element, FILE * in_file) {

    if (element == NULL) {     
        fprintf(in_file, "_");
        return;
    }

    if (element->left && (IS_ELEM(element->left, syntax_t, OP_END) && !(element->right))) {
        print_inorder(element->left, in_file);
        return;
    }

    fprintf(in_file, "(");
    print_inorder(element->left, in_file);

            
            if (ELEM_TYPE == value_t) {

                fprintf(in_file,"%lg", element->value);

            } else if (ELEM_TYPE == variable_t) {

                printf("%d\n", (int)element->value.number);
                fprintf(in_file, "%s", variables_table.table[(int)element->value.number].name);

            } else {

                fprintf(in_file, "%s", get_op_symbol(ELEM_OP_NUM));
            }
        
    
    
    print_inorder(element->right, in_file);
    fprintf(in_file,")");

    return;
}

int print_inorder_program(diff_tree_element * element) {
    FILE * in_file = fopen("in_program.txt", "w");
    IS_NULL_PTR(in_file);

    print_inorder(element, in_file);

    fclose(in_file);
}

int main(void) {         
    size_t len = read_program();
    token_array parsed_program = {};
    variables_table.size = 0;
    
    parsed_program.tokens = parse_str_lexically(len);

    printf("--------------------------\n");

    diff_tree_element * tree = get_program(&parsed_program);
    set_parents(tree, tree);

    tree_visualize(tree);
    tree_visualize(tree);

    print_to_file_c_program(tree);

    print_inorder_program(tree);

    tree_dtor(&tree);
    return 0;
} 

void print_to_file_c_program(diff_tree_element * element) {

    pfile = fopen("c_program.txt", "w");

    print_node(element);
    fclose(pfile);
}

#define CHECK_END(element) (element && (IS_ELEM(element, syntax_t, OP_END)))

void print_node(diff_tree_element * element) {

    NULL_ELEM;

    if (IS_ELEM(element, syntax_t, OP_END)) {

        print_node(element->left);

        if (!(element->left && (IS_ELEM(element->left, syntax_t, OP_WHILE) || 
                                IS_ELEM(element->left, syntax_t, OP_IF)))) {

            if (!(IS_ELEM(element->left, syntax_t, OP_END) && !(element->right))) {   // not to print junk nodes([;] <- [;] <- [;] -> [smth])

                if (!(CHECK_END(element->left) && CHECK_END(element->right))) {       // not to print ([;] <- [;] -> [;])
                    fprintf(pfile, ";\n");                                            // separated for better understanding
                }
            }
        }

        print_node(element->right);
        
    } else if (ELEM_TYPE == value_t) {

        fprintf(pfile,"%.2lg", ELEM_DOUBLE);

    } else if (ELEM_TYPE == variable_t) {

        printf("%d\n", (int)element->value.number);
        fprintf(pfile, "%s", variables_table.table[(int)element->value.number].name);

    } else {

        print_complex_expression(element);
    }
}

void print_complex_expression(diff_tree_element * element) {

     NULL_ELEM;

     if (IS_ELEM(element, syntax_t, OP_IF) || IS_ELEM(element, syntax_t, OP_WHILE)) {

        fprintf(pfile, "%s (", get_op_symbol(ELEM_OP_NUM));
        print_node(element->left);
        fprintf(pfile, ")");

        fprintf(pfile, " {\n", get_op_symbol(ELEM_OP_NUM));
        print_node(element->right);
        fprintf(pfile, "}\n");

     } else {

        if (IS_ROUND_BRACKET) {
            fprintf(pfile,"(");
        }

        print_node(element->left);

        fprintf(pfile, " %s ", get_op_symbol(ELEM_OP_NUM));

        print_node(element->right);

        if (IS_ROUND_BRACKET) {
            fprintf(pfile,")");
        }
    }

    return;
}

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

                int num = set_variable(op);
                
                create_token(variable_t, num, op);
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











