#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>
#include "shell.h"

#define MAX_INPUT 1024
#define MAX_PIPE 10

int main() {
    char input[MAX_INPUT];

    while (1) {
        // 사용자, 호스트, 경로 표시
        char cwd[1024], hostname[256];
        struct passwd *pw = getpwuid(getuid());
        getcwd(cwd, sizeof(cwd));
        gethostname(hostname, sizeof(hostname));
        printf("%s@%s:%s$ ", pw->pw_name, hostname, cwd);
        fflush(stdout);

        // 입력
        if (fgets(input, MAX_INPUT, stdin) == NULL) break;
        input[strcspn(input, "\n")] = '\0';

        // 파이프라인 확인
        if (strchr(input, '|')) {
            char *cmds[MAX_PIPE];
            int count = 0;
            char *token = strtok(input, "|");
            while (token && count < MAX_PIPE) {
                cmds[count++] = token;
                token = strtok(NULL, "|");
            }
            handle_pipe(cmds, count);
            continue;
        }

        // 다중 명령어 처리 (;, &&, ||)
        char *saveptr;
        char *token = strtok_r(input, ";", &saveptr);
        while (token != NULL) {
            if (strstr(token, "&&")) {
                char *a = strtok(token, "&&");
                char *b = strtok(NULL, "&&");
                if (execute(a) == 0 && b) execute(b);
            } else if (strstr(token, "||")) {
                char *a = strtok(token, "||");
                char *b = strtok(NULL, "||");
                if (execute(a) != 0 && b) execute(b);
            } else {
                execute(token);
            }
            token = strtok_r(NULL, ";", &saveptr);
        }
    }
    return 0;
}