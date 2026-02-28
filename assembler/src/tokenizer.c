#include "tokenizer.h"
#include "util.h"

#include <ctype.h>

#define DELIMETER_CHARS " ,\n\t"

Tokenizer tokenizer_new(SourceTree *src) {
    return (Tokenizer) {
        .line = 0,
        .offset = 0,
        .src = src
    };
}

int get_next_word(Tokenizer *this, const char **next, size_t *next_len);
int parse_next_word(const char *word, size_t len, Token *out);

int get_next_word(Tokenizer *this, const char **next, size_t *next_len) {
    while (1) {
        if (this->line >= vec_line_length(&this->src->lines)) {
            return -1;
        }
        Line *current_line = vec_line_at(&this->src->lines, this->line);
        *next = vec_char_at(&current_line->text, this->offset);
        if (**next) {
            *next_len = strcspn(*next, DELIMETER_CHARS);
            this->offset += *next_len + strspn(*next + *next_len, DELIMETER_CHARS);
            return 0;
        }
        this->line++;
        this->offset = 0;
    }
}

int parse_next_word(const char *word, size_t len, Token *out) {
    struct {
        const char *str;
        enum TokenType tok;
        int reg;
    } words[] = {
        { .str = "r0",   .tok = TOK_REG, .reg = 0  },
        { .str = "r1",   .tok = TOK_REG, .reg = 1  },
        { .str = "r2",   .tok = TOK_REG, .reg = 2  },
        { .str = "r3",   .tok = TOK_REG, .reg = 3  },
        { .str = "r4",   .tok = TOK_REG, .reg = 4  },
        { .str = "r5",   .tok = TOK_REG, .reg = 5  },
        { .str = "r6",   .tok = TOK_REG, .reg = 6  },
        { .str = "r7",   .tok = TOK_REG, .reg = 7  },
        { .str = "r8",   .tok = TOK_REG, .reg = 8  },
        { .str = "r9",   .tok = TOK_REG, .reg = 9  },
        { .str = "r10",  .tok = TOK_REG, .reg = 10 },
        { .str = "r11",  .tok = TOK_REG, .reg = 11 },
        { .str = "r12",  .tok = TOK_REG, .reg = 12 },
        { .str = "r13",  .tok = TOK_REG, .reg = 13 },
        { .str = "r14",  .tok = TOK_REG, .reg = 14 },
        { .str = "r15",  .tok = TOK_REG, .reg = 15 },
        { .str = "[r0",  .tok = TOK_ADDR_REG_H, .reg = 0  },
        { .str = "[r1",  .tok = TOK_ADDR_REG_H, .reg = 1  },
        { .str = "[r2",  .tok = TOK_ADDR_REG_H, .reg = 2  },
        { .str = "[r3",  .tok = TOK_ADDR_REG_H, .reg = 3  },
        { .str = "[r4",  .tok = TOK_ADDR_REG_H, .reg = 4  },
        { .str = "[r5",  .tok = TOK_ADDR_REG_H, .reg = 5  },
        { .str = "[r6",  .tok = TOK_ADDR_REG_H, .reg = 6  },
        { .str = "[r7",  .tok = TOK_ADDR_REG_H, .reg = 7  },
        { .str = "[r8",  .tok = TOK_ADDR_REG_H, .reg = 8  },
        { .str = "[r9",  .tok = TOK_ADDR_REG_H, .reg = 9  },
        { .str = "[r10", .tok = TOK_ADDR_REG_H, .reg = 10 },
        { .str = "[r11", .tok = TOK_ADDR_REG_H, .reg = 11 },
        { .str = "[r12", .tok = TOK_ADDR_REG_H, .reg = 12 },
        { .str = "[r13", .tok = TOK_ADDR_REG_H, .reg = 13 },
        { .str = "[r14", .tok = TOK_ADDR_REG_H, .reg = 14 },
        { .str = "[r15", .tok = TOK_ADDR_REG_H, .reg = 15 },
        { .str = "r0]",  .tok = TOK_ADDR_REG_L, .reg = 0  },
        { .str = "r1]",  .tok = TOK_ADDR_REG_L, .reg = 1  },
        { .str = "r2]",  .tok = TOK_ADDR_REG_L, .reg = 2  },
        { .str = "r3]",  .tok = TOK_ADDR_REG_L, .reg = 3  },
        { .str = "r4]",  .tok = TOK_ADDR_REG_L, .reg = 4  },
        { .str = "r5]",  .tok = TOK_ADDR_REG_L, .reg = 5  },
        { .str = "r6]",  .tok = TOK_ADDR_REG_L, .reg = 6  },
        { .str = "r7]",  .tok = TOK_ADDR_REG_L, .reg = 7  },
        { .str = "r8]",  .tok = TOK_ADDR_REG_L, .reg = 8  },
        { .str = "r9]",  .tok = TOK_ADDR_REG_L, .reg = 9  },
        { .str = "r10]", .tok = TOK_ADDR_REG_L, .reg = 10 },
        { .str = "r11]", .tok = TOK_ADDR_REG_L, .reg = 11 },
        { .str = "r12]", .tok = TOK_ADDR_REG_L, .reg = 12 },
        { .str = "r13]", .tok = TOK_ADDR_REG_L, .reg = 13 },
        { .str = "r14]", .tok = TOK_ADDR_REG_L, .reg = 14 },
        { .str = "r15]", .tok = TOK_ADDR_REG_L, .reg = 15 },
        { .str = "nop",  .tok = TOK_KW_NOP  },
        { .str = "halt", .tok = TOK_KW_HALT },
        { .str = "mov",  .tok = TOK_KW_MOV  },
        { .str = "push", .tok = TOK_KW_PUSH },
        { .str = "pop",  .tok = TOK_KW_POP  },
        { .str = "add",  .tok = TOK_KW_ADD  },
        { .str = "adc",  .tok = TOK_KW_ADC  },
        { .str = "sub",  .tok = TOK_KW_SUB  },
        { .str = "sbc",  .tok = TOK_KW_SBC  },
        { .str = "inc",  .tok = TOK_KW_INC  },
        { .str = "dec",  .tok = TOK_KW_DEC  },
        { .str = "neg",  .tok = TOK_KW_NEG  },
        { .str = "shl",  .tok = TOK_KW_SHL  },
        { .str = "shr",  .tok = TOK_KW_SHR  },
        { .str = "and",  .tok = TOK_KW_AND  },
        { .str = "or",   .tok = TOK_KW_OR   },
        { .str = "xor",  .tok = TOK_KW_XOR  },
        { .str = "not",  .tok = TOK_KW_NOT  },
        { .str = "cmp",  .tok = TOK_KW_CMP  },
        { .str = "jmp",  .tok = TOK_KW_JMP  },
        { .str = "jr",   .tok = TOK_KW_JR   },
        { .str = "jz",   .tok = TOK_KW_JZ   },
        { .str = "jnz",  .tok = TOK_KW_JNZ  },
        { .str = "jc",   .tok = TOK_KW_JC   },
        { .str = "jnc",  .tok = TOK_KW_JNC  },
        { .str = "call", .tok = TOK_KW_CALL },
        { .str = "ret",  .tok = TOK_KW_RET  }
    };

    for (size_t i = 0; i < sizeof(words)/sizeof(*words); i++) {
        if (!strncasecmp(word, words[i].str, len)) {
            *out = (Token) {
                .type = words[i].tok,
                .word = word,
                .len = len,
                .data.reg = words[i].reg
            };
            return 0;
        }
    }

    char tmp[len + 1];
    memcpy(tmp, word, len);
    tmp[len] = 0;

    #define LABEL_FIRST_CHAR "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_"
    #define LABEL_CHAR LABEL_FIRST_CHAR"1234567890"
    if (strchr(LABEL_FIRST_CHAR, word[0])) {
        size_t label_len = strspn(tmp, LABEL_CHAR);
        if (label_len == len - 1 && tmp[label_len] == ':') {
            *out = (Token) {
                .type = TOK_LABEL_DEF,
                .word = word,
                .len = len,
                .data.label = {
                    .name = word,
                    .len = len - 1
                }
            };
            return 0;
        }
        if (label_len == len) {
            *out = (Token) {
                .type = TOK_LABEL_REF,
                .word = word,
                .len = len,
                .data.label = {
                    .name = word,
                    .len = len
                }
            };
            return 0;
        }
    }

    if (tmp[0] == '[' && tmp[len - 1] == ']') {
        size_t label_len = strspn(tmp + 1, LABEL_CHAR);
        if (label_len == len - 2 && tmp[label_len + 1] == ']') {
            *out = (Token) {
                .type = TOK_ADDR_LABEL,
                .word = word,
                .len = len,
                .data.label = {
                    .name = word + 1,
                    .len = len - 2
                }
            };
            return 0;
        }
    }

    char *end;
    long tmp_value = strtol(tmp, &end, 0);
    if (!*end) {
        reci(tmp_value < 0 || tmp_value > UINT16_MAX,
             "parse const: out of uint16 range: %s", tmp);
        *out = (Token) {
            .type = TOK_CONST,
            .word = word,
            .len = len,
            .data.const_val = (uint16_t)tmp_value
        };
        return 0;
    }

    if (tmp[0] == '[' && tmp[len - 1] == ']') {
        long tmp_addr_value = strtol(tmp + 1, &end, 0);
        if (*end != ']') {
            reci(tmp_addr_value < 0 || tmp_addr_value > UINT16_MAX,
                 "parse addr: out of uint16 range: %s", tmp + 1);
            *out = (Token) {
                .type = TOK_ADDR_CONST,
                .word = word,
                .len = len,
                .data.address = (uint16_t)tmp_addr_value
            };
            return 0;
        }
    }

    return -1;
}

