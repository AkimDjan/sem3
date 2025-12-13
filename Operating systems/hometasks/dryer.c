// dryer.c
#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define RECORD_SIZE 32

static int safe_write_all(int fd, const void *buf, size_t n) {
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

static int safe_read_all(int fd, void *buf, size_t n) {
    char *p = buf;
    while (n > 0) {
        ssize_t r = read(fd, p, n);
        if (r < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        if (r == 0) return 0; // EOF
        p += r;
        n -= r;
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
    const char *fifo_in = "table_in.fifo";   
    const char *fifo_out = "table_out.fifo"; 

    int fd_in = open(fifo_in, O_RDONLY);
    if (fd_in < 0) {
        perror("dryer: open table_in for read");
        return 1;
    }
    int fd_out = open(fifo_out, O_WRONLY);
    if (fd_out < 0) {
        perror("dryer: open table_out for write");
        close(fd_in);
        return 1;
    }

    char buf[RECORD_SIZE];
    while (1) {
        int rr = safe_read_all(fd_in, buf, RECORD_SIZE);
        if (rr == -1) {
            perror("dryer: read failed");
            break;
        }
        if (rr == 0) break; 
        buf[RECORD_SIZE-1] = '\0';
        char type[RECORD_SIZE];
        strncpy(type, buf, RECORD_SIZE-1);
        type[RECORD_SIZE-1] = '\0';
        if (type[0] == '\0') continue;

        printf("Dryer: got %s from table\n", type);
        fflush(stdout);
        int dry_time = get_time("dryer.txt", type);
        printf("Dryer: drying %s (%d sec)\n", type, dry_time);
        fflush(stdout);
        sleep(dry_time);

        char token = 'T';
        if (safe_write_all(fd_out, &token, 1) == -1) {
            perror("dryer: write token failed");
            break;
        }
        printf("Dryer: finished drying %s (free space available)\n", type);
        fflush(stdout);
    }
    close(fd_in);
    close(fd_out);
    return 0;
}
