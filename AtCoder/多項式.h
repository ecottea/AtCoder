#pragma once
#include "header.h"
#include "組合せ論.h"
#include "合同式.h"
// ■■■■■ 多項式 ■■■■■



//【多項式】
/*
* 多項式を表す構造体
*
* Polynomial() : O(1)
*	零多項式 f = 0 で初期化する．
*
* Polynomial(c0) : O(1)
*	定数多項式 f = c0 で初期化する．
*
* Polynomial(c) : O(|c|)
*	f(x) = c[0] + c[1] x + ... + c[n - 1] x^(n-1) で初期化する．
*
* c + f ／ f + c : O(1)
*	多項式 f と定数 c の和を返す．+= も使用可．
*
* f - c : O(1)
*	多項式 f と定数 c の差を返す．-= も使用可．
*
* c - f : O(n)
*	定数 c と多項式 f の差を返す．
*
* f + g : O(n)
*	多項式 f, g の和を返す．+= も使用可．
*
* -f : O(n)
*	多項式 f の加法逆元を返す．
*
* f - g : O(n)
*	多項式 f, g の差を返す．-= も使用可．
*
* c * f ／ f * c : O(n)
*	多項式 f と定数 c の積を返す．*= も使用可．
*
* f / c : O(n)
*	多項式 f を定数 c で割った多項式を返す．/= も使用可．
*
* assign(v) : O(n)
*	多項式 f の不定元 x に v を代入した値を返す．
*
* deg() : O(1)
*	多項式 f の次数を返す．
*
* shift_coef(d) : O(n)
*	係数列を d だけシフトした多項式を返す．
*	（d >= 0 なら x^d の乗算，d < 0 なら x^(-d) で割った商と等価）
*/
template <class T> struct Polynomial {
	int n; // 多項式の係数の個数（次数 + 1）
	vector<T> c; // 多項式の係数列

	// コンストラクタ（零多項式，定数多項式，係数列で初期化）
	Polynomial() : n(0) {}
	Polynomial(const T& c0) : n(1), c({ c0 }) {}
	Polynomial(const vector<T>& c_) : n(sz(c_)), c(c_) {}

	// 代入
	Polynomial(const Polynomial& f) = default;
	Polynomial& operator=(const Polynomial& f) = default;
	Polynomial& operator=(const T& c0) { n = 1; c = { c0 }; return *this; }

	// アクセス
	T const& operator[](int i) const { return c[i]; }
	T& operator[](int i) { return c[i]; }

	// 加算
	Polynomial& operator+=(const Polynomial& g) {
		if (n >= g.n) {
			rep(i, g.n) c[i] += g.c[i];
		}
		else {
			rep(i, n) c[i] += g.c[i];
			repi(i, n, g.n - 1)	c.push_back(g.c[i]);
			n = g.n;
		}
		return *this;
	}
	Polynomial operator+(const Polynomial& g) const { Polynomial h = *this; h += g; return h; }

	// 定数加算
	Polynomial& operator+=(const T& sc) {
		if (n == 0) { n = 1; c = { sc }; }
		else { c[0] += sc; }
		return *this;
	}
	Polynomial operator+(const T& sc) const { Polynomial h = *this; h += sc; return h; }
	friend Polynomial operator+(const T& sc, const Polynomial& f) { return f + sc; }

	// 加法逆元
	Polynomial operator-() { Polynomial h = *this; h *= -1; return h; }

	// 減算
	Polynomial& operator-=(const Polynomial& g) {
		if (n >= g.n) {
			rep(i, g.n) c[i] -= g.c[i];
		}
		else {
			rep(i, n) c[i] -= g.c[i];
			repi(i, n, g.n - 1) c.push_back(-g.c[i]);
			n = g.n;
		}
		return *this;
	}
	Polynomial operator-(const Polynomial& g) const { Polynomial h = *this; h -= g; return h; }

	// 定数減算
	Polynomial& operator-=(const T& sc) {
		if (n == 0) { n = 1; c = { sc }; }
		else { c[0] -= sc; }
		return *this;
	}
	Polynomial operator-(const T& sc) const { Polynomial h = *this; h -= sc; return h; }
	friend Polynomial operator-(const T& sc, const Polynomial& f) { return -(f - sc); }

	// 定数倍
	Polynomial& operator*=(const T& sc) {
		rep(i, n) c[i] *= sc;
		return *this;
	}
	Polynomial operator*(const T& sc) const { Polynomial h = *this; h *= sc; return h; }
	friend Polynomial operator*(const T& sc, const Polynomial& f) { return f * sc; }

	// 右からの定数除算
	Polynomial& operator/=(const T& sc) {
		rep(i, n) c[i] /= sc;
		return *this;
	}
	Polynomial operator/(const T& sc) const { Polynomial h = *this; h /= sc; return h; }

	// 積
	Polynomial& operator*=(const Polynomial& g) {
		// TODO
	}
	Polynomial operator*(const Polynomial& g) const { Polynomial h = *this; h *= g; return h; }

	// 累乗
	Polynomial pow(ll d) const {
		Polynomial res(1), pow2 = *this;
		while (d > 0) {
			if ((d & 1) != 0) res *= pow2;
			pow2 *= pow2;
			d /= 2;
		}
		return res;
	}

	// 係数反転
	Polynomial rev() const {
		Polynomial h = *this;
		reverse(all(h.c));
		return h;
	}

	// 不要な高次項の除去
	Polynomial& resize() {
		// 最高次の係数が非 0 になるまで削る．
		while (n > 0 && c[n - 1] == 0) {
			c.pop_back();
			n--;
		}
		return *this;
	}

	// 高次項の除去
	Polynomial& resize(int d) {
		// x^d 以上の項を除去する．
		n = d;
		c.resize(d);
		return *this;
	}

	// 不定元への代入
	T assign(const T& x) const {
		T val = 0;
		repir(i, n - 1, 0) {
			val = val * x + c[i];
		}
		return val;
	}

	// 次数
	int deg() const { return n - 1; }

	// 係数のシフト
	Polynomial shift_coef(int d) const {
		Polynomial f = *this;
		if (d > 0) {
			f.n += d;
			vector<T> zeros(d);
			f.c.insert(f.c.begin(), zeros.begin(), zeros.end());
		}
		else if (d < 0) {
			f.n -= d;
			if (f.n <= 0) {
				f.c.clear();
				f.n = 0;
			}
			else {
				f.c.erase(f.c.begin(), f.c.begin() - d);
			}
		}
		return f;
	}

	// デバッグ出力
	friend ostream& operator<<(ostream& os, const Polynomial& f) {
		if (f.n == 0)
			os << 0;
		else {
			rep(i, f.n) {
				os << f.c[i] << "x^" << i;
				if (i < f.n - 1) os << " + ";
			}
		}
		return os;
	}
};


