#ifndef INCLUDE_OP_CODES_H
#define INCLUDE_OP_CODES_H

#define OP_NOP      0x00
#define OP_HALT     0x04
#define OP_MOV      0x21
#define OP_MOV_IMM  0x26
#define OP_MOV_REG  0x2A
#define OP_MOV_MEM  0x2E
#define OP_PUSH     0x31
#define OP_POP      0x35
#define OP_MOV_ADDR 0x38
#define OP_MOV_MEM2 0x3C
#define OP_SHL      0x41
#define OP_SHR      0x45
#define OP_INC      0x49
#define OP_DEC      0x4D
#define OP_NOT      0x51
#define OP_ADD      0x61
#define OP_SUB      0x65
#define OP_OR       0x69
#define OP_XOR      0x6D
#define OP_ADC      0x71
#define OP_SBC      0x75
#define OP_AND      0x79
#define OP_CMP      0xA5
#define OP_JMP      0xC0
#define OP_JR       0xC5
#define OP_JZ       0xC8
#define OP_JNZ      0xCC
#define OP_JC       0xD0
#define OP_JNC      0xD4
#define OP_JS       0xD8
#define OP_JNS      0xDC
#define OP_CALL     0xE0
#define OP_RET      0xE4

#endif // INCLUDE_OP_CODES_H
