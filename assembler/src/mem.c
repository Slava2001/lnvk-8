#define LOG_LVL INFO
#include "mem.h"
#include "util.h"

typedef struct MemBlock {
    void *ptr;
    size_t size;
    const char *file;
    size_t line;
    struct MemBlock *next;
} MemBlock;

MemBlock *root = NULL;

void* _mem_alloc(size_t size, const char *file, size_t line) {
    logenter();
    void *ptr = malloc(size);
    die(!ptr, "allocate memory: size: %zu, file: %s:%zu", size, file, line);

    MemBlock *block = (MemBlock *)malloc(sizeof(MemBlock));
    die(!block, "allocate memory for mem block: size: %zu, file: %s:%zu", size, file, line);

    block->ptr = ptr;
    block->size = size;
    block->next = root;
    block->file = file;
    block->line = line;
    root = block;
    logd("Allocate %zu bytes on %p, file: %s:%zu", size, ptr);
    return ptr;
}

// return pointer to pointer to block with specifed addres
static MemBlock **find_mem_block(void *ptr);

static MemBlock **find_mem_block(void *ptr) {
    logenter();
    MemBlock **ptr_to_block_ptr = &root;
    MemBlock *block = *ptr_to_block_ptr;
    while (block) {
        if (block->ptr == ptr) {
            return ptr_to_block_ptr;
        }
        ptr_to_block_ptr = &block->next;
        block = *ptr_to_block_ptr;
    }
    return NULL;
}

void* _mem_realloc(void *ptr, size_t size, const char *file, size_t line) {
    logenter();
    MemBlock **ptr_to_block_ptr = find_mem_block(ptr);
    die(!ptr_to_block_ptr, "realloc unallocated block: addr: %p, file: %s:%zu", ptr, file, line);

    MemBlock *block = *ptr_to_block_ptr;
    void *new_ptr = realloc(block->ptr, size);
    logd("Realloc %zu bytes on %p, new size: %zu, on %p", block->size, block->ptr, size, new_ptr);
    block->ptr = new_ptr;
    block->size = size;
    block->file = file;
    block->line = line;
    return new_ptr;
}

void _mem_free(void *ptr, const char *file, size_t line) {
    logenter();
    MemBlock **ptr_to_block_ptr = find_mem_block(ptr);
    die(!ptr_to_block_ptr, "free unallocated block: addr: %p, file: %s:%zu", ptr, file, line);

    MemBlock *block = *ptr_to_block_ptr;
    *ptr_to_block_ptr = block->next;

    logd("Free %zu bytes on %p", block->size, block->ptr);
    free(block->ptr);
    free(block);
}

void _mem_check_allocated(void) {
    logenter();
    MemBlock *node = root;
    while (node) {
        loge("Memory leak on addr %p, size: %zu, allocated in file: %s:%zu",
              node->ptr, node->size, node->file, node->line);
        node = node->next;
    }
}
