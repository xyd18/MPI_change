#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#define MPI_Aint uint64_t
unsigned long align_size = 4096;
unsigned long buf_size = 4096;

void *send_buf, *recv_buf;
static void* fresh_ptr;
void* skampi_buffer;
static void* real_skampi_buffer;
void* get_send_buffer();
void* get_recv_buffer();    
void set_send_buffer(void* buffer);
void set_recv_buffer(void* buffer);
void check_buf();
void init_buffer();
MPI_Aint get_send_buffer_usage();
MPI_Aint get_recv_buffer_usage();
void set_send_buffer_usage(MPI_Aint n);
void set_recv_buffer_usage(MPI_Aint n);
static void *align_ptr(void *p, int a);
void set_skampi_buffer(int size);
static int alignment_fix(void *p, int a);
static int skampi_real_buffer_size;
static int skampi_buffer_alignment = 32;
int send_buffer_alignment = 32;
int recv_buffer_alignment = 32;
int skampi_buffer_size;
static MPI_Aint send_buffer_usage;
static MPI_Aint recv_buffer_usage;
int main() {
    MPI_Init(NULL, NULL);
    set_send_buffer_usage(16);
    set_recv_buffer_usage(16);
    set_skampi_buffer(65536);
    check_buf();
    MPI_Send(get_send_buffer(), 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    MPI_Recv(get_recv_buffer(), 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Finalize();
    return 0;
}
void set_send_buffer_usage(MPI_Aint n) {
    send_buffer_usage = n;
}
void set_recv_buffer_usage(MPI_Aint n) {
    recv_buffer_usage = n;
}
void* get_send_buffer() {
    return send_buf;
}
void* get_recv_buffer() {
    return recv_buf;
}
MPI_Aint get_send_buffer_usage() {
    return send_buffer_usage;
}
MPI_Aint get_recv_buffer_usage() {
    return recv_buffer_usage;
}
void set_send_buffer(void* buffer) {
    send_buf = buffer;
}
void set_recv_buffer(void* buffer) {
    recv_buf = buffer;
}
static int alignment_fix(void *p, int a) {
#ifdef SKAMPI_MPI
  MPI_Aint absolute_address;
#else
  unsigned long absolute_address;
#endif // SKAMPI_MPI
  unsigned long address;
  int fix;

#ifdef SKAMPI_MPI
#if MPI_VERSION < 2
  MPI_Address(p, &absolute_address);
#else
  MPI_Get_address(p, &absolute_address);
#endif
#else
  absolute_address = (unsigned long)p;
#endif // SKAMPI_MPI

  address = (unsigned long) absolute_address;

  if (a<=0) {
    fix = 0;
  } else {
    /* if( address % a == 0 ) fix = 0; else fix = a - (address % a); */
    fix = (a - (address % a)) % a;

    /* alignment should be for a not for 2*a */ 
    if( (address+fix) % (2*a) == 0 )
      fix = fix+a;
  }
  return fix;
}


static void *align_ptr(void *p, int a) {
  return p + alignment_fix(p, a);
}
void init_buffer(void){
  fresh_ptr = skampi_buffer;
}
void set_skampi_buffer(int size) { // 65536
  int alignments;

  if( skampi_buffer != NULL ) {
    free(real_skampi_buffer);
  }

  alignments = 8*skampi_buffer_alignment;
  real_skampi_buffer =  malloc(size + alignments);
//   assert( real_skampi_buffer != NULL );
  skampi_buffer_size = size;
  skampi_real_buffer_size = size + alignments;
  skampi_buffer = align_ptr(real_skampi_buffer, skampi_buffer_alignment);
  memset(real_skampi_buffer, 0x55, skampi_real_buffer_size); 
  /* touch buffer so that we can be sure to actually 'have' it in memory */
  init_buffer();
}



void check_buf() {
    void* tmp_ptr;
    if( fresh_ptr - skampi_buffer > skampi_real_buffer_size - send_buffer_alignment ) {
        init_buffer();
    }
    else {
        tmp_ptr = align_ptr(fresh_ptr, send_buffer_alignment);
        if( tmp_ptr - skampi_buffer > skampi_real_buffer_size - get_send_buffer_usage() - recv_buffer_alignment) {
            init_buffer();
        } 
        else {
            tmp_ptr = align_ptr(tmp_ptr+get_send_buffer_usage(), recv_buffer_alignment);
            if( tmp_ptr - skampi_buffer > skampi_real_buffer_size - get_recv_buffer_usage()) {
	            init_buffer();
            }
        }
    }
    tmp_ptr = align_ptr(fresh_ptr, send_buffer_alignment);
    set_send_buffer(tmp_ptr);
    set_recv_buffer(align_ptr(tmp_ptr+get_send_buffer_usage(), recv_buffer_alignment)); 
    fresh_ptr = get_recv_buffer() + get_recv_buffer_usage();
}