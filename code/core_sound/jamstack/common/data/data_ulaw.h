#ifndef JAMSTACK_DATA_ULAW_H_
#define JAMSTACK_DATA_ULAW_H_
#include <stdint.h>

const int16_t ulaw_decode_table[256] = {

    4,      12,     20,     28,     36,     44,     52,     60,     68,     76,     84,     92,     100,    108,
    116,    124,    136,    152,    168,    184,    200,    216,    232,    248,    264,    280,    296,    312,
    328,    344,    360,    376,    400,    432,    464,    496,    528,    560,    592,    624,    656,    688,
    720,    752,    784,    816,    848,    880,    928,    992,    1056,   1120,   1184,   1248,   1312,   1376,
    1440,   1504,   1568,   1632,   1696,   1760,   1824,   1888,   1984,   2112,   2240,   2368,   2496,   2624,
    2752,   2880,   3008,   3136,   3264,   3392,   3520,   3648,   3776,   3904,   4096,   4352,   4608,   4864,
    5120,   5376,   5632,   5888,   6144,   6400,   6656,   6912,   7168,   7424,   7680,   7936,   8320,   8832,
    9344,   9856,   10368,  10880,  11392,  11904,  12416,  12928,  13440,  13952,  14464,  14976,  15488,  16000,
    16768,  17792,  18816,  19840,  20864,  21888,  22912,  23936,  24960,  25984,  27008,  28032,  29056,  30080,
    31104,  32128,  -4,     -12,    -20,    -28,    -36,    -44,    -52,    -60,    -68,    -76,    -84,    -92,
    -100,   -108,   -116,   -124,   -136,   -152,   -168,   -184,   -200,   -216,   -232,   -248,   -264,   -280,
    -296,   -312,   -328,   -344,   -360,   -376,   -400,   -432,   -464,   -496,   -528,   -560,   -592,   -624,
    -656,   -688,   -720,   -752,   -784,   -816,   -848,   -880,   -928,   -992,   -1056,  -1120,  -1184,  -1248,
    -1312,  -1376,  -1440,  -1504,  -1568,  -1632,  -1696,  -1760,  -1824,  -1888,  -1984,  -2112,  -2240,  -2368,
    -2496,  -2624,  -2752,  -2880,  -3008,  -3136,  -3264,  -3392,  -3520,  -3648,  -3776,  -3904,  -4096,  -4352,
    -4608,  -4864,  -5120,  -5376,  -5632,  -5888,  -6144,  -6400,  -6656,  -6912,  -7168,  -7424,  -7680,  -7936,
    -8320,  -8832,  -9344,  -9856,  -10368, -10880, -11392, -11904, -12416, -12928, -13440, -13952, -14464, -14976,
    -15488, -16000, -16768, -17792, -18816, -19840, -20864, -21888, -22912, -23936, -24960, -25984, -27008, -28032,
    -29056, -30080, -31104, -32128

};

#endif
