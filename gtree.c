#include "headers/gtree.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

NODE* make_node(int id, unsigned int isDir, char name[], int name_len){

    NODE* new_node = (NODE*) malloc( sizeof(NODE) ); // init new_node with space for name

    new_node->id=id;

    new_node->num_childs = 0; 

    new_node->children_capacity = isDir?10:0;

    new_node->isDir = isDir;

    if(isDir)
        new_node->children = (NODE **) malloc(sizeof(NODE *) * new_node->children_capacity ); // have enough space by default for 10 children 
    
    new_node->name_len = name_len;

    new_node->name = (char *) malloc(35); // limit size of 35 charachters for name

    strcpy(new_node->name, name); // initialize name

    return new_node;

}

/* you have to freee children before destroying node */
void destroy_node(NODE* target) {

    int i;
    if(target->isDir){
        free(target->children);
        free(target);
        free(target->name);
    } else {
        free(target->name);
        free(target);
    }

}

int add_child(NODE* newnode, NODE* parent) {

    if ( !parent->isDir ) return NOT_A_DIRECTORY; // can only add children to directories

    // Increment children space
    if (parent->num_childs == parent->children_capacity){
        parent->children = (NODE **) realloc(parent->children, sizeof(NODE *) * parent->children_capacity * 2 ); 
        parent->children_capacity = parent->children_capacity * 2;
    }

    parent->children[parent->num_childs] = newnode;

    parent->num_childs++;

    newnode->parent = parent;

    return SUCCESS;

}

// Static helper cannot be accessed outside of file
static void remove_recusive_helper(NODE *root) {

    if( root->num_childs == 0 ){
        destroy_node(root);
        return;   
    }

    int i;

    for(i = root->num_childs - 1; i >= 0; i--){
        remove_recusive_helper(root->children[i]);
    }

    destroy_node(root);

}



// removes child simply, must be freed before removed
void remove_child_simple(NODE *root, int index){

    int i;
    NODE *test;
    for(i = index; i < root->num_childs-1; i++){
        test = root->children[i];
        root->children[i] = root->children[i + 1];
        test = root->children[i];
    }
    root->children[i] = NULL;  
    root->num_childs--;

}

// finds node cannot be accessed outside of file 
int find_node(NODE* root, const char *name) {

    int i;
    NODE *ptr = NULL;

    for(i = 0; i < root->num_childs; i++){
        if ( ! strcmp(root->children[i]->name, name )  ){
            return i;
        }
    }

    return -1;

}


// removes file children from root
int remove_file(NODE *root, char *name){
    
    int i;
    i = find_node(root, name);
    if ( i >= 0 ) {
        if(!root->isDir) return NOT_A_DIRECTORY;
        if(root->children[i]->isDir) return NOT_A_FILE;
        destroy_node(root->children[i]);
        remove_child_simple(root, i);
    } 

    return SUCCESS;

}

int remove_recusive(NODE *root, char *name) {

    NODE* ptr = NULL;
    int i;

    if(!root->isDir) return NOT_A_DIRECTORY;
    
    i = find_node(root, name);

    if ( i >= 0 ) ptr = root->children[i];

    if(ptr && !ptr->isDir) return NOT_A_DIRECTORY;

    if (i < 0) return NOT_FOUND;

    remove_recusive_helper(ptr);

    // shift values downward here

    remove_child_simple(root, i);

    return SUCCESS;

}

static void print_tree_helper(NODE* root, int depth) {
    
    int i;
    for(i = 0; i < depth; i++) printf("\t"); // proper indentation
    
    // directories are green 
    if( root->isDir ) printf("\033[0;32m");
    // files are blue
    else { printf("\033[0;34m"); }
    
    printf("/%s\n", root->name);

    printf("\033[0m");

    for(i = 0; i < root->num_childs; i++){

        if(root->children[i]->isDir) {   }

        print_tree_helper(root->children[i], depth + 1);
    }

}

int name_exists(const char* name, NODE* target) {
    int j = find_node(target, name);

    if ( j >= 0 ) {
        printf("Directory already exists \n");
        return 0;
    }

    return 1;
}

void print_tree(NODE* root){

    print_tree_helper(root, 0);

}

