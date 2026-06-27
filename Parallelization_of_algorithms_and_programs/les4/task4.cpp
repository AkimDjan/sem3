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
    if (size < 2) {
        if (rank == 0) {cout << "Need minimum 2 process\n";}
        MPI_Finalize();
        return 0;
    }
    int received_value;
    MPI_Status st;
    if (rank == 0) {
        int i = 0; int value_to_send = i; int next_process = 1;
        cout << "Process " << rank << ": starting sending data\n";
        cout << "Process " << rank << ": send value " << value_to_send << " to process " << next_process <<"\n";
        Check_For_SUCCESS(MPI_Send(&value_to_send, 1, MPI_INT, next_process, 1, MPI_COMM_WORLD));
        Check_For_SUCCESS(MPI_Recv(&received_value, 1, MPI_INT, size - 1, 1, MPI_COMM_WORLD, &st));
        cout << "Process " << rank << ": get value " << received_value << " from process " << st.MPI_SOURCE <<"\n";
        cout << "Circle has ended!\n";
    } 
    else {
        Check_For_SUCCESS(MPI_Recv(&received_value, 1, MPI_INT, rank - 1, 1, MPI_COMM_WORLD, &st));
        cout << "Process " << rank << ": get value " << received_value << " from process " << st.MPI_SOURCE << "\n";
        int value_to_send = received_value + rank;
        int next_process = (rank + 1) % size;
        cout << "Process " << rank << ": sending value " << value_to_send << " to process " << next_process << "\n";
        Check_For_SUCCESS(MPI_Send(&value_to_send, 1, MPI_INT, next_process, 1, MPI_COMM_WORLD));
    }
    MPI_Finalize();
    return 0;
}