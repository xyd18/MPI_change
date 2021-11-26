#include <iostream>
#include <mpi.h> /* PROVIDES THE BASIC MPI DEFINITION AND TYPES */
// #include "utils.hpp"

using namespace std;

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int *data_new = (int *)(malloc(sizeof(int)));
    int *data = (int *)(malloc(sizeof(int)));
    MPI_Send(data, sizeof(int), 0, 0, 0, 0);
    return 0;
}
