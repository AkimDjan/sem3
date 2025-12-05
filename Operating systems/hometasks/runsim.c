#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_CMD_LEN 256
#define MAX_ARGS 32

void parse_command(char *line, char *argv[]) {
    int i = 0;
    char *token = strtok(line, " \t\n");
    while (token != NULL && i < MAX_ARGS - 1) {
        argv[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    argv[i] = NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <max_processes>\n", argv[0]);
        return 1;
    }
    int max_procs = atoi(argv[1]);
    if (max_procs <= 0) {
        fprintf(stderr, "Error: max_processes must be positive\n");
        return 1;
    }
    char line[MAX_CMD_LEN];
    int running = 0;
    while (1) {
        while (running > 0) {
            pid_t done = waitpid(-1, NULL, WNOHANG);
            if (done <= 0) break;
            running--;
        }
        if (fgets(line, sizeof(line), stdin) == NULL) {
            while (running > 0) {
                wait(NULL);
                running--;
            }
            break;
        }
        if (line[0] == '\n' || line[0] == '\0') continue;
        if (running >= max_procs) {
            fprintf(stderr, "Error: maximum number of processes (%d) reached\n", max_procs);
            continue;
        }
        char *args[MAX_ARGS];
        parse_command(line, args);
        if (args[0] == NULL) continue;
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            continue;
        } else if (pid == 0) {
            execvp(args[0], args);
            perror("exec failed");
            exit(1);
        } else {
            running++;
        }
    }
    return 0;
}
