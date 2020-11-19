#include "headers/path.h"
#include <stdlib.h>
#include <string.h>


inline char* last_token(PATH* target){
    if(target->num_tokens > 0) {
        return target->tokens[target->num_tokens - 1];
    }
    return NULL;
}

// destroy path
void destroy_path(PATH* path){

    int i = 0;
    while(path->tokens[i]){
        free(path->tokens[i++]);
    }

    free(path->tokens);
    free(path);

}

// tokenizes path and returns 1 if it is absolute or 0 if relative pointer must be freed
PATH* make_path(char *path){

   char *path_copy = (char *) malloc ( sizeof(char *)*strlen(path) );
   char *path_copy2 = (char *) malloc ( sizeof(char *)*strlen(path) );

   // dont modify string that is being sent as parameter 
   strcpy(path_copy, path);
   strcpy(path_copy2, path); 

   int i;
   // Extract the first token
   char * token = strtok(path_copy, "/");
   
   // create dynamic path must be destoyed using destroy_path method 
   PATH* to_return = (PATH *) malloc( sizeof(PATH) );
   to_return->is_absolute = 0;

   // if leading slash is present it is an absolute path 
   if( !strncmp("/", path, 1) ) to_return->is_absolute = 1;     

   // count number of tokens in path 
   i = 0;
   // count number of tokens
   while( token != NULL ) {
      i++;
      token = strtok(NULL, "/");
   }

   to_return->num_tokens = i; 

   // allocate space necessary for i pointers plus one null at the end 
   to_return->tokens = (char **) malloc(sizeof(char *) * (i + 1) );

   // Symbolizing end of list
   to_return->tokens[i] = NULL;

   token = strtok(path_copy2, "/");

   i = 0;  
   // get length of token and initialize correct index in token list
   while ( token != NULL ) {

       // make space in to_return 
       to_return->tokens[i] = (char *) malloc ( sizeof(strlen(token)) );
       strcpy(to_return->tokens[i], token);
       token = strtok(NULL, "/");
       i++;

   }
   // free resources used in this function
   free(path_copy);
   free(path_copy2);

   /*
    to be freed:
        to_return ptr
        to_return->children
        to_return->children[i]
   */

   return to_return;
}
