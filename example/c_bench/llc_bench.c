#include <unistd.h>
#include "mpi.h"
unsigned long align_size = 4096;
unsigned long buf_size = 128;

int main() {
    MPI_Init(NULL, NULL);
    void *send_buf, *recv_buf;
    send_buf = malloc(buf_size);
    recv_buf = malloc(buf_size);
    MPI_Send(send_buf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    MPI_Recv(recv_buf, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Finalize();
    return 0;
}

