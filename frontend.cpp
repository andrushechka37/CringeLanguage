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


// TODO: there is some sort of copypast(for letters and symbols two different if's) can cause problems
// TODO: end of funcs is are separated from the main (zero_t is putted in two places
// TODO: names of operators should not be in token

// TODO: REWRITE SCANF OF STR PARSE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! -1-2-3---4-4-4
// в лексическом анализаторе не оч красиво это написано- переписать

static int get_size_of_file(FILE * file);

static void set_token(types_of_node type, double value, element_info * elem, char name[]);

static void get_word(int * ip, char * op);
static int get_number(int * ip);

FILE * pfile = NULL;
variables_info variables_table;

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

    } else if (ELEM_TYPE == function_t) {

        fprintf(in_file, "$%s", variables_table.table[(int)element->value.number].name);

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

    FILE * file = fopen("log_down.md", "w");

    diff_tree_element * tree = get_program(&parsed_program, file);

    fclose(file);
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
    if (pfile == NULL) {
        printf("open error\n");
        return;
    }

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

            if (!(IS_ELEM(element->left, syntax_t, OP_END) && !(element->right))) {   // not to print junk nodes([smth] <- [;] <- [;] -> [smth])

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

    } else if (ELEM_TYPE == function_t) {

        fprintf(pfile, "%s", variables_table.table[(int)element->value.number].name);

        if (element->right != NULL) {
            fprintf(pfile, " {\n");
            print_node(element->right);
            fprintf(pfile, "\n}\n");
        }

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
    int sign = 1;

    if (program[*ip] == '-') {
        sign = -1;
        (*ip)++;
    }

    while ('0' <= program[*ip] && program[*ip] <= '9') {
        value = value * 10 + program[*ip] - '0';
        (*ip)++;
    }

    return value * sign;
}

#define VAR_NUM (variables_table.size)

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

#define IS_PARSED_TOKEN(typee, value) (parsed_program[size].type == typee && parsed_program[size].number == value)
      
element_info * parse_str_lexically(size_t len) {

    element_info * parsed_program = (element_info *) calloc(len, sizeof(element_info));
    IS_NULL_PTR(parsed_program);

    int ip = 0;
    int size = 0;

    int brackets[100] = {};
    int brackets_ip = 0;
    
    while (ip < len) {

        if (isspace(cur_char) != 0) {

            ip++;
            continue;

        } else if (isdigit(cur_char) != 0) {                   
                                 
            create_token(value_t, get_number(&ip), "number");        
                                                           
        } else if (isalpha(cur_char) != 0) { // non letters are restricted

            char op[OP_NAME_LEN] = "";      // for operators consisted of letters, variables and functions
            get_word(&ip, op);
                
            if (is_func_name(op) != -1) {

                create_right_token(is_func_name(op), op);
                
            } else {

                if (parsed_program[size - 1].type == syntax_t && parsed_program[size - 1].number == OP_FUNC) {

                    int num = put_name_to_table(op);
                    create_token(function_t, num, op);

                } else {

                    int num = put_name_to_table(op);
                    
                    create_token(variable_t, num, op);
                }
            }

        } else {

            char op[2] = ""; // for non letters operators or brackets
            op[0] = cur_char;
            create_right_token(is_one_char_symbol(cur_char), op);
            ip++;

            if (IS_PARSED_TOKEN(syntax_t, OP_FIG_C)) {
                if (brackets[brackets_ip] == 1) {
                    printf("%d   ,%d-type %lg-value,     %s\n", size, parsed_program[size].type, parsed_program[size].number, parsed_program[size].name);
                    size++;
                    create_token(zero_t, -1, "end of func");
                }
                brackets_ip--;
            }

            if (IS_PARSED_TOKEN(syntax_t, OP_FIG_O)) {
                if (parsed_program[size - 1].type == function_t) {
                    brackets_ip++;
                    brackets[brackets_ip] = 1;
                } else {
                    brackets_ip++;
                    brackets[brackets_ip] = 0;
                }
            }
        }

        printf("%d   ,%d-type %lg-value,     %s\n", size, parsed_program[size].type, parsed_program[size].number, parsed_program[size].name);
        size++;
    }

    create_token(zero_t, -1, "end of func");

    return parsed_program;
}