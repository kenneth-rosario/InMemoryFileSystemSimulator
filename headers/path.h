#ifndef PATH_H
#define PATH_H

/* structure that represent a path */
typedef struct path_name {
    int num_tokens;
    char** tokens;
    unsigned int is_absolute:1;
} PATH;

/* create a PATH instance,  must be destroyed */
PATH *make_path(char* path);
/* PATH destroyer function */
void destroy_path(PATH* target);
/* simple way of getting last token */
char* last_token(PATH* target);

#endif