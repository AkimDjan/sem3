// controller.c
#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

const char *fifo_in = "table_in.fifo";   // washer -> dryer
const char *fifo_out = "table_out.fifo"; // dryer -> washer

pid_t washer_pid = -1;
pid_t dryer_pid = -1;

void cleanup() {
    unlink(fifo_in);
    unlink(fifo_out);
}

void term_handler(int sig) {
    if (washer_pid > 0) kill(washer_pid, SIGTERM);
    if (dryer_pid > 0) kill(dryer_pid, SIGTERM);
    cleanup();
    _exit(1);
}

static int write_all(int fd, const void *buf, size_t n) {
    const char *p = buf;
    while (n > 0) {
        ssize_t w = write(fd, p, n);
        if (w < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        p += w;
        n -= w;
    }
    return 0;
}

int main() {
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
    unlink(fifo_in);
    unlink(fifo_out);
    if (mkfifo(fifo_in, 0666) == -1) {
        perror("mkfifo table_in");
        return 1;
    }
    if (mkfifo(fifo_out, 0666) == -1) {
        perror("mkfifo table_out");
        unlink(fifo_in);
        return 1;
    }
    int ctrl_in_fd = open(fifo_in, O_RDWR);
    if (ctrl_in_fd < 0) {
        perror("controller open fifo_in O_RDWR");
        cleanup();
        return 1;
    }
    int ctrl_out_fd = open(fifo_out, O_RDWR);
    if (ctrl_out_fd < 0) {
        perror("controller open fifo_out O_RDWR");
        close(ctrl_in_fd);
        cleanup();
        return 1;
    }

    char token = 'T';
    for (int i = 0; i < table_limit; ++i) {
        // кладем токены свободных мест на стол
        if (write_all(ctrl_out_fd, &token, 1) == -1) {
            perror("controller write initial tokens");
            close(ctrl_in_fd);
            close(ctrl_out_fd);
            cleanup();
            return 1;
        }
    }

    washer_pid = fork();
    if (washer_pid == 0) {
        execl("./washer", "washer", NULL);
        perror("exec washer");
        _exit(1);
    } else if (washer_pid < 0) {
        perror("fork washer");
        close(ctrl_in_fd);
        close(ctrl_out_fd);
        cleanup();
        return 1;
    }

    dryer_pid = fork();
    if (dryer_pid == 0) {
        execl("./dryer", "dryer", NULL);
        perror("exec dryer");
        _exit(1);
    } else if (dryer_pid < 0) {
        perror("fork dryer");
        kill(washer_pid, SIGTERM);
        close(ctrl_in_fd);
        close(ctrl_out_fd);
        cleanup();
        return 1;
    }

    // ждем обоих детей
    int status;
    waitpid(washer_pid, &status, 0);
    waitpid(dryer_pid, &status, 0);
    close(ctrl_in_fd);
    close(ctrl_out_fd);
    cleanup();
    printf("All done.\n");
    return 0;
}
