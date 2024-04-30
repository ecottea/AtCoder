#pragma once
#include "header.h"
// ■■■■■ 多項式 ■■■■■


//【一次多項式】
/*
* Poly1<t>() : O(1)
*	零多項式 f(x) = 0 で初期化する．
*
* Poly1<t>(T b) : O(1)
*	定数多項式 f(x) = b で初期化する．
*
* Poly1<t>(T a, T b) : O(1)
*	f(x) = a x + b で初期化する．
*
* c + f, f + c, f + g : O(1)
* f - c, c - f, f - g : O(1)
* c * f, f * c, -f, f / c : O(1)
*	和，差，定数倍の結果を返す．
*
* T f.assign(T c) : O(1)
*	f(c) を返す．
*
* double f.solve() : O(1)
*	f(x) = 0 の解を返す．
*
* double f.solve(Poly1 g) : O(1)
*	f(x) = g(x) の解を返す．
*/
template <class T>
struct Poly1 {
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
	bool operator==(const T& c) const { return a == 0 && b == c; }
	bool operator!=(const T& c) const { return !(*this == c); }

	// 加算
	Poly1& operator+=(const Poly1& g) { a += g.a; b += g.b; return *this; }
	Poly1 operator+(const Poly1& g) const { return Poly1(*this) += g; }
	Poly1& operator+=(const T& c) { b += c; return *this; }
	Poly1 operator+(const T& c) const { return Poly1(*this) += c; }
	friend Poly1 operator+(const T& c, const Poly1& f) { return f + c; }

	// 減算
	Poly1& operator-=(const Poly1& g) { a -= g.a; b -= g.b; return *this; }
	Poly1 operator-(const Poly1& g) const { return Poly1(*this) -= g; }
	Poly1& operator-=(const T& c) { b -= c; return *this; }
	Poly1 operator-(const T& c) const { return Poly1(*this) -= c; }
	friend Poly1 operator-(const T& c, const Poly1& f) { return -f + c; }

	// 定数倍
	Poly1& operator*=(const T& c) { a *= c; b *= c; return *this; }
	Poly1 operator*(const T& c) const { return Poly1(*this) *= c; }
	friend Poly1 operator*(const T& c, const Poly1& f) { return f * c; }
	Poly1& operator/=(const T& c) { a /= c; b /= c; return *this; }
	Poly1 operator/(const T& c) const { return Poly1(*this) /= c; }
	Poly1 operator-() const { return Poly1(*this) *= -1; }

	// 不定元への代入
	T assign(const T& x) const { return a * x + b; }

	// 一次方程式を解く
	double solve() const { return -(double)b / a; }
	double solve(const Poly1& g) const { return (*this - g).solve(); }

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Poly1& f) {
		os << f.a << " x + " << f.b; return os;
	}
#endif
};


//【二次方程式】O(1)
/*
* 方程式 a x^2 + b x + c = 0 の実数解を昇順に sol に格納し，その個数を返す．
* ただし解が無限個の場合は sol は空，戻り値は INF とする．
*
* 制約 : b^2-4ac の計算でオーバーフローが起こらない
*/
template <class T = ll, class D = double>
int quadratic_equation(T a, T b, T c, vector<D>& sol) {
	// verify : https://atcoder.jp/contests/tricky/tasks/tricky_2

	sol.clear();

	// 非 0 と決定した係数は小文字，そうでない係数は大文字で表すものとする．

	// B x + C = 0 の場合
	if (a == 0) {
		// C = 0 の場合
		if (b == 0) {
			// 0 = 0 の場合
			if (c == 0) return INF;
			// c = 0 の場合
			else return 0;
		}
		// 一次方程式 b x + C = 0 の場合
		else {
			sol.push_back(D(-c) / b);
			return 1;
		}
	}

	// a を正とする．
	if (a < 0) {
		a *= -1;
		b *= -1;
		c *= -1;
	}

	// d : 判別式（桁落ち防止のためにここまでは整数で計算する）
	T d = b * b - 4 * a * c;

	// 実数解が 0 個の場合
	if (d < 0) return 0;

	// 実数解が 1 個の場合
	if (d == 0) {
		sol.push_back(-b / (D(2) * a));
		return 1;
	}

	// 実数解が 2 個の場合
	D x1, x2;

	// 解の公式より
	//		x = (-b + √d) / 2a, (-b - √d) / 2a
	// であるが，異符号の和は桁落ちを招くので，一方は代わりに
	//		x = 2c / (-b - √d), 2c / (-b + √d)
	// を用いる．
	D sqrt_d = sqrt(D(d));
	if (b > 0) {
		x1 = (-b - sqrt_d) / (D(2) * a);
		x2 = D(2) * c / (-b - sqrt_d);
	}
	else {
		x1 = (-b + sqrt_d) / (D(2) * a);
		x2 = D(2) * c / (-b + sqrt_d);
	}

	if (x1 > x2) swap(x1, x2);
	sol.push_back(x1);
	sol.push_back(x2);
	return 2;
}


