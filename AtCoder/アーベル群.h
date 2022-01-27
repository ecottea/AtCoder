#pragma once
#include "header.h"
// ¡¡¡¡¡ ƒA[ƒxƒ‹ŒQ ¡¡¡¡¡


//yƒA[ƒxƒ‹ŒQz
/*
* ƒA[ƒxƒ‹ŒQ (S, op, o, inv) ‚ğ•\‚·D
*
* ‚·‚È‚í‚¿CW‡ S ‚Æ‚»‚Ìã‚Ì“ñ€‰‰Z + : S ~ S ¨ S ‚Å
*	Œ‹‡—¥ : Ía, b, c ¸ S,  (a + b) + c = a + (b + c)
*	—ëŒ³   : Ía ¸ S,        a + o = o + a = a
*   ‹tŒ³   : Ía ¸ S,        a + inv(a) = inv(a) + a = o
*	ŒğŠ·—¥ : Ía, b ¸ S,     a + b = b + a
* ‚ğ–‚½‚·‚à‚Ì‚Æ‚·‚éD
*/


//y‰ÁZ ƒA[ƒxƒ‹ŒQz
using S201 = ll;
S201 op201(S201 a, S201 b) { return a + b; }
S201 o201() { return 0; }
S201 inv201(S201 a) { return -a; }
#define Add_abel_group S201, op201, o201, inv201


//yæZ ƒA[ƒxƒ‹ŒQz
using S202 = mint;
S202 op202(S202 a, S202 b) { return a * b; }
S202 o202() { return 1; }
S202 inv202(S202 a) { return a.inv(); }
#define Mul_abel_group S202, op202, o202, inv202


//yxor ƒA[ƒxƒ‹ŒQz
using S203 = int;
S203 op203(S203 a, S203 b) { return a ^ b; }
S203 o203() { return 0; }
S203 inv203(S203 a) { return a; }
#define XOR_abel_group S203, op203, o203, inv203


