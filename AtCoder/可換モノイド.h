#pragma once
#include "header.h"
// ■■■■■ 可換モノイド ■■■■■


//【可換モノイド】
/*
* 可換モノイド (S, op, o) を表す（op は + をオーバーロードする）
*
* すなわち，集合 S とその上の二項演算 + : S × S → S で
*	結合律 : ∀a, b, c ∈ S， (a + b) + c = a + (b + c)
*	単位元 : ∀a ∈ S,        a + o = o + a = a
*	交換律 : ∀a, b ∈ S,     a + b = b + a
* を満たすものとする．
*/
template <class S, S(*op)(S, S), S(*o_)()>
struct Commutative_monoid {
	S v;

	// 単位元
	static S o() { return o_(); }

	// コンストラクタ
	Commutative_monoid() : v(o()) {}
	Commutative_monoid(S a) : v(a) {}

	// 比較
	bool operator==(const Commutative_monoid& a) const { return v == a.v; }
	bool operator!=(const Commutative_monoid& a) const { return v != a.v; }

	// 和
	Commutative_monoid& operator+=(const Commutative_monoid& a) {
		if (v == o()) return *this = a;
		if (a.v == o()) return *this;
		return *this = op(v, a.v);
	}
	Commutative_monoid operator+(const Commutative_monoid& a) const {
		return Commutative_monoid(*this) += a;
	}

	// 入出力
	friend istream& operator>>(istream& is, Commutative_monoid& a) { is >> a.v; return is; }
	friend ostream& operator<<(ostream& os, const Commutative_monoid& a) {
#ifdef _MSC_VER
		if (a.v == o()) return os << "o";
#endif
		return os << a.v;
	}
};


//【加算 可換モノイド】
using S1 = int;
S1 op(S1 a, S1 b) { return a + b; }
S1 o1() { return 0; }
using T = Commutative_monoid<S1, op, o1>;


//【乗算 可換モノイド】
using S2 = int;
S2 op(S2 a, S2 b) { return a * b; }
S2 o2() { return 1; }
using T = Commutative_monoid<S2, op, o2>;


//【max 可換モノイド】
// verify : https://codeforces.com/contest/689/problem/D
using S3 = int;
S3 op(S3 a, S3 b) { return max(a, b); }
S3 o3() { return -INF; }
using T = Commutative_monoid<S3, op, o3>;


//【min 可換モノイド】
// verify : https://codeforces.com/contest/689/problem/D
using S4 = int;
S4 op(S4 a, S4 b) { return min(a, b); }
S4 o4() { return INF; }
using T = Commutative_monoid<S4, op, o4>;


//【xor 可換モノイド】
/*
* (Z/2Z)^d 上の 加算 モノイドともみなせる．
*/
using S10 = int;
S10 op(S10 a, S10 b) { return a ^ b; }
S10 o10() { return 0; }
using T = Commutative_monoid<S10, op, o10>;


//【or 可換モノイド】
/*
* (Z/2Z)^d 上の max モノイドともみなせる．
*/
using S11 = int;
S11 op(S11 a, S11 b) { return a | b; }
S11 o11() { return 0; }
using T = Commutative_monoid<S11, op, o11>;


//【and 可換モノイド】
/*
* (Z/2Z)^d 上の min モノイドともみなせる．
*/
using S12 = int;
S12 op(S12 a, S12 b) { return a & b; }
S12 o12() { return ~0; }
using T = Commutative_monoid<S12, op, o12>;


