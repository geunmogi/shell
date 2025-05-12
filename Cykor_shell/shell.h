#ifndef SHELL_H
#define SHELL_H

int execute(char *cmd);
void handle_pipe(char *cmds[], int num_pipes);

#endif