#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

void Check_For_SUCCESS(int MPI_FUNC) {if (MPI_FUNC != MPI_SUCCESS) {MPI_Abort(MPI_COMM_WORLD, -1);}}

int main(int argc, char *argv[]){    
    double *u, *unew, *u_global, delta, maxdelta;
    double eps=1.e-6;
    double h, tau;
    int N; int i;
    double global_maxdelta;
    int count = 0;
    int rank, size;
    int jbeg, jend;
    int *recvcounts, *displs;
    FILE *ff;
    Check_For_SUCCESS(MPI_Init(&argc, &argv));
    Check_For_SUCCESS(MPI_Comm_rank(MPI_COMM_WORLD, &rank));
    Check_For_SUCCESS(MPI_Comm_size(MPI_COMM_WORLD, &size));
    if(argc != 2){
        if(rank == 0) printf("Usage: exefile npoints\n");
        MPI_Abort(MPI_COMM_WORLD, -1);
    }
    N = atoi(argv[1]);
    if (N == 0) {
        if (rank == 0) printf("Set N to 1000\n");
        N = 1000;
    } else {if (rank == 0) printf("Set N to %d\n", N);}
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
    for(i = 0; i <= local_n + 1; i++){
        u[i] = 0; unew[i] = 0;
    }
    if(rank == 0) {
        unew[0] = u[0] = 1.0;
    }
    if(rank == size-1) {
        unew[local_n+1] = u[local_n+1] = 0.0;
    }
    h = 1.0/N;
    tau = 0.5*(h*h);
    global_maxdelta = 0.0;
    while(1){   
        for (i=1; i<=local_n; i++){
            unew[i] = u[i] + (tau/(h*h))*(u[i-1] - 2*u[i] + u[i+1]); 
        }
        maxdelta = 0;
        for (i=1; i<=local_n; i++){
            delta = fabs(unew[i]-u[i]);
            if(delta > maxdelta) maxdelta = delta;
        }    
        Check_For_SUCCESS(MPI_Reduce(&maxdelta, &global_maxdelta, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD));
        Check_For_SUCCESS(MPI_Bcast(&global_maxdelta, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD));
        if(global_maxdelta < eps) {break;}
        count++;
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
        if (rank == 0) unew[0] = 1.0;
        if (rank == size-1) unew[local_n+1] = 0.0;
        for(i=0; i<=local_n+1; i++){
            u[i] = unew[i];
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
