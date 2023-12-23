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
* 方程式 a x^2 + b x + c = 0 の実数解を sol に格納し，その個数を返す．
*/
int quadratic_equation(ll a, ll b, ll c, vd& sol) {
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
			sol.push_back(-(double)c / b);
			return 1;
		}
	}

	// a を正とする．
	if (a < 0) {
		a *= -1;
		b *= -1;
		c *= -1;
	}

	// cnt : 実数解の個数
	int cnt;

	// 4 a c がオーバーフローする可能性があるので丁寧に判定
	if (c < 0) cnt = 2;
	else if (4 * c > b * b / a) cnt = 0;
	else if (b * b > 4 * a * c) cnt = 2;
	else cnt = 1;

	// 実数解が 0 個の場合
	if (cnt == 0) return 0;

	// 実数解が 2 個の場合
	if (cnt == 2) {
		// d : 判別式 b^2 - 4 a c
		double d = (double)b * b - 4. * a * c;

		double x1, x2;

		// 解の公式より
		//		x = (-b + √d) / 2a, (-b - √d) / 2a
		// であるが，異符号の和は精度の低下を招くので，一方は変わりに
		//		x = 2c / (-b - √d), 2c / (-b + √d)
		// を用いる．
		if (b > 0) {
			x1 = (-b - sqrt(d)) / (2. * a);
			x2 = 2. * c / (-b - sqrt(d));
		}
		else {
			x1 = (-b + sqrt(d)) / (2. * a);
			x2 = 2. * c / (-b + sqrt(d));
		}

		sol.push_back(x1);
		sol.push_back(x2);
		return 2;
	}
	// 実数解が 1 個の場合
	else {
		sol.push_back(-b / (2. * a));
		return 1;
	}
}


//【疎多項式】
/*
* SPoly<T>() : O(1)
*	零多項式 f(x) = 0 で初期化する．
*
* SPoly<T>(T c0) : O(1)
*	定数多項式 f(x) = c0 で初期化する．
*
* SPoly<T>(vector<pair<ll, T>> dcs) : O(n)
*	n 個の (次数, 係数) の組で初期化する．
*
* c + f, f + c : O(n), f + g : O(n + m)（n, m は f, g それぞれの項数）
* f - c, c - f : O(n), f - g : O(n + m)
* c * f, f * c, -f : O(n)
*	和，差，定数倍の結果を返す．
*
* f * g : O(n m)
*	積を返す．
*/
template <class T>
struct SPoly {
	// 次数 → 係数
	unordered_map<ll, T> c;

	// 初期化
	SPoly() {}
	SPoly(const T& c0) { c[0] = c0; }
	SPoly(const vector<pair<ll, T>>& dcs) { repe(p, dcs) c[p.first] = p.second; }

	// 代入
	SPoly(const SPoly& f) = default;
	SPoly& operator=(const SPoly& f) = default;
	SPoly& operator=(const T& c0) { c.clear(); c[0] = c0; return *this; }

	// 比較
	bool operator==(const SPoly& g) const { return c == g.c; }
	bool operator!=(const SPoly& g) const { return !(*this == g); }

	// アクセス
	T const& operator[](ll i) const { return c.at(i); }
	T& operator[](ll i) { return c[i]; }

	// 加算
	SPoly& operator+=(const SPoly& g) { repe(p, g.c) c[p.first] += p.second; return *this; }
	SPoly operator+(const SPoly& g) const { return SPoly(*this) += g; }
	SPoly& operator+=(const T& c0) { c[0] += c0; return *this; }
	SPoly operator+(const T& c0) const { return SPoly(*this) += c0; }
	friend SPoly operator+(const T& c0, const SPoly& f) { return f + c0; }

	// 減算
	SPoly& operator-=(const SPoly& g) { repe(p, g.c) c[p.first] -= p.second; return *this; }
	SPoly operator-(const SPoly& g) const { return SPoly(*this) -= g; }
	SPoly& operator-=(const T& c0) { c[0] -= c0; return *this; }
	SPoly operator-(const T& c0) const { return SPoly(*this) -= c0; }
	friend SPoly operator-(const T& c, const SPoly& f) { return -f + c; }

	// 定数倍
	SPoly& operator*=(const T& c0) { repea(p, c) p.second *= c0; return *this; }
	SPoly operator*(const T& c0) const { return SPoly(*this) *= c0; }
	friend SPoly operator*(const T& c0, const SPoly& f) { return f * c0; }
	SPoly operator-() const { return SPoly(*this) *= -1; }

	// 積
	SPoly operator*(const SPoly& g) const {
		SPoly res;
		repe(pf, c) repe(pg, g.c) res.c[pf.first + pg.first] += pf.second * pg.second;
		return res;
	}
	SPoly& operator*=(const SPoly& g) { *this = *this * g; return *this; }

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const SPoly& f) {
		map<ll, T> dcs;
		repe(p, f.c) dcs[p.first] = p.second;
		for (auto it = dcs.begin(); it != dcs.end(); it++) {
			os << it->second << "x^" << it->first << (next(it) != dcs.end() ? " + " : "");
		}
		return os;
	}
#endif
};


//【展開係数】O(n m^2 log d)
/*
* 有理式 f(x) / g(x) を形式的冪級数に展開したときの x^d の係数を返す．
*
* 制約 : g[0] != 0
*/
mint bostan_mori(const SPoly<mint>& f, const SPoly<mint>& g, ll d) {
	//【方法】
	// 分母分子に g(-x) を掛けることにより
	//		f(x) / g(x) = f(x) g(-x) / g(x) g(-x)
	// を得る．ここで g(x) g(-x) は偶多項式なので
	//		g(x) g(-x) = e(x^2)
	// と表すことができる．
	// 
	// 分子について
	//		f(x) g(-x) = E(x^2) + x O(x^2)
	// というように偶多項式部分と奇多項式部分に分けると，d が偶数のときは
	//		[x^d] f(x) g(-x) / g(x) g(-x)
	//		= [x^d] E(x^2) / e(x^2)
	//		= [x^(d/2)] E(x) / e(x)
	// となり，d が奇数のときは
	//		[x^d] f(x) g(-x) / g(x) g(-x)
	//		= [x^d] x O(x^2) / e(x^2)
	//		= [x^((d-1)/2)] O(x) / e(x)
	// となる．
	//
	// これを繰り返せば d を半分ずつに減らしていくことができる．

	// d = 0 のときは定数項を返す．
	if (d == 0) {
		return f[0] / g[0];
	}

	// f2(x) = f(x) g(-x), g2(x) = g(x) g(-x) を求める．
	SPoly<mint> f2, g2 = g;
	repea(p, g2.c) {
		if (p.first % 2 == 1) {
			p.second *= -1;
		}
	}
	f2 = f * g2;
	g2 *= g;

	// f3(x) = E(x) or O(x), g3(x) = e(x) を求める．
	SPoly<mint> f3, g3;
	if (d % 2 == 0) {
		repe(p, f2.c) {
			if (p.first % 2 == 0) {
				f3.c[p.first / 2] = p.second;
			}
		}
	}
	else {
		repe(p, f2.c) {
			if (p.first % 2 == 1) {
				f3.c[(p.first - 1) / 2] = p.second;
			}
		}
	}
	repe(p, g2.c) {
		if (p.first % 2 == 0) {
			g3.c[p.first / 2] = p.second;
		}
	}

	// d を半分にして再帰を回す．
	return bostan_mori(f3, g3, d / 2);
}


