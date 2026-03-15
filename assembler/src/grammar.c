#include "grammar.h"
#include "util.h"

int encode_op(VecByte *buff, uint8_t op);
int encode_op_reg_reg(VecByte *buff, uint8_t op, uint8_t rd, uint8_t rs);
int encode_op_reg(VecByte *buff, uint8_t op, uint8_t rd);
int encode_op_reg_imm8(VecByte *buff, uint8_t op, uint8_t rd, uint8_t imm);
int encode_op_reg_reg_reg(VecByte *buff, uint8_t op, uint8_t rd, uint8_t rs1, uint8_t rs2);
int encode_op_reg_imm16(VecByte *buff, uint8_t op, uint8_t rd, uint16_t imm);
int encode_op_imm16(VecByte *buff, uint8_t op, uint16_t imm);

int en_nop(struct EncodeCtx *encode_ctx);
int en_halt(struct EncodeCtx *encode_ctx);
int en_mov_reg_reg(struct EncodeCtx *encode_ctx);
int en_mov_reg_const(struct EncodeCtx *encode_ctx);
int en_mov_reg_addr_const(struct EncodeCtx *encode_ctx);
int en_mov_reg_addr_label(struct EncodeCtx *encode_ctx);
int en_mov_reg_addr_regs(struct EncodeCtx *encode_ctx);
int en_mov_addr_regs_reg(struct EncodeCtx *encode_ctx);
int en_mov_addr_const_reg(struct EncodeCtx *encode_ctx);
int en_mov_addr_label_reg(struct EncodeCtx *encode_ctx);
int en_push(struct EncodeCtx *encode_ctx);
int en_pop(struct EncodeCtx *encode_ctx);
int en_add_reg_reg(struct EncodeCtx *encode_ctx);
int en_adc_reg_reg(struct EncodeCtx *encode_ctx);
int en_sub_reg_reg(struct EncodeCtx *encode_ctx);
int en_sbc_reg_reg(struct EncodeCtx *encode_ctx);
int en_inc_reg(struct EncodeCtx *encode_ctx);
int en_dec_reg(struct EncodeCtx *encode_ctx);
int en_neg_reg(struct EncodeCtx *encode_ctx);
int en_shl_reg(struct EncodeCtx *encode_ctx);
int en_shr_reg(struct EncodeCtx *encode_ctx);
int en_and_reg_reg(struct EncodeCtx *encode_ctx);
int en_or_reg_reg(struct EncodeCtx *encode_ctx);
int en_xor_reg_reg(struct EncodeCtx *encode_ctx);
int en_not_reg(struct EncodeCtx *encode_ctx);
int en_cmp_reg_reg(struct EncodeCtx *encode_ctx);
int en_jmp_const(struct EncodeCtx *encode_ctx);
int en_jmp_label_ref(struct EncodeCtx *encode_ctx);
int en_jr_addr_regs(struct EncodeCtx *encode_ctx);
int en_jz_const(struct EncodeCtx *encode_ctx);
int en_jz_label_ref(struct EncodeCtx *encode_ctx);
int en_jnz_const(struct EncodeCtx *encode_ctx);
int en_jnz_label_ref(struct EncodeCtx *encode_ctx);
int en_jc_const(struct EncodeCtx *encode_ctx);
int en_jc_label_ref(struct EncodeCtx *encode_ctx);
int en_jnc_const(struct EncodeCtx *encode_ctx);
int en_jnc_label_ref(struct EncodeCtx *encode_ctx);
int en_call_const(struct EncodeCtx *encode_ctx);
int en_call_label_ref(struct EncodeCtx *encode_ctx);
int en_ret(struct EncodeCtx *encode_ctx);
int en_label_def(struct EncodeCtx *encode_ctx);

