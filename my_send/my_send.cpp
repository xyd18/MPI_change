#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <math.h>
#include "mpi.h"
#include <boost/atomic/atomic.hpp>
#include <boost/atomic/fences.hpp>
#include <boost/atomic/ipc_atomic.hpp>
#include <boost/atomic/ipc_atomic_ref.hpp>
#include <boost/atomic/ipc_atomic_flag.hpp>
#include <boost/memory_order.hpp>
// #include "my_send.h"
#include "mem_manager.h"
#include "utils.h"






void* my_malloc(int bytes) {
    int offset = getNewMem(bytes);
    void* ret =  mmap(NULL, bytes, PROT_READ|PROT_WRITE, MAP_SHARED, getFd(), offset);
    set_offset(ret, offset, bytes);
    return ret;
}
static void handler(int sig, siginfo_t *si, void *unused) { // TODO: 处理函divide的情况。注意每次都要拷贝包含关系最外层的内存
    // 暂时不考虑函divide的情况
    // printf("in handler\n");
    void* addr = si->si_addr;
    int length = get_length(addr);
    for(int i = 0; i < form_size; i++) {
        if(myform[i].addr < addr && addr < (char*)myform[i].addr + myform[i].ori_len) { // 找到addr最小的
            addr = myform[i].addr;
        }
    }
    int offset = get_offset(si->si_addr);
    if(offset == -1) {
        printf("segmentation fault \n");
        exit(0);
    }
    int len = get_length(addr);
    bool writable = setWritable(offset, si->si_addr, length);
    if(writable) {
        // printf("is writable\n");
        return;
    }

    // freeMem(offset);// TODO: 要free所有范围内的部分，还要map到新的地方，并且切分
    for(int i = 0; i < form_size; i++) { // 更新范围内所有的offset，长度不必更新。
        if(addr <= myform[i].addr && myform[i].addr < (char*)addr + len) {
            freeMem(get_offset(myform[i].addr));
        }
    }
    
    void* temp = my_malloc(len);
    memcpy(temp, addr, len);
    int temp_offset = get_offset(temp);
    locked_divide(temp_offset, temp_offset + ((char*)si->si_addr - (char*)addr), length); // 只考虑切分成三块/两块
    void* result = mmap(addr, len, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_FIXED, getFd(), temp_offset);
    if(result == MAP_FAILED) {
        printf("map failed! \n");
        exit(0);
    }
    for(int i = 0; i < form_size; i++) { // 更新范围内所有的offset，长度不必更新。
        if(addr <= myform[i].addr && myform[i].addr < (char*)addr + len) {
            myform[i].offset = temp_offset + (char*)myform[i].addr - (char*)addr;
        }
    }
    munmap(temp, len);
}
class sigaction_helper{
    public:
    sigaction_helper() {
        struct sigaction sa;
        sa.sa_flags = SA_SIGINFO;
        sigemptyset(&sa.sa_mask);
        sa.sa_sigaction = handler;
        if (sigaction(SIGSEGV, &sa, NULL) == -1)
            printf("sigaction error");
    }
};
sigaction_helper sh;

int my_send(int dest, int tag, void* data, int bytes) {
    int* arr = (int*)malloc(3 * sizeof(int));
    manage_divide(data, bytes);
    int offset = get_offset(data);
    if(offset == -1) {
        printf("error\n");
        exit(0);
    }
    arr[0] = bytes, arr[1] = offset;
    int ret = MPI_Send(arr, 2, MPI_INT, dest, tag, MPI_COMM_WORLD);
    int temp_offset = get_offset(data);
    mapToMem(temp_offset);
    free(arr);
    mprotect(data, bytes, PROT_READ); // 发送完成，修改权限为只读
    return ret;
}

int my_recv(int dest, int tag, void* data, int bytes) {
    MPI_Status status;
    int *arr = (int*) malloc(3 * sizeof(int));
    int ret = MPI_Recv(arr, 2, MPI_INT, dest, tag, MPI_COMM_WORLD, &status);
    int size = arr[0], offset = arr[1];
    manage_divide(data, size);
    int old_offset = get_offset(data);
    freeMem(old_offset);
    munmap(data, size); // 解除原先的映射
    void* result = mmap(data, size, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_FIXED, getFd(), offset);
    set_offset(data, offset, size);
    if(result == MAP_FAILED) {
        printf("map failed! \n");
        exit(0);
    }
    mprotect(data, size, PROT_READ); // 收到消息，修改权限为只读
    free(arr);
    return ret;
}

void my_free(void* addr) {
    for(int i = 0; i < form_size; i++) {
        if(addr == myform[i].addr) {
            int ori_len = myform[i].ori_len;
            for(int j = 0; j < form_size; j++) {
                if(addr <= myform[j].addr && myform[j].addr < (char*)addr + ori_len) {
                    freeMem(myform[j].offset);
                    myform[i].clear();
                }
            }
            return;
        }
    }
    // 一般方式分配内存
    free(addr);
}
