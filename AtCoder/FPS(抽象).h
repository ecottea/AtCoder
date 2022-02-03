#pragma once
#include "header.h"
// ■■■■■ 形式的冪級数（抽象代数上） ■■■■■


//【形式的冪級数（可換環）】
/*
* FPS<S, add, o, mi, mul, e>() : O(1)
*	零多項式 f = o() で初期化する．
*   係数は可換環 <S, add, o, mi, mul, e> の元とする．
*
* FPS<S, add, o, mi, mul, e>(c0) : O(1)
*	定数多項式 f = c0 で初期化する．
*
* FPS<S, add, o, mi, mul, e>(c0, n) : O(d)
*	n 次未満の項をもつ定数多項式 f = c0 で初期化する．
*
* FPS<S, add, o, mi, mul, e>(c) : O(|c|)
*	f(x) = c[0] + c[1] x + ... + c[n - 1] x^(n-1) で初期化する．
*
* c + f, f + c : O(1)	f + g : O(n)
* f - c : O(1)			c - f, f - g, -f : O(n)
* c * f, f * c : O(n)	f * g : O(n^2)		f * g_sp : O(n k)（k : g の項数）
*						f / g : O(n^2)		f / g_sp : O(n k)（k : g の項数）
*	形式的冪級数としての和，差，積，商の結果を返す．
*	g_sp はスパース多項式であり，{次数, 係数} の次数昇順の組の vector で表す．
*	制約 : 商では g(0) = e()
*
* f.inv(d) : O(n^2)
*	1 / f mod x^d を返す．
*	制約 : f(0) = e()
*
* f.quotient(g) : O(n^2)
* f.reminder(g) : O(n^2)
* f.quotient_remainder(g) : O(n^2)
*	多項式としての f を g で割った商，余り，商と余りの組を返す．
*
* f.deg(), f.size() : O(1)
*	多項式 f の次数[+1]を返す．
*
* FPS::monomial(d) : O(d)
*	単項式 x^d を返す．
*
* f.assign(c) : O(n)
*	多項式 f の不定元 x に c を代入した値を返す．
*
* f.resize(d) : O(1)
*	mod x^d をとる．
*
* f.resize() : O(n)
*	不要な高次の項を削る．
*
* f >> d, f << d : O(n)
*	係数列を d だけ右[左]シフトした多項式を返す．
*  （右シフトは x^d の乗算，左シフトは x^d で割った商と等価）
*
* power_mod(f, d, g) : O(m^2 log d)　（m = deg g）
*	f(x)^d % g(x) を返す．
*/
template <class S, S(*add)(S, S), S(*o)(), S(*mi)(S), S(*mul)(S, S), S(*e)()>
struct FPS {
	using SFPS = vector<pair<int, S>>;

	int n; // 係数の個数（次数 + 1）
	vector<S> c; // 係数列

	// コンストラクタ（零元，定数，次数指定付き定数，係数列で初期化）
	FPS() : n(0) {}
	FPS(const S& c0) : n(1), c({ c0 }) {}
	FPS(const S& c0, int n_) : n(n_), c(n) { c[0] = c0; }
	FPS(const vector<S>& c_) : n(sz(c_)), c(c_) {}

	// 代入
	FPS(const FPS& f) = default;
	FPS& operator=(const FPS& f) = default;
	FPS& operator=(const S& c0) { n = 1; c = { c0 }; return *this; }

	// アクセス
	S const& operator[](int i) const { return c[i]; }
	S& operator[](int i) { return c[i]; }

	// 次数
	int deg() const { return n - 1; }
	int size() const { return n; }

	// 加算
	FPS& operator+=(const FPS& g) {
		if (n >= g.n) rep(i, g.n) c[i] = add(c[i], g[i]);
		else {
			rep(i, n) c[i] = add(c[i], g[i]);
			repi(i, n, g.n - 1)	c.push_back(g[i]);
			n = g.n;
		}
		return *this;
	}
	FPS operator+(const FPS& g) const { return FPS(*this) += g; }

	// 定数加算
	FPS& operator+=(const S& sc) {
		if (n == 0) { n = 1; c = { sc }; }
		else { c[0] = add(c[0], sc); }
		return *this;
	}
	FPS operator+(const S& sc) const { return FPS(*this) += sc; }
	friend FPS operator+(const S& sc, const FPS& f) { return f + sc; }

	// 減算
	FPS& operator-=(const FPS& g) {
		if (n >= g.n) rep(i, g.n) c[i] = add(c[i], mi(g[i]));
		else {
			rep(i, n) c[i] = add(c[i], mi(g[i]));
			repi(i, n, g.n - 1) c.push_back(mi(g[i]));
			n = g.n;
		}
		return *this;
	}
	FPS operator-(const FPS& g) const { return FPS(*this) -= g; }

