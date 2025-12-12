// controller.c
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
#include <string.h>

sem_t *table_sem = NULL;
const char *fifo_path = "table.fifo";
const char *sem_name = "/table_sem";
pid_t washer_pid = -1;
pid_t dryer_pid = -1;

void cleanup_children() {
    if (washer_pid > 0) kill(washer_pid, SIGTERM);
    if (dryer_pid > 0) kill(dryer_pid, SIGTERM);
}

void cleanup_all() {
    if (table_sem != NULL) {
        sem_close(table_sem);
        sem_unlink(sem_name);
        table_sem = NULL;
    }
    unlink(fifo_path);
}

void term_handler(int sig) {
    cleanup_children();
    cleanup_all();
    _exit(1);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, term_handler);
    signal(SIGTERM, term_handler);

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

    sem_unlink(sem_name);
    unlink(fifo_path);

    table_sem = sem_open(sem_name, O_CREAT | O_EXCL, 0644, table_limit);
    if (table_sem == SEM_FAILED) {
        perror("sem_open failed");
        return 1;
    }

    if (mkfifo(fifo_path, 0666) == -1) {
        perror("mkfifo failed");
        sem_close(table_sem);
        sem_unlink(sem_name);
        return 1;
    }

    washer_pid = fork();
    if (washer_pid == 0) {
        execl("./washer", "washer", NULL);
        perror("exec washer failed");
        _exit(1);
    } else if (washer_pid < 0) {
        perror("fork washer failed");
        cleanup_all();
        return 1;
    }

    dryer_pid = fork();
    if (dryer_pid == 0) {
        execl("./dryer", "dryer", NULL);
        perror("exec dryer failed");
        _exit(1);
    } else if (dryer_pid < 0) {
        perror("fork dryer failed");
        kill(washer_pid, SIGTERM);
        cleanup_all();
        return 1;
    }

    int status;
    waitpid(washer_pid, &status, 0);
    waitpid(dryer_pid, &status, 0);

    cleanup_all();
    printf("All done.\n");
    return 0;
}
