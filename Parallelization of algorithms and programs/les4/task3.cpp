#include <iostream>
#include <mpi.h>
using namespace std;

void Check_For_SUCCESS(int MPI_FUNC) {
    if (MPI_FUNC != MPI_SUCCESS) {MPI_Abort(MPI_COMM_WORLD, -1);}
}

int main(int argc, char* argv[]) {
    Check_For_SUCCESS(MPI_Init(&argc, &argv));
    int size; int rank;
    Check_For_SUCCESS(MPI_Comm_size(MPI_COMM_WORLD, &size));
    Check_For_SUCCESS(MPI_Comm_rank(MPI_COMM_WORLD, &rank));
    cout<<"my size: "<< size<< " ; my rank: "<<rank<<"  ;"<<"Hello, world!\n";
    MPI_Finalize();
}