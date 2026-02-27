#ifndef ERRORS_H
#define ERRORS_H

#include "log.h"

#define rci(cond, code, fmt, ...) \
    do { if (cond) { loge("Failed to "fmt, ##__VA_ARGS__); return (code); } } while(0)
#define reci(cond, fmt, ...) rci(cond, -1, fmt, ##__VA_ARGS__)
#define __SEGFAULT // *((int*)NULL)=0
#define die(cond, fmt, ...) \
    do { if (cond) { loge("Failed to "fmt, ##__VA_ARGS__); __SEGFAULT;exit (-1); } } while(0)

#endif // ERRORS_H
