#pragma once
#include "header.h"
// ■■■■■ 可換環 ■■■■■


//【可換環】
/*
* 可換環 (S, add, o, mins, mul, e) を表す．
* （add は + を，mins は - を，mul は * をオーバーロードする）
*
* すなわち，(S, add, o, mins) がアーベル群，(S, mul, e) が可換モノイドで，
*	分配律 : ∀a, b, c ∈ S， a(b + c) = a b + a c, (a + b)c = a c + b c
* を満たすものとする．
*/
template <class S, S(*add)(S, S), S(*o_)(), S(*mins)(S), S(*mul)(S, S), S(*e_)()>
struct CRing {
	S v;

	// 零元，単位元
	static S o() { return o_(); }
	static S e() { return e_(); }

	// コンストラクタ
	CRing() : v(o()) {}
	CRing(S a) : v(a) {}

	// 比較
	bool operator==(const CRing& a) const { return v == a.v; }
	bool operator!=(const CRing& a) const { return v != a.v; }

	// 和
	CRing& operator+=(const CRing& a) {
		if (v == o()) return *this = a;
		if (a.v == o()) return *this;
		return *this = add(v, a.v);
	}
	CRing operator+(const CRing& a) const { return CRing(*this) += a; }

	// マイナス元
	CRing operator-() const { return mins(v); }

	// 差
	CRing& operator-=(const CRing& a) { return *this += -a; }
	CRing operator-(const CRing& a) const { return CRing(*this) -= a; }

	// 積
	CRing& operator*=(const CRing& a) {
		if (v == o() || a.v == o()) return *this = o();
		if (v == e()) return *this = a;
		if (a.v == e()) return *this;
		return *this = mul(v, a.v);
	}
	CRing operator*(const CRing& a) const { return CRing(*this) *= a; }

	// 入出力
	friend istream& operator>>(istream& is, CRing& a) { is >> a.v; return is; }
	friend ostream& operator<<(ostream& os, const CRing& a) {
		if (a.v == o()) return os << "o";
		if (a.v == e()) return os << "e";
		return os << a.v;
	}
};


//【加算 - 乗算 可換環】
using S1 = ll;
S1 add(S1 x, S1 y) { return x + y; }
S1 o1() { return 0; }
S1 mins(S1 x) { return -x; }
S1 mul(S1 x, S1 y) { return x * y; }
S1 e1() { return 1; }
using T = CRing<S1, add, o1, mins, mul, e1>;


//【xor - and 可換環】
using S2 = unsigned int;
S2 add(S2 x, S2 y) { return x ^ y; }
S2 o2() { return 0; }
S2 mins(S2 x) { return x; }
S2 mul(S2 x, S2 y) { return x & y; }
S2 e2() { return ~0; }
using T = CRing<S2, add, o2, mins, mul, e2>;


