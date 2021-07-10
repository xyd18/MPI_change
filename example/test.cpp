#include <iostream>
#include <mpi.h> /* PROVIDES THE BASIC MPI DEFINITION AND TYPES */

using namespace std;

int main() {
    int *data = (int*)(malloc(sizeof(int)));
    MPI_Send(data, sizeof(int), 0, 0, 0, 0);
    return 0;
}
