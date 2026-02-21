#ifndef UNUSED_H
#define UNUSED_H

#define UNUSED_0()
#define UNUSED_1(A0) (void)A0;
#define UNUSED_2(A0, ...) (void)A0; UNUSED_1(__VA_ARGS__)
#define UNUSED_3(A0, ...) (void)A0; UNUSED_2(__VA_ARGS__)
#define UNUSED_4(A0, ...) (void)A0; UNUSED_3(__VA_ARGS__)
#define UNUSED_5(A0, ...) (void)A0; UNUSED_4(__VA_ARGS__)
#define UNUSED_N(A0, A1, A2, A3, A4, A5, CNT, ...) UNUSED ## CNT
#define UNUSED(...) UNUSED_N(__VA_ARGS__, _6, _5, _4, _3, _2, _1, _0)(__VA_ARGS__)

#endif // UNUSED_H
