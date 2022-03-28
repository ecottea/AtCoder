#pragma once
#include "header.h"
// ■■■■■ モノイド ■■■■■


//【モノイド】
/*
* モノイド (S, op, e) を表す．
* 
* すなわち，集合 S とその上の二項演算 * : S × S → S で
*	結合律 : ∀a, b, c ∈ S， (a b) c = a (b c)
*	単位元 : ∀a ∈ S,        a e = e a = a
* を満たすものとする．
*/


//【加算 モノイド】
using S1 = int;
S1 op1(S1 a, S1 b) { return a + b; }
S1 e1() { return 0; }
#define Add_monoid S1, op1, e1


//【乗算 モノイド】
using S2 = int;
S2 op2(S2 a, S2 b) { return a * b; }
S2 e2() { return 1; }
#define Mul_monoid S2, op2, e2


//【逆乗算 モノイド】
// verify : https://atcoder.jp/contests/arc025/tasks/arc025_4
using S20 = int;
S20 op20(S20 a, S20 b) { return b * a; }
S20 e20() { return 1; }
#define Invmul_monoid S20, op20, e20


//【max モノイド】
// verify : https://atcoder.jp/contests/abc134/tasks/abc134_c
using S3 = int;
S3 op3(S3 a, S3 b) { return max(a, b); }
S3 e3() { return -INF; }
#define Max_monoid S3, op3, e3


//【min モノイド】
using S4 = int;
S4 op4(S4 a, S4 b) { return min(a, b); }
S4 e4() { return INF; }
#define Min_monoid S4, op4, e4


//【xor モノイド】
using S10 = int;
S10 op10(S10 a, S10 b) { return a ^ b; }
S10 e10() { return 0; }
#define XOR_monoid S10, op10, e10


//【or モノイド】
// verify : https://atcoder.jp/contests/abc157/tasks/abc157_e
using S11 = int;
S11 op11(S11 a, S11 b) { return a | b; }
S11 e11() { return 0; }
#define OR_monoid S11, op11, e11


//【and モノイド】
using S12 = int;
S12 op12(S12 a, S12 b) { return a & b; }
S12 e12() { return ~0; }
#define AND_monoid S12, op12, e12


//【gcd モノイド】
// verify : https://atcoder.jp/contests/arc017/tasks/arc017_4
using S15 = ll;
S15 op15(S15 a, S15 b) { return gcd(a, b); }
S15 e15() { return 0; }
#define GCD_monoid S15, op15, e15


//【lcm モノイド】
using S16 = ll;
S16 op16(S16 a, S16 b) { return a / gcd(a, b) * b; }
S16 e16() { return 1; }
#define LCM_monoid S16, op16, e16


//【左変更 モノイド】
using S5 = int;
S5 e5() { return INF; } // 使わない値なら何でも OK
S5 op5(S5 a, S5 b) { return a == e5() ? b : a; }
#define Lupdate_monoid S5, op5, e5


//【右変更 モノイド】
using S6 = int;
S6 e6() { return INF; } // 使わない値なら何でも OK
S6 op6(S6 a, S6 b) { return b == e6() ? a : b; }
#define Rupdate_monoid S6, op6, e6


//【文字列連結 モノイド】
using S7 = string;
S7 op7(S7 a, S7 b) { return a + b; }
S7 e7() { return ""; }
#define Join_monoid S7, op7, e7


//【アフィン変換の合成 モノイド】
/*
* S ∋ f = {a, b} : 一次関数 f(x) = a x + b を表す．
* f op g : 合成した一次関数 f o g を返す．
* 
* 行列 (a, b; 0, 1) の全体が積に関して作っているモノイドともみなせる．
*/
using S8 = pair<mint, mint>;
S8 op8(S8 f, S8 g) {
	mint a, b, c, d;
	tie(a, b) = f; // f(x) = a x + b;
	tie(c, d) = g; // g(x) = c x + d;

	// (f o g)(x) = a (c x + d) + b = (a c)x + (a d + b)
	return { a * c, a * d + b };
}
S8 e8() { return { 1, 0 }; } // e(x) = x = 1 x + 0
#define Affine_composite_monoid S8, op8, e8


//【アフィン変換の逆合成 モノイド】
/*
* S ∋ f = {a, b} : 一次関数 f(x) = a x + b を表す．
* f op g : 逆向きに合成した一次関数 g o f を返す．
*/
// verify : https://judge.yosupo.jp/problem/point_set_range_composite
using S9 = pair<mint, mint>;
S9 op9(S9 f, S9 g) {
	mint a, b, c, d;
	tie(a, b) = g; // g(x) = a x + b;
	tie(c, d) = f; // f(x) = c x + d;

	// (g o f)(x) = a (c x + d) + b = (a c)x + (a d + b)
	return { a * c, a * d + b };
}
S9 e9() { return { 1, 0 }; } // e(x) = x = 1 x + 0
#define Affine_invcomposite_monoid S9, op9, e9