#define N(tok, ...) [tok] = &(struct GrammarTreeNode) { .next = { __VA_ARGS__ } }
#define L(tok, cb_f) [tok] = &(struct GrammarTreeNode) { .cb = cb_f }
const struct GrammarTreeNode grammar_tree = {
    .next = {
        L(TOK_KW_NOP, en_nop),
        L(TOK_KW_HALT, en_halt),
        N(TOK_KW_MOV,
            N(TOK_REG,
                L(TOK_REG, en_mov_reg_reg),
                L(TOK_CONST, en_mov_reg_const),
                L(TOK_ADDR_CONST, en_mov_reg_addr_const),
                L(TOK_ADDR_LABEL, en_mov_reg_addr_label),
                N(TOK_ADDR_REG_H, L(TOK_ADDR_REG_L, en_mov_reg_addr_regs))
            ),
            N(TOK_ADDR_REG_H, N(TOK_ADDR_REG_L, L(TOK_REG, en_mov_addr_regs_reg))),
            N(TOK_ADDR_CONST, L(TOK_REG, en_mov_addr_const_reg)),
            N(TOK_ADDR_LABEL, L(TOK_REG, en_mov_addr_label_reg))
        ),
        N(TOK_KW_PUSH, L(TOK_REG, en_push)),
        N(TOK_KW_POP, L(TOK_REG, en_pop)),

        N(TOK_KW_ADD, N(TOK_REG, L(TOK_REG, en_add_reg_reg))),
        N(TOK_KW_ADC, N(TOK_REG, L(TOK_REG, en_adc_reg_reg))),
        N(TOK_KW_SUB, N(TOK_REG, L(TOK_REG, en_sub_reg_reg))),
        N(TOK_KW_SBC, N(TOK_REG, L(TOK_REG, en_sbc_reg_reg))),
        N(TOK_KW_INC, L(TOK_REG, en_inc_reg)),
        N(TOK_KW_DEC, L(TOK_REG, en_dec_reg)),
        N(TOK_KW_NEG, L(TOK_REG, en_neg_reg)),
        N(TOK_KW_SHL, L(TOK_REG, en_shl_reg)),
        N(TOK_KW_SHR, L(TOK_REG, en_shr_reg)),
        N(TOK_KW_AND, N(TOK_REG, L(TOK_REG, en_and_reg_reg))),
        N(TOK_KW_OR, N(TOK_REG, L(TOK_REG, en_or_reg_reg))),
        N(TOK_KW_XOR, N(TOK_REG, L(TOK_REG, en_xor_reg_reg))),
        N(TOK_KW_NOT, L(TOK_REG, en_not_reg)),
        N(TOK_KW_CMP, N(TOK_REG, L(TOK_REG, en_cmp_reg_reg))),
        N(TOK_KW_JMP,
            L(TOK_CONST, en_jmp_const),
            L(TOK_LABEL_REF, en_jmp_label_ref)
        ),
        N(TOK_KW_JR, N(TOK_ADDR_REG_H, L(TOK_ADDR_REG_L, en_jr_addr_regs))),
        N(TOK_KW_JZ,
            L(TOK_CONST, en_jz_const),
            L(TOK_LABEL_REF, en_jz_label_ref)
        ),
        N(TOK_KW_JNZ,
            L(TOK_CONST, en_jnz_const),
            L(TOK_LABEL_REF, en_jnz_label_ref)
        ),
        N(TOK_KW_JC,
            L(TOK_CONST, en_jc_const),
            L(TOK_LABEL_REF, en_jc_label_ref)
        ),
        N(TOK_KW_JNC,
            L(TOK_CONST, en_jnc_const),
            L(TOK_LABEL_REF, en_jnc_label_ref)
        ),
        N(TOK_KW_CALL,
            L(TOK_CONST, en_call_const),
            L(TOK_LABEL_REF, en_call_label_ref)
        ),
        L(TOK_KW_RET, en_ret),
        L(TOK_LABEL_DEF, en_label_def)
    }
};

// ============================================================================
// ENCODE FUNCTIONS
// ============================================================================

int encode_op(VecByte *buff, uint8_t op) {
    vec_byte_push(buff, op);
    return 0;
}

int encode_op_reg(VecByte *buff, uint8_t op, uint8_t rd) {
    vec_byte_push(buff, op);
    vec_byte_push(buff, rd << 4);
    return 0;
}

int encode_op_reg_reg(VecByte *buff, uint8_t op, uint8_t rd, uint8_t rs) {
    vec_byte_push(buff, op);
    vec_byte_push(buff, (rd << 4) | rs);
    return 0;
}

int encode_op_reg_imm8(VecByte *buff, uint8_t op, uint8_t rd, uint8_t imm) {
    vec_byte_push(buff, op);
    vec_byte_push(buff, (rd << 4));
    vec_byte_push(buff, imm);
    return 0;
}

int encode_op_reg_reg_reg(VecByte *buff, uint8_t op, uint8_t rd, uint8_t rs1, uint8_t rs2) {
    vec_byte_push(buff, op);
    vec_byte_push(buff, (rd << 4) | rs1);
    vec_byte_push(buff, (rs2 << 4));
    return 0;
}

