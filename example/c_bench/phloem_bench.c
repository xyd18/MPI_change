#include <unistd.h>
#include "mpi.h"
unsigned long align_size = 4096;
unsigned long buf_size = 128;
int main() {
    MPI_Init(NULL, NULL);
    void *combuf = malloc(buf_size);;
    MPI_Send(combuf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    MPI_Recv(combuf, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Finalize();
    return 0;
}