	// 定数減算
	FPS& operator-=(const S& sc) { *this += mi(sc); return *this; }
	FPS operator-(const S& sc) const { return FPS(*this) -= sc; }
	friend FPS operator-(const S& sc, const FPS& f) { return FPS(sc) - f; }

	// マイナス元
	FPS operator-() const {
		FPS res(*this);
		rep(i, n) c[i] = mi(c[i]);
		return res;
	}

	// 定数倍
	FPS& operator*=(const S& sc) { rep(i, n) c[i] = mul(c[i], sc); return *this; }
	FPS operator*(const S& sc) const { return FPS(*this) *= sc; }
	friend FPS operator*(const S& sc, const FPS& f) { return f * sc; }

	// 積
	FPS& operator*=(const FPS& g) {
		int m = g.deg();
		resize(n + m);

		// 後ろからインライン配る DP
		repir(i, n - 1, 0) {
			// 上位項に係数倍して配っていく．
			repi(j, 1, m) {
				if (i + j >= n) break;

				c[i + j] = add(c[i + j], mul(c[i], g[j]));
			}

			// 定数項は最後に配るか消去しないといけない．
			c[i] = mul(c[i], g[0]);
		}

		return *this;
	}
	FPS operator*(const FPS& g) const { return FPS(*this) *= g; }

	// 除算
	FPS inv(int d) const {
		// 参考：https://nyaannyaan.github.io/library/fps/formal-power-series.hpp

		//【方法】
		// 1 / f mod x^d を求めることは，
		//		f g = 1 (mod x^d)
		// なる g を求めることである．
		// この d の部分を 1, 2, 4, ..., 2^i と倍々にして求めていく．
		//
		// d = 1 のときについては
		//		g = 1 / f[0] (mod x^1)
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
		//		⇔ g - 2 h + f h^2 = 0 (mod x^(2 k)) 　(f g = 1 (mod x^d) より)
		//		⇔ g = (2 - f h) h (mod x^(2 k))
		// を得る．
		//
		// この手順を d <= 2^i となる i まで繰り返し，d 次以上の項を削除すればよい．

		assert(c[0] == e());

		FPS g(e());
		for (int k = 1; k < d; k *= 2) {
			g = ((e() + e()) - *this * g) * g;
			g.resize(2 * k);
		}

		return g.resize(d);
	}
	FPS& operator/=(const FPS& g) { return *this *= g.inv(n); }
	FPS operator/(const FPS& g) const { return FPS(*this) /= g; }

	// 余り付き除算
	FPS quotient(const FPS& g) const {
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

		if (n < g.n) return FPS();
		return ((this->rev() / g.rev()).resize(n - g.n + 1)).rev();
	}
	FPS reminder(const FPS& g) const { return (*this - this->quotient(g) * g).resize(g.n - 1); }
	pair<FPS, FPS> quotient_remainder(const FPS& g) const {
		pair<FPS, FPS> res;
		res.first = this->quotient(g);
		res.second = (*this - res.first * g).resize(g.n - 1);
		return res;
	}

	// スパース積
	FPS& operator*=(const SFPS& g) {
		// g の定数項だけ例外処理
		auto it0 = g.begin();
		S g0;
		if (it0->first == 0) {
			g0 = it0->second;
			it0++;
		}

		// 後ろからインライン配る DP
		repir(i, n - 1, 0) {
			// 上位項に係数倍して配っていく．
			for (auto it = it0; it != g.end(); it++) {
				int j; S gj;
				tie(j, gj) = *it;

				if (i + j >= n) break;

				c[i + j] = add(c[i + j], mul(c[i], gj));
			}

			// 定数項は最後に配るか消去しないといけない．
			c[i] = mul(c[i], g0);
		}

		return *this;
	}
	FPS operator*(const SFPS& g) const { return FPS(*this) *= g; }

	// スパース商
	FPS& operator/=(const SFPS& g) {
		// g の定数項だけ例外処理
		auto it0 = g.begin();
		assert(it0->first == 0 && it0->second == e());
		it0++;

		// 前からインライン配る DP（後ろに累積効果あり）
		rep(i, n) {
			// 上位項に係数倍して配っていく．
			for (auto it = it0; it != g.end(); it++) {
				int j; S gj;
				tie(j, gj) = *it;

				if (i + j >= n) break;

				c[i + j] = add(c[i + j], mi(mul(c[i], gj)));
			}
		}

		return *this;
	}
	FPS operator/(const SFPS& g) const { return FPS(*this) /= g; }

