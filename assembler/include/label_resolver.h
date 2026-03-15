#ifndef LABEL_RESOLVER_H
#define LABEL_RESOLVER_H

#include "types.h"

typedef struct LabelDescriptor {
    VecChar name;
    size_t offset;
} LabelDescriptor;

vec_decl(LabelDescriptor, LabelDescriptor, label_descriptor);

typedef struct LabelResolver {
    VecLabelDescriptor define;
    VecLabelDescriptor request;
} LabelResolver;

LabelResolver label_resolver_init(void);
int label_resolver_request(
    LabelResolver *this, const char *name_str, size_t name_len, size_t offset
);
int label_resolver_define(
    LabelResolver *this, const char *name_str, size_t name_len, size_t offset
);
int label_resolver_resolve(LabelResolver *this, VecByte *buff);
void label_resolver_delete(LabelResolver *this);

#endif // LABEL_RESOLVER_H
