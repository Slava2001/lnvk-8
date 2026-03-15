#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "tokenizer.h"
#include "asm.h"
#include "vec.h"
#include "label_resolver.h"

struct EncodeCtx {
    LabelResolver *labels;
    VecByte *buff;
    VecToken *toks;
};

typedef int (*leaf_cb)(struct EncodeCtx *encode_ctx);

struct GrammarTreeNode {
    struct GrammarTreeNode *next[TOK_COUNT];
    leaf_cb cb;
};

extern const struct GrammarTreeNode grammar_tree;

#endif // GRAMMAR_H