	// 係数反転
	FPS rev() const { FPS h = *this; reverse(all(h.c)); return h; }

	// 単項式
	static FPS monomial(int d) {
		FPS mono(o(), d + 1);
		mono[d] = e();
		return mono;
	}

	// 不要な高次項の除去
	FPS& resize() {
		// 最高次の係数が非 0 になるまで削る．
		while (n > 0 && c[n - 1] == o()) {
			c.pop_back();
			n--;
		}
		return *this;
	}

	// 高次項の除去
	FPS& resize(int d) {
		// x^d 以上の項を除去する．
		n = d;
		c.resize(d);
		return *this;
	}

	// 不定元への代入
	S assign(const S& x) const {
		S val;
		repir(i, n - 1, 0) val = add(mul(val, x), c[i]);
		return val;
	}

	// 係数のシフト
	FPS& operator>>=(int d) {
		n += d;
		c.insert(c.begin(), d, o());
		return *this;
	}
	FPS& operator<<=(int d) {
		n -= d;
		if (n <= 0) { c.clear(); n = 0; }
		else c.erase(c.begin(), c.begin() + d);
		return *this;
	}
	FPS operator>>(int d) const { return FPS(*this) >>= d; }
	FPS operator<<(int d) const { return FPS(*this) <<= d; }

	// 累乗の剰余
	friend FPS power_mod(const FPS& f, ll d, const FPS& g) {
		FPS res(e()), pow2(f);
		while (d > 0) {
			if (d & 1LL) res = (res * pow2).reminder(g);
			pow2 = (pow2 * pow2).reminder(g);
			d /= 2;
		}
		return res;
	}

	// デバッグ出力
	friend ostream& operator<<(ostream& os, const FPS& f) {
		if (f.n == 0) os << o();
		else {
			rep(i, f.n) {
				os << f[i] << "x^" << i;
				if (i < f.n - 1) os << " + ";
			}
		}
		return os;
	}
};


//【展開係数／ボスタン－森法】O(n^2 log d)
/*
* 有理式 f(x) / g(x) を形式的冪級数に展開したときの x^d の係数を返す．
*
* 制約 : deg f < deg g, g[0] = 1
*/
template <class S, S(*add)(S, S), S(*o)(), S(*mi)(S), S(*mul)(S, S), S(*e)()>
S coef(const FPS<S, add, o, mi, mul, e>& f, const FPS<S, add, o, mi, mul, e>& g, ll d) {
	// 参考 : http://q.c.titech.ac.jp/docs/progs/polynomial_division.html
	// verify : https://atcoder.jp/contests/abc009/tasks/abc009_4

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

	assert(g[0] == e());

	// d = 0 のときは定数項を返す．
	if (d == 0) return f[0];

	// f2(x) = f(x) g(-x), g2(x) = g(x) g(-x) を求める．
	FPS<S, add, o, mi, mul, e> f2, g2(g);
	rep(i, g2.n) {
		if (i % 2) g2[i] = mi(g2[i]);
	}
	f2 = f * g2;
	g2 *= g;

	// f3(x) = E(x) or O(x), g3(x) = e(x) を求める．
	FPS<S, add, o, mi, mul, e> f3, g3;
	if (d % 2 == 0) {
		for (int i = 0; 2 * i < f2.n; i++) {
			f3.c.push_back(f2[2 * i]);
		}
	}
	else {
		for (int i = 0; 2 * i + 1 < f2.n; i++) {
			f3.c.push_back(f2[2 * i + 1]);
		}
	}
	f3.n = sz(f3.c);
	rep(i, g.n) {
		g3.c.push_back(g2[2 * i]);
	}
	g3.n = sz(g3.c);

	// d を半分にして再帰を回す．
	return coef(f3, g3, d / 2);
}


//【線形漸化式】O(d^2 log n)
/*
* 初項 a[0..d) と漸化式 a[i] = Σj=[0..d) c[j]a[i-1-j] で定義される
* 数列 a について，a[n] の値を返す．
*
* 利用：【展開係数／ボスタン－森法】
*/
template <class S, S(*add)(S, S), S(*o)(), S(*mi)(S), S(*mul)(S, S), S(*e)()>
S linearly_recurrent_sequence(const vector<S>& a, const vector<S>& c, ll n) {
	// verify : https://atcoder.jp/contests/abc009/tasks/abc009_4

	int d = sz(a);

	FPS<S, add, o, mi, mul, e> A(a), C(c);
	FPS<S, add, o, mi, mul, e> Dnm = e() - (C >> 1);
	FPS<S, add, o, mi, mul, e> Num = (Dnm * A).resize(d);

	return coef(Num, Dnm, n);
}


