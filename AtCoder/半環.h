#pragma once
#include "header.h"
// ■■■■■ 半環 ■■■■■


//【半環】
/*
* 半環 (S, add, o, mul, e) を表す（add は + を，mul は * をオーバーロードする）
*
* すなわち，(S, add, o) が可換モノイド，(S, mul, e) がモノイドで，
*	分配律 : ∀a, b, c ∈ S， a(b + c) = a b + a c, (a + b)c = a c + b c
*	零倍   : ∀a ∈ S,        a o = o a = o
* を満たすものとする．
*/
template <class S, S(*add)(S, S), S(*o_)(), S(*mul)(S, S), S(*e_)()>
struct Semiring {
	// 参考 : https://nyaannyaan.github.io/library/math/semiring.hpp

	S v;

	// 零元，単位元
	static S o() { return o_(); }
	static S e() { return e_(); }

	// コンストラクタ
	Semiring() : v(o()) {}
	Semiring(S a) : v(a) {}

	// 比較
	bool operator==(const Semiring& a) const { return v == a.v; }
	bool operator!=(const Semiring& a) const { return v != a.v; }

	// 和
	Semiring& operator+=(const Semiring& a) {
		if (v == o()) return *this = a;
		if (a.v == o()) return *this;
		return *this = add(v, a.v);
	}
	Semiring operator+(const Semiring& a) const { return Semiring(*this) += a; }

	// 積
	Semiring operator*(const Semiring& a) const {
		if (v == o() || a.v == o()) return o();
		if (v == e()) return a;
		if (a.v == e()) return *this;
		return mul(v, a.v);
	}

	// 入出力
	friend istream& operator>>(istream& is, Semiring& a) { is >> a.v; return is; }
	friend ostream& operator<<(ostream& os, const Semiring& a) {
		if (a.v == o()) return os << "o";
		if (a.v == e()) return os << "e";
		return os << a.v;
	}
};


//【加算 - 乗算 半環】
/*
* 特に半環上の正方行列に自然に和と積を定めれば，これもまた（非可換）半環となる．
*/
using S1 = mint;
S1 add(S1 x, S1 y) { return x + y; }
S1 o1() { return 0; }
S1 mul(S1 x, S1 y) { return x * y; }
S1 e1() { return 1; }
using T = Semiring<S1, add, o1, mul, e1>;


//【xor - and 半環】
using S2 = unsigned int;
S2 add(S2 x, S2 y) { return x ^ y; }
S2 o2() { return 0; }
S2 mul(S2 x, S2 y) { return x & y; }
S2 e2() { return ~0; }
using T = Semiring<S2, add, o2, mul, e2>;


//【min - plus 半環（トロピカル半環）】
using S3 = ll;
S3 add(S3 x, S3 y) { return min(x, y); }
S3 o3() { return INFL; }
S3 mul(S3 x, S3 y) { return x + y; }
S3 e3() { return 0; }
using T = Semiring<S3, add, o3, mul, e3>;


//【max - plus 半環（トロピカル半環）】
using S4 = ll;
S4 add(S4 x, S4 y) { return max(x, y); }
S4 o4() { return -INFL; }
S4 mul(S4 x, S4 y) { return x + y; }
S4 e4() { return 0; }
using T = Semiring<S4, add, o4, mul, e4>;


//【トロピカルアフィン変換の max - 合成 半環】
/*
* S ∋ f = {a, b} : トロピカル一次関数 f(x) = max(a + x, b) を表す．
* f add g : max(f, g)(x) = max(max(a + c) + x, max(b, d)) を表す．
* f mul g : 合成したトロピカル一次関数 f o g を返す．
*/
using S5 = pll; // 斉次行列 (a, b; -∞, 0)
S5 add(S5 f, S5 g) {
	ll a, b, c, d;
	tie(a, b) = f; // 行列 (a, b; -∞, 0)
	tie(c, d) = g; // 行列 (c, d; -∞, 0)

	// (a, b; -∞, 0)+(c, d; -∞, 0) = (max(a, c), max(b, d); -∞, 0)
	return { max(a, c), max(b, d) };
}
S5 o5() { return { -INFL, -INFL }; }
S5 mul(S5 f, S5 g) {
	ll a, b, c, d;
	tie(a, b) = f; // 行列 (a, b; -∞, 0)
	tie(c, d) = g; // 行列 (c, d; -∞, 0)

	// (a, b; -∞, 0).(c, d; -∞, 0) = (a + c, max(a + d, b); -∞, 0)
	return { a + c, max(a + d, b) };
}
S5 e5() { return { 0, -INFL }; }
using T = Semiring<S5, add, o5, mul, e5>;


//【トロピカルアフィン変換の max - 逆合成 半環】
/*
* S ∋ f = {a, b} : トロピカル一次関数 f(x) = max(a + x, b) を表す．
* f add g : max(f, g)(x) = max(max(a + c) + x, max(b, d)) を表す．
* f mul g : 逆合成したトロピカル一次関数 g o f を返す．
* 
* グラフの隣接行列の成分として用いるなら，通る向きが f → g なのでこれを用いる．
*/
using S6 = pll; // 斉次行列 (a, b; -∞, 0)
S6 add(S6 f, S6 g) {
	ll a, b, c, d;
	tie(a, b) = g; // 行列 (a, b; -∞, 0)
	tie(c, d) = f; // 行列 (c, d; -∞, 0)

	// (a, b; -∞, 0)+(c, d; -∞, 0) = (max(a, c), max(b, d); -∞, 0)
	return { max(a, c), max(b, d) };
}
S6 o6() { return { -INFL, -INFL }; }
S6 mul(S6 f, S6 g) {
	ll a, b, c, d;
	tie(a, b) = g; // 行列 (a, b; -∞, 0)
	tie(c, d) = f; // 行列 (c, d; -∞, 0)

	// (a, b; -∞, 0).(c, d; -∞, 0) = (a + c, max(a + d, b); -∞, 0)
	return { a + c, max(a + d, b) };
}
S6 e6() { return { 0, -INFL }; }
using T = Semiring<S6, add, o6, mul, e6>;


