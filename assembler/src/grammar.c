#include "grammar.h"
#include "util.h"

#define GRAMMAR_TREE_DEPTH 4
struct GrammarTreeEntry {
    encoder_t encoder;
    size_t len;
    enum TokenType types[GRAMMAR_TREE_DEPTH];
};

#define ALL_ENTRYS(PRC)                                        \
    PRC(TOK_KW_NOP),                                           \
    PRC(TOK_KW_HALT),                                          \
    PRC(TOK_KW_MOV,  TOK_REG, TOK_REG),                        \
    PRC(TOK_KW_MOV,  TOK_REG, TOK_CONST),                      \
    PRC(TOK_KW_MOV,  TOK_REG, TOK_ADDR_REG_H, TOK_ADDR_REG_L), \
    PRC(TOK_KW_MOV,  TOK_ADDR_REG_H, TOK_ADDR_REG_L, TOK_REG), \
    PRC(TOK_KW_MOV,  TOK_REG, TOK_ADDR_CONST),                 \
    PRC(TOK_KW_MOV,  TOK_REG, TOK_ADDR_LABEL),                 \
    PRC(TOK_KW_MOV,  TOK_ADDR_CONST, TOK_REG),                 \
    PRC(TOK_KW_MOV,  TOK_ADDR_LABEL, TOK_REG),                 \
    PRC(TOK_KW_PUSH, TOK_REG),                                 \
    PRC(TOK_KW_POP,  TOK_REG),                                 \
    PRC(TOK_KW_ADD,  TOK_REG, TOK_REG),                        \
    PRC(TOK_KW_ADC,  TOK_REG, TOK_REG),                        \
    PRC(TOK_KW_SUB,  TOK_REG, TOK_REG),                        \
    PRC(TOK_KW_SBC,  TOK_REG, TOK_REG),                        \
    PRC(TOK_KW_INC,  TOK_REG),                                 \
    PRC(TOK_KW_DEC,  TOK_REG),                                 \
    PRC(TOK_KW_NEG,  TOK_REG),                                 \
    PRC(TOK_KW_SHL,  TOK_REG),                                 \
    PRC(TOK_KW_SHR,  TOK_REG),                                 \
    PRC(TOK_KW_AND,  TOK_REG, TOK_REG),                        \
    PRC(TOK_KW_OR,   TOK_REG, TOK_REG),                        \
    PRC(TOK_KW_XOR,  TOK_REG, TOK_REG),                        \
    PRC(TOK_KW_NOT,  TOK_REG),                                 \
    PRC(TOK_KW_CMP,  TOK_REG, TOK_REG),                        \
    PRC(TOK_KW_JMP,  TOK_CONST),                               \
    PRC(TOK_KW_JMP,  TOK_LABEL_REF),                           \
    PRC(TOK_KW_JR,   TOK_ADDR_REG_H, TOK_ADDR_REG_L),          \
    PRC(TOK_KW_JZ,   TOK_CONST),                               \
    PRC(TOK_KW_JZ,   TOK_LABEL_REF),                           \
    PRC(TOK_KW_JNZ,  TOK_CONST),                               \
    PRC(TOK_KW_JNZ,  TOK_LABEL_REF),                           \
    PRC(TOK_KW_JC,   TOK_CONST),                               \
    PRC(TOK_KW_JC,   TOK_LABEL_REF),                           \
    PRC(TOK_KW_JNC,  TOK_CONST),                               \
    PRC(TOK_KW_JNC,  TOK_LABEL_REF),                           \
    PRC(TOK_KW_CALL, TOK_CONST),                               \
    PRC(TOK_KW_CALL, TOK_LABEL_REF),                           \
    PRC(TOK_KW_RET)

#define ENCODE_FN_7(a, b, c, d, e, f, g) ENC_ ## a ## b ## c ## d ## e ## f ## g
#define ENCODE_FN(...) ENCODE_FN_7(__VA_ARGS__, , , , , ,)
#define ENCODE_FN_DECL(...) size_t ENCODE_FN(__VA_ARGS__)(const Token *tokens, size_t len, uint8_t *output, size_t output_size);
#define ENCODE_FN_IMPL(...) size_t ENCODE_FN(__VA_ARGS__)(const Token *tokens, size_t len, uint8_t *output, size_t output_size) {return 0;}
ALL_ENTRYS(ENCODE_FN_DECL)
ALL_ENTRYS(ENCODE_FN_IMPL)

#define GTE(encode_f, ...) ENCODE_FN_DECL(__VA_ARGS__) {                   \
    .encoder = ENCODE_FN(__VA_ARGS__),                                     \
    .len = sizeof((enum TokenType[]){__VA_ARGS__})/sizeof(enum TokenType), \
    .types = {__VA_ARGS__}                                                 \
}
const struct GrammarTreeEntry grammar_tree_entries[] = {
    ALL_ENTRYS(GTE)
};
#undef GTE

GrammarNode* grammar_new(void) {
    GrammarNode *root = mem_alloc(sizeof(GrammarNode));
    for (size_t i = 0; i < sizeof(grammar_tree_entries)/sizeof(*grammar_tree_entries); i++) {
        GrammarNode *current = root;
        const struct GrammarTreeEntry *entry = &grammar_tree_entries[i];
        for (size_t j = 0; j < entry->len; j++) {
            enum TokenType type = entry->types[j];
            if (!current->next[type]) {
                current->next[type] = mem_alloc(sizeof(GrammarNode));
                memset(current->next[type], 0, sizeof(GrammarNode));
            }
            current = current->next[type];
        }
        current->encoder = entry->encoder;
    }
    return root;
}

void grammar_delete(GrammarNode* this) {
    if (!this) {
        return;
    }
    for (size_t i = 0; i < TOK_COUNT; i++) {
        grammar_delete(this->next[i]);
    }
    mem_free(this);
}
