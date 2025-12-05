#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#define MAX_CMD_LENGTH 1024

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <taskfile>\n", argv[0]);
        return 1;
    }
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    char line[MAX_CMD_LENGTH];
    int line_number = 0;
    while (fgets(line, sizeof(line), file)) {
        line_number++;
        if (strlen(line) <= 1) continue;
        line[strcspn(line, "\n")] = '\0';
        int delay;
        char command[MAX_CMD_LENGTH];
        if (sscanf(line, "%d %1023[^\n]", &delay, command) != 2) {
            fprintf(stderr, "Error in string %d: invalid format\n", line_number);
            continue;
        }
        pid_t pid = fork();
        if (pid == 0) {
            sleep(delay);
            char* args[64];
            int arg_count = 0;
            char* token = strtok(command, " ");
            while (token != NULL && arg_count < 63) {
                args[arg_count++] = token;
                token = strtok(NULL, " ");
            }
            args[arg_count] = NULL;
            execvp(args[0], args);
            perror("exec failed");
            exit(1);
        } else if (pid > 0) {
            printf("Planned comand: '%s' with delay %d seconds (PID: %d)\n", command, delay, pid);
        } else {
            perror("fork failed");
        }
    }
    fclose(file);
    int status;
    while (wait(&status) > 0) {
        if (WIFEXITED(status)) {
            printf("Process ended with code: %d\n", WEXITSTATUS(status));
        }
    }
    printf("All comands are executed!\n");
    return 0;
}