#pragma once
#include "header.h"
// ■■■■■ 多項式 ■■■■■


//【一次多項式】
/*
* Poly1() : O(1)
*	零多項式 f(x) = 0 で初期化する．
*
* Poly1(T b) : O(1)
*	定数多項式 f(x) = b で初期化する．
*
* Poly1(T a, T b) : O(1)
*	f(x) = a x + b で初期化する．
*
* c + f, f + c, f + g : O(1)
* f - c, c - f, f - g : O(1)
* c * f, f * c, -f : O(1)
*	和，差，定数倍の結果を返す．
*
* T f.assign(T c) : O(1)
*	f(c) を返す．
*
* double f.solve() : O(1)
*	f(x) = 0 の解を返す．
*
* double f.solve(Poly g) : O(1)
*	f(x) = g(x) の解を返す．
*/
template <class T> struct Poly1 {
	// f(x) = a x + b の係数
	T a, b;

	// コンストラクタ（0，定数，係数列で初期化）
	Poly1() : a(0), b(0) {}
	Poly1(const T& b_) : a(0), b(b_) {}
	Poly1(const T& a_, const T& b_) : a(a_), b(b_) {}

	// 代入
	Poly1(const Poly1& f) = default;
	Poly1& operator=(const Poly1& f) = default;
	Poly1& operator=(const T& b_) { a = 0; b = b_; return *this; }

	// 比較
	bool operator==(const Poly1& g) const { return a == g.a && b == g.b; }
	bool operator!=(const Poly1& g) const { return !(*this == g); }

	// 加算
	Poly1& operator+=(const Poly1& g) { a += g.a; b += g.b;	return *this; }
	Poly1 operator+(const Poly1& g) const { return Poly1(*this) += g; }
	Poly1& operator+=(const T& c) { b += c; return *this; }
	Poly1 operator+(const T& c) const { return Poly1(*this) += c; }
	friend Poly1 operator+(const T& c, const Poly1& f) { return f + c; }

	// 減算
	Poly1& operator-=(const Poly1& g) { a -= g.a; b -= g.b;	return *this; }
	Poly1 operator-(const Poly1& g) const { return Poly1(*this) -= g; }
	Poly1& operator-=(const T& c) { b -= c; return *this; }
	Poly1 operator-(const T& c) const { return Poly1(*this) -= c; }
	friend Poly1 operator-(const T& c, const Poly1& f) { return -f + c; }

	// 定数倍
	Poly1& operator*=(const T& c) { a *= c; b *= c; return *this; }
	Poly1 operator*(const T& c) const { return Poly1(*this) *= c; }
	friend Poly1 operator*(const T& c, const Poly1& f) { return f * c; }
	Poly1 operator-() const { return Poly1(*this) *= -1; }

	// 不定元への代入
	T assign(const T& x) const { return a * x + b; }

	// 一次方程式を解く
	double solve() const { return -(double)b / a; }
	double solve(const Poly1& g) const { return (*this - g).solve(); }

	// デバッグ出力
	friend ostream& operator<<(ostream& os, const Poly1& f) {
		os << f.a << " x + " << f.b; return os;
	}
};


