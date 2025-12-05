#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <errno.h>

#define MAX_LINE 128

int get_time(const char *filename, const char *type) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror(filename);
        exit(1);
    }
    char t[32];
    int sec;
    while (fscanf(f, "%31[^:]: %d\n", t, &sec) == 2) {
        if (strcmp(t, type) == 0) {
            fclose(f);
            return sec;
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
    char type[32];
    int count;
    while (fscanf(dirty, "%31[^:]: %d\n", type, &count) == 2) {
        for (int i = 0; i < count; i++) {
            if (sem_wait(table_sem) == -1) {
                perror("washer: sem_wait failed");
                break;
            }
            int wash_time = get_time("washer.txt", type);
            printf("Washer: washing %s (%d sec)\n", type, wash_time);
            fflush(stdout);
            sleep(wash_time);
            if (write(fd, type, strlen(type) + 1) == -1) {
                perror("washer: write to fifo failed");
                sem_post(table_sem); 
                break;
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