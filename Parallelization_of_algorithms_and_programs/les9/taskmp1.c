#include <stdio.h>

int main() { //последовательная часть
    printf("Sequential part 1\n");
#pragma omp parallel
    {
        printf("Parallel part\n");    
    }
    printf("Sequential part 2\n");
    return 0;
}