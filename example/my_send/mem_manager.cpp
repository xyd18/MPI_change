#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/container/stable_vector.hpp>
#include <boost/atomic/atomic.hpp>
#include "utils.h"
#include "mem_manager.h"
#define form_size 10
int Fd = -1;

Myform myform[form_size];

class mem_info { 
public:
    int high_addr; // 内存段尾地址
    int low_addr; // 内存段首地址
    int count; // 引用计数
    mem_info(int l, int h) {
        this->low_addr = l;
        this->high_addr = h;
        count = 0;
    }
    mem_info() {
        this->high_addr = 0;
        this->low_addr = 0;
        this->count = 0;
    }
};
typedef boost::interprocess::allocator<mem_info, boost::interprocess::managed_shared_memory::segment_manager>temp_alloc;
typedef boost::container::vector<mem_info, temp_alloc> vec;
boost::interprocess::managed_shared_memory segment;
boost::interprocess::offset_ptr<vec> mem_manager;
boost::atomic_int* process_num;
boost::interprocess::interprocess_mutex *mtx;

class manager_helper {
public:
    manager_helper() {
        printf("manager_helper()1\n");
        // boost::interprocess::shared_memory_object::remove("MemManagerMemory");
        segment = boost::interprocess::managed_shared_memory(boost::interprocess::open_or_create,"MemManagerMemory",65536);
        printf("manager_helper()2\n");
        const temp_alloc alloc_inst = temp_alloc(segment.get_segment_manager());
        printf("manager_helper()3\n");
        mem_manager = segment.find_or_construct<vec>("memManager")(alloc_inst);
        printf("manager_helper()4\n");
        process_num = (boost::atomic_int*)segment.allocate(sizeof(int));
        printf("manager_helper()5\n");
        mtx = segment.find_or_construct<boost::interprocess::interprocess_mutex>("mtx")();
        printf("manager_helper()6\n");
        *process_num = 0;
        printf("manager_helper()7\n");
        mem_info first(0, FILE_SIZE);
        printf("manager_helper()8\n");
        mtx->lock();
        if(mem_manager->size() == 0) {
            mem_manager->push_back(first);
        }
        printf("manager_helper()9\n");
        mtx->unlock();
        printf("manager_helper()10\n");
    }
    ~manager_helper() {}
}mh;

int addProcessNum() {
    int num = process_num->fetch_add(1);
    return num + 1;
}
int setProcessNumOne() {
    process_num->store(1);
    return 1;
}
int getProcessNum() {
    return process_num->load();
}

int subProcessNum() {
    int num = process_num->fetch_sub(1);
    return num - 1;
}
int open_mem(int bytes) { 
    int fd = shm_open("/test12", O_CREAT|O_RDWR|O_EXCL, 0777); // 默认在/dev/shm/下
    if(fd < 0) {
        fd = shm_open("/test12", O_RDWR, 0777);
        // printf("open only\n");
        int num = addProcessNum();
        if(fd < 0) {
            printf("error when opening file\n");
            exit(1);
        }
    }
    else {
        // printf("create \n");
        ftruncate(fd, bytes);  // 设置文件的大小
        int num = setProcessNumOne();
    }
    return fd;
}
class shm_helper {
    public:
    shm_helper() {
        if(Fd == -1) {
            Fd = open_mem(FILE_SIZE);
        }
    }
    ~shm_helper() {
        printf("calling destructor of shm_helper \n");
        int num = subProcessNum();
        mtx->lock();
        if(num == 0) {
            shm_unlink("/test12");
            // printf("removing \n");
            boost::interprocess::shared_memory_object::remove("MemManagerMemory");
        }
        mtx->unlock();

    }
};
shm_helper g;



int getFd() {
    return Fd;
}


int getAlignedAddr(int addr) { // 将addr转换为对齐的地址
    int pagesize = sysconf(_SC_PAGE_SIZE);
    if(addr % pagesize != 0) {
        int tmp = addr % pagesize;
        addr += (pagesize - tmp);
    }
    return addr;
}



int getCount(int offset) {
    int res = -1;
    vec::iterator it = mem_manager->begin();
    for(;it != mem_manager->end(); it++) {
        if(it->low_addr == offset) {
            res = it->count;
            break;
        }
    }
    return res;
}

