#include <stdlib.h>
#include <stdio.h>
#include "diff_project/diff.h"
#include "recursive_down.h"
#include "diff_project/deff_dump.h"
#include "frontend.h"
#include <ctype.h>

#include <math.h>
#include <string.h>
// russian names are in lex anal trouble
// retie with ;
static int get_size_of_file(FILE * file);

static void set_token(types_of_node type, double value, element_info * elem);

static void get_word(int * ip, char * op);
static int get_number(int * ip);
              
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
                                 
            create_token(value_t, get_number(&ip));        
                                                           
        } else if (isalpha(cur_char) != 0) { // non letters are restricted

            char op[OP_NAME_LEN] = "";
            get_word(&ip, op);
                
            if (is_func_name(op) != -1) {
                create_right_token(is_func_name(op));
            } else {
                create_token(variable_t, 7777);
            }

        } else {
            create_right_token(is_one_char_symbol(cur_char));
            ip++;
        }

        printf("%d-type %lg-value\n", parsed_program[size].type, parsed_program[size].number);
        size++;
    }
    parsed_program[size].type = zero_t;
    return parsed_program;
}




int main(void) {
    size_t len = read_program();
    token_array parsed_program = {};
    parsed_program.tokens = parse_str_lexically(len);
    int i = 0;
    printf("--------------------------\n");
    // while(i < 10) {
    //     printf("%d-type %lg-value\n", parsed_program.tokens[i].type, parsed_program.tokens[i].number);
    //     i++;
    // }
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

static void set_token(types_of_node type, double value, element_info * elem) {
    elem->number = value;
    elem->type = type;
}

static void get_word(int * ip, char * op) {
        int i = 0;
        while ('a' <= program[*ip] && program[*ip] <= 'z') { // opname len check 
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

