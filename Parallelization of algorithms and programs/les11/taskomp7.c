#include <stdio.h>
#include <omp.h>

int main() {
    int count = 0;
    int ready[2] = {0, 0};
    int turn = 0;
#pragma omp parallel num_threads(2) shared(count, ready, turn)
    { 
        int i = omp_get_thread_num();
        int j;
        for (j = 0; j < 1000000; j++) {
            ready[i] = 1;
            turn = 1 - i;
            #pragma omp flush(ready, turn)
            while (1) {
                if (!(ready[1 - i] && turn == (1 - i))) {break;}
            }
            count++;
            ready[i] = 0;
            #pragma omp flush(ready)
        }
    }
    printf("count = %d\n", count);  
    return 0;
}
