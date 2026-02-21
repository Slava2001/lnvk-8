#ifndef VEC_H
#define VEC_H

#include "mem.h"

#define vec_decl(type, type_suffix, func_suffix)                          \
typedef struct Vec ## type_suffix {                                       \
    type *data;                                                           \
    size_t size;                                                          \
    size_t capacity;                                                      \
} Vec ## type_suffix;                                                     \
Vec ## type_suffix vec_ ## func_suffix ## _new(void);                     \
size_t vec_ ## func_suffix ## _push(Vec ## type_suffix *this, type val);  \
void vec_ ## func_suffix ## _pusha(                                       \
    Vec ## type_suffix *this, const type *val, size_t val_cnt);           \
type vec_ ## func_suffix ## _pop(Vec ## type_suffix *this);               \
type *vec_ ## func_suffix ## _at(Vec ## type_suffix *this, size_t index); \
void vec_ ## func_suffix ## _del(Vec ## type_suffix *this);               \
size_t vec_ ## func_suffix ## _length(Vec ## type_suffix *this);          \
void vec_ ## func_suffix ## _replace_range(                               \
    Vec ## type_suffix *this,                                             \
    const type *val, size_t val_cnt,                                      \
    size_t start_index, size_t end_index);

#define vec_impl(type, type_suffix, func_suffix)                                  \
Vec ## type_suffix vec_ ## func_suffix ## _new(void) {                            \
    logenter();                                                                   \
    return (Vec ## type_suffix) {                                                 \
        .data = mem_alloc(sizeof(type)),                                          \
        .size = 0,                                                                \
        .capacity = 1                                                             \
    };                                                                            \
}                                                                                 \
size_t vec_ ## func_suffix ## _push(Vec ## type_suffix *this, type val) {         \
    logenter();                                                                   \
    logd("Vector: data: %p size: %zu capacity: %zu",                              \
         this->data, this->size, this->capacity);                                 \
    if (this->size >= this->capacity) {                                           \
        this->capacity = this->capacity * 2;                                      \
        this->data = mem_realloc(this->data, this->capacity * sizeof(type));      \
    }                                                                             \
    this->data[this->size] = val;                                                 \
    this->size++;                                                                 \
    return this->size - 1;                                                        \
}                                                                                 \
void vec_ ## func_suffix ## _pusha(                                               \
    Vec ## type_suffix *this, const type *val, size_t val_cnt)                    \
{                                                                                 \
    logenter();                                                                   \
    for (size_t i = 0; i < val_cnt; i++) {                                        \
        vec_ ## func_suffix ## _push(this, val[i]);                               \
    }                                                                             \
}                                                                                 \
type vec_ ## func_suffix ## _pop(Vec ## type_suffix *this) {                      \
    logenter();                                                                   \
    die(!this->size, "Pop from empty vector");                                    \
    this->size--;                                                                 \
    return this->data[this->size];                                                \
}                                                                                 \
type *vec_ ## func_suffix ## _at(Vec ## type_suffix *this, size_t index) {        \
    logenter();                                                                   \
    die(index >= this->size, "index %zu out of range 0-%zu", index, this->size);  \
    return &this->data[index];                                                    \
}                                                                                 \
void vec_ ## func_suffix ## _del(Vec ## type_suffix *this) {                      \
    logenter();                                                                   \
    this->size = 0;                                                               \
    this->capacity = 0;                                                           \
    mem_free(this->data);                                                         \
    this->data = NULL;                                                            \
}                                                                                 \
size_t vec_ ## func_suffix ## _length(Vec ## type_suffix *this) {                 \
    logenter();                                                                   \
    return this->size;                                                            \
}                                                                                 \
void vec_ ## func_suffix ## _replace_range(                                       \
    Vec ## type_suffix *this,                                                     \
    const type *val, size_t val_cnt,                                              \
    size_t start_index, size_t end_index)                                         \
{                                                                                 \
    logenter();                                                                   \
    die(end_index < start_index || end_index - 1 > this->size,                    \
        "replace range. Invalid range: %zu..%zu, available: 0..%zd",              \
        start_index, end_index, (ssize_t)this->size - 1);                         \
    if (this->capacity < this->size + val_cnt) {                                  \
        this->capacity = this->capacity + this->size + val_cnt;                   \
        this->data = mem_realloc(this->data, this->capacity * sizeof(type));      \
    }                                                                            \
    ssize_t shift = (ssize_t)val_cnt -                                            \
                    ((ssize_t)end_index - (ssize_t)start_index + 1);              \
    if (shift > 0) {                                                              \
        for (ssize_t i = (ssize_t)this->size - 1; i > (ssize_t)end_index; i--) {  \
            this->data[i + shift] = this->data[i];                                \
        }                                                                         \
    } else if (shift < 0) {                                                       \
        for (ssize_t i = (ssize_t)end_index; i < (ssize_t)this->size; i++) {      \
            this->data[i + shift] = this->data[i];                                \
        }                                                                         \
    }                                                                             \
    for (size_t i = 0; i < val_cnt; i++) {                                        \
        this->data[start_index + i] = val[i];                                     \
    }                                                                             \
}

#endif // VEC_H
