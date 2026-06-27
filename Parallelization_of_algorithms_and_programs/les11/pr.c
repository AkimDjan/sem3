#include <stdbool.h>
#include <omp.h>
#include <stdio.h>

int main() {
    int count = 0;
    bool flag[2];
    int turn;

    flag[0] = flag[1] = false;
    turn = 0;

    #pragma omp parallel num_threads(2) shared(flag, count, turn)
    {
        int id = omp_get_thread_num();
        int other = 1 - id;

        for (int iter = 0; iter < 100; ++iter) {
            flag[id] = true;
            turn = other;
            #pragma omp flush(flag, turn)

            while (flag[other] && turn == other) {

            }

            for (int j = 0; j < 1000000; ++j) {
                count++;
            }

            flag[id] = false;
            #pragma omp flush(flag)
        }
    }

    printf("count = %d\n", count);
    return 0;
}
