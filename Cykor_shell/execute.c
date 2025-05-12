#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>
#include "shell.h"

#define MAX_ARGS 64

int execute(char *cmd) {
    char *args[MAX_ARGS];
    int i = 0;
    args[i] = strtok(cmd, " ");
    while (args[i] && i < MAX_ARGS - 1) args[++i] = strtok(NULL, " ");
    args[i] = NULL;
    if (!args[0]) return 0;

    if (strcmp(args[0], "exit") == 0) {
        printf("Bye!\n");
        exit(0);
    }
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "cd: 인자가 필요해\n");
        } else if (chdir(args[1]) != 0) {
            perror("cd 실패");
        }
        return 0;
    }
    if (strcmp(args[0], "pwd") == 0) {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)))
            printf("%s\n", cwd);
        else
            perror("pwd 실패");
        return 0;
    }

    int background = 0;
    for (int j = 0; args[j]; j++) {
        if (strcmp(args[j], "&") == 0) {
            background = 1;
            args[j] = NULL;
            break;
        }
    }

    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        perror("exec 실패");
        exit(1);
    } else if (!background) {
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }
    return 0;
}