#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>

void Check_For_SUCCESS(int MPI_FUNC) {if (MPI_FUNC != MPI_SUCCESS) {MPI_Abort(MPI_COMM_WORLD, -1);}}

int main(int argc, char *argv[]){    
    double *u, *unew, *u_global, delta, maxdelta, local_maxdelta;
    double eps=1.e-6;
    double h, tau;
    int N; int i; int t;
    double global_maxdelta;
    int count = 0;
    int rank, size;
    int jbeg, jend;
    int *recvcounts, *displs;
    FILE *ff;
    int provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_SERIALIZED, &provided);
    Check_For_SUCCESS(MPI_Comm_rank(MPI_COMM_WORLD, &rank));
    Check_For_SUCCESS(MPI_Comm_size(MPI_COMM_WORLD, &size));
    if (provided < MPI_THREAD_SERIALIZED) {
        if (rank == 0) printf("MPI_THREAD_SERIALIZED not supported\n");
        MPI_Abort(MPI_COMM_WORLD, -1);
    }
    //omp_set_num_threads(3);
    if(argc != 2){
        if(rank == 0) printf("Usage: exefile npoints\n");
        MPI_Abort(MPI_COMM_WORLD, -1);
    }
    N = atoi(argv[1]);
    if (N == 0) {
        if (rank == 0) printf("Set N to 1000\n");
        N = 1000;
    } else if (rank == 0) {
        printf("Set N to %d\n", N);
    }
    int local_n = (N-1) / size;
    int rem = (N-1) % size;
    if(rank < rem) {
        local_n++;
        jbeg = rank * local_n + 1;
    } else {jbeg = rank * local_n + rem + 1;}
    jend = jbeg + local_n - 1;
    if((u = malloc((local_n + 2)*sizeof(double))) == NULL){
        printf("Process %d: Can't allocate memory for u\n", rank);
        MPI_Abort(MPI_COMM_WORLD, -1);
    }
    if((unew = malloc((local_n + 2)*sizeof(double))) == NULL){
        printf("Process %d: Can't allocate memory for unew\n", rank);
        free(u);
        MPI_Abort(MPI_COMM_WORLD, -1);
    }
    // Инициализация массивов
    for(i = 0; i <= local_n + 1; i++){
        u[i] = 0; 
        unew[i] = 0;
    }
    // Граничные условия
    if(rank == 0) {
        unew[0] = u[0] = 1.0;
    }
    if(rank == size-1) {
        unew[local_n+1] = u[local_n+1] = 0.0;
    }
    h = 1.0/N;
    tau = 0.5*(h*h);
    global_maxdelta = 0.0;  
    double *thread_maxes = NULL;
    int num_threads = 0;  
#pragma omp parallel private(delta) shared(maxdelta, global_maxdelta, count)
    {
#pragma omp single
        {
            num_threads = omp_get_num_threads();
            thread_maxes = (double*)malloc(num_threads * sizeof(double));
        }
        int my_thread_id = omp_get_thread_num();
        while(1){   
            // Распараллеливание вычисления нового состояния
#pragma omp for
            for (i=1; i<=local_n; i++){
                unew[i] = u[i] + (tau/(h*h))*(u[i-1] - 2*u[i] + u[i+1]); 
            }
            thread_maxes[my_thread_id] = 0.0;
#pragma omp for
            for (i=1; i<=local_n; i++){
                delta = fabs(unew[i]-u[i]);
                // сразу обновляем в массиве, сравнивая с текущим значением для этого потока
                if (delta > thread_maxes[my_thread_id]) {
                    thread_maxes[my_thread_id] = delta;
                }
            }
#pragma omp single
            {   
                local_maxdelta = 0.0;
                for (t = 0; t < num_threads; t++) {
                    if (thread_maxes[t] > local_maxdelta) {
                        local_maxdelta = thread_maxes[t];
                    }
                }
                maxdelta = local_maxdelta;
                Check_For_SUCCESS(MPI_Reduce(&maxdelta, &global_maxdelta, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD));
                Check_For_SUCCESS(MPI_Bcast(&global_maxdelta, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD));
            }            
            // Проверка условия выхода (все потоки проверяют)
            if(global_maxdelta < eps) break;
#pragma omp single
            {
            // Увеличение счетчика в одном потоке
                count++;
            }
            // Обмен граничными элементами (выполняется в одном потоке)
#pragma omp single
            {
                if(rank%2==0) {
                    if(rank < size-1) {
                        Check_For_SUCCESS(MPI_Send(&unew[local_n], 1, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD));
                        Check_For_SUCCESS(MPI_Recv(&unew[local_n+1], 1, MPI_DOUBLE, rank+1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE));
                    }
                    if(rank > 0) {
                        Check_For_SUCCESS(MPI_Send(&unew[1], 1, MPI_DOUBLE, rank-1, 1, MPI_COMM_WORLD));
                        Check_For_SUCCESS(MPI_Recv(&unew[0], 1, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE));
                    }
                } else {
                    if(rank > 0) {
                        Check_For_SUCCESS(MPI_Recv(&unew[0], 1, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE));
                        Check_For_SUCCESS(MPI_Send(&unew[1], 1, MPI_DOUBLE, rank-1, 1, MPI_COMM_WORLD));
                    }
                    if(rank < size-1) {
                        Check_For_SUCCESS(MPI_Recv(&unew[local_n+1], 1, MPI_DOUBLE, rank+1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE));
                        Check_For_SUCCESS(MPI_Send(&unew[local_n], 1, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD));
                    }
                }                
                // Установка граничных значений после обмена
                if (rank == 0) unew[0] = 1.0;
                if (rank == size-1) unew[local_n+1] = 0.0;
            }
            // Копирование данных (все потоки участвуют)
#pragma omp for
            for(i=0; i<=local_n+1; i++){
                u[i] = unew[i];
            }
        }
    }
    if(rank == 0) {
        printf("%d steps\n", count);
        recvcounts = malloc(size*sizeof(int));
        displs = malloc(size*sizeof(int));
        for(i=0; i<size; i++) {
            int loc_n = (N-1) / size;
            if(i < rem) loc_n++;
            recvcounts[i] = loc_n;
        }
        displs[0] = 1;
        for(i=1; i<size; i++) {
            displs[i] = displs[i-1] + recvcounts[i-1];
        }
        u_global = malloc((N+1) * sizeof(double));
        u_global[0] = 1.0;
        u_global[N] = 0.0;
    }
    Check_For_SUCCESS(MPI_Gatherv(&unew[1], local_n, MPI_DOUBLE, rank == 0 ? u_global : NULL, rank == 0 ? recvcounts : NULL, rank == 0 ? displs : NULL, MPI_DOUBLE, 0, MPI_COMM_WORLD));
    if(rank == 0) {
        if ((ff = fopen("seq_mpi", "w+")) == NULL){
            printf("Can't open file\n");
        } else {
            for(i=0; i<N+1; i++)
                fprintf(ff, "%f\n", u_global[i]);
            fclose(ff);
        }
        free(u_global);
        free(recvcounts);
        free(displs);
    }
    free(u); free(unew);
    MPI_Finalize();
    return 0;  
}