bool setWritable(int offset, void* addr, int len) {
    boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> l(*mtx);    bool ret = false;
    if(getCount(offset) == 1) {
        ret = true;
        mprotect(addr, len, PROT_READ|PROT_WRITE); // 如果引用计数为1，则直接修改当前内存可写
    }
    return ret;
}

int getNewMem(int size) { // 分配新的内存。这里需要引用计数为0
    boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> l(*mtx);
    vec::iterator it = mem_manager->begin();
    for(; it != mem_manager->end(); it++) {
        int aligned_low = getAlignedAddr(it->low_addr);
        int length = it->high_addr - aligned_low;
        if(length >= size && it->count == 0) { // 找到适合的内存块
            if(aligned_low == it->low_addr) { // 恰好对齐
                if(length == size) { // 大小恰好相等
                    it->count = 1; // 引用计数
                    // printf("offset in getNewMem is: %d\n", aligned_low);
                    return aligned_low; // 返回低地址
                }
                else { // length大，需要切分
                    int split_addr = it->low_addr + size;
                    mem_info tmp_info(it->low_addr, split_addr);
                    tmp_info.count = 1;
                    mem_manager->insert(it, tmp_info);
                    vec::iterator next = it + 1;
                    next->low_addr = split_addr;
                    // printf("offset in getNewMem is: %d\n", aligned_low);
                    return aligned_low;
                }
            }
            else { // 不对齐
                if(length == size) { // 分成两份
                    mem_info tmp_info(it->low_addr, aligned_low);
                    mem_manager->insert(it, tmp_info);
                    vec::iterator ori_it = it + 1;
                    ori_it->count = 1;
                    ori_it->low_addr = aligned_low;
                    // printf("offset in getNewMem is: %d\n", aligned_low);
                    return aligned_low;
                }
                else { // 分成三份
                    int ori_low = it->low_addr;
                    it->low_addr = aligned_low + size;
                    mem_info now_info(aligned_low, aligned_low + size);
                    now_info.count = 1;
                    mem_manager->insert(it, now_info);
                    mem_info pre(ori_low, aligned_low);
                    mem_manager->insert(it, pre);
                    return aligned_low;
                }
            }
        }
    }
    printf("do not find such memory in getNewMem\n");
    exit(0);
    return -1; // 没有符合的内存
}

void mapToMem(int offset) { // 将某一指针map到offset位置的内存，增加其引用计数
    boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> l(*mtx);
    bool flag = false;
    vec::iterator it = mem_manager->begin();
    for(;it != mem_manager->end(); it++) {
        if(it->low_addr == offset) {
            it->count++;
            flag = true;
            break;
        }
    }
    if(!flag) {
        printf("do not find such memory in maptoMem\n");
        exit(0); // 没找到相应的内存  
    }

}


void printAll() {
    for(vec::iterator it = mem_manager->begin(); it != mem_manager->end(); it++) {
        printf("it->low_addr: %d\n", it->low_addr);
    }
    
}

void freeMem(int offset) { // 要free一片内存。去上下两个位置看下能不能合并
    boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> l(*mtx);
    vec::iterator it = mem_manager->begin();
    for(; it != mem_manager->end(); it++) {
        if(it->low_addr == offset) {
            if(it->count > 1) {
                // printf("count is: %d, it->low: %d\n", it->count, it->low_addr);
                it->count--; // 有两个进程在引用，则减少一个引用数
                return;
            }
            else if(it->count == 1) { // 当前只有一个进程在用，引用计数归零，上下查看引用情况
                it->count--;
                
                if(it != mem_manager->begin()) {
                    vec::iterator pre = it - 1;
                    if(pre->count == 0) { // 合并前一项
                        it->low_addr = pre->low_addr;
                        mem_manager->erase(pre);
                        it--;
                    }
                }
                if(it + 1 != mem_manager->end()) {
                    vec::iterator next = it + 1;
                    if(next->count == 0) { // 合并后一项
                        it->high_addr = next->high_addr;
                        mem_manager->erase(next);
                    }
                }
                return;
            }
        }
    }
    printf("do not find such memory in freeMem\n");
    exit(0); // 没有相应的内存
}

