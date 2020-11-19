#include "headers/reader.h"
#include "headers/gtree.h"
#include "headers/path.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Initial Private global variables for state management */
static NODE* g_cwd_node;
static NODE* g_root;
static int id_counter;
static unsigned int g_is_init = 0;

/* command list */
static char *commands[] = {"mkdir", "rmdir", "ls", "cd", 
    "pwd", "touch", "rm", NULL };

void initialize() {
    g_root = make_node(0, 1, "", 1);
    g_cwd_node = g_root;
    id_counter = 1;
    g_is_init = 1; 
}

void verify_initialized() { if( !g_is_init ) exit(1); }

void pwd(){


    NODE* ptr = g_cwd_node;
    int i = 0;
    int j = 0;

    verify_initialized();

    // find depth    
    while(ptr->parent != NULL) {
        i++;
        ptr = ptr->parent;
    }

    char *strings[i];

    ptr = g_cwd_node;

    j = i;

    while (ptr->parent != NULL) {
        strings[--j] = ptr->name;
        ptr = ptr->parent;
    }

    for ( j = 0; j < i; j++ ){
        printf("/%s", strings[j]);
    }

    printf("\n");

}


// if path is valid this function returns the node otherwise returns null
NODE* find_node_from_path(PATH *path, int upto) {

    int i = 0;
    int j;
    NODE* ptr; 

    verify_initialized();

    ptr = path->is_absolute?g_root:g_cwd_node;

    while(path->tokens[i]){
        
        if ( i > upto ) { return ptr; }

        j = find_node(ptr, path->tokens[i]);
        
        if ( j >= 0 && i != path->num_tokens - 1 && ptr->children[j]->isDir ) {
            ptr = ptr->children[j];
        }
        // a file was part of the path and it was not the last element 
        else if ( j >= 0 && i != path->num_tokens - 1 && !ptr->children[j]->isDir ) {
            return NULL;
        }

        else if ( j >= 0 )  {
            ptr = ptr->children[j];
        }

        else {
            ptr = NULL;
            break;
        }

        i++;

    } 

    return ptr;

}

/* Finish ls */
void ls(char *path){
    
    PATH* p_path = make_path(path);
    NODE* newnode;
    char* last_name;
    int result = -1;
    int j;

    verify_initialized();

    NODE* result_node = find_node_from_path(p_path, p_path->num_tokens - 1);

    if ( result_node == NULL ) { 

        printf("invalid path %s", path);

    } else if(result_node->isDir)  {
        
        for(j = 0; j < result_node->num_childs; j++){
            if ( result_node->children[j]->isDir ) { printf("\033[0;32m"); }
            else { printf("\033[0;34m"); }
            printf("[id:%d, name:%s] ", result_node->children[j]->id, result_node->children[j]->name);
            
            printf("\033[0m");
        }
        printf("\n");
    
    } else {
        printf("path: %s not a directory", path);
    }

}


void mkdir(char *path){

    PATH* p_path = make_path(path);
    NODE* newnode;
    char* last_name;
    int result = -1;
    int j;

    verify_initialized();

    NODE* result_node = find_node_from_path(p_path, p_path->num_tokens - 2);

    if ( result_node == NULL ) {

        printf("invalid path %s", path);

    } else {

        if ( p_path->num_tokens <= 0 ) {
            printf("path: %s is invalid \n", path);
            destroy_path(p_path);
            return;
        }

        last_name = last_token(p_path);

        if ( !name_exists(last_name, result_node ) ) {
            destroy_path(p_path);
            return;
        }

        if( !result_node->isDir ){
            printf("path: %s is invalid \n", path);
            destroy_path(p_path);
            return;
        }

        // create node
        newnode = make_node(id_counter++, 1, last_name, strlen(last_name) );
        result = add_child(newnode, result_node);

        switch(result) {

            case SUCCESS:
                print_tree(g_root);        
                break;
            case NOT_A_DIRECTORY:   
                printf("path: %s is not a dirtectory", path);
                destroy_node(newnode);
                break;
            default:
                break;

        }

    }

    destroy_path(p_path);

}


