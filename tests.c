#include "headers/gtree.h"
#include "headers/reader.h"
#include <stdio.h>

void test_gtree() {
    NODE* root = make_node(2, 1, "", 7);
    NODE* c1 = make_node(3, 1, "test_dir", 9);
    NODE* c2 = make_node(4, 1, "test_dir2", 10);
    NODE* c11 = make_node(5, 0, "test_file", 10);
    NODE* c12 = make_node(6, 0, "test_file2", 11);
    NODE* c21 = make_node(7, 0, "test_file3", 11);
    NODE* c22 = make_node(8, 0, "test_file4", 11);

    add_child(c1, root);
    add_child(c2, root);
    add_child(c11, c1);
    add_child(c12, c1);
    add_child(c21, c2);
    add_child(c22, c2);

    print_tree(root);

    remove_recusive(root, "test_dir2");

    print_tree(root);

    remove_recusive(c1, "test_file");
    
    print_tree(root);
    
    remove_file(c1, "test_file");

    print_tree(root);
}


void test_make_path() {

    char dimelo[] = "/testing/one/2/three";

    PATH* path = make_path(dimelo);

    int i = 0;

    while(path->tokens[i]){
        printf(" %s \n ", path->tokens[i]);
        i++;
    }

    destroy_path(path);

}

void test_reader() {

    initialize();

    mkdir("c");

    mkdir("c/b");

    mkdir("c/b/a");

    mkdir("c/b/a/d");

    touch("/c/f");

    touch("/c/b/h");

    rmdir("c");

    // mkdir();

    // mkdir("otravez");

}