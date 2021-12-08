#pragma once
#include "header.h"
// ■■■■■ 全順序集合 ■■■■■


//【全順序集合】
/*
* 全順序集合 (S, comp, ninf, inf) を表す（comp は < をオーバーロードする）
*
* すなわち，集合 S とその上の二項関係 < : S × S → bool で
*	推移律 : ∀a, b, c ∈ S， a < b かつ b < c ⇒ a < c
*	三分律 : ∀a, b ∈ S,     a < b, a = b, a > b のちょうど 1 つが成立する
*	最小元 : ∀a ∈ S,        ninf <= a
*	最大元 : ∀a ∈ S,        a <= inf
* を満たすものとする．
*/
template <class S, bool(*comp)(S, S), S(*ninf_)(), S(*inf_)()>
struct Wellordered_set {
	S v;

	// 最小元，最大元
	static S ninf() { return ninf_(); }
	static S inf() { return inf_(); }

	// コンストラクタ
	Wellordered_set() : v(inf()) {} // 使わない
	Wellordered_set(S a) : v(a) {}

	// 比較
	bool operator<(const Wellordered_set& a) const {
		if (a.v == ninf() || v == inf()) return false;
		if (v == ninf() || a.v == inf()) return true;
		return comp(v, a.v);
	}
	bool operator>(const Wellordered_set& a) const { return a < *this; }
	bool operator>=(const Wellordered_set& a) const { return !(*this < a); }
	bool operator<=(const Wellordered_set& a) const { return !(*this > a); }
	bool operator!=(const Wellordered_set& a) const { return (*this < a) || (*this > a); }
	bool operator==(const Wellordered_set& a) const { return !(*this != a); }

	// 入出力
	friend istream& operator>>(istream& is, Wellordered_set& a) { is >> a.v; return is; }
	friend ostream& operator<<(ostream& os, const Wellordered_set& a) {
#ifdef _MSC_VER
		if (a.v == ninf()) return os << "ninf";
		if (a.v == inf()) return os << "inf";
#endif
		return os << a.v;
	}
};


//【昇順 全順序集合】
using S1 = int;
bool comp(S1 a, S1 b) { return a < b; }
S1 ninf1() { return -INF; }
S1 inf1() { return INF; }
using T = Wellordered_set<S1, comp, ninf1, inf1>;


//【降順 全順序集合】
using S2 = int;
bool comp(S2 a, S2 b) { return a > b; }
S2 ninf2() { return INF; }
S2 inf2() { return -INF; }
using T = Wellordered_set<S2, comp, ninf2, inf2>;


