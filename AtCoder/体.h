#pragma once
#include "header.h"
#include "不偏ゲーム.h"
// ■■■■■ 体 ■■■■■


//【体の定義】
/*
* 体 (S, add, o, mi, mul, e, inv) を表す．
*
* すなわち，(S, add, o, mi) がアーベル群，(S\{o}, mul, e, inv) が群で，
*	分配律 : ∀a, b, c ∈ S， a(b + c) = a b + a c, (a + b)c = a c + b c
* を満たすものとする．
*/


//【ニム和 - ニム積 体】
/* verify : https://yukicoder.me/submissions/1014852 */
Nim_product NP;
using SC01 = ull;
SC01 addC01(SC01 x, SC01 y) { return x ^ y; }
SC01 oC01() { return 0; }
SC01 miC01(SC01 x) { return x; }
SC01 mulC01(SC01 x, SC01 y) { return NP.prod(x, y); }
SC01 eC01() { return 1; }
SC01 invC01(SC01 x) { return NP.inv(x); }
#define NimAdd_NimMul_field SC01, addC01, oC01, miC01, mulC01, eC01, invC01



// ======================================================

//【体】
/*
* 体 (S, add, o, mi, mul, e, inv) の元を表す（add, mi, mul は +, -, *, / をそれぞれオーバーロードする）
*/
template <class S, S(*add)(S, S), S(*o_)(), S(*mi)(S), S(*mul)(S, S), S(*e_)(), S(*inv_)(S)>
struct Field {
	// verify : https://yukicoder.me/submissions/1014852
	
	S v;

	// 零元
	static S o() { return o_(); }

	// 単位元
	static S e() { return e_(); }

	// コンストラクタ
	Field() : v(o()) {}
	Field(S v) : v(v) {}

	// キャスト
	operator S() const { return v; }

	// 比較
	bool operator==(const Field& b) const { return v == b.v; }
	bool operator!=(const Field& b) const { return v != b.v; }

	// 単項演算
	Field operator-() const { return Field(mi(v)); }
	Field inv() const { return Field(inv_(v)); }

	// 二項演算
	Field& operator+=(const Field& b) { v = add(v, b.v); return *this; }
	Field& operator-=(const Field& b) { v = add(v, mi(b.v)); return *this; }
	Field& operator*=(const Field& b) { v = mul(v, b.v); return *this; }
	Field& operator/=(const Field& b) { v = mul(v, inv_(b.v)); return *this; }
	friend Field operator+(Field a, const Field& b) { a += b; return a; }
	friend Field operator-(Field a, const Field& b) { a -= b; return a; }
	friend Field operator*(Field a, const Field& b) { a *= b; return a; }
	friend Field operator/(Field a, const Field& b) { a /= b; return a; }

	// 入出力
	friend istream& operator>>(istream& is, Field& a) { is >> a.v; return is; }
	friend ostream& operator<<(ostream& os, const Field& a) {
		//#ifdef _MSC_VER
		//		if (a.v == o()) return os << "o";
		//		if (a.v == e()) return os << "e";
		//#endif
		return os << a.v;
	}
};


