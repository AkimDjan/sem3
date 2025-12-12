// dryer.c
#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <errno.h>

#define RECORD_SIZE 32

static int safe_read_all(int fd, void *buf, size_t count) {
    char *p = buf;
    while (count > 0) {
        ssize_t r = read(fd, p, count);
        if (r < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        if (r == 0) return 0;
        p += r;
        count -= r;
    }
    return 1;
}

int get_time(const char *filename, const char *type) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror(filename);
        exit(1);
    }
    char line[256];
    char t[128];
    int sec;
    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, " %127[^: ] : %d", t, &sec) == 2) {
            if (strcmp(t, type) == 0) {
                fclose(f);
                return sec;
            }
        }
    }
    fclose(f);
    fprintf(stderr, "No time for %s in %s\n", type, filename);
    exit(1);
}

int main() {
    const char *fifo_path = "table.fifo";
    sem_t *table_sem = sem_open("/table_sem", 0);
    if (table_sem == SEM_FAILED) {
        perror("dryer: sem_open failed");
        return 1;
    }

    int fd = open(fifo_path, O_RDONLY);
    if (fd < 0) {
        perror("dryer: open fifo failed");
        sem_close(table_sem);
        return 1;
    }

    char buf[RECORD_SIZE];
    while (1) {
        int rr = safe_read_all(fd, buf, RECORD_SIZE);
        if (rr == -1) {
            perror("dryer: read failed");
            break;
        }
        if (rr == 0) break;
        buf[RECORD_SIZE - 1] = '\0';
        char type[RECORD_SIZE];
        strncpy(type, buf, RECORD_SIZE - 1);
        type[RECORD_SIZE - 1] = '\0';
        if (type[0] == '\0') continue;
        printf("Dryer: got %s from table\n", type);
        fflush(stdout);
        int dry_time = get_time("dryer.txt", type);
        printf("Dryer: drying %s (%d sec)\n", type, dry_time);
        fflush(stdout);
        sleep(dry_time);
        while (sem_post(table_sem) == -1) {
            if (errno == EINTR) continue;
            perror("dryer: sem_post failed");
            break;
        }
        printf("Dryer: finished drying %s (free space available)\n", type);
        fflush(stdout);
    }

    close(fd);
    sem_close(table_sem);
    return 0;
}
