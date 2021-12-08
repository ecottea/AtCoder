#pragma once
#include "header.h"
// ■■■■■ アーベル群 ■■■■■


//【アーベル群】
/*
* アーベル群 (S, op, o, inv) を表す（op は + を，inv は - をオーバーロードする）
*
* すなわち，集合 S とその上の二項演算 + : S × S → S で
*	結合律 : ∀a, b, c ∈ S,  (a + b) + c = a + (b + c)
*	零元   : ∀a ∈ S,        a + o = o + a = a
*   逆元   : ∀a ∈ S,        a + inv(a) = inv(a) + a = o
*	交換律 : ∀a, b ∈ S,     a + b = b + a
* を満たすものとする．
*/
template <class S, S(*op)(S, S), S(*o_)(), S(*inv)(S)>
struct Abel_group {
	S v;

	// 零元
	static S o() { return o_(); }

	// コンストラクタ
	Abel_group() : v(o()) {}
	Abel_group(S a) : v(a) {}

	// 比較
	bool operator==(const Abel_group& a) const { return v == a.v; }
	bool operator!=(const Abel_group& a) const { return v != a.v; }

	// 和
	Abel_group& operator+=(const Abel_group& a) {
		if (v == o()) return *this = a;
		if (a.v == o()) return *this;
		return *this = op(v, a.v);
	}
	Abel_group operator+(const Abel_group& a) const { return Abel_group(*this) += a; }

	// マイナス元
	Abel_group operator-() const { return inv(v); }

	// 差
	Abel_group& operator-=(const Abel_group& a) { return *this += -a; }
	Abel_group operator-(const Abel_group& a) const { return Abel_group(*this) -= a; }

	// 入出力
	friend istream& operator>>(istream& is, Abel_group& a) { is >> a.v; return is; }
	friend ostream& operator<<(ostream& os, const Abel_group& a) {
#ifdef _MSC_VER
		if (a.v == o()) return os << "o";
#endif
		return os << a.v;
	}
};


//【加算 アーベル群】
// verify : https://atcoder.jp/contests/abc005/tasks/abc005_4
using S1 = ll;
S1 op(S1 a, S1 b) { return a + b; }
S1 o1() { return 0; }
S1 inv(S1 a) { return -a; }
using T = Abel_group<S1, op, o1, inv>;


//【乗算 アーベル群】
using S2 = mint;
S2 op(S2 a, S2 b) { return a * b; }
S2 o2() { return 1; }
S2 inv(S2 a) { return a.inv(); }
using T = Abel_group<S2, op, o2, inv>;


//【xor アーベル群】
using S3 = int;
S3 op(S3 a, S3 b) { return a ^ b; }
S3 o3() { return 0; }
S3 inv(S3 a) { return a; }
using T = Abel_group<S3, op, o3, inv>;


