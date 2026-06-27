#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    const char *limit_str = getenv("TABLE_LIMIT");
    if (!limit_str) {
        fprintf(stderr, "Error: set TABLE_LIMIT\n");
        exit(1);
    }

    printf("TABLE_LIMIT = %s\n", limit_str);

    pid_t washer = fork();
    if (washer == 0) {
        execl("./washer", "washer", NULL);
        perror("exec washer");
        exit(1);
    }

    pid_t dryer = fork();
    if (dryer == 0) {
        execl("./dryer", "dryer", NULL);
        perror("exec dryer");
        exit(1);
    }

    wait(NULL);
    wait(NULL);

    printf("All done.\n");
    return 0;
}
