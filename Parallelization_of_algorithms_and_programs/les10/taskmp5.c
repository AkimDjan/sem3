#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

double f(double x) {
    return sqrt(4 - x * x);
}

int main() {
    long long N = 0;
    FILE *fin = fopen("N.dat", "r");
    if (!fin) {
        printf("Can't open N.dat\n");
        return -1;
    }
    fscanf(fin, "%lld", &N);
    fclose(fin);
    if (N <= 0) {
        printf("Invalid N value: %lld\n", N);
        return -1;
    }
    int num_cores = omp_get_num_procs();
    omp_set_num_threads(num_cores);
    double h = 2.0 / (double)N;  
    double pi = 0.0;             
    #pragma omp parallel reduction(+:pi)
    {
        int num_threads = omp_get_num_threads();
        int thread_id = omp_get_thread_num();
        long long chunk_size = N / num_threads;
        long long remainder = N % num_threads; 
        long long start_i = thread_id * chunk_size + (thread_id < remainder ? thread_id : remainder);
        long long end_i = start_i + chunk_size + (thread_id < remainder ? 1 : 0);
        double local_sum = 0.0;
        for (long long i = start_i; i < end_i; i++) {
            double x_i = i * h;           
            double x_i_plus_1 = (i + 1) * h;
            double S_i = 0.5 * h * (f(x_i) + f(x_i_plus_1));
            local_sum += S_i;
        }
        pi = local_sum;
    }
    printf("pi = %.17f\n", pi);
    printf("N = %lld segments\n", N);
    printf("Used %d threads\n", omp_get_max_threads());
    return 0;
}