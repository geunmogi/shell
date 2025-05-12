#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "shell.h"

#define MAX_ARGS 64

void handle_pipe(char *cmds[], int num_pipes) {
    int pipefd[2 * (num_pipes - 1)];
    for (int i = 0; i < num_pipes - 1; i++) {
        if (pipe(pipefd + i*2) < 0) {
            perror("pipe");
            exit(1);
        }
    }

    int j = 0;
    for (int i = 0; i < num_pipes; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            if (i != 0) {
                dup2(pipefd[(i - 1) * 2], 0);
            }
            if (i != num_pipes - 1) {
                dup2(pipefd[i * 2 + 1], 1);
            }
            for (int k = 0; k < 2 * (num_pipes - 1); k++) close(pipefd[k]);

            char *args[MAX_ARGS];
            int x = 0;
            args[x] = strtok(cmds[i], " ");
            while (args[x] && x < MAX_ARGS - 1) args[++x] = strtok(NULL, " ");
            args[x] = NULL;
            execvp(args[0], args);
            perror("pipe exec");
            exit(1);
        }
    }
    for (int i = 0; i < 2 * (num_pipes - 1); i++) close(pipefd[i]);
    for (int i = 0; i < num_pipes; i++) wait(NULL);
}