int encode_op_reg_imm16(VecByte *buff, uint8_t op, uint8_t rd, uint16_t imm) {
    vec_byte_push(buff, op);
    vec_byte_push(buff, (rd << 4));
    vec_byte_push(buff, (uint8_t)((imm >> 8) & 0xFF));
    vec_byte_push(buff, (uint8_t)(imm & 0xFF));
    return 0;
}

int encode_op_imm16(VecByte *buff, uint8_t op, uint16_t imm) {
    vec_byte_push(buff, op);
    vec_byte_push(buff, (uint8_t)((imm >> 8) & 0xFF));
    vec_byte_push(buff, (uint8_t)(imm & 0xFF));
    return 0;
}

// ============================================================================
// ENCODE CALLBACK FUNCTIONS
// ============================================================================

int en_nop(struct EncodeCtx *encode_ctx) {
    return encode_op(encode_ctx->buff, 0x00);
}

int en_halt(struct EncodeCtx *encode_ctx) {
    return encode_op(encode_ctx->buff, 0x04);
}

int en_mov_reg_reg(struct EncodeCtx *encode_ctx) {
    Token *rd = vec_token_at(encode_ctx->toks, 1);
    Token *rs = vec_token_at(encode_ctx->toks, 2);
    return encode_op_reg_reg(encode_ctx->buff, 0x21, rd->data.reg, rs->data.reg);
}

int en_mov_reg_const(struct EncodeCtx *encode_ctx) {
    Token *rd = vec_token_at(encode_ctx->toks, 1);
    Token *imm = vec_token_at(encode_ctx->toks, 2);
    return encode_op_reg_imm8(encode_ctx->buff, 0x26, rd->data.reg, (uint8_t)imm->data.const_val);
}

int en_mov_reg_addr_const(struct EncodeCtx *encode_ctx) {
    Token *rd = vec_token_at(encode_ctx->toks, 1);
    Token *addr = vec_token_at(encode_ctx->toks, 2);
    return encode_op_reg_imm16(encode_ctx->buff, 0x38, rd->data.reg, addr->data.address);
}

int en_mov_reg_addr_label(struct EncodeCtx *encode_ctx) {
    Token *rd = vec_token_at(encode_ctx->toks, 1);
    Token *label = vec_token_at(encode_ctx->toks, 2);
    label_resolver_request(
        encode_ctx->labels,
        label->data.label.name,
        label->data.label.len,
        vec_byte_length(encode_ctx->buff) + 2
    );
    return encode_op_reg_imm16(encode_ctx->buff, 0x38, rd->data.reg, 0xFFFF);
}

int en_mov_reg_addr_regs(struct EncodeCtx *encode_ctx) {
    Token *rd = vec_token_at(encode_ctx->toks, 1);
    Token *rs1 = vec_token_at(encode_ctx->toks, 2);
    Token *rs2 = vec_token_at(encode_ctx->toks, 3);
    return encode_op_reg_reg_reg(encode_ctx->buff,
                                 0x2A, rd->data.reg, rs1->data.reg, rs2->data.reg);
}

int en_mov_addr_regs_reg(struct EncodeCtx *encode_ctx) {
    Token *rd1 = vec_token_at(encode_ctx->toks, 1);
    Token *rd2 = vec_token_at(encode_ctx->toks, 2);
    Token *rs = vec_token_at(encode_ctx->toks, 3);
    return encode_op_reg_reg_reg(encode_ctx->buff,
                                 0x2E, rs->data.reg, rd1->data.reg, rd2->data.reg);
}

int en_mov_addr_const_reg(struct EncodeCtx *encode_ctx) {
    Token *addr = vec_token_at(encode_ctx->toks, 1);
    Token *rs = vec_token_at(encode_ctx->toks, 2);
    return encode_op_reg_imm16(encode_ctx->buff, 0x3C, rs->data.reg, addr->data.address);
}

int en_mov_addr_label_reg(struct EncodeCtx *encode_ctx) {
    Token *rd = vec_token_at(encode_ctx->toks, 1);
    Token *label = vec_token_at(encode_ctx->toks, 2);
    label_resolver_request(
        encode_ctx->labels,
        label->data.label.name,
        label->data.label.len,
        vec_byte_length(encode_ctx->buff) + 2
    );
    return encode_op_reg_imm16(encode_ctx->buff, 0x3C, rd->data.reg, 0xFFFF);
}

int en_push(struct EncodeCtx *encode_ctx) {
    Token *rs = vec_token_at(encode_ctx->toks, 1);
    return encode_op_reg_reg(encode_ctx->buff, 0x31, 0, rs->data.reg);
}

