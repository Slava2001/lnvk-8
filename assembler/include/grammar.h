#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "tokenizer.h"

typedef size_t (*encoder_t)(const Token *tokens, size_t len, uint8_t *output, size_t output_size);

typedef struct GrammarNode GrammarNode;
struct GrammarNode {
    encoder_t encoder;
    GrammarNode *next[TOK_COUNT];
};

GrammarNode* grammar_new(void);
void grammar_delete(GrammarNode* this);

#endif // GRAMMAR_H
