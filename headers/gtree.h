// GENERAL TREE ADT

#ifndef G_TREE_H
#define G_TREE_H

# define NOT_FOUND 1
# define NOT_A_DIRECTORY 2
# define NOT_A_FILE 3
# define SUCCESS 0

/* tree node with enough meta data to model a file or directory in a file system */
typedef struct node {
    
    struct node* parent; // parent node 
    unsigned int isDir:1; // Single bit flag determining if node represents a directory
    int id; // unique id
    int name_len; // name length
    int num_childs; // number of children
    int children_capacity; // how many children fit the children list
    struct node **children; // A pointer to consecutive pointers of Nodes ( a dynamic list )
    char* name; // name of directory or file size limit of 35 letters

} NODE;

/* Has to be freed */
NODE *make_node(int id, unsigned int isDir, char name[], int name_len);

/* destroy target node */
void destroy_node(NODE* target);

/* adds child to target node */
int add_child(NODE *newnode, NODE *parent);

/* Prints tree with indented children */
void print_tree(NODE *root);

/* Remove recursively */
int remove_recusive(NODE *root, char *name);

/* Remove file */
int remove_file(NODE *root, char *name);

/* find index */
int find_node(NODE* root, const char *name);

/* check if node exists */
int name_exists(const char* name, NODE* target);

#endif



