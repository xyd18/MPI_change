#ifndef _UTILS_H
#define _UTILS_H
#include <unistd.h>
#define FILE_SIZE 67108864 * 16// 4096 * 32

#define page_size  sysconf(_SC_PAGE_SIZE);
#endif // !_UTILS_H