#include <mpi.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <stdlib.h>

using namespace std;

void Check_For_SUCCESS(int MPI_FUNC) {if (MPI_FUNC != MPI_SUCCESS) {MPI_Abort(MPI_COMM_WORLD, -1);}}

int main(int argc, char* argv[]) {
    Check_For_SUCCESS(MPI_Init(&argc, &argv));
    int rank, size;
    Check_For_SUCCESS(MPI_Comm_rank(MPI_COMM_WORLD, &rank));
    Check_For_SUCCESS(MPI_Comm_size(MPI_COMM_WORLD, &size));
    srandom(rank+1);
    int local_len = random()%7+1;
    vector<double> local_data(local_len, sqrt(rank));
    vector<int> recvcounts(size);
    Check_For_SUCCESS(MPI_Gather(&local_len, 1, MPI_INT, recvcounts.data(), 1, MPI_INT, 0, MPI_COMM_WORLD));
	vector<int> displs; vector<double> recvbuf;
    if (rank == 0) {
        displs.resize(size);
        displs[0] = 0;
        for (int i=1; i<size; ++i) {displs[i] = displs[i-1]+recvcounts[i-1];} //не добавляем + i как в прошлой задаче, информация в массиве идет подряд
        int total_len = displs[size-1]+recvcounts[size-1];
        recvbuf.resize(total_len);
    }
    Check_For_SUCCESS(MPI_Gatherv(local_data.data(), local_len, MPI_DOUBLE, recvbuf.data(), recvcounts.data(), displs.data(), MPI_DOUBLE, 0, MPI_COMM_WORLD));
    if (rank == 0) {
        ofstream fout("out.txt");
        for (int i=0; i<size; ++i) {
            int start = displs[i];
            int end = start+recvcounts[i];
            for (int j=start; j<end; ++j) {
                fout << recvbuf[j];
                if (j < end-1) fout << " ";
            }
            fout << "\n";
        }
        fout.close();
    }
    MPI_Finalize();
    return 0;
}


