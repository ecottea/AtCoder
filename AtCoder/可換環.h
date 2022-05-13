#pragma once
#include "header.h"
// ¡¡¡¡¡ ‰ÂŠ·ŠÂ ¡¡¡¡¡


//y‰ÂŠ·ŠÂz
/*
* ‰ÂŠ·ŠÂ (S, add, o, mi, mul, e) ‚ğ•\‚·D
*
* ‚·‚È‚í‚¿C(S, add, o, mi) ‚ªƒA[ƒxƒ‹ŒQC(S, mul, e) ‚ª‰ÂŠ·ƒ‚ƒmƒCƒh‚ÅC
*	•ª”z—¥ : Ía, b, c ¸ SC a(b + c) = a b + a c, (a + b)c = a c + b c
* ‚ğ–‚½‚·‚à‚Ì‚Æ‚·‚éD
*/


//y‰ÁZ - æZ ‰ÂŠ·ŠÂz
/* verify : https://judge.yosupo.jp/problem/bitwise_and_convolution */
using S401 = ll;
S401 add401(S401 x, S401 y) { return x + y; }
S401 o401() { return 0; }
S401 mi401(S401 x) { return -x; }
S401 mul401(S401 x, S401 y) { return x * y; }
S401 e401() { return 1; }
#define Add_mul_cring S401, add401, o401, mi401, mul401, e401


//yXOR - AND ‰ÂŠ·ŠÂz
using S402 = unsigned int;
S402 add402(S402 x, S402 y) { return x ^ y; }
S402 o402() { return 0; }
S402 mi402(S402 x) { return x; }
S402 mul402(S402 x, S402 y) { return x & y; }
S402 e402() { return ~0; }
#define XOR_AND_cring S402, add402, o402, mi402, mul402, e402


