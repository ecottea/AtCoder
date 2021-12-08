#pragma once
#include "header.h"
// ■■■■■ 整列集合 ■■■■■


//【整列集合】
/*
* 整列集合 (S, comp, inf) を表す（comp は < をオーバーロードする）
*
* すなわち，集合 S とその上の二項関係 < : S × S → bool で
*	推移律 : ∀a, b, c ∈ S， a < b かつ b < c ⇒ a < c
*	三分律 : ∀a, b ∈ S,     a < b, a = b, a > b のちょうど 1 つが成立する
*	最小元の存在 : ∀A ⊂ S, ∃a ∈ A, ∀x ∈ A,  a <= x
* を満たすものとし，仮想的な最大元を inf で表す．
*/
template <class S, bool(*comp)(S, S), S(*inf_)()>
struct Wellordered_set {
	S v;

	// 最大元
	static S inf() { return inf_(); }

	// コンストラクタ
	Wellordered_set() : v(inf()) {}
	Wellordered_set(S a) : v(a) {}

	// 比較
	bool operator<(const Wellordered_set& a) const {
		if (v == inf()) return false;
		if (a.v == inf()) return true;
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
		if (a.v == inf()) return os << "inf";
#endif
		return os << a.v;
	}
};


//【昇順 整列集合】
using S1 = int;
bool comp(S1 a, S1 b) { return a < b; }
S1 inf1() { return INF; }
using T = Wellordered_set<S1, comp, inf1>;


//【降順 整列集合】
// verify : https://atcoder.jp/contests/abc228/tasks/abc228_f
using S2 = int;
bool comp(S2 a, S2 b) { return a > b; }
S2 inf2() { return -INF; }
using T = Wellordered_set<S2, comp, inf2>;


