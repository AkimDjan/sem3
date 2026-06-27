#include <mpi.h>
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

void Check_For_SUCCESS(int MPI_FUNC) {if (MPI_FUNC != MPI_SUCCESS) {MPI_Abort(MPI_COMM_WORLD, -1);}}

int main(int argc, char* argv[]) {

    int local_size = rank+1;
    vector<int> local_data(local_size, rank + 1);
    vector<int> recvcounts;
    vector<int> displs;
    vector<int> result;
    if (rank == 0) {
        recvcounts.resize(size); displs.resize(size);
        for (int i=0; i<size; ++i) {recvcounts[i] = i+1;}
        displs[0]=0;
        for (int i=1; i<size; ++i) {displs[i] = displs[i-1] + recvcounts[i-1] + i;}
        int total_size = displs[size-1] + recvcounts[size-1];
        result.resize(total_size, 0);
    }
    Check_For_SUCCESS(MPI_Gatherv(local_data.data(), local_size, MPI_INT, result.data(), recvcounts.data(), displs.data(), MPI_INT, 0, MPI_COMM_WORLD));
    if (rank == 0) {
	ofstream fout("out.txt");
        for (size_t i=0; i<result.size(); ++i) {
            fout << result[i];
            if (i != result.size() - 1)
                fout << " ";
        }
        fout << "\n";
        fout.close();
    }
    MPI_Finalize();
    return 0;
}