//【単項式】O(d)
/*
* 単項式 f = x^d を返す．
*/
template <class T> Polynomial<T> monomial(int d) {
	vector<T> coef(d + 1);
	coef[d] = 1;
	Polynomial<T> mono(coef);
	return mono;
}


//【ラグランジュ補間】O(d + log mod)
/*
* ラグランジュ補間を用いて d 次多項式 f の 0, ..., d における値から f(x) を求める．
* 
* fval[i] : 多項式 f の点 i における値 f(i)
* x : 多項式の値を求めたい点
* 
* 戻り値 : 多項式 f の点 x における値 f(x)
* 
* 利用：【階乗と二項係数（mint利用）】
*/
mint lagrange_interpolation(const vm& fval, mint x) {
	int d = sz(fval) - 1; // 多項式 f の次数
	factorial_mint fm(d);

	// (x - i) の左からの累積積
	// mul_l[i] = (x - 0)(x - 1) ... (x - (i - 1))
	vector<mint> mul_l(d + 1);
	mul_l[0] = 1;
	repi(i, 1, d) {
		mul_l[i] = mul_l[i - 1] * (x - (i - 1));
	}

	// (x - i) の右からの累積積
	// mul_r[i] = (x - (i + 1)) ... (x - (d - 1))(x - d)
	vector<mint> mul_r(d + 1);
	mul_r[d] = 1;
	repir(i, d - 1, 0) {
		mul_r[i] = (x - (i + 1)) * mul_r[i + 1];
	}

	// ラグランジュ基底の線形結合を計算する．
	mint res = 0;
	repi(i, 0, d) {
		mint sign = ((d - i) % 2 ? -1 : 1);
		res += fval[i] * mul_l[i] * mul_r[i]
			* fm.factorial_inv(i) * sign * fm.factorial_inv(d - i);
	}
	return res;
}


