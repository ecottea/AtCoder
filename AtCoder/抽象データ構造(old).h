#pragma once
#include "header.h"


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
	// verify : https://judge.yosupo.jp/problem/point_set_range_composite

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