//【トロピカルアフィン変換の合成 モノイド】
/*
* S ∋ f = {a, b} : トロピカル一次関数 f(x) = max(a + x, b) を表す．
* f op g : 合成したトロピカル一次関数 f o g を返す．
*
* トロピカル半環上の行列 (a, b; -∞, 0) の全体が積に関して作っているモノイドともみなせる．
*/
using S13 = pair<ll, ll>;
S13 op13(S13 f, S13 g) {
	ll a, b, c, d;
	tie(a, b) = f; // f(x) = max(a + x, b);
	tie(c, d) = g; // g(x) = max(c + x, d);

	// (f o g)(x) = max(a + max(c + x, d), b) = max((a + c) + x, max(a + d, b))
	return { a + c, max(a + d, b) };
}
S13 e13() { return { 0, -INFL }; } // e(x) = x = max(0 + x, -∞)
#define Tropical_affine_composite_monoid S13, op13, e13


//【トロピカルアフィン変換の逆合成 モノイド】
/*
* S ∋ f = {a, b} : トロピカル一次関数 f(x) = max(a + x, b) を表す．
* f op g : 合成したトロピカル一次関数 g o f を返す．
*/
// verify : https://atcoder.jp/contests/yahoo-procon2017-qual/tasks/yahoo_procon2017_qual_d
using S14 = pair<ll, ll>;
S14 op14(S14 f, S14 g) {
	ll a, b, c, d;
	tie(a, b) = g; // g(x) = max(a + x, b);
	tie(c, d) = f; // f(x) = max(c + x, d);

	// (g o f)(x) = max(a + max(c + x, d), b) = max((a + c) + x, max(a + d, b))
	return { a + c, max(a + d, b) };
}
S14 e14() { return { 0, -INFL }; } // e(x) = x = max(0 + x, -∞)
#define Tropical_affine_invcomposite_monoid S14, op14, e14


//【ビット列上 転倒数 モノイド】
/*
* S ∋ x = {inv, c0, c1} : 列 x の転倒数，0 の個数，1 の個数の組
* x op y : 列 x, y を連結した列
*/
using S17 = tuple<ll, ll, ll>;
S17 op17(S17 x, S17 y) {
	ll x_inv, y_inv, x_c0, x_c1, y_c0, y_c1;
	tie(x_inv, x_c0, x_c1) = x;
	tie(y_inv, y_c0, y_c1) = y;

	// まず x, y それぞれをソートするのに x_inv + y_inv 回の隣接互換が必要．
	// その後 x の右側に寄った x_c1 個の 1 と y の左側に寄った y_c0 個の 0 を
	// 交換するのに x_c1 * y_c0 回の隣接互換が必要．
	ll inv = x_inv + y_inv + x_c1 * y_c0;
	ll c0 = x_c0 + y_c0, c1 = x_c1 + y_c1;

	return { inv, c0, c1 };
}
S17 e17() { return { 0LL, 0, 0 }; }
#define Inversion_monoid S17, op17, e17


//【置換の合成 モノイド】（参照渡ししていないので遅い）
/*
* S ∋ f[0..n) : 置換 i → f[i] を表す．
* f op g : 合成置換 f o g を返す．
*/
// verify : https://atcoder.jp/contests/abc013/tasks/abc013_4
using S18 = vi;
S18 op18(S18 a, S18 b) {
	if (sz(a) == 0) return b;
	if (sz(b) == 0) return a;

	int n = sz(a);
	S18 res(n);
	rep(i, n) res[i] = a[b[i]];

	return res;
}
S18 e18() { return S18(); }
#define Permutation_composite_monoid S18, op18, e18


//【置換の逆合成 モノイド】（参照渡ししていないので遅い）
/*
* S ∋ f[0..n) : 置換 i → f[i] を表す．
* f op g : 合成置換 g o f を返す．
*/
// verify : https://atcoder.jp/contests/abc013/tasks/abc013_4
using S19 = vi;
S19 op19(S19 a, S19 b) {
	if (sz(a) == 0) return b;
	if (sz(b) == 0) return a;

	int n = sz(a);
	S19 res(n);
	rep(i, n) res[i] = b[a[i]];

	return res;
}
S19 e19() { return S19(); }
#define Permutation_invcomposite_monoid S19, op19, e19


