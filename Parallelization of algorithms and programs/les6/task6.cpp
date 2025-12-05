#include <mpi.h>
#include <iostream>
#include <cmath>
#include <fstream>


void Check_For_SUCCESS(int MPI_FUNC) {if (MPI_FUNC != MPI_SUCCESS) {MPI_Abort(MPI_COMM_WORLD, 1);}}

double f(double x) {return sqrt(4-x*x);}

int main(int argc, char* argv[]) {
    int rank, size;
    Check_For_SUCCESS(MPI_Init(&argc, &argv));
    Check_For_SUCCESS(MPI_Comm_rank(MPI_COMM_WORLD, &rank));
    Check_For_SUCCESS(MPI_Comm_size(MPI_COMM_WORLD, &size));
    long long N=0;
    if (rank == 0) {
        std::ifstream fin("N.dat");
        if (!fin) {
            std::cerr << "Can't open N.dat\n";
            MPI_Abort(MPI_COMM_WORLD, -1);
        }
        fin >> N; fin.close();
    } 
    Check_For_SUCCESS(MPI_Bcast(&N, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD)); 
    double h = 2.0/static_cast<double>(N);
    long long min_seg = N/size; int ost = N%size;
    long long local_n = min_seg+(rank<ost ? 1 : 0);
    long long start_idx=0;
    if (rank < ost) {start_idx = rank*(min_seg+1);}
    else {start_idx = ost*(min_seg+1)+(rank-ost)*min_seg;}
    double local_sum = 0.0;
    for (long long i=0; i<local_n; i++) {
        long long index = start_idx+i;
        double x_i_plus_1 = (index+1)*h; double x_i = index*h;
        double S_i = 0.5*h*(f(x_i) + f(x_i_plus_1));
        local_sum += S_i;
    }
    double pi = 0.0;
    Check_For_SUCCESS(MPI_Reduce(&local_sum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD));
    if (rank == 0) {
        std::cout.precision(17);
        std::cout <<"pi=" << pi << "\n";
    }
    MPI_Finalize();
    return 0;
}