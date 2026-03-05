#include "asm.h"
#include "util.h"
#include "grammar.h"

vec_impl(Token, Token, token);
vec_impl(uint8_t, Byte, byte);

Asm asm_new(Tokenizer *tokenizer) {
    return (Asm) {
        .tokenizer = tokenizer
    };
}

int asm_assemble(Asm *this, VecByte *buff) {
    VecToken tokens = vec_token_new();
    struct EncodeCtx encode_ctx = {
        .buff = buff,
        .toks = &tokens
    };
    int rv = 0;
    const struct GrammarTreeNode *node = &grammar_tree;
    while (1) {
        Token token = tokenizer_next(this->tokenizer);
        if (token.type == TOK_UNKNOWN) {
            loge("Failed to get next token: Unknown token");
            rv = -1;
            break;
        }
        if (token.type == TOK_EOF) {
            break;
        }
        vec_token_push(&tokens, token);

        if (!node->next[token.type]) {
            loge("Unexpected token %s in sequence:", token_type_to_str(token.type));
            for (size_t i = 0; i < vec_token_length(&tokens); i++) {
                Token *t = vec_token_at(&tokens, i);
                loge("%-15s (%.*s)", token_type_to_str(t->type), (int)t->len, t->word);
            }
        }

        node = node->next[token.type];

        if (!node->cb) {
            continue;
        }

        if (node->cb(&encode_ctx)) {
            loge("Failed to exec leaf callback for sequence:");
            for (size_t i = 0; i < vec_token_length(&tokens); i++) {
                Token *t = vec_token_at(&tokens, i);
                loge("%-15s (%.*s)", token_type_to_str(t->type), (int)t->len, t->word);
            }
            rv = -1;
            break;
        }

        node = &grammar_tree;
        vec_token_clear(&tokens);
    }
    vec_token_del(&tokens);
    return rv;
}

void asm_delete(Asm *this) {
    memset(this, 0, sizeof(*this));
}