Token tokenizer_next(Tokenizer *this) {
    const char *next = NULL;
    size_t next_len = 0;

    if (get_next_word(this, &next, &next_len)) {
        return (Token) { .type = TOK_EOF };
    }
    Token token = { .type = TOK_UNKNOWN, .word = next, .len = next_len };
    int rv = parse_next_word(next, next_len, &token);

    if (rv || token.type == TOK_UNKNOWN) {
        Line *l = vec_line_at(&this->src->lines, this->line);
        SrcFile *f = vec_srcfile_at(&this->src->files, l->file_index);
        loge("Failed to parse token: %.*s from %s:%zu %.*s",
             (int)next_len, next, f->name.data, l->number, (int)l->text.size, l->text.data);
    }
    return token;
}

void tokenizer_delete(Tokenizer *this) {
    memset(this, 0, sizeof(*this));
}

const char *token_type_to_str(enum TokenType type) {
    const char *token_type_to_str[TOK_COUNT] = {
        [TOK_UNKNOWN]    = "TOK_UNKNOWN",
        [TOK_KW_NOP]     = "TOK_KW_NOP",
        [TOK_KW_HALT]    = "TOK_KW_HALT",
        [TOK_KW_MOV]     = "TOK_KW_MOV",
        [TOK_KW_PUSH]    = "TOK_KW_PUSH",
        [TOK_KW_POP]     = "TOK_KW_POP",
        [TOK_KW_ADD]     = "TOK_KW_ADD",
        [TOK_KW_ADC]     = "TOK_KW_ADC",
        [TOK_KW_SUB]     = "TOK_KW_SUB",
        [TOK_KW_SBC]     = "TOK_KW_SBC",
        [TOK_KW_INC]     = "TOK_KW_INC",
        [TOK_KW_DEC]     = "TOK_KW_DEC",
        [TOK_KW_NEG]     = "TOK_KW_NEG",
        [TOK_KW_SHL]     = "TOK_KW_SHL",
        [TOK_KW_SHR]     = "TOK_KW_SHR",
        [TOK_KW_AND]     = "TOK_KW_AND",
        [TOK_KW_OR]      = "TOK_KW_OR",
        [TOK_KW_XOR]     = "TOK_KW_XOR",
        [TOK_KW_NOT]     = "TOK_KW_NOT",
        [TOK_KW_CMP]     = "TOK_KW_CMP",
        [TOK_KW_JMP]     = "TOK_KW_JMP",
        [TOK_KW_JR]      = "TOK_KW_JR",
        [TOK_KW_JZ]      = "TOK_KW_JZ",
        [TOK_KW_JNZ]     = "TOK_KW_JNZ",
        [TOK_KW_JC]      = "TOK_KW_JC",
        [TOK_KW_JNC]     = "TOK_KW_JNC",
        [TOK_KW_CALL]    = "TOK_KW_CALL",
        [TOK_KW_RET]     = "TOK_KW_RET",
        [TOK_REG]        = "TOK_REG",
        [TOK_LABEL_DEF]  = "TOK_LABEL_DEF",
        [TOK_LABEL_REF]  = "TOK_LABEL_REF",
        [TOK_CONST]      = "TOK_CONST",
        [TOK_ADDR_CONST] = "TOK_ADDR_CONST",
        [TOK_ADDR_LABEL] = "TOK_ADDR_LABEL",
        [TOK_ADDR_REG_L] = "TOK_ADDR_REG_L",
        [TOK_ADDR_REG_H] = "TOK_ADDR_REG_H",
        [TOK_EOF]        = "TOK_EOF"
    };
    return token_type_to_str[type];
}
