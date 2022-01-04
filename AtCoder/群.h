#pragma once
#include "header.h"
// ■■■■■ 群 ■■■■■


//【群】
/*
* 群 (S, op, e, inv) を表す（op は * をオーバーロードする）
*
* すなわち，集合 S とその上の二項演算 * : S × S → S で
*	結合律 : ∀a, b, c ∈ S， (a b) c = a (b c)
*	単位元 : ∀a ∈ S,        a e = e a = a
*	逆元   : ∀a ∈ S,        a inv(a) = inv(a) a = e
* を満たすものとする．
*/
template <class S, S(*op)(S, S), S(*e_)(), S(*inv_)(S)>
struct Group {
	// verify : https://judge.yosupo.jp/problem/queue_operate_all_composite

	S v;

	// 単位元
	static S e() { return e_(); }

	// コンストラクタ
	Group() : v(e()) {}
	Group(S a) : v(a) {}

	// 比較
	bool operator==(const Group& a) const { return v == a.v; }
	bool operator!=(const Group& a) const { return v != a.v; }

	// 積
	Group operator*(const Group& a) const {
		if (v == e()) return a;
		if (a.v == e()) return *this;
		return op(v, a.v);
	}

	// 逆元
	Group inv() const { return inv_(v); }

	// 入出力
	friend istream& operator>>(istream& is, Group& a) { is >> a.v; return is; }
	friend ostream& operator<<(ostream& os, const Group& a) {
#ifdef _MSC_VER
		if (a.v == e()) return os << "e";
#endif
		return os << a.v;
	}
};


//【加算 群】
using S1 = ll;
S1 op(S1 a, S1 b) { return a + b; }
S1 o1() { return 0; }
S1 inv(S1 a) { return -a; }
using T = Group<S1, op, o1, inv>;


//【乗算 群】
using S2 = mint;
S2 op(S2 a, S2 b) { return a * b; }
S2 o2() { return 1; }
S2 inv(S2 a) { return a.inv(); }
using T = Group<S2, op, o2, inv>;


//【xor 群】
using S3 = int;
S3 op(S3 a, S3 b) { return a ^ b; }
S3 o3() { return 0; }
S3 inv(S3 a) { return a; }
using T = Group<S3, op, o3, inv>;


//【可逆アフィン変換の合成 群】
/*
* S ∋ f = {a, b} : 一次関数 f(x) = a x + b を表す．（a != 0）
* f op g : 合成した一次関数 f o g を返す．
*
* 正則行列 (a, b; 0, 1) の全体が積に関して作っている群ともみなせる．
*/
// verify : https://judge.yosupo.jp/problem/queue_operate_all_composite
using S4 = pair<mint, mint>;
S4 op(S4 f, S4 g) {
	mint a, b, c, d;
	tie(a, b) = f; // f(x) = a x + b;
	tie(c, d) = g; // g(x) = c x + d;

	// (f o g)(x) = a (c x + d) + b = (a c)x + (a d + b)
	return { a * c, a * d + b };
}
S4 e4() { return { 1, 0 }; } // e(x) = x = 1 x + 0
S4 inv(S4 f) {
	mint a, b;
	tie(a, b) = f; // f(x) = a x + b;

	// f(x) = a x + b ⇔ x = (1/a) f(x) - b/a
	return { a.inv(), -b / a };
}
using T = Group<S4, op, e4, inv>;


//【ビット列上 転倒数 群】
/*
* S ∋ x = {inv, c0, c1} : 列 x の転倒数，0 の個数，1 の個数の組
* x op y : 列 x, y を連結した列
*/
// 参考 : https://qiita.com/hamko/items/92660ac5aed9df4d346d
// verify : https://atcoder.jp/contests/dwacon5th-prelims/tasks/dwacon5th_prelims_c
using S5 = tuple<ll, ll, ll>;
S5 op(S5 x, S5 y) {
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
S5 e5() { return { 0LL, 0, 0 }; }
S5 inv(S5 x) {
	ll x_inv, x_c0, x_c1;
	tie(x_inv, x_c0, x_c1) = x;
	
	// ビット列を拡張し，0^(-1), 1^(-1) という要素を認めることにする．
	// これらは 0, 1 それぞれ -1 個として数え，転倒については逆の振る舞いをする．
	return { x_c0 * x_c1 - x_inv, -x_c0, -x_c1 };
}
using T = Group<S5, op, e5, inv>;