int en_pop(struct EncodeCtx *encode_ctx) {
    Token *rd = vec_token_at(encode_ctx->toks, 1);
    return encode_op_reg_reg(encode_ctx->buff, 0x35, rd->data.reg, 0);
}

int en_add_reg_reg(struct EncodeCtx *encode_ctx) {
    Token *rd = vec_token_at(encode_ctx->toks, 1);
    Token *rs = vec_token_at(encode_ctx->toks, 2);
    return encode_op_reg_reg(encode_ctx->buff, 0x41, rd->data.reg, rs->data.reg);
}

int en_adc_reg_reg(struct EncodeCtx *encode_ctx) {
    Token *rd = vec_token_at(encode_ctx->toks, 1);
    Token *rs = vec_token_at(encode_ctx->toks, 2);
    return encode_op_reg_reg(encode_ctx->buff, 0x45, rd->data.reg, rs->data.reg);
}

int en_sub_reg_reg(struct EncodeCtx *encode_ctx) {
    Token *rd = vec_token_at(encode_ctx->toks, 1);
    Token *rs = vec_token_at(encode_ctx->toks, 2);
    return encode_op_reg_reg(encode_ctx->buff, 0x49, rd->data.reg, rs->data.reg);
}

int en_sbc_reg_reg(struct EncodeCtx *encode_ctx) {
    Token *rd = vec_token_at(encode_ctx->toks, 1);
    Token *rs = vec_token_at(encode_ctx->toks, 2);
    return encode_op_reg_reg(encode_ctx->buff, 0x4D, rd->data.reg, rs->data.reg);
}

int en_inc_reg(struct EncodeCtx *encode_ctx) {
    Token *rd = vec_token_at(encode_ctx->toks, 1);
    return encode_op_reg_reg(encode_ctx->buff, 0x41, rd->data.reg, rd->data.reg);
}

int en_dec_reg(struct EncodeCtx *encode_ctx) {
    Token *rd = vec_token_at(encode_ctx->toks, 1);
    return encode_op_reg_reg(encode_ctx->buff, 0x49, rd->data.reg, rd->data.reg);
}

int en_neg_reg(struct EncodeCtx *encode_ctx) {
    Token *rd = vec_token_at(encode_ctx->toks, 1);
    return encode_op_reg_reg(encode_ctx->buff, 0x4B, rd->data.reg, rd->data.reg);
}

int en_shl_reg(struct EncodeCtx *encode_ctx) {
    Token *rd = vec_token_at(encode_ctx->toks, 1);
    return encode_op_reg(encode_ctx->buff, 0x4C, rd->data.reg);
}

int en_shr_reg(struct EncodeCtx *encode_ctx) {
    Token *rd = vec_token_at(encode_ctx->toks, 1);
    return encode_op_reg(encode_ctx->buff, 0x4D, rd->data.reg);
}

int en_and_reg_reg(struct EncodeCtx *encode_ctx) {
    Token *rd = vec_token_at(encode_ctx->toks, 1);
    Token *rs = vec_token_at(encode_ctx->toks, 2);
    return encode_op_reg_reg(encode_ctx->buff, 0x4E, rd->data.reg, rs->data.reg);
}

int en_or_reg_reg(struct EncodeCtx *encode_ctx) {
    Token *rd = vec_token_at(encode_ctx->toks, 1);
    Token *rs = vec_token_at(encode_ctx->toks, 2);
    return encode_op_reg_reg(encode_ctx->buff, 0x4F, rd->data.reg, rs->data.reg);
}

int en_xor_reg_reg(struct EncodeCtx *encode_ctx) {
    Token *rd = vec_token_at(encode_ctx->toks, 1);
    Token *rs = vec_token_at(encode_ctx->toks, 2);
    return encode_op_reg_reg(encode_ctx->buff, 0x50, rd->data.reg, rs->data.reg);
}

int en_not_reg(struct EncodeCtx *encode_ctx) {
    Token *rd = vec_token_at(encode_ctx->toks, 1);
    return encode_op_reg_reg(encode_ctx->buff, 0x51, rd->data.reg, rd->data.reg);
}

int en_cmp_reg_reg(struct EncodeCtx *encode_ctx) {
    Token *rd = vec_token_at(encode_ctx->toks, 1);
    Token *rs = vec_token_at(encode_ctx->toks, 2);
    return encode_op_reg_reg(encode_ctx->buff, 0x52, rd->data.reg, rs->data.reg);
}

int en_jmp_const(struct EncodeCtx *encode_ctx) {
    Token *addr = vec_token_at(encode_ctx->toks, 1);
    return encode_op_reg_imm16(encode_ctx->buff, 0xC0, 0, addr->data.address);
}

