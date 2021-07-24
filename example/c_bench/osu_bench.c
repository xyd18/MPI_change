#include <unistd.h>
#include <stdlib.h>
#include "mpi.h"
unsigned long align_size = 4096;
void allocate_memory(void** s_buf, void** r_buf) {
    posix_memalign(s_buf, align_size, 128);
    posix_memalign(r_buf, align_size, 128);
}
int main() {
    MPI_Init(NULL, NULL);
    void *send_buf, *recv_buf;
    allocate_memory(&send_buf, &recv_buf);
    MPI_Send(send_buf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    MPI_Recv(recv_buf, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Finalize();
    return 0;
}

