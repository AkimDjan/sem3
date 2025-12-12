// washer.c
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

static int safe_write_all(int fd, const void *buf, size_t count) {
    const char *p = buf;
    while (count > 0) {
        ssize_t w = write(fd, p, count);
        if (w < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        p += w;
        count -= w;
    }
    return 0;
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
        perror("washer: sem_open failed");
        return 1;
    }

    int fd = open(fifo_path, O_WRONLY);
    if (fd < 0) {
        perror("washer: open fifo failed");
        sem_close(table_sem);
        return 1;
    }

    FILE *dirty = fopen("dishes.txt", "r");
    if (!dirty) {
        perror("dishes.txt");
        close(fd);
        sem_close(table_sem);
        return 1;
    }

    char line[256];
    char type[128];
    int count;
    while (fgets(line, sizeof(line), dirty)) {
        if (sscanf(line, " %127[^: ] : %d", type, &count) != 2) continue;
        for (int i = 0; i < count; ++i) {
            while (sem_wait(table_sem) == -1) {
                if (errno == EINTR) continue;
                perror("washer: sem_wait failed");
                fclose(dirty);
                close(fd);
                sem_close(table_sem);
                return 1;
            }
            int wash_time = get_time("washer.txt", type);
            printf("Washer: washing %s (%d sec)\n", type, wash_time);
            fflush(stdout);
            sleep(wash_time);
            char buf[RECORD_SIZE];
            memset(buf, 0, sizeof(buf));
            strncpy(buf, type, RECORD_SIZE - 1);
            if (safe_write_all(fd, buf, RECORD_SIZE) == -1) {
                perror("washer: write to fifo failed");
                sem_post(table_sem);
                fclose(dirty);
                close(fd);
                sem_close(table_sem);
                return 1;
            }
            printf("Washer: put %s on the table\n", type);
            fflush(stdout);
        }
    }

    fclose(dirty);
    close(fd);
    sem_close(table_sem);
    return 0;
}
