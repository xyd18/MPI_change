#ifndef _MY_SEND_
#define _MY_SEND_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>


int my_send(int dest, int tag, void* data, int bytes);
int my_recv(int dest, int tag, void* data, int bytes);
void* my_malloc(int bytes);
int open_mem(int bytes);
// int get_or_set_offset(void* addr, int offset);
int get_offset(void* addr);
void set_offset(void* addr, int offset, int bytes);
void my_free(void*);
#endif // !_MY_SEND_
