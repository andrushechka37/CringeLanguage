#pragma once
#include "tree.h"

#define NUMBER_NODE(value) node_ctor(value, value_class, NULL, NULL, NULL)

#define ADD(left, right) node_ctor(OP_ADD, operator_class, left, right, NULL)
#define SUB(left, right) node_ctor(OP_SUB, operator_class, left, right, NULL)
#define MUL(left, right) node_ctor(OP_MUL, operator_class, left, right, NULL)
#define DIV(left, right) node_ctor(OP_DIV, operator_class, left, right, NULL)
#define POW(left, right) node_ctor(OP_POW, operator_class, left, right, NULL)

#define SQRT(arg) node_ctor(OP_SQRT, operator_class, NULL, arg, NULL)

#define VAR node_ctor(0, variable_t, NULL, NULL, NULL);
#define IF(left, right) node_ctor(OP_IF, syntax_class, left, right, NULL);
#define WHILE(left, right) node_ctor(OP_WHILE, syntax_class, left, right, NULL);

#define LESS(left, right) node_ctor(OP_LESS, syntax_class, left, right, NULL);
#define MORE(left, right) node_ctor(OP_MORE, syntax_class, left, right, NULL);
#define EQUAL(left, right) node_ctor(OP_EQUAL, syntax_class, left, right, NULL);



#define NEQUAL(left, right) node_ctor(OP_NEQUAL, syntax_class, left, right, NULL);


#define SIN(arg) node_ctor(OP_SIN, operator_class, NULL, arg, NULL)
#define COS(arg) node_ctor(OP_COS, operator_class, NULL, arg, NULL)

#define PRINT(arg) node_ctor(OP_PRINT, syntax_class, NULL, arg, NULL)

#define COPY_L copy_node(element->left)
#define COPY_R copy_node(element->right)

#define DIF_L left_node
#define DIF_R right_node

diff_tree_element * tree_diff(diff_tree_element * element);
diff_tree_element * copy_node(diff_tree_element * original);

void consts_eval(diff_tree_element * element);
void delete_fictive_nodes(diff_tree_element * element);
void tree_simplify(diff_tree_element * element);
void set_parents(diff_tree_element * root, diff_tree_element * parent);

void throw_away_node(diff_tree_element * element, char junk_side);
void single_node_dtor(elem_ptr * element);