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
// TODO: end of funcs is are separated from the main (zero_class is putted in two places
// TODO: names of operators should not be in token

// TODO: REWRITE SCANF OF STR PARSE   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! -1-2-3---4-4-4
// в лексическом анализаторе не оч красиво это написано- переписать

// убрать нейм
// сделать норм считывание

// передача имени файла

// TODO: CLEAN HEADERS
// TODO: remove c printers

static int get_size_of_file(FILE * file);

static void set_token(types_of_node type, double value, token_info * elem, char name[]);

static void get_word(int * index, char * op);
static int get_number(int * index);

int read_file_to_buffer(const char file[]);

int print_inorder_program(diff_tree_element * element);

FILE * pfile = NULL;
variables_info variables_table;


const char * get_input_file_name(int argc, const char * argv[], const char * default_name) {

    if (argc == 1) {
        return default_name;
    }

    return argv[1];
}

int main(int argc, const char * argv[]) {

    // TODO(DONE): Why name very generic function that works on _any_ file read_program

    const char * default_input_file = "program.txt";
    const char * input_file = get_input_file_name(argc, argv, default_input_file);
    size_t len = read_file_to_buffer(input_file);


    // TODO(DONE): Also, probably shouldn't hide something as important as _main_ file
    //       name from the main by using default arguments, just write it explicitly.

    // cmdline_args args {};
    // parse_cmdline_args(&args, argv, argc);
    //
    // char* output_file =
    //     array_get_or_default(args.plain, 0, "my_default_selection.andrushechka34");
    // read_program(output_file);
    //
    // read_program(argv[1] ?: "my_default_selection.andrushechka34");

    // TODO(DONE): HOW CAN IT BE "PARSED" if it's stored in _TOKEN_ array

    tokens token_array = {};
    variables_table.size = 0;
    
    token_array.tokens = parse_str_lexically(len);

    printf("--------------------------\n");

    FILE * file = fopen("log_down.md", "w");

    diff_tree_element * tree = get_program(&token_array, file);

    fclose(file);
    set_parents(tree, tree);

    // tree_visualize(tree);
    tree_visualize(tree);

    //print_to_file_c_program(tree);

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

#define CHECK_END(element) (element && (IS_ELEM(element, syntax_class, OP_END)))

void print_node(diff_tree_element * element) {

    NULL_ELEM;

    if (IS_ELEM(element, syntax_class, OP_END)) {

        print_node(element->left);

        if (!(element->left && (IS_ELEM(element->left, syntax_class, OP_WHILE) || 
                                IS_ELEM(element->left, syntax_class, OP_IF)))) {

            if (!(IS_ELEM(element->left, syntax_class, OP_END) && !(element->right))) {   // not to print junk nodes([smth] <- [;] <- [;] -> [smth])

                if (!(CHECK_END(element->left) && CHECK_END(element->right))) {       // not to print ([;] <- [;] -> [;])
                    fprintf(pfile, ";\n");                                            // separated for better understanding
                }
            }
        }

        print_node(element->right);
        
    } else if (ELEM_TYPE == value_class) {

        fprintf(pfile,"%.2lg", ELEM_DOUBLE);

    } else if (ELEM_TYPE == variable_class) {

        printf("%d\n", (int)element->value.number);
        fprintf(pfile, "%s", variables_table.table[(int)element->value.number].name);

    } else if (ELEM_TYPE == function_class) {

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

     if (IS_ELEM(element, syntax_class, OP_IF) || IS_ELEM(element, syntax_class, OP_WHILE)) {

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

int read_file_to_buffer(const char file[]) {

    FILE * pfile = fopen(file, "r");

    int len = get_size_of_file(pfile);
    program = (char *) calloc(len, sizeof(char));

    fread(program, sizeof(char), len, pfile);

    fclose(pfile);
    return len;
}

static void set_token(types_of_node type, double value, token_info * elem, char name[]) {

    elem->number = value;
    elem->type = type;
    strcpy(elem->name, name);
}

static void get_word(int * index, char * op) {

    int i = 0;

    while (('a' <= program[*index] && program[*index] <= 'z') || program[*index] == '_') { // opname len check 
        op[i] = program[*index];                        // sscanf
        i++;    
        (*index)++;

        if (i > OP_NAME_LEN) {
            printf("name len overflow\n");
            return;
        }
    }
}

static int get_number(int * index) {    // sscanf // %n

    int value = 0;
    int sign = 1;

    if (program[*index] == '-') {
        sign = -1;
        (*index)++;
    }

    while ('0' <= program[*index] && program[*index] <= '9') {
        value = value * 10 + program[*index] - '0';
        (*index)++;
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
    
    if (VAR_NUM > SYMBOL_TABLE_MAX_CAPACITY) {
        printf("!!!!!!!!!!!!!!!!!!too much variables");
    }
    
    return VAR_NUM - 1;
}

#define IS_PARSED_TOKEN(typee, value) (token_array[size].type == typee && token_array[size].number == value)
#define CREATE_TOKEN(type, value, name) set_token(type, value, &(token_array[size]), name)
#define CUR_CHAR program[index]

token_info * parse_str_lexically(size_t len) {

    token_info * token_array = (token_info *) calloc(len, sizeof(token_info));
    IS_NULL_PTR(token_array);

    int index = 0;
    int size = 0;

    int brackets[100] = {};
    int brackets_ip = 0;
    
    while (index < len) {

        if (isspace(CUR_CHAR) != 0) {

            index++;
            continue;

        } else if (isdigit(CUR_CHAR) != 0 || ((CUR_CHAR == '-') && isdigit(program[index + 1]))) {                   
                                 
            CREATE_TOKEN(value_class, get_number(&index), "number");        
                                                           
        } else if (isalpha(CUR_CHAR) != 0) { // non letters are restricted

            char op[OP_NAME_LEN] = "";      // for operators consisted of letters, variables and functions
            get_word(&index, op);

            
            if (is_func_name(op) != -1) {

                create_right_token(is_func_name(op), op);
                
            } else {

                // checks if there is $ symbol
                if (token_array[size - 1].type == syntax_class && token_array[size - 1].number == OP_FUNC) {

                    int num = put_name_to_table(op);
                    CREATE_TOKEN(function_class, num, op);

                } else {

                    int num = put_name_to_table(op);
                    
                    CREATE_TOKEN(variable_class, num, op);
                }
            }

        } else {

            char op[2] = ""; // for non letters operators or brackets
            op[0] = CUR_CHAR;
            create_right_token(is_one_char_symbol(CUR_CHAR), op);
            index++;

            if (IS_PARSED_TOKEN(syntax_class, OP_FIG_C)) {
                if (brackets[brackets_ip] == 1) {
                    //printf("%d   ,%d-type %lg-value,     %s\n", size, token_array[size].type, token_array[size].number, token_array[size].name);
                    size++;
                    CREATE_TOKEN(zero_class, -1, "end of func");
                }
                brackets_ip--;
            }

            if (IS_PARSED_TOKEN(syntax_class, OP_FIG_O)) {
                
                if (token_array[size - 1].type == function_class) {
                    brackets_ip++;
                    brackets[brackets_ip] = 1;
                } else {
                    brackets_ip++;
                    brackets[brackets_ip] = 0;
                }
            }
        }

        //printf("%d   ,%d-type %lg-value,     %s\n", size, token_array[size].type, token_array[size].number, token_array[size].name);
        size++;
    }

    CREATE_TOKEN(zero_class, -1, "end of func");

    return token_array;
}




void print_inorder(diff_tree_element * element, FILE * in_file) {

    if (element == NULL) { 
        fprintf(in_file, "_");
        return;
    }

    if (element->left && (IS_ELEM(element->left, syntax_class, OP_END) && !(element->right))) {

        print_inorder(element->left, in_file);
        return;
    }

    fprintf(in_file, "(");
    print_inorder(element->left, in_file);

    switch (ELEM_TYPE) {

        case value_class:
            fprintf(in_file,"%lg", element->value);
            break;

        case variable_class:
            printf("%d\n", (int)element->value.number);
            fprintf(in_file, "%s", variables_table.table[(int)element->value.number].name);
            break;

        case function_class:
            fprintf(in_file, "$%s", variables_table.table[(int)element->value.number].name);
            break;
    
        default:
            fprintf(in_file, "%s", get_op_symbol(ELEM_OP_NUM));
            break;
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

