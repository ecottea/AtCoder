#pragma once
#include "header.h"
// ■■■■■ 形式的冪級数（抽象代数上） ■■■■■


//【形式的冪級数（可換環）】
/*
* FPS<S, add, o, mi, mul, e>() : O(1)
*	零多項式 f = o() で初期化する．
*   係数は可換環 <S, add, o, mi, mul, e> の元とする．
*
* FPS<S, add, o, mi, mul, e>(S c0) : O(1)
*	定数多項式 f = c0 で初期化する．
*
* FPS<S, add, o, mi, mul, e>(S c0, int n) : O(n)
*	n 次未満の項をもつ定数多項式 f = c0 で初期化する．
*
* FPS<S, add, o, mi, mul, e>(vS c) : O(n)
*	f(x) = c[0] + c[1] x + ... + c[n-1] x^(n-1) で初期化する．
*
* c + f, f + c : O(1)	f + g : O(n)
* f - c : O(1)			c - f, f - g, -f : O(n)
* c * f, f * c : O(n)	f * g : O(n^1.6)		f * g_sp : O(n k)（k : g の項数）
*						f / g : O(n^1.6)		f / g_sp : O(n k)（k : g の項数）
*	形式的冪級数としての和，差，積，商の結果を返す．
*	g_sp はスパース多項式であり，{次数, 係数} の次数昇順の組の vector で表す．
*	制約 : 商では g(0) = e()
*
* FPS f.inv(int d) : O(n^1.6)
*	1 / f mod x^d を返す．
*	制約 : f(0) = e()
*
* FPS f.quotient(FPS g) : O(n^1.6)
* FPS f.reminder(FPS g) : O(n^1.6)
* pair<FPS, FPS> f.quotient_remainder(FPS g) : O(n^1.6)
*	多項式としての f を g で割った商，余り，商と余りの組を返す．
*	制約 : g の最高次の係数は e()
*
* int f.deg(), int f.size() : O(1)
*	多項式 f の次数[+1]を返す．
*
* FPS::monomial(int d) : O(d)
*	単項式 x^d を返す．
*
* S f.assign(S c) : O(n)
*	多項式 f の不定元 x に c を代入した値を返す．
*
* f.resize(int d) : O(d)
*	mod x^d をとる．
*
* f.resize() : O(n)
*	不要な高次の項を削る．
*
* f >> d, f << d : O(n)
*	係数列を d だけ右[左]シフトした多項式を返す．
*  （右シフトは x^d の乗算，左シフトは x^d で割った商と等価）
*
* FPS power_mod(FPS f, ll d, FPS g) : O(m^1.6 log d)　（m = deg g）
*	f(x)^d mod g(x) を返す．
*/
template <class S, S(*add)(S, S), S(*o)(), S(*mi)(S), S(*mul)(S, S), S(*e)()>
struct FPS {
	using SFPS = vector<pair<int, S>>;

	int n; // 係数の個数（次数 + 1）
	vector<S> c; // 係数列

	// コンストラクタ（零元，定数，次数指定付き定数，係数列で初期化）
	FPS() : n(0) {}
	FPS(const S& c0) : n(1), c({ c0 }) {}
	FPS(const S& c0, int n_) : n(n_), c(n, o()) { c[0] = c0; }
	FPS(const vector<S>& c_) : n(sz(c_)), c(c_) {}

	// 代入
	FPS(const FPS& f) = default;
	FPS& operator=(const FPS& f) = default;
	FPS& operator=(const S& c0) { n = 1; c = { c0 }; return *this; }

	// 比較
	[[nodiscard]] bool operator==(const FPS& g) const { return c == g.c; }
	[[nodiscard]] bool operator!=(const FPS& g) const { return c != g.c; }

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

	// 定数倍
	FPS& operator*=(const S& sc) { rep(i, n) c[i] = mul(c[i], sc); return *this; }
	FPS operator*(const S& sc) const { return FPS(*this) *= sc; }
	friend FPS operator*(const S& sc, const FPS& f) { return f * sc; }

	// 加法逆元
	FPS operator-() const { return FPS(*this) *= mi(e()); }

