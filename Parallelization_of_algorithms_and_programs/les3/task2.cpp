#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char* argv[]) {
    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) {MPI_Abort(MPI_COMM_WORLD, -1);}
    cout<<"Hello, world!\n";
    MPI_Finalize();
}