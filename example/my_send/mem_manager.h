#ifndef _MEM_MANAGER_
#define _MEM_MANAGER_
#define form_size 10

struct Myform {
    int offset;
    void* addr;
    int len;
    int ori_len;
    void clear() {
        this->addr = NULL;
        this->len = 0;
        this->ori_len = 0;
        this->offset = 0;
    }
    Myform():offset(0), addr(nullptr), len(0), ori_len(0){}
};
extern Myform myform[form_size];

int addProcessNum();
int getFd();
int setProcessNumOne();
int getProcessNum();
int getAlignedAddr(int addr);
int getNewMem(int size);
void mapToMem(int offset);
void freeMem(int offset);
int subProcessNum();
int getCount(int offset);
void printAll();
bool setWritable(int offset, void* addr, int len);
void manage_divide(void* addr, int length);
void set_offset(void* addr, int offset, int bytes);
int get_length(void* addr);
int get_offset(void* addr);
void divide(int ori_offset, int offset, int length);
void locked_divide(int ori_offset, int offset, int length);

#endif
