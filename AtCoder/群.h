#pragma once
#include "header.h"
// ¡¡¡¡¡ ŒQ ¡¡¡¡¡


//yŒQz
/*
* ŒQ (S, op, e, inv) ‚ğ•\‚·D
*
* ‚·‚È‚í‚¿CW‡ S ‚Æ‚»‚Ìã‚Ì“ñ€‰‰Z * : S ~ S ¨ S ‚Å
*	Œ‹‡—¥ : Ía, b, c ¸ SC (a b) c = a (b c)
*	’PˆÊŒ³ : Ía ¸ S,        a e = e a = a
*	‹tŒ³   : Ía ¸ S,        a inv(a) = inv(a) a = e
* ‚ğ–‚½‚·‚à‚Ì‚Æ‚·‚éD
*/


//y‰ÁZ ŒQz
using S601 = ll;
S601 op601(S601 a, S601 b) { return a + b; }
S601 o601() { return 0; }
S601 inv601(S601 a) { return -a; }
#define Add_group S601, op601, o601, inv601


//yæZ ŒQz
using S602 = mint;
S602 op602(S602 a, S602 b) { return a * b; }
S602 o602() { return 1; }
S602 inv602(S602 a) { return a.inv(); }
#define Mul_group S602, op602, o602, inv602


//yxor ŒQz
using S603 = int;
S603 op603(S603 a, S603 b) { return a ^ b; }
S603 o603() { return 0; }
S603 inv603(S603 a) { return a; }
#define XOR_group S603, op603, o603, inv603


//y‰Â‹tƒAƒtƒBƒ“•ÏŠ·‚Ì‡¬ ŒQz
/*
* S ¹ f = {a, b} : ˆêŸŠÖ” f(x) = a x + b ‚ğ•\‚·Dia != 0j
* f op g : ‡¬‚µ‚½ˆêŸŠÖ” f o g ‚ğ•Ô‚·D
*
* ³‘¥s—ñ (a, b; 0, 1) ‚Ì‘S‘Ì‚ªÏ‚ÉŠÖ‚µ‚Äì‚Á‚Ä‚¢‚éŒQ‚Æ‚à‚İ‚È‚¹‚éD
*/
// verify : https://judge.yosupo.jp/problem/queue_operate_all_composite
using S604 = pair<mint, mint>;
S604 op604(S604 f, S604 g) {
	mint a, b, c, d;
	tie(a, b) = f; // f(x) = a x + b;
	tie(c, d) = g; // g(x) = c x + d;

	// (f o g)(x) = a (c x + d) + b = (a c)x + (a d + b)
	return { a * c, a * d + b };
}
S604 e604() { return { 1, 0 }; } // e(x) = x = 1 x + 0
S604 inv604(S604 f) {
	mint a, b;
	tie(a, b) = f; // f(x) = a x + b;

	// f(x) = a x + b Ì x = (1/a) f(x) - b/a
	return { a.inv(), -b / a };
}
#define Affine_composite_group S604, op604, e604, inv604


//yƒrƒbƒg—ñã “]“|” ŒQz
/*
* S ¹ x = {inv, c0, c1} : —ñ x ‚Ì“]“|”C0 ‚ÌŒÂ”C1 ‚ÌŒÂ”‚Ì‘g
* x op y : —ñ x, y ‚ğ˜AŒ‹‚µ‚½—ñ
*/
// Ql : https://qiita.com/hamko/items/92660ac5aed9df4d346d
// verify : https://atcoder.jp/contests/dwacon5th-prelims/tasks/dwacon5th_prelims_c
using S605 = tuple<ll, ll, ll>;
S605 op605(S605 x, S605 y) {
	ll x_inv, y_inv, x_c0, x_c1, y_c0, y_c1;
	tie(x_inv, x_c0, x_c1) = x;
	tie(y_inv, y_c0, y_c1) = y;

	// ‚Ü‚¸ x, y ‚»‚ê‚¼‚ê‚ğƒ\[ƒg‚·‚é‚Ì‚É x_inv + y_inv ‰ñ‚Ì—×ÚŒİŠ·‚ª•K—vD
	// ‚»‚ÌŒã x ‚Ì‰E‘¤‚ÉŠñ‚Á‚½ x_c1 ŒÂ‚Ì 1 ‚Æ y ‚Ì¶‘¤‚ÉŠñ‚Á‚½ y_c0 ŒÂ‚Ì 0 ‚ğ
	// ŒğŠ·‚·‚é‚Ì‚É x_c1 * y_c0 ‰ñ‚Ì—×ÚŒİŠ·‚ª•K—vD
	ll inv = x_inv + y_inv + x_c1 * y_c0;
	ll c0 = x_c0 + y_c0, c1 = x_c1 + y_c1;

	return { inv, c0, c1 };
}
S605 e605() { return { 0LL, 0, 0 }; }
S605 inv605(S605 x) {
	ll x_inv, x_c0, x_c1;
	tie(x_inv, x_c0, x_c1) = x;
	
	// ƒrƒbƒg—ñ‚ğŠg’£‚µC0^(-1), 1^(-1) ‚Æ‚¢‚¤—v‘f‚ğ”F‚ß‚é‚±‚Æ‚É‚·‚éD
	// ‚±‚ê‚ç‚Í 0, 1 ‚»‚ê‚¼‚ê -1 ŒÂ‚Æ‚µ‚Ä”‚¦C“]“|‚É‚Â‚¢‚Ä‚Í‹t‚ÌU‚é•‘‚¢‚ğ‚·‚éD
	return { x_c0 * x_c1 - x_inv, -x_c0, -x_c1 };
}
#define Inversion_group S605, op605, e605, inv605


//y’uŠ·‚Ì‡¬ ŒQziQÆ“n‚µ‚µ‚Ä‚¢‚È‚¢‚Ì‚Å’x‚¢j
/*
* S ¹ f[0..n) : ’uŠ· i ¨ f[i] ‚ğ•\‚·D
* f op g : ‡¬’uŠ· f o g ‚ğ•Ô‚·D
*/
using S606 = vi;
S606 op606(S606 a, S606 b) {
	if (sz(a) == 0) return b;
	if (sz(b) == 0) return a;

	int n = sz(a);
	S606 res(n);
	rep(i, n) res[i] = a[b[i]];

	return res;
}
S606 e606() { return S606(); }
S606 inv606(S606 a) {
	if (sz(a) == 0) return a;

	int n = sz(a);
	S606 res(n);
	rep(i, n) res[a[i]] = i;

	return res;
}
#define Permutation_composite_group S606, op606, e606, inv606


//y’uŠ·‚Ì‹t‡¬ ŒQziQÆ“n‚µ‚µ‚Ä‚¢‚È‚¢‚Ì‚Å’x‚¢j
/*
* S ¹ f[0..n) : ’uŠ· i ¨ f[i] ‚ğ•\‚·D
* f op g : ‡¬’uŠ· g o f ‚ğ•Ô‚·D
*/
using S607 = vi;
S607 op607(S607 a, S607 b) {
	if (sz(a) == 0) return b;
	if (sz(b) == 0) return a;

	int n = sz(a);
	S607 res(n);
	rep(i, n) res[i] = b[a[i]];

	return res;
}
S607 e607() { return S607(); }
S607 inv607(S607 a) {
	if (sz(a) == 0) return a;

	int n = sz(a);
	S607 res(n);
	rep(i, n) res[a[i]] = i;

	return res;
}
#define Permutation_invcomposite_group S607, op607, e607, inv607


