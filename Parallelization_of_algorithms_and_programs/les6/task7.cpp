#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


void Check_For_SUCCESS(int MPI_FUNC) {if (MPI_FUNC != MPI_SUCCESS) {MPI_Abort(MPI_COMM_WORLD, -1);}}

int main(int argc, char* argv[]) {
    const int N = 120; int rank, size;
    Check_For_SUCCESS(MPI_Init(&argc, &argv));
    Check_For_SUCCESS(MPI_Comm_rank(MPI_COMM_WORLD, &rank));
    Check_For_SUCCESS(MPI_Comm_size(MPI_COMM_WORLD, &size));
    if (N % size != 0) {MPI_Abort(MPI_COMM_WORLD, -1); return -1;}
    int local_n = N / size;
    int *local_arr = static_cast<int*>(malloc(local_n * sizeof(int))); 
    for (int i = 0; i < local_n; i++) {local_arr[i] = rank;}
    int *global_arr = NULL;
    if (rank == 0) {global_arr = static_cast<int*>(malloc(N * sizeof(int)));}
    Check_For_SUCCESS(MPI_Gather(local_arr, local_n, MPI_INT, global_arr, local_n, MPI_INT, 0, MPI_COMM_WORLD));
    if (rank == 0) {
        FILE *f = fopen("out.txt", "w");
        if (!f) {MPI_Abort(MPI_COMM_WORLD, -1);}
        for (int i = 0; i < N; i++) {fprintf(f, "%d ", global_arr[i]);}
        fclose(f);
        free(global_arr);
    }
    free(local_arr);
    MPI_Finalize();
    return 0;
}