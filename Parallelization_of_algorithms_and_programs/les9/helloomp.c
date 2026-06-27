#include <stdio.h>
#include <omp.h>

int main() {
    printf("Ammount of cores on thread: %d\n", omp_get_num_procs());
    omp_set_num_threads(6);
#pragma omp parallel
    {
        printf("Hello, OMP! My number: %d, total number of threads: %d \n", omp_get_thread_num(), omp_get_num_threads());
    }
    printf("Last number: %d, total number of threads: %d \n", omp_get_thread_num(), omp_get_num_threads());
}