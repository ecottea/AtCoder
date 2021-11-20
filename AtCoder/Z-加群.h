#pragma once
#include "header.h"
// ■■■■■ Z-加群 ■■■■■


//【Z-加群】
/*
* Z-加群 (S, op, o, inv, mul) を表す．
*（op は + を，inv は - を，mul は * をオーバーロードする）
*
* すなわち，(S, op, o, inv) がアーベル群であり，Z の S への左作用 * : Z × S → S が
*	累加 : ∀x ∈ S，∀a ∈ Z,  a x = (x の a 個の和) = mul(a, x)
* を満たすものとする．
*/
template <class S, S(*op)(S, S), S(*o_)(), S(*inv)(S), S(*mul)(int, S)>
struct Z_module {
	S v;

	// 零元
	static S o() { return o_(); }

	// コンストラクタ
	Z_module() : v(o()) {}
	Z_module(S x) : v(x) {}

	// 比較
	bool operator==(const Z_module& x) const { return v == x.v; }
	bool operator!=(const Z_module& x) const { return v != x.v; }

	// 和
	Z_module& operator+=(const Z_module& x) {
		if (v == o()) return *this = x;
		if (x.v == o()) return *this;
		return *this = op(v, x.v);
	}
	Z_module operator+(const Z_module& x) const { return Z_module(*this) += x; }

	// マイナス元
	Z_module operator-() const { return inv(v); }

	// 差
	Z_module& operator-=(const Z_module& x) { return *this += -x; }
	Z_module operator-(const Z_module& x) const { return Z_module(*this) -= x; }

	// Z-作用
	Z_module& operator*=(int a) {
		if (a == 0) return *this = o();
		if (a > 0) return *this = mul(a, v);
		return *this = -mul(-a, v);
	}
	Z_module operator*(int a) const { return Z_module(*this) *= a; }
	friend Z_module operator*(int a, const Z_module& x) { return x * a; }

	// 入出力
	friend istream& operator>>(istream& is, Z_module& x) { is >> x.v; return is; }
	friend ostream& operator<<(ostream& os, const Z_module& x) {
#ifdef _MSC_VER
		if (x.v == o()) return os << "o";
#endif
		return os << x.v;
	}
};


//【加算 Z-加群】
using S1 = ll;
S1 op(S1 x, S1 y) { return x + y; }
S1 o1() { return 0; }
S1 inv(S1 x) { return -x; }
S1 mul(int a, S1 x) { return a * x; }
using T = Z_module<S1, op, o1, inv, mul>;


//【乗算 Z-加群】
using S2 = mint;
S2 op(S2 x, S2 y) { return x * y; }
S2 o2() { return 1; }
S2 inv(S2 x) { return x.inv(); }
S2 mul(int a, S2 x) { return x.pow(a); }
using T = Z_module<S2, op, o2, inv, mul>;


