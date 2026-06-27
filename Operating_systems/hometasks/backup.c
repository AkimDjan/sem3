#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>

void compress_file(const char *path) {
    pid_t pid = fork();
    if (pid == 0) {
        execlp("gzip", "gzip", "-f", path, (char *)NULL);
        perror("execlp gzip failed");
        exit(1);
    } else if (pid > 0) {
        waitpid(pid, NULL, 0);
    } else {
        perror("fork failed");
    }
}

int copy_file(const char *src, const char *dst) {
    int fd_src = open(src, O_RDONLY);
    if (fd_src < 0) {
        perror(src);
        return -1;
    }
    int fd_dst = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_dst < 0) {
        perror(dst);
        close(fd_src);
        return -1;
    }
    char buf[4096];
    ssize_t n;
    while ((n = read(fd_src, buf, sizeof(buf))) > 0) {
        if (write(fd_dst, buf, n) != n) {
            perror("write error");
            close(fd_src);
            close(fd_dst);
            return -1;
        }
    }
    if (n < 0) perror("read error");
    close(fd_src);
    close(fd_dst);
    return 0;
}

void ensure_dir(const char *path) {
    struct stat st;
    if (stat(path, &st) == -1) {
        if (mkdir(path, 0755) == -1) {
            perror(path);
        }
    }
}

void backup_directory(const char *src_dir, const char *dst_dir) {
    DIR *dir = opendir(src_dir);
    if (!dir) {
        perror(src_dir);
        return;
    }
    ensure_dir(dst_dir);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        char src_path[512], dst_path[512], dst_gz_path[512];
        snprintf(src_path, sizeof(src_path), "%s/%s", src_dir, entry->d_name);
        snprintf(dst_path, sizeof(dst_path), "%s/%s", dst_dir, entry->d_name);
        snprintf(dst_gz_path, sizeof(dst_gz_path), "%s.gz", dst_path);
        struct stat st_src, st_dst;
        if (stat(src_path, &st_src) < 0) {
            perror(src_path);
            continue;
        }
        if (S_ISDIR(st_src.st_mode)) {
            backup_directory(src_path, dst_path);
        } else if (S_ISREG(st_src.st_mode)) {
            int need_copy = 0;
            if (stat(dst_gz_path, &st_dst) == -1) {
                need_copy = 1;
            } else if (st_src.st_mtime > st_dst.st_mtime) {
                need_copy = 1;
            }

            if (need_copy) {
                printf("Backing up %s → %s.gz\n", src_path, dst_path);
                if (copy_file(src_path, dst_path) == 0)
                    compress_file(dst_path);
            }
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_dir> <dest_dir>\n", argv[0]);
        return 1;
    }
    backup_directory(argv[1], argv[2]);
    return 0;
}
