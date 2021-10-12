#pragma once
#include "header.h"
#include "組合せ論.h"
#include "合同式.h"
// ■■■■■ 多項式（形式的べき級数を含む） ■■■■■



//【多項式，形式的冪級数】
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
* f * g : O((n + m) log(n + m))
*	多項式 f, g の積を返す．*= も使用可．（deg g = m）
*	制約：T = ll, modint998244353
*
* f.pow(d) : O(n d log n log d) ?
*	多項式 f の d 乗を返す．
*	制約：T = ll, modint998244353
*
* f.inv(d) : O((n + d) log(n + d)) ?
*	多項式 f の x^d を法とする乗法逆元を返す．
*	制約：f(0) ≠ 0, T = modint998244353
*
* f / g : O(n log n)
*	多項式 f を g で割った商を返す．
*	制約：T = modint998244353
*
* f % g : O(n log n)
*	多項式 f を g で割った余りを返す．
*	制約：T = modint998244353
*
* d % f : O(n log n log d) ?
*	多項式 x^d を f で割った余りを返す．
*	制約：T = modint998244353
*
* assign(v) : O(n)
*	多項式 f の不定元 x に v を代入した値を返す．
*
* deg() : O(1)
*	多項式 f の次数を返す．
*
* shift(d) : O(n)
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
	Polynomial& operator*=(const Polynomial& g);
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

	// 乗法逆元
	Polynomial inv(int d) const;

	// 商
	Polynomial operator/(const Polynomial& g) const;
	Polynomial& operator/=(const Polynomial& g) { return *this = *this / g; }

	// 余り
	Polynomial operator%(const Polynomial& g) const {
		return (*this - (*this / g) * g).resize(g.n - 1);
	}
	Polynomial& operator%=(const Polynomial& g) { return *this = *this % g; }

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
	Polynomial shift(int d) const {
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

// 積
template <> inline Polynomial<ll>& Polynomial<ll>::operator*=(const Polynomial<ll>& g) {
	c = convolution_ll(c, g.c);
	n = sz(c);
	return *this;
}
template <> inline Polynomial<modint998244353>& Polynomial<modint998244353>::operator*=(const Polynomial<modint998244353>& g) {
	c = convolution(c, g.c);
	n = sz(c);
	return *this;
}

// 乗法逆元
template <> Polynomial<modint998244353> Polynomial<modint998244353>::inv(int d) const {
	// 参考：https://nyaannyaan.github.io/library/fps/formal-power-series.hpp

	//【方法】
	// f^(-1) mod x^d を求めることは，
	//		f g = 1 (mod x^d)
	// なる g を求めることである．
	// この d の部分を 1, 2, 4, ..., 2^i と倍々にして求めていく．
	//
	// d = 1 のときについては
	//		g = f[0]^(-1) (mod x^1)
	// である．
	//
	// 次に，
	//		g = h (mod x^k)
	// が求まっているとして
	//		g mod x^(2 k)
	// を求める．最初の式を変形していくことで
	//		g - h = 0 (mod x^k)
	//		⇒ (g - h)^2 = 0 (mod x^(2 k))
	//		⇔ g^2 - 2 g h + h^2 = 0 (mod x^(2 k))
	//		⇒ f g^2 - 2 f g h + f h^2 = 0 (mod x^(2 k))
	//		⇔ g - 2 h + f h^2 = 0 (mod x^(2 k)) (f g = 1 (mod x^d) より)
	//		⇔ g = (2 - f h) h (mod x^(2 k))
	// を得る．
	//
	// この手順を d <= 2^i となる i まで繰り返し，d 次以上の項を削除すればよい．

	Polynomial<modint998244353> g(c[0].inv());
	for (int k = 1; k < d; k *= 2) {
		g = (modint998244353(2) - *this * g) * g;
		g.resize(2 * k);
	}

	return g.resize(d);
}

// 商
template <> Polynomial<modint998244353> Polynomial<modint998244353>::operator/(const Polynomial<modint998244353>& g) const {
	// 参考 : https://nyaannyaan.github.io/library/fps/formal-power-series.hpp

	//【方法】
	// f(x) = g(x) q(x) + r(x) となる q(x) を求める．
	// f の次数は n - 1, g の次数は m - 1 とする．(n >= m)
	// 従って q の次数は n - m，r の次数は m - 2 となる．
	// 
	// f^R で f の係数列を逆順にした多項式を表す．すなわち
	//		f^R(x) := f(1/x) x^(n-1)
	// である．他の多項式も同様とする．
	//
	// 最初の式で x → 1/x と置き換えると，
	//		f(1/x) = g(1/x) q(1/x) + r(1/x)
	//		⇔ f(1/x) x^(n-1) = g(1/x) q(1/x) x^(n-1) + r(1/x) x^(n-1)
	//		⇔ f(1/x) x^(n-1) = g(1/x) x^(m-1) q(1/x) x^(n-m) + r(1/x) x^(m-2) x^(n-m+1)
	//		⇔ f^R(x) = g^R(x) q^R(x) + r^R(x) x^(n-m+1)
	//		⇒ f^R(x) = g^R(x) q^R(x) (mod x^(n-m+1))
	// 	    ⇒ q^R(x) = f^R(x) / g^R(x)  (mod x^(n-m+1))
	// を得る．
	// 	   
	// これで q を mod x^(n-m+1) で正しく求めることができることになるが，
	// q の次数は n - m であったから，q 自身を正しく求めることができた．

	if (n >= g.n) {
		return ((this->rev() * g.rev().inv(n - g.n + 1)).resize(n - g.n + 1)).rev();
	}
	else {
		return Polynomial<modint998244353>();
	}
}

// x^d % f を返す．
Polynomial<modint998244353> operator%(ll d, const Polynomial<modint998244353>& f) {
	Polynomial<modint998244353> res(1), pow2({ 0, 1 });
	while (d > 0) {
		if (d & 1) {
			res = (res * pow2) % f;
		}
		pow2 = (pow2 * pow2) % f;
		d /= 2;
	}
	return res;
}


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


//【展開係数／ボスタン－森法】O(n log n log d)
/*
* 有理式 f(x) / g(x) を形式的べき級数に展開したときの x^d の係数を返す．
*
* 制約 : deg f < deg g,
*/
modint998244353 coef(const Polynomial<modint998244353>& f, const Polynomial<modint998244353>& g, ll d) {
	// 参考 : http://q.c.titech.ac.jp/docs/progs/polynomial_division.html

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
		return f.c[0] / g.c[0];
	}

	// f2(x) = f(x) g(-x), g2(x) = g(x) g(-x) を求める．
	Polynomial<modint998244353> f2, g2 = g;
	rep(i, g2.n) {
		if (i % 2) {
			g2.c[i] *= -1;
		}
	}
	f2 = f * g2;
	g2 *= g;

	// f3(x) = E(x) or O(x), g3(x) = e(x) を求める．
	Polynomial<modint998244353> f3, g3;
	if (d % 2 == 0) {
		for (int i = 0; 2 * i < f2.n; i++) {
			f3.c.push_back(f2.c[2 * i]);
		}
	}
	else {
		for (int i = 0; 2 * i + 1 < f2.n; i++) {
			f3.c.push_back(f2.c[2 * i + 1]);
		}
	}
	f3.n = sz(f3.c);
	rep(i, g.n) {
		g3.c.push_back(g2.c[2 * i]);
	}
	g3.n = sz(g3.c);

	// d を半分にして再帰を回す．
	return coef(f3, g3, d / 2);
}


