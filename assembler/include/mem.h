#ifndef MEM_H
#define MEM_H

#include "util.h"

#define mem_alloc(size) _mem_alloc(size, __FILE__, __LINE__)
void* _mem_alloc(size_t size, const char *file, size_t line);

#define mem_realloc(ptr, size) _mem_realloc(ptr, size, __FILE__, __LINE__)
void* _mem_realloc(void *ptr, size_t size, const char *file, size_t line);

#define mem_free(ptr) _mem_free(ptr, __FILE__, __LINE__)
void _mem_free(void *ptr, const char *file, size_t line);

#define mem_check_allocated() _mem_check_allocated()
void _mem_check_allocated(void);

#endif // MEM_H