int en_jmp_label_ref(struct EncodeCtx *encode_ctx) {
    Token *label = vec_token_at(encode_ctx->toks, 1);
    label_resolver_request(
        encode_ctx->labels,
        label->data.label.name,
        label->data.label.len,
        vec_byte_length(encode_ctx->buff) + 1
    );
    return encode_op_imm16(encode_ctx->buff, 0xC0, 0xFFFF);
}

int en_jr_addr_regs(struct EncodeCtx *encode_ctx) {
    Token *rs1 = vec_token_at(encode_ctx->toks, 1);
    Token *rs2 = vec_token_at(encode_ctx->toks, 2);
    return encode_op_reg_reg(encode_ctx->buff, 0xC5, rs1->data.reg, rs2->data.reg);
}

int en_jz_const(struct EncodeCtx *encode_ctx) {
    Token *addr = vec_token_at(encode_ctx->toks, 1);
    return encode_op_reg_imm16(encode_ctx->buff, 0xC8, 0, addr->data.address);
}

int en_jz_label_ref(struct EncodeCtx *encode_ctx) {
    Token *label = vec_token_at(encode_ctx->toks, 1);
    label_resolver_request(
        encode_ctx->labels,
        label->data.label.name,
        label->data.label.len,
        vec_byte_length(encode_ctx->buff) + 1
    );
    return encode_op_imm16(encode_ctx->buff, 0xC8, 0xFFFF);
}

int en_jnz_const(struct EncodeCtx *encode_ctx) {
    Token *addr = vec_token_at(encode_ctx->toks, 1);
    return encode_op_reg_imm16(encode_ctx->buff, 0xCC, 0, addr->data.address);
}

int en_jnz_label_ref(struct EncodeCtx *encode_ctx) {
    Token *label = vec_token_at(encode_ctx->toks, 1);
    label_resolver_request(
        encode_ctx->labels,
        label->data.label.name,
        label->data.label.len,
        vec_byte_length(encode_ctx->buff) + 1
    );
    return encode_op_imm16(encode_ctx->buff, 0xCC, 0xFFFF);
}

int en_jc_const(struct EncodeCtx *encode_ctx) {
    Token *addr = vec_token_at(encode_ctx->toks, 1);
    return encode_op_reg_imm16(encode_ctx->buff, 0xD0, 0, addr->data.address);
}

int en_jc_label_ref(struct EncodeCtx *encode_ctx) {
    Token *label = vec_token_at(encode_ctx->toks, 1);
    label_resolver_request(
        encode_ctx->labels,
        label->data.label.name,
        label->data.label.len,
        vec_byte_length(encode_ctx->buff) + 1
    );
    return encode_op_imm16(encode_ctx->buff, 0xD0, 0xFFFF);
}

int en_jnc_const(struct EncodeCtx *encode_ctx) {
    Token *addr = vec_token_at(encode_ctx->toks, 1);
    return encode_op_reg_imm16(encode_ctx->buff, 0xD4, 0, addr->data.address);
}

int en_jnc_label_ref(struct EncodeCtx *encode_ctx) {
    Token *label = vec_token_at(encode_ctx->toks, 1);
    label_resolver_request(
        encode_ctx->labels,
        label->data.label.name,
        label->data.label.len,
        vec_byte_length(encode_ctx->buff) + 1
    );
    return encode_op_imm16(encode_ctx->buff, 0xD4, 0xFFFF);
}

int en_call_const(struct EncodeCtx *encode_ctx) {
    Token *addr = vec_token_at(encode_ctx->toks, 1);
    return encode_op_reg_imm16(encode_ctx->buff, 0xD8, 0, addr->data.address);
}

int en_call_label_ref(struct EncodeCtx *encode_ctx) {
    Token *label = vec_token_at(encode_ctx->toks, 1);
    label_resolver_request(
        encode_ctx->labels,
        label->data.label.name,
        label->data.label.len,
        vec_byte_length(encode_ctx->buff) + 1
    );
    return encode_op_imm16(encode_ctx->buff, 0xD8, 0xFFFF);
}

int en_ret(struct EncodeCtx *encode_ctx) {
    return encode_op(encode_ctx->buff, 0xDC);
}

int en_label_def(struct EncodeCtx *encode_ctx) {
    Token *label = vec_token_at(encode_ctx->toks, 0);
    label_resolver_define(
        encode_ctx->labels,
        label->data.label.name,
        label->data.label.len,
        vec_byte_length(encode_ctx->buff)
    );
    return 0;
}
