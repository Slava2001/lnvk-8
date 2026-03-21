#include "op_codes.h"
#define bin_to_hex_000 00
#define bin_to_hex_001 01
#define bin_to_hex_010 02
#define bin_to_hex_011 03
#define bin_to_hex_100 04
#define bin_to_hex_101 05
#define bin_to_hex_110 06
#define bin_to_hex_111 07
#define bin_to_hex(a, b, c) bin_to_hex_ ## a ## b ## c
#define case_xx(test_index, code, op_a, op_b, flags_in, res, flags) \
    code ## op_a ## op_b ## flags_in ## res ## flags ## test_index
#define case_x(test_index, code, op_a, op_b, flags_in, res, flags) \
    case_xx(test_index, code, op_a, op_b, flags_in, res, flags)
#define case(test_index, code, op_a, op_b, flag_c_in, flag_z_in, flag_s_in, _, res, flag_c, flag_z, flag_s) \
    case_x(test_index, code, op_a, op_b, bin_to_hex(flag_c_in, flag_z_in, flag_s_in), res, bin_to_hex(flag_c, flag_z, flag_s))



case(00, OP_SHL, 00, 00, 0, 0, 0, EXPECT:, 00, 0, 1, 0)
case(01, OP_SHL, FF, 00, 0, 0, 0, EXPECT:, FE, 1, 0, 1)
case(02, OP_SHL, 80, 00, 0, 0, 0, EXPECT:, 00, 1, 1, 0)
case(03, OP_SHL, 01, 00, 0, 0, 0, EXPECT:, 02, 0, 0, 0)
case(04, OP_SHL, 7F, 00, 0, 0, 0, EXPECT:, FE, 0, 0, 1)
case(10, OP_SHR, 00, 00, 0, 0, 0, EXPECT:, 00, 0, 1, 0)
case(11, OP_SHR, 01, 00, 0, 0, 0, EXPECT:, 00, 1, 1, 0)
case(12, OP_SHR, FF, 00, 0, 0, 0, EXPECT:, 7F, 1, 0, 0)
case(13, OP_SHR, 80, 00, 0, 0, 0, EXPECT:, 40, 0, 0, 0)
case(14, OP_SHR, 02, 00, 0, 0, 0, EXPECT:, 01, 0, 0, 0)
case(20, OP_INC, 00, 00, 0, 0, 0, EXPECT:, 01, 0, 0, 0)
case(21, OP_INC, FF, 00, 0, 0, 0, EXPECT:, 00, 0, 1, 0)
case(22, OP_INC, 7F, 00, 0, 0, 0, EXPECT:, 80, 0, 0, 1)
case(23, OP_INC, FE, 00, 0, 0, 0, EXPECT:, FF, 0, 0, 1)
case(24, OP_INC, 01, 00, 0, 0, 0, EXPECT:, 02, 0, 0, 0)
case(30, OP_DEC, 00, 00, 0, 0, 0, EXPECT:, FF, 0, 0, 1)
case(31, OP_DEC, 01, 00, 0, 0, 0, EXPECT:, 00, 0, 1, 0)
case(32, OP_DEC, 80, 00, 0, 0, 0, EXPECT:, 7F, 0, 0, 0)
case(33, OP_DEC, FF, 00, 0, 0, 0, EXPECT:, FE, 0, 0, 1)
case(34, OP_DEC, 02, 00, 0, 0, 0, EXPECT:, 01, 0, 0, 0)
case(40, OP_NOT, 00, 00, 0, 0, 0, EXPECT:, FF, 0, 0, 1)
case(41, OP_NOT, FF, 00, 0, 0, 0, EXPECT:, 00, 0, 1, 0)
case(42, OP_NOT, 80, 00, 0, 0, 0, EXPECT:, 7F, 0, 0, 0)
case(43, OP_NOT, 7F, 00, 0, 0, 0, EXPECT:, 80, 0, 0, 1)
case(44, OP_NOT, 55, 00, 0, 0, 0, EXPECT:, AA, 0, 0, 1)
case(50, OP_ADD, 00, 00, 0, 0, 0, EXPECT:, 00, 0, 1, 0)
case(51, OP_ADD, FF, 01, 0, 0, 0, EXPECT:, 00, 1, 1, 0)
case(52, OP_ADD, 7F, 01, 0, 0, 0, EXPECT:, 80, 0, 0, 1)
case(53, OP_ADD, 80, 80, 0, 0, 0, EXPECT:, 00, 1, 1, 0)
case(54, OP_ADD, 0F, 01, 0, 0, 0, EXPECT:, 10, 0, 0, 0)
case(60, OP_SUB, 00, 00, 0, 0, 0, EXPECT:, 00, 1, 1, 0)
case(61, OP_SUB, 00, 01, 0, 0, 0, EXPECT:, FF, 0, 0, 1)
case(62, OP_SUB, 80, 01, 0, 0, 0, EXPECT:, 7F, 1, 0, 0)
case(63, OP_SUB, 01, 01, 0, 0, 0, EXPECT:, 00, 1, 1, 0)
case(64, OP_SUB, FF, 01, 0, 0, 0, EXPECT:, FE, 1, 0, 1)
case(70, OP_OR, 00, 00, 0, 0, 0, EXPECT:, 00, 0, 1, 0)
case(71, OP_OR, 00, FF, 0, 0, 0, EXPECT:, FF, 0, 0, 1)
case(72, OP_OR, 55, AA, 0, 0, 0, EXPECT:, FF, 0, 0, 1)
case(73, OP_OR, 80, 01, 0, 0, 0, EXPECT:, 81, 0, 0, 1)
case(74, OP_OR, 7F, 00, 0, 0, 0, EXPECT:, 7F, 0, 0, 0)
case(80, OP_XOR, FF, FF, 0, 0, 0, EXPECT:, 00, 0, 1, 0)
case(81, OP_XOR, AA, 55, 0, 0, 0, EXPECT:, FF, 0, 0, 1)
case(82, OP_XOR, 80, 80, 0, 0, 0, EXPECT:, 00, 0, 1, 0)
case(83, OP_XOR, 01, 00, 0, 0, 0, EXPECT:, 01, 0, 0, 0)
case(84, OP_XOR, 7F, FF, 0, 0, 0, EXPECT:, 80, 0, 0, 1)
case(90, OP_ADC, 00, 00, 1, 0, 0, EXPECT:, 01, 0, 0, 0)
case(91, OP_ADC, FF, 00, 1, 0, 0, EXPECT:, 00, 1, 1, 0)
case(92, OP_ADC, 7F, 00, 1, 0, 0, EXPECT:, 80, 0, 0, 1)
case(93, OP_ADC, FF, FF, 1, 0, 0, EXPECT:, FF, 1, 0, 1)
case(94, OP_ADC, 01, 01, 1, 0, 0, EXPECT:, 03, 0, 0, 0)
case(A0, OP_SBC, 00, 00, 1, 0, 0, EXPECT:, FF, 0, 0, 1)
case(A1, OP_SBC, 01, 01, 1, 0, 0, EXPECT:, FF, 0, 0, 1)
case(A2, OP_SBC, 02, 01, 1, 0, 0, EXPECT:, 00, 1, 1, 0)
case(A3, OP_SBC, 80, 01, 1, 0, 0, EXPECT:, 7E, 1, 0, 0)
case(A4, OP_SBC, 00, 01, 1, 0, 0, EXPECT:, FE, 0, 0, 1)
case(B0, OP_AND, FF, FF, 0, 0, 0, EXPECT:, FF, 0, 0, 1)
case(B1, OP_AND, 00, FF, 0, 0, 0, EXPECT:, 00, 0, 1, 0)
case(B2, OP_AND, AA, 55, 0, 0, 0, EXPECT:, 00, 0, 1, 0)
case(B3, OP_AND, F0, 0F, 0, 0, 0, EXPECT:, 00, 0, 1, 0)
case(B4, OP_AND, 80, FF, 0, 0, 0, EXPECT:, 80, 0, 0, 1)
case(C0, OP_SUB, 00, 00, 0, 0, 0, EXPECT:, 00, 1, 1, 0)
case(C1, OP_SUB, 01, 00, 0, 0, 0, EXPECT:, 01, 1, 0, 0)
case(C2, OP_SUB, 00, 01, 0, 0, 0, EXPECT:, FF, 0, 0, 1)
case(C3, OP_SUB, 80, 80, 0, 0, 0, EXPECT:, 00, 1, 1, 0)
case(C4, OP_SUB, FF, 01, 0, 0, 0, EXPECT:, FE, 1, 0, 1)
0xffffffffffffff
