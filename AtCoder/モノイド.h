#pragma once
#include "header.h"
// ■■■■■ モノイド ■■■■■


//【モノイド】
/*
* モノイド (S, op, e) を表す（op は * をオーバーロードする）
* 
* すなわち，集合 S とその上の二項演算 * : S × S → S で
*	結合律 : ∀a, b, c ∈ S， (a b) c = a (b c)
*	単位元 : ∀a ∈ S,        a e = e a = a
* を満たすものとする．
*/
template <class S, S(*op)(S, S), S(*e_)()>
struct Monoid {
	S v;

	// 単位元
	static S e() { return e_(); }

	// コンストラクタ
	Monoid() : v(e()) {}
	Monoid(S a) : v(a) {}

	// 比較
	bool operator==(const Monoid& a) const { return v == a.v; }
	bool operator!=(const Monoid& a) const { return v != a.v; }

	// 積
	Monoid operator*(const Monoid& a) const {
		if (v == e()) return a;
		if (a.v == e()) return *this;
		return op(v, a.v);
	}

	// 入出力
	friend istream& operator>>(istream& is, Monoid& a) { is >> a.v; return is; }
	friend ostream& operator<<(ostream& os, const Monoid& a) {
#ifdef _MSC_VER
		if (a.v == e()) return os << "e";
#endif
		return os << a.v;
	}
};


//【加算 モノイド】
using S1 = int;
S1 op(S1 a, S1 b) { return a + b; }
S1 e1() { return 0; }
using T = Monoid<S1, op, e1>;


//【乗算 モノイド】
using S2 = int;
S2 op(S2 a, S2 b) { return a * b; }
S2 e2() { return 1; }
using T = Monoid<S2, op, e2>;


//【max モノイド】
using S3 = int;
S3 op(S3 a, S3 b) { return max(a, b); }
S3 e3() { return -INF; }
using T = Monoid<S3, op, e3>;


//【min モノイド】
using S4 = int;
S4 op(S4 a, S4 b) { return min(a, b); }
S4 e4() { return INF; }
using T = Monoid<S4, op, e4>;


//【左変更 モノイド】
using S5 = int;
S5 op(S5 a, S5 b) { return a; }
S5 e5() { return INF; } // 使わない値なら何でも OK
using T = Monoid<S5, op, e5>;


//【右変更 モノイド】
using S6 = int;
S6 op(S6 a, S6 b) { return b; }
S6 e6() { return INF; } // 使わない値なら何でも OK
using T = Monoid<S6, op, e6>;


//【文字列連結 モノイド】
using S7 = string;
S7 op(S7 a, S7 b) { return a + b; }
S7 e7() { return ""; }
using T = Monoid<S7, op, e7>;


//【アフィン変換の合成 モノイド】
/*
* S ∋ f = {a, b} : 一次関数 f(x) = a x + b を表す．
* f op g : 合成した一次関数 f o g を返す．
* 
* 行列 (a, b; 0, 1) の全体が積に関して作っているモノイドともみなせる．
*/
using S8 = pair<mint, mint>;
S8 op(S8 f, S8 g) {
	mint a, b, c, d;
	tie(a, b) = f; // f(x) = a x + b;
	tie(c, d) = g; // g(x) = c x + d;

	// (f o g)(x) = a (c x + d) + b = (a c)x + (a d + b)
	return { a * c, a * d + b };
}
S8 e8() { return { 1, 0 }; } // e(x) = x = 1 x + 0
using T = Monoid<S8, op, e8>;


//【アフィン変換の逆合成 モノイド】
/*
* S ∋ f = {a, b} : 一次関数 f(x) = a x + b を表す．
* f op g : 逆向きに合成した一次関数 g o f を返す．
*/
using S9 = pair<mint, mint>;
S9 op(S9 f, S9 g) {
	mint a, b, c, d;
	tie(a, b) = g; // g(x) = a x + b;
	tie(c, d) = f; // f(x) = c x + d;

	// (g o f)(x) = a (c x + d) + b = (a c)x + (a d + b)
	return { a * c, a * d + b };
}
S9 e9() { return { 1, 0 }; } // e(x) = x = 1 x + 0
using T = Monoid<S9, op, e9>;


//【xor モノイド】
/*
* (Z/2Z)^d 上の 加算 モノイドともみなせる．
*/
using S10 = int;
S10 op(S10 a, S10 b) { return a ^ b; }
S10 e10() { return 0; }
using T = Monoid<S10, op, e10>;


//【or モノイド】
/*
* (Z/2Z)^d 上の max モノイドともみなせる．
*/
using S11 = int;
S11 op(S11 a, S11 b) { return a | b; }
S11 e11() { return 0; }
using T = Monoid<S11, op, e11>;


//【and モノイド】
/*
* (Z/2Z)^d 上の min モノイドともみなせる．
*/
using S12 = int;
S12 op(S12 a, S12 b) { return a & b; }
S12 e12() { return ~0; }
using T = Monoid<S12, op, e12>;


//【トロピカルアフィン変換の合成 モノイド】
/*
* S ∋ f = {a, b} : トロピカル一次関数 f(x) = max(a + x, b) を表す．
* f op g : 合成したトロピカル一次関数 f o g を返す．
*
* トロピカル半環上の行列 (a, b; -∞, 0) の全体が積に関して作っているモノイドともみなせる．
*/
using S13 = pair<ll, ll>;
S13 op(S13 f, S13 g) {
	ll a, b, c, d;
	tie(a, b) = f; // f(x) = max(a + x, b);
	tie(c, d) = g; // g(x) = max(c + x, d);

	// (f o g)(x) = max(a + max(c + x, d), b) = max((a + c) + x, max(a + d, b))
	return { a + c, max(a + d, b) };
}
S13 e13() { return { 0, -INFL }; } // e(x) = x = max(0 + x, -∞)
using T = Monoid<S13, op, e13>;


