#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_LINE 128

typedef struct {
    char type[32];
    int time;
} DishTime;

int get_time(const char *filename, const char *type) {
    FILE *f = fopen(filename, "r");
    if (!f) { perror(filename); exit(1); }

    DishTime d;
    while (fscanf(f, "%31[^:]: %d\n", d.type, &d.time) == 2) {
        if (strcmp(d.type, type) == 0) {
            fclose(f);
            return d.time;
        }
    }
    fclose(f);
    fprintf(stderr, "No time for %s in %s\n", type, filename);
    exit(1);
}

int main() {
    const char *fifo_path = "table.fifo";
    mkfifo(fifo_path, 0666);

    int fd = open(fifo_path, O_WRONLY);
    if (fd < 0) { perror("open fifo"); exit(1); }

    FILE *dirty = fopen("dishes.txt", "r");
    if (!dirty) { perror("dishes.txt"); exit(1); }

    char type[32];
    int count;

    while (fscanf(dirty, "%31[^:]: %d\n", type, &count) == 2) {
        for (int i = 0; i < count; i++) {
            int t = get_time("washer.txt", type);
            printf("Washer: washing %s (%d sec)\n", type, t);
            sleep(t);
            write(fd, type, strlen(type) + 1);
        }
    }

    close(fd);
    fclose(dirty);
    return 0;
}
