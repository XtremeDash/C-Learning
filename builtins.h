#ifndef BUILTINS_H
#define BUILTINS_H

int parse(char *input, char *args[]);
void clear(void);
void read_line(char *buffer, int size);
void exec_pipe(char *left[], char *right[]);

#endif