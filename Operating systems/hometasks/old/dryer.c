#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_LINE 128

int get_time(const char *filename, const char *type) {
    FILE *f = fopen(filename, "r");
    if (!f) { perror(filename); exit(1); }

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
    int fd = open(fifo_path, O_RDONLY);
    if (fd < 0) { perror("open fifo"); exit(1); }

    char type[32];
    while (read(fd, type, sizeof(type)) > 0) {
        int t = get_time("dryer.txt", type);
        printf("Dryer: drying %s (%d sec)\n", type, t);
        sleep(t);
    }

    close(fd);
    unlink(fifo_path);
    return 0;
}