int get_offset(void* addr) {
    for(int i = 0; i < form_size; i++) {
        if(addr == myform[i].addr) {
            return myform[i].offset;
        }
    }
    return -1; // 没找到
}

int get_length(void* addr) {
    for(int i = 0; i < form_size; i++) {
        if(addr == myform[i].addr) {
            return myform[i].ori_len;
        }
    }
    return -1; // 没找到
}

void set_offset(void* addr, int offset, int bytes) { // 实现更新表内容或者添加表内容
    int empty_place = -1;
    for(int i = 0; i < form_size; i++) {
        if(myform[i].addr == NULL) {
            empty_place = i;
        }
        if(myform[i].addr == addr) { // 已经在表中,更新
            myform[i].offset = offset;
            myform[i].len = bytes;
            return;
        }
    }
    if(empty_place == -1) {
        printf("overflow! \n");
        exit(0); // 越界
    }
    // 不在表中，加入表内
    myform[empty_place].addr = addr;
    myform[empty_place].offset = offset;
    myform[empty_place].ori_len = bytes;
    myform[empty_place].len = bytes;
}

void divide(int ori_offset, int offset, int length) { // ori_offset 为被切分块的offset， offset为新块的offset，length为新块的长度
    vec::iterator it = mem_manager->begin();
    for(; it != mem_manager->end(); it++) {
        if(it->low_addr == ori_offset) {
            int ori_low = it->low_addr, ori_high = it->high_addr, count = it->count;
            if(ori_high > offset + length) { // 分成三份
                it->low_addr = offset + length;
                mem_info mid(offset, offset + length);
                mid.count = count;
                vec::iterator temp = mem_manager->insert(it, mid);
                mem_info pre(ori_low, offset);
                pre.count = count;
                mem_manager->insert(temp, pre);
            }
            else if(ori_high == offset + length){ // 分成两份
                
                it->low_addr = offset;
                mem_info pre(ori_low, offset);
                pre.count = count;
                mem_manager->insert(it, pre);
            }
        }
    }
}

void locked_divide(int ori_offset, int offset, int length) {
    boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> l(*mtx);
    if(ori_offset == offset) return;
    vec::iterator it = mem_manager->begin();
    for(; it != mem_manager->end(); it++) {
        if(it->low_addr == ori_offset) {
            int ori_low = it->low_addr, ori_high = it->high_addr, count = it->count;
            if(ori_high > offset + length) { // 分成三份
                it->low_addr = offset + length;
                mem_info mid(offset, offset + length);
                mid.count = count;
                vec::iterator temp = mem_manager->insert(it, mid);
                mem_info pre(ori_low, offset);
                pre.count = count;
                mem_manager->insert(temp, pre);
            }
            else if(ori_high == offset + length){ // 分成两份
                
                it->low_addr = offset;
                mem_info pre(ori_low, offset);
                pre.count = count;
                mem_manager->insert(it, pre);
            }
        }
    }
}


void manage_divide(void* addr, int length) { // 要求addr页对齐。
    boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> l(*mtx);
    for(int i = 0; i < form_size; i++) {
        if((char*)myform[i].addr + myform[i].len > (char*)addr && addr > myform[i].addr) { 
            // 这里的范围用len确定，这样可以保证找到的addr是最内存包含的。
            myform[i].len = (char*)addr - (char*)myform[i].addr;
            int ori_offset = myform[i].offset, new_offset = myform[i].offset + myform[i].len;
            set_offset(addr, myform[i].offset + myform[i].len, length);
            int offset_2 = myform[i].offset + myform[i].len + length;
            int new_len = myform[i].ori_len - (myform[i].len + length);
            if(new_len > 0) {
                set_offset((char*)addr + length, myform[i].offset + myform[i].len + length, new_len);
            }
            divide(ori_offset, new_offset, length);
            return;
        }
        else if(myform[i].addr == addr) {
            if(length < myform[i].len) {
                set_offset((char*)addr + length, myform[i].offset + length, myform[i].len - length);
                myform[i].len = length;
                divide(myform[i].offset, myform[i].offset + length, myform[i].len - length);
            }
        }
    }
}