	// 積
	vector<S> karatsuba(const vector<S>& f, const vector<S>& g) const {
		// 参考 : https://37zigen.com/fast-multiplication/

		int n = sz(f), m = sz(g);

		// min(n, m) が小さいときは愚直に積を計算する．
		if (n <= 60 || m <= 60) {
			vector<S> h(n + m - 1, o());
			rep(i, n) rep(j, m) h[i + j] = add(h[i + j], mul(f[i], g[j]));
			return h;
		}

		int L = min({ max(n, m) / 2, n, m });

		int n0 = min(L, n), n1 = n - n0;
		int m0 = min(L, m), m1 = m - m0;

		vector<S> f0(n0, o()), f1(n1, o()), f01(max(n0, n1), o());
		vector<S> g0(m0, o()), g1(m1, o()), g01(max(m0, m1), o());
		rep(i, n0) {
			f0[i] = f[i];
			f01[i] = f[i];
		}
		rep(i, n1) {
			f1[i] = f[n0 + i];
			f01[i] = add(f01[i], f[n0 + i]);
		}
		rep(j, m0) {
			g0[j] = g[j];
			g01[j] = g[j];
		}
		rep(j, m1) {
			g1[j] = g[m0 + j];
			g01[j] = add(g01[j], g[m0 + j]);
		}

		vector<S> h0 = karatsuba(f0, g0);
		vector<S> h01 = karatsuba(f01, g01);
		vector<S> h1 = karatsuba(f1, g1);

		vector<S> h(n + m - 1, o());
		rep(i, sz(h0)) {
			h[i] = add(h[i], h0[i]);
			h[L + i] = add(h[L + i], mi(h0[i]));
		}
		rep(i, sz(h1)) {
			h[2 * L + i] = add(h[2 * L + i], h1[i]);
			h[L + i] = add(h[L + i], mi(h1[i]));
		}
		rep(i, sz(h01)) {
			h[L + i] = add(h[L + i], h01[i]);
		}

		return h;
	}
	FPS operator*(const FPS& g) const {
		// verify : https://judge.yosupo.jp/problem/convolution_mod_2_64

		if (n == 0 || g.n == 0) return FPS();
		return FPS(karatsuba(c, g.c));
	}
	FPS& operator*=(const FPS& g) { *this = *this * g; return *this; }

	// 除算
	FPS inv(int d) const {
		// verify : https://atcoder.jp/contests/abc327/tasks/abc327_g

		Assert(c[0] == e());

		FPS g(e());
		for (int k = 1; k < d; k *= 2) {
			g = (add(e(), e()) - *this * g) * g;
			g.resize(2 * k);
		}

		return g.resize(d);
	}
	FPS& operator/=(const FPS& g) { return *this *= g.inv(n); }
	FPS operator/(const FPS& g) const { return FPS(*this) /= g; }

