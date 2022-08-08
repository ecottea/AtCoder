#pragma once
#include "header.h"
#include "行列.h"
// ■■■■■ 群 ■■■■■


//【群】
/*
* 群 (S, op, e, inv) を表す．
*
* すなわち，集合 S とその上の二項演算 * : S × S → S で
*	結合律 : ∀a, b, c ∈ S， (a b) c = a (b c)
*	単位元 : ∀a ∈ S,        a e = e a = a
*	逆元   : ∀a ∈ S,        a a^(-1) = a^(-1) a = e
* を満たすものとする．
*/


//【加算 アーベル群】
/* verify : https://atcoder.jp/contests/aising2019/tasks/aising2019_d */
using S601 = ll;
S601 op601(S601 a, S601 b) { return a + b; }
S601 e601() { return 0; }
S601 inv601(S601 a) { return -a; }
#define Add_group S601, op601, e601, inv601


//【乗算 アーベル群】
using S602 = mint;
S602 op602(S602 a, S602 b) { return a * b; }
S602 e602() { return 1; }
S602 inv602(S602 a) { return a.inv(); }
#define Mul_group S602, op602, e602, inv602


//【XOR アーベル群】
using S603 = int;
S603 op603(S603 a, S603 b) { return a ^ b; }
S603 e603() { return 0; }
S603 inv603(S603 a) { return a; }
#define XOR_group S603, op603, e603, inv603


//【可逆アフィン変換の合成 群】
/*
* S ∋ f = {a, b} : 一次関数 f(x) = a x + b を表す．（a != 0）
* f op g : 合成した一次関数 f o g を返す．
*
* 正則行列 (a, b; 0, 1) の全体が積に関して作っている群ともみなせる．
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

	// f(x) = a x + b ⇔ x = (1/a) f(x) - b/a
	return { a.inv(), -b / a };
}
#define AffineComposite_group S604, op604, e604, inv604


//【ビット列上 転倒数 群】
/*
* S ∋ x = {inv, c0, c1} : 列 x の転倒数，0 の個数，1 の個数の組
* x op y : 列 x, y を連結した列
*/
// 参考 : https://qiita.com/hamko/items/92660ac5aed9df4d346d
// verify : https://atcoder.jp/contests/dwacon5th-prelims/tasks/dwacon5th_prelims_c
using S605 = tuple<ll, ll, ll>;
S605 op605(S605 x, S605 y) {
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
S605 e605() { return { 0LL, 0, 0 }; }
S605 inv605(S605 x) {
	ll x_inv, x_c0, x_c1;
	tie(x_inv, x_c0, x_c1) = x;
	
	// ビット列を拡張し，0^(-1), 1^(-1) という要素を認めることにする．
	// これらは 0, 1 それぞれ -1 個として数え，転倒については逆の振る舞いをする．
	return { x_c0 * x_c1 - x_inv, -x_c0, -x_c1 };
}
#define Inversion_group S605, op605, e605, inv605


//【置換の合成 群】（参照渡ししていないので遅い）
/*
* S ∋ f[0..n) : 置換 i → f[i] を表す．
* f op g : 合成置換 f o g を返す．
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
#define PermutationComposite_group S606, op606, e606, inv606


//【置換の逆合成 群】（参照渡ししていないので遅い）
/*
* S ∋ f[0..n) : 置換 i → f[i] を表す．
* f op g : 合成置換 g o f を返す．
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
#define PermutationInvComposite_group S607, op607, e607, inv607


