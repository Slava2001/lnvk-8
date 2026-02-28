#ifndef ASM_H
#define ASM_H

#include "tokenizer.h"
#include "errors.h"

typedef struct Asm {
    Tokenizer *tokenizer;
} Asm;

Asm asm_new(Tokenizer *tokenizer);
ssize_t asm_assemble(Asm *this, uint8_t *output, size_t output_size);
void asm_delete(Asm *this);

#endif // ASM_H
