/* MAIN PROGRAM ABSTRACTION INTERFACE */
#ifndef READER_H
#define READER_H
#include "gtree.h"
#include "path.h"



NODE* find_node_from_path(PATH *path, int upto);
void initialize();
void mkdir(char *path);
void cd(char *path);
void touch(char *path);
void ls(char *path);
void pwd();
void mainloop();
void rmsimple(char *path, int isDir);

#define rm(x) rmsimple((x), 0)
#define rmdir(x) rmsimple((x), 1)

#endif