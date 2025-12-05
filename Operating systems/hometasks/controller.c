#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <signal.h>
#include <errno.h>

sem_t *table_sem = NULL;
const char *fifo_path = "table.fifo";

void cleanup() {
    if (table_sem != NULL) {
        sem_close(table_sem);
        sem_unlink("/table_sem");
    }
    unlink(fifo_path);
}

void signal_handler(int sig) {
    cleanup();
    exit(1);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    const char *limit_str = getenv("TABLE_LIMIT");
    if (!limit_str) {
        fprintf(stderr, "Error: set TABLE_LIMIT environment variable\n");
        return 1;
    }
    int table_limit = atoi(limit_str);
    if (table_limit <= 0) {
        fprintf(stderr, "Error: TABLE_LIMIT must be positive integer\n");
        return 1;
    }
    printf("TABLE_LIMIT = %d\n", table_limit);
    sem_unlink("/table_sem");
    unlink(fifo_path);
    table_sem = sem_open("/table_sem", O_CREAT | O_EXCL, 0644, table_limit);
    if (table_sem == SEM_FAILED) {
        perror("sem_open failed");
        return 1;
    }
    if (mkfifo(fifo_path, 0666) == -1) {
        perror("mkfifo failed");
        sem_close(table_sem);
        sem_unlink("/table_sem");
        return 1;
    }
    pid_t washer = fork();
    if (washer == 0) {
        execl("./washer", "washer", NULL);
        perror("exec washer failed");
        exit(1);
    } else if (washer < 0) {
        perror("fork washer failed");
        cleanup();
        return 1;
    }
    pid_t dryer = fork();
    if (dryer == 0) {
        execl("./dryer", "dryer", NULL);
        perror("exec dryer failed");
        exit(1);
    } else if (dryer < 0) {
        perror("fork dryer failed");
        kill(washer, SIGTERM); 
        cleanup();
        return 1;
    }
    int status;
    waitpid(washer, &status, 0);
    waitpid(dryer, &status, 0);
    cleanup();
    printf("All done.\n");
    return 0;
}