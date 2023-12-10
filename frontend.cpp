#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "diff_project/diff.h"
#include "diff_project/recursive_down.h"
#include "diff_project/deff_dump.h"




int main(void) {
    diff_tree_element * tree = get_expression();
    set_parents(tree, tree);
    print_tex(tree);
    tree_visualize(tree);
    diff_tree_element * tree2 = tree_diff(tree);
    tree_dtor(&tree);
    set_parents(tree2, tree2);
    verify(tree2);
    print_tex(tree2, "not_opt.md");
    tree_simplify(tree2);
    tree_visualize(tree2);
    print_tex(tree2);
} 
