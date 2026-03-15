#include "label_resolver.h"
#include "util.h"

vec_impl(LabelDescriptor, LabelDescriptor, label_descriptor);

int add_desc(VecLabelDescriptor *vec, const char *name_str, size_t name_len, size_t offset);
void label_vec_del(VecLabelDescriptor *vec);

LabelResolver label_resolver_init(void) {
    return (LabelResolver) {
        .define = vec_label_descriptor_new(),
        .request = vec_label_descriptor_new()
    };
}

int add_desc(VecLabelDescriptor *vec, const char *name_str, size_t name_len, size_t offset) {
    VecChar name = vec_char_new();
    vec_char_pusha(&name, name_str, name_len);
    vec_label_descriptor_push(vec, (LabelDescriptor) {
        .name = name,
        .offset = offset
    });
    return 0;
}

int label_resolver_request(
    LabelResolver *this, const char *name_str, size_t name_len, size_t offset
) {
    return add_desc(&this->request, name_str, name_len, offset);
}

int label_resolver_define(
    LabelResolver *this, const char *name_str, size_t name_len, size_t offset
) {
    return add_desc(&this->define, name_str, name_len, offset);
}

int label_resolver_resolve(LabelResolver *this, VecByte *buff) {
    size_t def_len = vec_label_descriptor_length(&this->define);
    for (size_t i = 0; i < vec_label_descriptor_length(&this->request); i++) {
        LabelDescriptor *req = vec_label_descriptor_at(&this->request, i);
        size_t j;
        for (j = 0; j < def_len; j++) {
            LabelDescriptor *def = vec_label_descriptor_at(&this->define, j);
            if (vec_char_length(&req->name) == vec_char_length(&def->name) &&
                !strncmp(req->name.data, def->name.data, vec_char_length(&req->name)))
            {
                uint8_t *ptr = vec_byte_at(buff, req->offset);
                ptr[0] = def->offset & 0xFF;
                ptr[1] = (def->offset >> 8) & 0xFF;
                break;
            }
        }
        reci(j == def_len, "resolve labels: Undefined label '%.*s'",
                           (int)vec_char_length(&req->name), req->name.data);
    }
    return 0;
}

void label_vec_del(VecLabelDescriptor *vec) {
    for (size_t i = 0; i < vec_label_descriptor_length(vec); i++) {
        vec_char_del(&vec_label_descriptor_at(vec, i)->name);
    }
    vec_label_descriptor_del(vec);
}

void label_resolver_delete(LabelResolver *this) {
    label_vec_del(&this->define);
    label_vec_del(&this->request);
    memset(this, 0, sizeof(*this));
}
