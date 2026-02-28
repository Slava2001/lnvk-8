#include "asm.h"
#include "util.h"
#include "grammar.h"

Asm asm_new(Tokenizer *tokenizer) {
    return (Asm) {
        .tokenizer = tokenizer
    };
}

ssize_t asm_assemble(Asm *this, uint8_t *output, size_t output_size) {
    UNUSED(this, output, output_size);
    // ssize_t len = 0;
    // while (1)
    // {
    //     Token token = tokenizer_next(this->tokenizer);
    //     if (token.type == TOK_UNKNOWN) {
    //         return -1;
    //     }
    //     if (token.type == TOK_EOF) {
    //         return len;
    //     }
    // }
    // // unreachable
    // return -1;
    GrammarNode *gtree = grammar_new();
    reci(!gtree, "create grammar tree");
    grammar_delete(gtree);
    return 0;
}

void asm_delete(Asm *this) {
    memset(this, 0, sizeof(*this));
}
