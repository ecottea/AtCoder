#pragma once
#include "header.h"
// ¡¡¡¡¡ Z-‰ÁŒQ ¡¡¡¡¡


//yZ-‰ÁŒQz
/*
* Z-‰ÁŒQ (S, op, o, inv, mul) ‚ğ•\‚·D
*
* ‚·‚È‚í‚¿C(S, op, o, inv) ‚ªƒA[ƒxƒ‹ŒQ‚Å‚ ‚èCZ ‚Ì S ‚Ö‚Ì¶ì—p * : Z ~ S ¨ S ‚ª
*	—İ‰Á : Íx ¸ SCÍa ¸ Z,  a x = (x ‚Ì a ŒÂ‚Ì˜a) = mul(a, x)
* ‚ğ–‚½‚·‚à‚Ì‚Æ‚·‚éD
*/


//y‰ÁZ Z-‰ÁŒQz
using S301 = ll;
S301 op301(S301 x, S301 y) { return x + y; }
S301 o301() { return 0; }
S301 inv301(S301 x) { return -x; }
S301 mul301(int a, S301 x) { return a * x; }
#define Add_Zmodule S301, op301, o301, inv301, mul301


//yæZ Z-‰ÁŒQz
using S302 = mint;
S302 op302(S302 x, S302 y) { return x * y; }
S302 o302() { return 1; }
S302 inv302(S302 x) { return x.inv(); }
S302 mul302(int a, S302 x) { return x.pow(a); }
#define Mul_Zmodule S302, op302, o302, inv302, mul302


