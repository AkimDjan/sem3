// washer.c
#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define RECORD_SIZE 32
#define MAX_LINE 256

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

static ssize_t safe_read_some(int fd, void *buf, size_t n) {
    char *p = buf;
    while (1) {
        ssize_t r = read(fd, p, n);
        if (r < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        return r;
    }
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
    const char *fifo_in = "table_in.fifo";   // washer -> dryer
    const char *fifo_out = "table_out.fifo"; // dryer -> washer 
    // открываем fifo: читаем токены освобождения в table_out, пишем вымытую посуду в table_in
    int fd_out = open(fifo_out, O_RDONLY);
    if (fd_out < 0) {
        perror("washer: open table_out for read");
        return 1;
    }
    int fd_in = open(fifo_in, O_WRONLY);
    if (fd_in < 0) {
        perror("washer: open table_in for write");
        close(fd_out);
        return 1;
    }
    FILE *dirty = fopen("dishes.txt", "r");
    if (!dirty) {
        perror("dishes.txt");
        close(fd_out);
        close(fd_in);
        return 1;
    }
    char line[MAX_LINE];
    char type[RECORD_SIZE];
    int count;
    while (fgets(line, sizeof(line), dirty)) {
        if (sscanf(line, " %31[^: ] : %d", type, &count) != 2) continue;
        for (int i = 0; i < count; ++i) {
            // синхронизация: резервируем слот на столе
            // прочитываем токен из table_out.fifo и если токенов нет, чтение блокируется до момента, когда dryer/контроллер запишут токен
            char token;
            ssize_t r = safe_read_some(fd_out, &token, 1);
            if (r <= 0) {
                // ошибка чтения или EOF, завершаем
                if (r < 0) perror("washer: read token failed");
                break;
            }
            // теперь слот зарезервирован, моем
            int wash_time = get_time("washer.txt", type);
            printf("Washer: washing %s (%d sec)\n", type, wash_time);
            fflush(stdout);
            sleep(wash_time);
            // передача: кладём вымытую посуду на стол через table_in.fifo
            char buf[RECORD_SIZE];
            memset(buf, 0, sizeof(buf));
            strncpy(buf, type, RECORD_SIZE - 1);
            if (safe_write_all(fd_in, buf, RECORD_SIZE) == -1) {
                perror("washer: write to table_in failed");
                close(fd_out);
                close(fd_in);
                fclose(dirty);
                return 1;
            }
            printf("Washer: put %s on the table\n", type);
            fflush(stdout);
        }
    }
    // закрываем запись в table_in, чтобы dryer получил EOF после всех предметов
    fclose(dirty);
    close(fd_in);
    // закрываем дескриптор чтения токенов и завершаем (controller держит запись открытой)
    close(fd_out);
    return 0;
}
