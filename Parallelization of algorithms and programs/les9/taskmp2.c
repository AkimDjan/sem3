#include <stdio.h>
#include <omp.h>

int main() {
    printf("Sequential part 1\n");
    omp_set_num_threads(3);
#pragma omp parallel
    {
        printf("Parallel part\n");    
    }
    printf("Sequential part 2\n");
    return 0;
}