#ifndef ASM_H
#define ASM_H

#include "tokenizer.h"
#include "errors.h"
#include "vec.h"

vec_decl(uint8_t, Byte, byte);
vec_decl(Token, Token, token);

typedef struct Asm {
    Tokenizer *tokenizer;
} Asm;

Asm asm_new(Tokenizer *tokenizer);
int asm_assemble(Asm *this, VecByte *buf);
void asm_delete(Asm *this);

#endif // ASM_H
