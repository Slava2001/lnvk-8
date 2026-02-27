#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "load_src.h"

#include <stdint.h>

enum TokenType {
    TOK_KW_NOP,
    TOK_KW_HALT,
    TOK_KW_MOV,
    TOK_KW_PUSH,
    TOK_KW_POP,
    TOK_KW_ADD,
    TOK_KW_ADC,
    TOK_KW_SUB,
    TOK_KW_SBC,
    TOK_KW_INC,
    TOK_KW_DEC,
    TOK_KW_NEG,
    TOK_KW_SHL,
    TOK_KW_SHR,
    TOK_KW_AND,
    TOK_KW_OR,
    TOK_KW_XOR,
    TOK_KW_NOT,
    TOK_KW_CMP,
    TOK_KW_JMP,
    TOK_KW_JR,
    TOK_KW_JZ,
    TOK_KW_JNZ,
    TOK_KW_JC,
    TOK_KW_JNC,
    TOK_KW_CALL,
    TOK_KW_RET,
    TOK_RG_0,
    TOK_RG_1,
    TOK_RG_2,
    TOK_RG_3,
    TOK_RG_4,
    TOK_RG_5,
    TOK_RG_6,
    TOK_RG_7,
    TOK_RG_8,
    TOK_RG_9,
    TOK_RG_10,
    TOK_RG_11,
    TOK_RG_12,
    TOK_RG_13,
    TOK_RG_14,
    TOK_RG_15,
    TOK_LABEL_DEF,
    TOK_LABEL_REF,
    TOK_CONST,
    TOK_ADDR,
    TOK_ADDR_LABEL,
    TOK_UNKNOWN,
    TOK_EOF
};

typedef struct Token
{
    enum TokenType type;
    const char *word;
    size_t len;
    union
    {
        uint8_t const_value;
        uint16_t addres;
        struct {
            const char *name;
            size_t len;
        } label;
    } data;
} Token;

typedef struct Tokenizer {
    SourceTree *src;
    size_t line;
    size_t offset;
} Tokenizer;

Tokenizer tokenizer_new(SourceTree *src);
Token tokenizer_next(Tokenizer *this);
void tokenizer_delete(Tokenizer *this);

#endif // TOKENIZER_H