//【平方根】O(n log n)
/*
* √f(x) mod x^d を返す．（なければ find = false を格納する）
*
* 利用：【平方剰余／トネリ－シャンクスのアルゴリズム】
*/
Polynomial<modint998244353> sqrt(const Polynomial<modint998244353>& f, int d, bool& find) {
	// 参考 : https://nyaannyaan.github.io/library/fps/fps-sqrt.hpp

	//【方法】
	// √(x^2 f(x)) = x √f(x) なので f(0) が平方剰余として一般性を失わない．
	// f(0) は平方剰余なので，c0^2 = f(0) なる t が存在する．よって mod x^1 では
	//		c0^2 ≡ f(x) mod x^1
	// が成り立つ．
	//
	// mod x^k で
	//		h(x)^2 ≡ f(x) mod x^k
	// が成り立っていると仮定すると，ニュートン法より
	//		g = h - (h^2 - f) / (h^2)'
	//   ⇔ g = (h + f / h) / 2
	// と置くと
	//		g(x)^2 ≡ f(x) mod x^(2 k)
	// が成り立つ．
	//
	// これを繰り返せば所望の g が求まる．

	// 最低次の項を見つける．
	int i0 = 0;
	while (i0 <= f.deg() && f[i0] == 0) i0++;

	// 零多項式なら平方根も零多項式である．
	if (i0 == f.deg() + 1) {
		find = true;
		Polynomial<modint998244353> g;
		g.resize(d);
		return g;
	}

	// 最低次の項が奇数次の項なら平方根は存在しない．
	if (i0 % 2 == 1) {
		find = false;
		return modint998244353(0);
	}

	// 最低次の項の係数が平方剰余でなければ平方根は存在しない．
	int c0 = (f[i0] == 1 ? 1 : sqrt(f[i0])); // 1 のことが多いので高速化
	if (c0 == -1) {
		find = false;
		return modint998244353(0);
	}

	// 定数項が 0 でないようにシフトした多項式を得る．
	Polynomial<modint998244353> fs = f.shift(-i0);

	// ニュートン法で g = √f を見つける．
	Polynomial<modint998244353> gs(c0);
	for (int k = 1; k < d; k *= 2) {
		gs = (gs + fs * gs.inv(2 * k)) / 2;
		gs.resize(2 * k);
	}

	// シフトした分を元に戻す．
	Polynomial<modint998244353> g = gs.shift(i0 / 2);
	g.resize(d);

	find = true;
	return g;
}


//【ラグランジュ補間】O(d)
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