	// 余り付き除算
	FPS quotient(const FPS& g) const {
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
		S g0 = o();
		if (it0->first == 0) {
			g0 = it0->second;
			it0++;
		}

		// 後ろからインライン配る DP
		repir(i, n - 1, 0) {
			// 上位項に係数倍して配っていく．
			for (auto it = it0; it != g.end(); it++) {
				auto [j, gj] = *it;
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
		// verify : https://atcoder.jp/contests/arc059/tasks/arc059_c

		// g の定数項だけ例外処理
		auto it0 = g.begin();
		Assert(it0->first == 0 && it0->second == e());
		it0++;

		// 前からインライン配る DP（後ろに累積効果あり）
		rep(i, n) {
			// 上位項に係数倍して配っていく．
			for (auto it = it0; it != g.end(); it++) {
				auto [j, gj] = *it;
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
		while (n > 0 && c[n - 1] == o()) {
			c.pop_back();
			n--;
		}
		return *this;
	}

	// 高次項の除去 or 0 埋め
	FPS& resize(int d) {
		n = d;
		c.resize(d, o());
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
			if (d & 1) res = (res * pow2).reminder(g);
			pow2 = (pow2 * pow2).reminder(g);
			d /= 2;
		}
		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const FPS& f) {
		if (f.n == 0) os << o();
		else {
			rep(i, f.n) {
				os << f[i] << "z^" << i;
				if (i < f.n - 1) os << " + ";
			}
		}
		return os;
	}
#endif
};


//【展開係数（可換環）】O(n^1.6 log N)
/*
* [z^N] f(z)/g(z) を返す．
*
* 制約 : g[0] = 1
*/
template <class S, S(*add)(S, S), S(*o)(), S(*mi)(S), S(*mul)(S, S), S(*e)()>
S bostan_mori(FPS<S, add, o, mi, mul, e> f, FPS<S, add, o, mi, mul, e> g, ll N) {
	// 参考 : http://q.c.titech.ac.jp/docs/progs/polynomial_division.html
	// verify : https://atcoder.jp/contests/abc009/tasks/abc009_4

	//【方法】
	// 分母分子に g(-z) を掛けることにより
	//		f(z) / g(z) = f(z) g(-z) / g(z) g(-z)
	// を得る．ここで g(z) g(-z) は偶多項式なので
	//		g(z) g(-z) = e(z^2)
	// と表すことができる．
	// 
	// 分子について
	//		f(z) g(-z) = E(z^2) + z O(z^2)
	// というように偶多項式部分と奇多項式部分に分けると，N が偶数のときは
	//		[z^N] f(z) g(-z) / g(z) g(-z)
	//		= [z^N] E(z^2) / e(z^2)
	//		= [z^(N/2)] E(z) / e(z)
	// となり，N が奇数のときは
	//		[z^N] f(z) g(-z) / g(z) g(-z)
	//		= [z^N] z O(z^2) / e(z^2)
	//		= [z^((N-1)/2)] O(z) / e(z)
	// となる．
	//
	// これを繰り返せば N を半分ずつに減らしていくことができる．

	Assert(g.n >= 1 && g[0] == e());

	// f(z) = 0 のときは 0 を返す．
	if (f.n == 0) return o();

	while (N > 0) {
		// f2(z) = f(z) g(-z), g2(z) = g(z) g(-z) を求める．
		FPS<S, add, o, mi, mul, e> f2, g2(g);
		rep(i, g2.n) if (i & 1) g2[i] = mi(g2[i]);
		f2 = f * g2;
		g2 *= g;

		// f3(z) = E(z) or O(z), g3(z) = e(z) を求める．
		f.c.clear(); g.c.clear();
		if (N & 1) rep(i, min<ll>(f2.n / 2, N / 2 + 1)) f.c.push_back(f2[2 * i + 1]);
		else rep(i, min<ll>((f2.n + 1) / 2, N / 2 + 1)) f.c.push_back(f2[2 * i]);
		f.n = sz(f.c);
		rep(i, min<ll>((g2.n + 1) / 2, N / 2 + 1)) g.c.push_back(g2[2 * i]);
		g.n = sz(g.c);

		// N を半分にして次のステップに進む．
		N /= 2;
	}

	// N = 0 になったら定数項を返す．
	return f[0];
}


//【線形漸化式（可換環）】O(n^1.6 log N)
/*
* 初項 a[0..n) と漸化式 a[i] = Σj=[0..n) c[j]a[i-1-j] で定義される
* 数列 a について，a[N] の値を返す．
*
* 利用：【展開係数（可換環）】
*/
template <class S, S(*add)(S, S), S(*o)(), S(*mi)(S), S(*mul)(S, S), S(*e)()>
S linearly_recurrent_sequence(const vector<S>& a, const vector<S>& c, ll N) {
	// verify : https://atcoder.jp/contests/abc009/tasks/abc009_4

	int n = sz(a);

	FPS<S, add, o, mi, mul, e> A(a), C(c);
	FPS<S, add, o, mi, mul, e> Dnm = e() - (C >> 1);
	FPS<S, add, o, mi, mul, e> Num = (Dnm * A).resize(n);

	return bostan_mori(Num, Dnm, N);
}


//【形式的冪級数（半環）】
/*
* SemiFPS<S, add, o, mul, e>() : O(1)
*	零多項式 f = o() で初期化する．
*   係数は半環 <S, add, o, mul, e> の元とする．
*
* SemiFPS<S, add, o, mul, e>(c0) : O(1)
*	定数多項式 f = c0 で初期化する．
*
* SemiFPS<S, add, o, mul, e>(c0, n) : O(d)
*	n 次未満の項をもつ定数多項式 f = c0 で初期化する．
*
* SemiFPS<S, add, o, mul, e>(c) : O(n)
*	f(z) = c[0] + c[1] z + ... + c[n - 1] z^(n-1) で初期化する．
*
* c + f, f + c : O(1)	f + g : O(n)
* c * f, f * c : O(n)	f * g : O(n^2)		f * g_sp : O(n k)（k : g の項数）
*	形式的冪級数としての和，積の結果を返す．
*	g_sp はスパース多項式であり，{次数, 係数} の次数昇順の組の vector で表す．
*
* f.acc(SFPS g_sp) : O(n k)
*	Σi=[0..∞) g(z)^i を掛ける．
*	g_sp はスパース多項式であり，{次数, 係数} の次数昇順の組の vector で表す．
*	制約：g(0) == o()
*
* int f.deg(), int f.size() : O(1)
*	多項式 f の次数[+1]を返す．
*
* SemiFPS::monomial(int d) : O(d)
*	単項式 z^d を返す．
*
* S f.assign(S c) : O(n)
*	多項式 f の不定元 z に c を代入した値を返す．
*
* f.resize(int d) : O(d)
*	mod z^d をとる．
*
* f.resize() : O(n)
*	不要な高次の項を削る．
*
* f >> d, f << d : O(n)
*	係数列を d だけ右[左]シフトした多項式を返す．
*  （右シフトは z^d の乗算，左シフトは z^d で割った商と等価）
*
* SemiFPS power_mod(SemiFPS f, ll p, int d) : O(d^2 log d)
*	f(z)^p % z^d を返す．
*/
template <class S, S(*add)(S, S), S(*o)(), S(*mul)(S, S), S(*e)()>
struct SemiFPS {
	using SFPS = vector<pair<int, S>>;

	int n; // 係数の個数（次数 + 1）
	vector<S> c; // 係数列

	// コンストラクタ（零元，定数，次数指定付き定数，係数列で初期化）
	SemiFPS() : n(0) {}
	SemiFPS(const S& c0) : n(1), c({ c0 }) {}
	SemiFPS(const S& c0, int n_) : n(n_), c(n, o()) { c[0] = c0; }
	SemiFPS(const vector<S>& c_) : n(sz(c_)), c(c_) {}

	// 代入
	SemiFPS(const SemiFPS& f) = default;
	SemiFPS& operator=(const SemiFPS& f) = default;
	SemiFPS& operator=(const S& c0) { n = 1; c = { c0 }; return *this; }

	// アクセス
	inline S const& operator[](int i) const { return c[i]; }
	inline S& operator[](int i) { return c[i]; }

	// 次数
	int deg() const { return n - 1; }
	int size() const { return n; }

	// 加算
	SemiFPS& operator+=(const SemiFPS& g) {
		if (n >= g.n) rep(i, g.n) c[i] = add(c[i], g[i]);
		else {
			rep(i, n) c[i] = add(c[i], g[i]);
			repi(i, n, g.n - 1)	c.push_back(g[i]);
			n = g.n;
		}
		return *this;
	}
	SemiFPS operator+(const SemiFPS& g) const { return SemiFPS(*this) += g; }

	// 定数加算
	SemiFPS& operator+=(const S& sc) {
		if (n == 0) { n = 1; c = { sc }; }
		else { c[0] = add(c[0], sc); }
		return *this;
	}
	SemiFPS operator+(const S& sc) const { return SemiFPS(*this) += sc; }
	friend SemiFPS operator+(const S& sc, const SemiFPS& f) { return f + sc; }

	// 定数倍
	SemiFPS& operator*=(const S& sc) { rep(i, n) c[i] = mul(c[i], sc); return *this; }
	SemiFPS operator*(const S& sc) const { return SemiFPS(*this) *= sc; }
	friend SemiFPS operator*(const S& sc, const SemiFPS& f) { return f * sc; }

	// 積
	SemiFPS& operator*=(const SemiFPS& g) {
		// verify : https://atcoder.jp/contests/abc310/tasks/abc310_h

		int m = g.deg();
		if (m == -1) return *this = SemiFPS();
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
	SemiFPS operator*(const SemiFPS& g) const { return SemiFPS(*this) *= g; }

	// スパース積
	SemiFPS& operator*=(const SFPS& g) {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_1_B

		// g の定数項だけ例外処理
		auto it0 = g.begin();
		S g0 = o();
		if (it0->first == 0) {
			g0 = it0->second;
			it0++;
		}

		// 後ろからインライン配る DP
		repir(i, n - 1, 0) {
			// 上位項に係数倍して配っていく．
			for (auto it = it0; it != g.end(); it++) {
				auto [j, gj] = *it;
				if (i + j >= n) break;
				c[i + j] = add(c[i + j], mul(c[i], gj));
			}

			// 定数項は最後に配るか消去しないといけない．
			c[i] = mul(c[i], g[0]);
		}

		return *this;
	}
	SemiFPS operator*(const SFPS& g) const { return SemiFPS(*this) *= g; }

	// スパース商
	SemiFPS& acc(const SFPS& g) {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_1_C

		// g の定数項だけ例外処理
		auto it0 = g.begin();
		Assert(it0->first != 0);

		// 前からインライン配る DP（後ろに累積効果あり）
		rep(i, n) {
			// 上位項に係数倍して配っていく．
			for (auto it = it0; it != g.end(); it++) {
				auto [j, gj] = *it;
				if (i + j >= n) break;
				c[i + j] = add(c[i + j], mul(c[i], gj));
			}
		}

		return *this;
	}

	// 係数反転
	SemiFPS rev() const { SemiFPS h = *this; reverse(all(h.c)); return h; }

	// 単項式
	static SemiFPS monomial(int d) {
		SemiFPS mono(o(), d + 1);
		mono[d] = e();
		return mono;
	}

	// 不要な高次項の除去
	SemiFPS& resize() {
		// 最高次の係数が非 0 になるまで削る．
		while (n > 0 && c[n - 1] == o()) {
			c.pop_back();
			n--;
		}
		return *this;
	}

	// 高次項の除去 or 0 埋め
	SemiFPS& resize(int d) {
		// verify : https://atcoder.jp/contests/abc310/tasks/abc310_h

		// x^d 以上の項を除去する．
		n = d;
		c.resize(d, o());
		return *this;
	}

	// 不定元への代入
	S assign(const S& x) const {
		S val(o());
		repir(i, n - 1, 0) val = add(mul(val, x), c[i]);
		return val;
	}

	// 係数のシフト
	SemiFPS& operator>>=(int d) {
		n += d;
		c.insert(c.begin(), d, o());
		return *this;
	}
	SemiFPS& operator<<=(int d) {
		// verify : https://atcoder.jp/contests/abc310/tasks/abc310_h

		n -= d;
		if (n <= 0) { c.clear(); n = 0; }
		else c.erase(c.begin(), c.begin() + d);
		return *this;
	}
	SemiFPS operator>>(int d) const { return SemiFPS(*this) >>= d; }
	SemiFPS operator<<(int d) const { return SemiFPS(*this) <<= d; }

	// 累乗の剰余
	friend SemiFPS power_mod(const SemiFPS& f, ll p, int d) {
		SemiFPS res(e()), pow2(f);
		while (p > 0) {
			if (p & 1) res = (res * pow2).resize(d);
			pow2 = (pow2 * pow2).resize(d);
			p /= 2;
		}
		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const SemiFPS& f) {
		if (f.n == 0) os << o();
		else {
			rep(i, f.n) {
				os << f[i] << "z^" << i;
				if (i < f.n - 1) os << " + ";
			}
		}
		return os;
	}
#endif
};