void touch(char *path){

    PATH *p_path;
    NODE *newnode;
    int result;
    NODE *result_node;
    char* last_name;

    verify_initialized();

    p_path=make_path(path); 

    if ( p_path->num_tokens == 0 ) { 

        printf("invalid path: %s\n", path);
        destroy_path(p_path);
        return;

    }

    result_node = find_node_from_path(p_path, p_path->num_tokens-2);

    if ( result_node == NULL ) {

        printf("invalid path: %s\n", path);

    } else {    

        last_name = last_token(p_path);
        newnode = make_node(id_counter++, 0, last_name, strlen(last_name) );
        result = add_child(newnode, result_node);

        switch(result) {

            case SUCCESS:
                print_tree(g_root);        
                break;
            case NOT_A_DIRECTORY:   
                printf("path: %s is not a dirtectory", path);
                destroy_node(newnode);
                break;
            default:
                break;

        }

    }

    destroy_path(p_path);

}

void rmsimple(char *path, int isDir){
    PATH *p_path = make_path(path);
    NODE* result_node;
    char* last_name;
    int result_status;

    verify_initialized();

    // if no tokens in path
    if ( p_path->num_tokens == 0 ){
        g_cwd_node = g_root;
        destroy_path(p_path);
        pwd();
        return;
    }

    result_node = find_node_from_path(p_path, p_path->num_tokens - 1);

    if ( !result_node ) {
        printf("invalid path: %s", path);
        destroy_path(p_path);
        return;
    }

    last_name = last_token(p_path);

    result_status = isDir?remove_recusive(result_node->parent, last_name):remove_file(result_node->parent, last_name);

    switch(result_status) {
        case SUCCESS:
            print_tree(g_root);        
            break;
        case NOT_A_DIRECTORY:   
            printf("path: %s is not a dirtectory\n", path);
            break;
        case NOT_A_FILE:
            printf("path: %s is not a file\n", path);
            break;
        default:
            break;
    }

    destroy_path(p_path);
}

void cd(char* path){

    PATH *p_path = make_path(path);

    // if no tokens in path
    if ( p_path->num_tokens == 0 ){
        g_cwd_node = g_root;
        destroy_path(p_path);
        pwd();
        return;
    }

    NODE *result_node = find_node_from_path(p_path, p_path->num_tokens - 1);

    if ( result_node == NULL || !result_node->isDir ) { 
        printf("invalid path or not a directory: %s\n", path); 
    }
    else {
        g_cwd_node = result_node;
        pwd();
    }

    destroy_path(p_path);

}



/* find index of command */
int findCmd(char *command) {

    int i = 0;
    while( commands[i] ) {
        if (!strcmp(command, commands[i]))
            return i; // found command: return index i
        i++;
    }

    return -1;

}

void mainloop(){

    char line[128];
    char command[16], pathname[64];
    char dname[64], bname[64];
    int i;

    initialize();
    
    while (1) {

        printf("$ >> ");    
        fgets(line, 128, stdin); // get at most 128 chars from stdin
        line[strlen(line)-1] = 0; // kill \n at end of line
        sscanf(line, "%s %s", command, pathname); 

        if(! strcmp(command, "quit") ){
            exit(0);
        }

        i = findCmd(command);

        switch(i) {
            case 0:
                mkdir(pathname);
                break;
            case 1:
                rmdir(pathname);
                break;
            case 2:
                ls(pathname);
                break;
            case 3:
                cd(pathname);
                break;
            case 4:
                pwd(pathname);
                break;
            case 5:
                touch(pathname);
                break;
            case 6:
                rm(pathname);
                break;
            default:
                printf("invalid command \n");
                break;
        }

        strcpy(pathname, "");

    }

}





