#pragma once
#include "header.h"
#include "合同式.h"
#include "二項係数.h"
#include "多項式.h"
#include "畳込み.h"
// ■■■■■ 形式的冪級数 ■■■■■


//【形式的冪級数】
/*
* MFPS() : O(1)
*	零多項式 f = 0 で初期化する．
*
* MFPS(mint c0) : O(1)
*	定数多項式 f = c0 で初期化する．
*
* MFPS(mint c0, int n) : O(n)
*	n 次未満の項をもつ定数多項式 f = c0 で初期化する．
*
* MFPS(vm c) : O(n)
*	f(z) = c[0] + c[1] z + ... + c[n - 1] z^(n-1) で初期化する．
*
* set_conv(vm(*CONV)(const vm&, const vm&)) : O(1)
*	畳込み用の関数を CONV に設定する．
*
* c + f, f + c : O(1)	f + g : O(n)
* f - c : O(1)			c - f, f - g, -f : O(n)
* c * f, f * c : O(n)	f * g : O(n log n)		f * g_sp : O(n |g|)
* f / c : O(n)			f / g : O(n log n)		f / g_sp : O(n |g|)
*	形式的冪級数としての和，差，積，商の結果を返す．
*	g_sp はスパース多項式であり，{次数, 係数} の次数昇順の組の vector で表す．
*	制約 : 商では g(0) != 0
*
* MFPS f.inv(int d) : O(n log n)
*	1 / f mod z^d を返す．
*	制約 : f(0) != 0
*
* MFPS f.quotient(MFPS g) : O(n log n)
* MFPS f.reminder(MFPS g) : O(n log n)
* pair<MFPS, MFPS> f.quotient_remainder(MFPS g) : O(n log n)
*	多項式としての f を g で割った商，余り，商と余りの組を返す．
*	制約 : g の最高次の係数は 0 でない
*
* int f.deg(), int f.size() : O(1)
*	多項式 f の次数[項数]を返す．
*
* MFPS::monomial(int d, mint c = 1) : O(d)
*	単項式 c z^d を返す．
*
* mint f.assign(mint c) : O(n)
*	多項式 f の不定元 z に c を代入した値を返す．
*
* f.resize(int d) : O(1)
*	mod z^d をとる．
*
* f.resize() : O(n)
*	不要な高次の項を削る．
*
* f >> d, f << d : O(n)
*	係数列を d だけ右[左]シフトした多項式を返す．
*  （右シフトは z^d の乗算，左シフトは z^d で割った商と等価）
*
* f.push_back(c) : O(1)
*	最高次の係数として c を追加する．
*/
struct MFPS {
	using SMFPS = vector<pim>;

	int n; // 係数の個数（次数 + 1）
	vm c; // 係数列
	inline static vm(*CONV)(const vm&, const vm&) = convolution; // 畳込み用の関数

	// コンストラクタ（0，定数，係数列で初期化）
	MFPS() : n(0) {}
	MFPS(mint c0) : n(1), c({ c0 }) {}
	MFPS(int c0) : n(1), c({ mint(c0) }) {}
	MFPS(mint c0, int d) : n(d), c(n) { if (n > 0) c[0] = c0; }
	MFPS(int c0, int d) : n(d), c(n) { if (n > 0) c[0] = c0; }
	MFPS(const vm& c_) : n(sz(c_)), c(c_) {}
	MFPS(const vi& c_) : n(sz(c_)), c(n) { rep(i, n) c[i] = c_[i]; }

	// 代入
	MFPS(const MFPS& f) = default;
	MFPS& operator=(const MFPS& f) = default;
	MFPS& operator=(const mint& c0) { n = 1; c = { c0 }; return *this; }

	void push_back(mint cn) { c.emplace_back(cn); ++n; }
	void pop_back() { c.pop_back(); --n; }
	[[nodiscard]] mint back() { return c.back(); }

	// 比較
	[[nodiscard]] bool operator==(const MFPS& g) const { return c == g.c; }
	[[nodiscard]] bool operator!=(const MFPS& g) const { return c != g.c; }

	// アクセス
	inline mint const& operator[](int i) const { return c[i]; }
	inline mint& operator[](int i) { return c[i]; }

	// 次数
	[[nodiscard]] int deg() const { return n - 1; }
	[[nodiscard]] int size() const { return n; }

	static void set_conv(vm(*CONV_)(const vm&, const vm&)) {
		// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_fibonacci

		CONV = CONV_;
	}

	// 加算
	MFPS& operator+=(const MFPS& g) {
		if (n >= g.n) rep(i, g.n) c[i] += g.c[i];
		else {
			rep(i, n) c[i] += g.c[i];
			repi(i, n, g.n - 1)	c.push_back(g.c[i]);
			n = g.n;
		}
		return *this;
	}
	[[nodiscard]] MFPS operator+(const MFPS& g) const { return MFPS(*this) += g; }

	// 定数加算
	MFPS& operator+=(const mint& sc) {
		if (n == 0) { n = 1; c = { sc }; }
		else { c[0] += sc; }
		return *this;
	}
	[[nodiscard]] MFPS operator+(const mint& sc) const { return MFPS(*this) += sc; }
	[[nodiscard]] friend MFPS operator+(const mint& sc, const MFPS& f) { return f + sc; }
	MFPS& operator+=(const int& sc) { *this += mint(sc); return *this; }
	[[nodiscard]] MFPS operator+(const int& sc) const { return MFPS(*this) += sc; }
	[[nodiscard]] friend MFPS operator+(const int& sc, const MFPS& f) { return f + sc; }

	// 減算
	MFPS& operator-=(const MFPS& g) {
		if (n >= g.n) rep(i, g.n) c[i] -= g.c[i];
		else {
			rep(i, n) c[i] -= g.c[i];
			repi(i, n, g.n - 1) c.push_back(-g.c[i]);
			n = g.n;
		}
		return *this;
	}
	[[nodiscard]] MFPS operator-(const MFPS& g) const { return MFPS(*this) -= g; }

	// 定数減算
	MFPS& operator-=(const mint& sc) { *this += -sc; return *this; }
	[[nodiscard]] MFPS operator-(const mint& sc) const { return MFPS(*this) -= sc; }
	[[nodiscard]] friend MFPS operator-(const mint& sc, const MFPS& f) { return -(f - sc); }
	MFPS& operator-=(const int& sc) { *this += -sc; return *this; }
	[[nodiscard]] MFPS operator-(const int& sc) const { return MFPS(*this) -= sc; }
	[[nodiscard]] friend MFPS operator-(const int& sc, const MFPS& f) { return -(f - sc); }

	// 加法逆元
	[[nodiscard]] MFPS operator-() const { return MFPS(*this) *= -1; }

	// 定数倍
	MFPS& operator*=(const mint& sc) { rep(i, n) c[i] *= sc; return *this; }
	[[nodiscard]] MFPS operator*(const mint& sc) const { return MFPS(*this) *= sc; }
	[[nodiscard]] friend MFPS operator*(const mint& sc, const MFPS& f) { return f * sc; }
	MFPS& operator*=(const int& sc) { *this *= mint(sc); return *this; }
	[[nodiscard]] MFPS operator*(const int& sc) const { return MFPS(*this) *= sc; }
	[[nodiscard]] friend MFPS operator*(const int& sc, const MFPS& f) { return f * sc; }

	// 右からの定数除算
	MFPS& operator/=(const mint& sc) { *this *= sc.inv(); return *this; }
	[[nodiscard]] MFPS operator/(const mint& sc) const { return MFPS(*this) /= sc; }
	MFPS& operator/=(const int& sc) { *this /= mint(sc); return *this; }
	[[nodiscard]] MFPS operator/(const int& sc) const { return MFPS(*this) /= sc; }

	// 積
	MFPS& operator*=(const MFPS& g) { c = CONV(c, g.c); n = sz(c); return *this; }
	[[nodiscard]] MFPS operator*(const MFPS& g) const { return MFPS(*this) *= g; }

	// 除算
	[[nodiscard]] MFPS inv(int d) const {
		// 参考：https://nyaannyaan.github.io/library/fps/formal-power-series.hpp
		// verify : https://judge.yosupo.jp/problem/inv_of_formal_power_series

		//【方法】
		// 1 / f mod z^d を求めることは，
		//		f g = 1 (mod z^d)
		// なる g を求めることである．
		// この d の部分を 1, 2, 4, ..., 2^i と倍々にして求めていく．
		//
		// d = 1 のときについては
		//		g = 1 / f[0] (mod z^1)
		// である．
		//
		// 次に，
		//		g = h (mod z^k)
		// が求まっているとして
		//		g mod z^(2 k)
		// を求める．最初の式を変形していくことで
		//		g - h = 0 (mod z^k)
		//		⇒ (g - h)^2 = 0 (mod z^(2 k))
		//		⇔ g^2 - 2 g h + h^2 = 0 (mod z^(2 k))
		//		⇒ f g^2 - 2 f g h + f h^2 = 0 (mod z^(2 k))
		//		⇔ g - 2 h + f h^2 = 0 (mod z^(2 k)) 　(f g = 1 (mod z^d) より)
		//		⇔ g = (2 - f h) h (mod z^(2 k))
		// を得る．
		//
		// この手順を d ≦ 2^i となる i まで繰り返し，d 次以上の項を削除すればよい．

		Assert(!c.empty());
		Assert(c[0] != 0);

		MFPS g(c[0].inv());
		for (int k = 1; k < d; k <<= 1) {
			int len = max(min(2 * k, d), 1);
			MFPS tmp(0, len);
			rep(i, min(len, n)) tmp[i] = -c[i];	// -f
			tmp *= g;							// -f h
			tmp.resize(len);
			tmp[0] += 2;						// 2 - f h
			g *= tmp;							// (2 - f h) h
			g.resize(len);
		}

		return g;
	}
	MFPS& operator/=(const MFPS& g) { return *this *= g.inv(max(n, g.n)); }
	[[nodiscard]] MFPS operator/(const MFPS& g) const { return MFPS(*this) /= g; }

	// 余り付き除算
	[[nodiscard]] MFPS quotient(const MFPS& g) const {
		// 参考 : https://nyaannyaan.github.io/library/fps/formal-power-series.hpp
		// verify : https://judge.yosupo.jp/problem/division_of_polynomials

		//【方法】
		// f(x) = g(x) q(x) + r(x) となる q(x) を求める．
		// f の次数は n-1, g の次数は m-1 とする．(n ≧ m)
		// 従って q の次数は n-m，r の次数は m-2 となる．
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
		// q の次数は n-m であったから，q 自身を正しく求めることができた．

		if (n < g.n) return MFPS();
		return ((this->rev() / g.rev()).resize(n - g.n + 1)).rev();
	}
	[[nodiscard]] MFPS reminder(const MFPS& g) const {
		// verify : https://judge.yosupo.jp/problem/division_of_polynomials

		return (*this - this->quotient(g) * g).resize();
	}
	[[nodiscard]] pair<MFPS, MFPS> quotient_remainder(const MFPS& g) const {
		// verify : https://judge.yosupo.jp/problem/division_of_polynomials

		pair<MFPS, MFPS> res;
		res.first = this->quotient(g);
		res.second = (*this - res.first * g).resize();
		return res;
	}

	// スパース積
	MFPS& operator*=(const SMFPS& g) {
		// g の定数項だけ例外処理
		auto it0 = g.begin();
		mint g0 = 0;
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

				c[i + j] += c[i] * gj;
			}

			// 定数項は最後に配るか消去しないといけない．
			c[i] *= g0;
		}

		return *this;
	}
	[[nodiscard]] MFPS operator*(const SMFPS& g) const { return MFPS(*this) *= g; }

	// スパース商
	MFPS& operator/=(const SMFPS& g) {
		// g の定数項だけ例外処理
		auto it0 = g.begin();
		Assert(it0->first == 0 && it0->second != 0);
		mint g0_inv = it0->second.inv();
		it0++;

		// 前からインライン配る DP（後ろに累積効果あり）
		rep(i, n) {

			// 定数項は最初に配らないといけない．
			c[i] *= g0_inv;

			// 上位項に係数倍して配っていく．
			for (auto it = it0; it != g.end(); it++) {
				auto [j, gj] = *it;

				if (i + j >= n) break;

				c[i + j] -= c[i] * gj;
			}
		}

		return *this;
	}
	[[nodiscard]] MFPS operator/(const SMFPS& g) const { return MFPS(*this) /= g; }

	// 係数反転
	[[nodiscard]] MFPS rev() const { MFPS h = *this; reverse(all(h.c)); return h; }

	// 単項式
	[[nodiscard]] static MFPS monomial(int d, mint coef = 1) {
		MFPS mono(0, d + 1);
		mono[d] = coef;
		return mono;
	}

	// 不要な高次項の除去
	MFPS& resize() {
		// 最高次の係数が非 0 になるまで削る．
		while (n > 0 && c[n - 1] == 0) {
			c.pop_back();
			n--;
		}
		return *this;
	}

	// x^d 以上の項を除去する．
	MFPS& resize(int d) {
		n = d;
		c.resize(d);
		return *this;
	}

	// 不定元への代入
	[[nodiscard]] mint assign(const mint& x) const {
		mint val = 0;
		repir(i, n - 1, 0) val = val * x + c[i];
		return val;
	}

	// 係数のシフト
	MFPS& operator>>=(int d) {
		n += d;
		c.insert(c.begin(), d, 0);
		return *this;
	}
	MFPS& operator<<=(int d) {
		n -= d;
		if (n <= 0) { c.clear(); n = 0; }
		else c.erase(c.begin(), c.begin() + d);
		return *this;
	}
	[[nodiscard]] MFPS operator>>(int d) const { return MFPS(*this) >>= d; }
	[[nodiscard]] MFPS operator<<(int d) const { return MFPS(*this) <<= d; }

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const MFPS& f) {
		if (f.n == 0) os << 0;
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


//【微分】O(n)
/*
* f'(z) を返す．
*/
MFPS derivative(const MFPS& f) {
	// verify : https://judge.yosupo.jp/problem/log_of_formal_power_series

	MFPS res;
	repi(i, 1, f.n - 1) res.c.push_back(f[i] * i);
	res.n = sz(res.c);
	return res;
}


//【不定積分】O(n)
/*
* ∫ f(z) dz を返す．（定数項は 0 とする）
*
* 制約：fm は (deg(f) + 1)! まで計算可能
*/
MFPS integral_fps(const MFPS& f, const Factorial_mint& fm) {
	// verify : https://judge.yosupo.jp/problem/log_of_formal_power_series

	MFPS res(0);
	repi(i, 0, f.n - 1) res.c.push_back(f[i] * fm.inv(i + 1));
	res.n = sz(res.c);
	return res;
}


//【対数関数】O(n log n)
/*
* log f(z) mod z^d を返す．
*
* 制約 : [z^0]f(z) = 1，fm は d! まで計算可能
*/
MFPS log_fps(const MFPS& f, int d, const Factorial_mint& fm) {
	// 参考 : https://qiita.com/hotman78/items/f0e6d2265badd84d429a
	// verify : https://judge.yosupo.jp/problem/log_of_formal_power_series

	//【方法】
	// g(z) = log f(z) とおく．両辺を z で微分して
	//		g'(z) = f'(z) / f(z)
	// を得るので，
	//		g(z) = ∫ f'(z) / f(z) dz
	// として計算すればよい．

	int n = sz(f);

	MFPS g(0, max(n - 1, 1));
	repi(i, 1, n - 1) g[i - 1] = f[i] * i;			// f'(z)
	g *= f.inv(d - 1);								// f'(z) / f(z)
	g.resize(d);
	repir(i, d - 1, 1) g[i] = g[i - 1] * fm.inv(i);	// ∫ f'(z) / f(z) dz
	g[0] = 0;

	return g;
}


//【指数関数】O(n log n)
/*
* exp f(z) mod z^d を返す．
*
* 制約 : [z^0]f(z) = 0，fm は d! まで計算可能
*
* 利用：【対数関数】
*/
MFPS exp_fps(const MFPS& f, int d, const Factorial_mint& fm) {
	// 参考 : https://qiita.com/hotman78/items/f0e6d2265badd84d429a
	// verify : https://judge.yosupo.jp/problem/exp_of_formal_power_series

	//【方法】
	// g(z) = exp f(z) とおき，方程式
	//		log g(z) = f(z)
	// に対してニュートン法を用いる．
	// 
	// [z^0]f(z) = 0 なので，mod z^1 では
	//		log(1) ≡ f(z) mod z^1
	// が成り立つ．
	//
	// mod z^k で
	//		log h(z) ≡ f(z) mod z^k
	// が成り立っていると仮定すると，ニュートン法より
	//		g = h - (log h - f) / (log h)'
	//   ⇔ g = h (f + 1 - log h)
	// と置くと
	//		log g(z) ≡ f(z) mod z^(2 k)
	// が成り立つ．
	//
	// これを繰り返せば所望の g が求まる．

	// ニュートン法で log g = f なる g を見つける．
	MFPS g(1);
	for (int k = 1; k < d; k <<= 1) {
		int len = max(min(2 * k, d), 1);
		auto tmp = log_fps(g, len, fm);							// log h
		rep(i, len) tmp[i] = (i < sz(f) ? f[i] : 0) - tmp[i];	// f - log h
		tmp[0] += 1;											// f + 1 - log h
		g *= tmp;												// h (f + 1 - log h)
		g.resize(len);
	}

	return g;
}


//【累乗】O(n log n)
/*
* f(z)^k mod z^d を返す．（0^0 = 1 とする）
*
* 制約 : k ≧ 0，fm は d! まで計算可能
*
* 利用：【指数関数】,【対数関数】
*/
MFPS pow_fps(const MFPS& f, ll k, int d, const Factorial_mint& fm) {
	// 参考 : https://qiita.com/hotman78/items/f0e6d2265badd84d429a
	// verify : https://judge.yosupo.jp/problem/pow_of_formal_power_series

	int n = sz(f);

	// k = 0 なら f^k = 1 である．
	if (k == 0) return MFPS(1, d);

	// i0 : 最低次の項の次数
	int i0 = 0;
	while (i0 < n && f[i0] == 0) i0++;

	// f = 0 なら f^k = 0 である．
	if (i0 == n) return MFPS(0, d);

	// 最低次の項の係数を記録する．
	mint c0 = f[i0];

	// 定数項が 1 になるようシフトかつ定数除算した多項式を得る．
	MFPS fs = (f << i0) / c0;

	// 最終的に k * i0 次以上の項しか残らないことに注意し，0 になるケースを処理する．
	if (i0 >= (d + k - 1) / k) return MFPS(0, d);
	int ds = (int)(d - k * i0);

	// f^k = exp(k log f(x)) を用いて f^k を計算する．
	MFPS gs = exp_fps(mint(k) * log_fps(fs, ds, fm), ds, fm);

	// シフトと定数除算した分を元に戻す．
	MFPS g = (gs * c0.pow(k)) >> (int)(k * i0);

	return g;
}


//【累乗（有理数）】O(n log n)
/*
* f(z)^(num/dnm) mod z^d を返す．
*
* 制約 : [z^0]f(z) = 1，fm は d! まで計算可能
*
* 利用：【指数関数】,【対数関数】
*/
MFPS rational_pow_fps(const MFPS& f, ll num, ll dnm, int d, const Factorial_mint& fm) {
	// verify : https://judge.yosupo.jp/problem/compositional_inverse_of_formal_power_series_large

	Assert(sz(f) > 0 && f[0] == 1);

	// f^(num/dnm) = exp(num/dnm log f(x)) を用いて f^(num/dnm) を計算する．	
	return exp_fps(log_fps(f, d, fm) * mint(num) / mint(dnm), d, fm);
}


//【累乗の剰余】O(m log m log d)　（m = deg g）
/*
* f(z)^d mod g(z) を返す．
* 
* 制約：g(z) の最高次の係数は非 0
* 
*（繰り返し二乗法）
*/
MFPS power_mod(const MFPS& f, ll d, const MFPS& g) {
	// verify : https://judge.yosupo.jp/problem/kth_term_of_linearly_recurrent_sequence

	//【方法】
	// MSB から順に繰り返し二乗法を適用すれば，必要なのは 2 乗と f 倍のみである．
	// f が低次の多項式であれば高速化が期待できる．

	MFPS res(1); int i = msb(d);

	while (i >= 0) {
		res = (res * res).reminder(g);
		if (getb(d, i)) res = (res * f).reminder(g);
		--i;
	}

	return res;
}


//【累乗の剰余（法が二項式）】O(m log d)
/*
* f(z)^d mod (z^m - c) を返す．
*/
MFPS power_mod(const MFPS& f, ll d, int m, mint c) {
	// verify : https://atcoder.jp/contests/abc309/tasks/abc309_h

	MFPS res(1), pow2(f);
	while (d > 0) {
		if (d & 1) {
			res *= pow2;
			repir(i, sz(res) - m - 1, 0) res[i] += c * res[i + m];
			res.resize(m);
		}
		pow2 *= pow2;
		repir(i, sz(pow2) - m - 1, 0) pow2[i] += c * pow2[i + m];
		pow2.resize(m);
		d /= 2;
	}
	res.resize(m);
	return res;
}


//【単項式の剰余】O(m log m log d)　（m = deg g）
/*
* z^d mod g(z) を返す．
*
* 制約：g(z) の最高次の係数は非 0
*
*（繰り返し二乗法）
*/
MFPS monomial_power_mod(ll d, const MFPS& g) {
	// 参考 : https://qiita.com/ryuhe1/items/da5acbcce4ac1911f47a
	// verify : https://judge.yosupo.jp/problem/kth_term_of_linearly_recurrent_sequence

	//【方法】
	// MSB から順に繰り返し二乗法を適用すれば，必要なのは 2 乗と z 倍のみである．
	// z 倍 (mod g(z)) は素朴に計算すれば O(m) で可能である．

	int m = sz(g); 
	if (m == 1) return MFPS(); 
	
	mint gm_inv = g[m - 1].inv();

	MFPS res(1); int i = msb(d);

	while (i >= 0) {
		res = (res * res).reminder(g);

		if (getb(d, i)) {
			// res(z) *= z (mod g(z)) をベタ書きする
			if (sz(res) == m - 1) {
				mint c = -res[m - 2] * gm_inv;
				repir(j, m - 2, 1) res[j] = res[j - 1] + g[j] * c;
				res[0] = g[0] * c;
			}
			else res >>= 1;
		}

		--i;
	}

	return res;
}


//【平方根】O(n log n)
/*
* √f(z) mod z^d の 1 つを返す．（なければ exist = false を格納する）
*
* 利用：【平方剰余】
*/
MFPS sqrt_fps(const MFPS& f, int d, bool& exist) {
	// 参考 : https://nyaannyaan.github.io/library/fps/fps-sqrt.hpp
	// verify : https://judge.yosupo.jp/problem/sqrt_of_formal_power_series

	//【方法】
	// √(z^2 f(z)) = z √f(z) なので f(0) が平方剰余として一般性を失わない．
	// f(0) は平方剰余なので，c0^2 = f(0) なる t が存在する．よって mod z^1 では
	//		c0^2 ≡ f(z) mod z^1
	// が成り立つ．
	//
	// mod z^k で
	//		h(z)^2 ≡ f(z) mod z^k
	// が成り立っていると仮定すると，ニュートン法より
	//		g = h - (h^2 - f) / (h^2)'
	//   ⇔ g = (h + f / h) / 2
	// と置くと
	//		g(z)^2 ≡ f(z) mod z^(2 k)
	// が成り立つ．
	//
	// これを繰り返せば所望の g が求まる．

	// 最低次の項を見つける．
	int i0 = 0;
	while (i0 <= f.deg() && f[i0] == 0) i0++;

	// 零多項式なら平方根も零多項式である．
	if (i0 == f.deg() + 1) {
		exist = true;
		MFPS g;
		g.resize(d);
		return g;
	}

	// 最低次の項が奇数次の項なら平方根は存在しない．
	if (i0 % 2 == 1) {
		exist = false;
		return MFPS();
	}

	// 最低次の項の係数が平方剰余でなければ平方根は存在しない．
	int c0 = (f[i0] == 1 ? 1 : cipolla(f[i0])); // 1 のことが多いので高速化
	if (c0 == -1) {
		exist = false;
		return MFPS();
	}

	// 定数項が 0 でないようにシフトした多項式を得る．
	MFPS fs = f << i0;

	// ニュートン法で g = √f を見つける．
	MFPS gs(c0);
	for (int k = 1; k < d; k *= 2) {
		gs = (gs + fs * gs.inv(2 * k)) / 2; // ここで 1/2 が必要
		gs.resize(2 * k);
	}

	// シフトした分を元に戻す．
	MFPS g = gs >> (i0 / 2);
	g.resize(d);

	exist = true;
	return g;
}


//【展開係数】O(n log n log N)
/*
* [z^N] f(z)/g(z) を返す．
*
* 制約 : deg f < deg g, g[0] ≠ 0
*/
mint bostan_mori(MFPS f, MFPS g, ll N) {
	// 参考 : http://q.c.titech.ac.jp/docs/progs/polynomial_division.html
	// verify : https://judge.yosupo.jp/problem/kth_term_of_linearly_recurrent_sequence

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

	Assert(g.n >= 1 && g[0] != 0);

	// f(z) = 0 のときは 0 を返す．
	if (f.n == 0) return 0;

	while (N > 0) {
		// f2(z) = f(z) g(-z), g2(z) = g(z) g(-z) を求める．
		MFPS f2, g2 = g;
		rep(i, g2.n) if (i & 1) g2[i] *= -1;
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
	return f[0] / g[0];
}


//【線形漸化式】O(n log n log N)
/*
* 初項 a[0..n) と漸化式 a[i] = Σj∈[0..n) c[j] a[i-1-j] で定義される
* 数列 a について，a[N] の値を返す．
*
* 利用：【展開係数】
*/
mint linearly_recurrent_sequence(const vm& a, const vm& c, ll N) {
	// verify : https://judge.yosupo.jp/problem/kth_term_of_linearly_recurrent_sequence

	int n = sz(c);
	if (n == 0) return 0;

	MFPS A(a), C(c);
	MFPS Dnm = 1 - (C >> 1);
	MFPS Num = (Dnm * A).resize(n);
	return bostan_mori(Num, Dnm, N);
}


//【線形漸化式の発見】O(n^2)
/*
* エスパー.h へ
*/


//【展開係数（分子がスパース）】O(n m log m log d)（n : f の項数，m : deg g）
/*
* 有理式 f(z)/g(z) を形式的冪級数に展開したときの z^d の係数を返す．
*
* 制約 : g[0] != 0
*
* 利用：【展開係数】
*/
mint bostan_mori(const MFPS::SMFPS& f, const MFPS& g, ll d) {
	// verify : https://atcoder.jp/contests/abc241/tasks/abc241_h

	//【方法】
	// 分子の c x^k という項からの [x^d](f / g) への寄与は
	//		c [x^(d - k)] (1 / g)
	// である．これを分子の全ての項について足し合わせる．
	mint res = 0;

	for (auto [deg, coef] : f) {
		if (d - deg < 0) continue;

		res += coef * bostan_mori(MFPS(1), g, d - deg);
	}

	return res;
}


//【展開係数（分母が二項式の積）】O(deg(g) m log N)
/*
*	f(z) = Σi∈[0..n) f[i] z^i
*	g(z) = Πj∈[0..m) (1 + c[j] z^d[j])
* とし [z^N] f(z)/g(z) を返す．
*
* 制約 : deg f < deg g, d[j] > 0
*/
mint bostan_mori(vm f, vector<pim> dcs, ll N) {
	// verify : https://yukicoder.me/problems/no/137

	//【備考】
	// 通常のボスタン-森法とは異なり，法が 998244353 でなくても使える．

	while (N > 0) {
		dump(sz(f));

		// f(z) = 0 のときは 0 を返す．
		if (sz(f) == 0) return 0;

		// g(z) の奇多項式因子で z ← -z としたものを分母分子に掛け，分母は z ← z^2 としておく．
		for (auto& [d, c] : dcs) {
			// 1 + c z^d

			if (d & 1) {
				f.resize(sz(f) + d);
				repir(i, sz(f) - 1, d) f[i] -= f[i - d] * c;
				c *= -c;
			}
			else {
				d /= 2;
			}
		}

		// N の偶奇に応じて f の偶[奇]多項式部分を取り出す．
		int i_ub = sz(f); int pt = 0;
		for (int i = (N & 1); i < i_ub; i += 2) f[pt++] = f[i];
		f.resize(pt);

		// N を半分にして次のステップに進む．
		N /= 2;
	}

	// N = 0 になったら定数項を返す．
	return sz(f) > 0 ? f[0] : 0;
}


//【展開係数】O(n log n log d)
/*
* [z^d] f(z)/g(z) を返す．
*
* 制約 : deg f < deg g, g[0] != 0
*
* 利用：【単項式の剰余】
*
*（高速きたまさ法）
*/
mint fast_kitamasa(const MFPS& f, const MFPS& g, ll d) {
	// 参考 : https://qiita.com/ryuhe1/items/da5acbcce4ac1911f47a
	// verify : https://judge.yosupo.jp/problem/kth_term_of_linearly_recurrent_sequence

	//【方法】
	// f(z)/g(z) = a[0] + a[1]z + a[2]z^2 + a[3]z^3 + ... とおく．
	// 
	// 例として g(z) = 1 - 2 z - 3 z^2, d = 3 の場合を考える．このとき，a[0..∞) は漸化式
	//		a[i] - 2 a[i-1] - 3 a[i-2] = 0	
	//		⇔ a[i] = 2 a[i-1] + 3 a[i-2]
	// を満たす．よって a[3] の代わりに
	//		a[3] = 2 a[2] + 3 a[1] = 2 (2 a[1] + 3 a[0]) + 3 a[1] = 7 a[1] + 6 a[0]
	// を足しても結果は変わらない．
	//
	// このときの a[0], a[1] につく重みの計算は，g(z) の係数を左右反転した
	//		M(z) = z^2 - 2 z - 3
	// で z^3 を割った余りを計算することで，
	//		z^3 = (z^2 - 2 z - 3)(z + 2) + (7 z + 6)
	// として求められる．
	// 
	// 一般の場合についても同様に考えることができ，z^d を mod M(z) で計算し，
	// 出てきた係数と a[0..2) との内積をとることにより求める値を得ることができる．

	// n : 分母 g(z) の項数
	int n = sz(g);

	// f(z)/g(z) = a[0] + a[1]z + a[2]z^2 + a[3]z^3 + ...
	MFPS a(f);
	a.resize(n);
	a /= g;

	// mod : 法となる n-1 次多項式
	MFPS mod(g);
	mod = mod.rev();

	// p : z^d
	MFPS p = monomial_power_mod(d, mod);

	// p と a[0..n-1) との内積をとる．
	mint res = 0;
	rep(i, sz(p)) res += a[i] * p[i];

	return res;
}


//【展開係数（間引き和）】O(n log n log(k d))
/*
* 有理式 f(z)/g(z) を形式的冪級数に展開したときの 1, z^d, z^2d, ..., z^(k-1)d の係数の和を返す．
*
* 制約 : deg f < deg g, g[0] != 0
* 
*（高速きたまさ法）
* 
* 利用：【累乗の剰余】
*/
mint thinning_sum(const MFPS& f, const MFPS& g, ll d, ll k) {
	// verify : https://csacademy.com/contest/iati-shumen-2017-day-1/task/superstition/statement/

	//【方法】
	// f(x)/g(x) = a[0] + a[1]x + a[2]x^2 + a[3]x^3 + ... とおく．
	// 
	// 例として g(x) = 1 - 2 x - 3 x^2 の場合を考える．このとき，a[0..∞) は漸化式
	//		a[i] - 2 a[i-1] - 3 a[i-2] = 0	
	//		⇔ a[i] = 2 a[i-1] + 3 a[i-2]
	// を満たす．よって例えば a[3] の代わりに
	//		a[3] = 2 a[2] + 3 a[1] = 2 (2 a[1] + 3 a[0]) + 3 a[1] = 7 a[1] + 6 a[0]
	// を足しても結果は変わらない．
	//
	// このときの a[0], a[1] につく重みの計算は，g(x) を左右反転した
	//		M(x) = x^2 - 2 x - 3
	// で x^3 を割った余りを計算することで，
	//		x^3 = (x^2 - 2 x - 3)(x + 2) + (7 x + 6)
	// として求められる．これは線形作用なので一括で行うことができる．
	// 
	// 以上より，1 + x^d + x^2d + ... + x^(k-1)d を mod M(x) で計算し，
	// 出てきた係数と a[0..2) との内積をとることにより求める値を得ることができる．

	// n : 分母 g(x) の項数
	int n = sz(g);

	// f(x)/g(x) = a[0] + a[1]x + a[2]x^2 + a[3]x^3 + ...
	MFPS a(f);
	a.resize(n);
	a /= g;

	// mod : 法となる n-1 次多項式
	MFPS mod(g);
	mod = mod.rev();

	// p : x^d
	MFPS p = power_mod(MFPS::monomial(1), d, mod);

	// p2 : 1 + x^d + x^2d + ... + x^(k-1)d
	MFPS p2(0), pow2(p), sumpow2(1);
	while (k > 0) {
		if (k & 1LL) p2 = (p2 * pow2 + sumpow2).reminder(mod);
		sumpow2 = (sumpow2 * (1 + pow2)).reminder(mod);
		pow2 = (pow2 * pow2).reminder(mod);
		k /= 2;
	}

	// p2 と a[0..n-1) との内積をとる．
	mint res = 0;
	rep(i, sz(p2)) res += a[i] * p2[i];

	return res;
}


//【展開係数（一括）】O((m+R-L) log(m+R-L) log R)
/*
* [z^[L..R)] f(z)/g(z) を返す．
*
* 制約 : g[0] != 0
*/
vm bostan_mori_msb(vm f, const vm& g, ll L, ll R) {
	// 参考 : https://qiita.com/ryuhe1/items/c18ddbb834eed724a42b
	// verify : https://judge.yosupo.jp/problem/consecutive_terms_of_linear_recurrent_sequence

	int n = sz(f), m = sz(g) - 1;

	if (n == 0) return vm(R - L);
	if (L == R) return vm();
	if (R == 1) return vm{ f[0] }; // https://x.com/noya2ruler/status/1809661975456325879

	Assert(m >= 0 && g[0] != 0);
	mint g0_inv = g[0].inv();

	if (n > R) {
		n = (int)R;
		f.resize(n);
	}

	if (m == 0) {
		vm res(R - L);
		rep(i, n - L) res[i] = f[i + L] * g0_inv;
		return res;
	}

	// 2^K : R 以上の最小の 2 冪
	int K = msb(R - 1) + 1;

	vvm q(K, vm(m + 1));
	repi(j, 0, m) q[0][j] = g[j] * g0_inv;

	vl d_min(K + 1), d_max(K + 1);
	d_min[0] = max(L - n + 1, 0LL); d_max[0] = R - 1;

	repi(k, 1, K - 1) {
		auto q_pos(q[k - 1]);
		for (int i = 1; i <= m; i += 2) q[k - 1][i] *= -1;
		auto qk_dbl = convolution(q_pos, q[k - 1]);
		repi(i, 0, m) q[k][i] = qk_dbl[i * 2];

		auto tmp = d_min[k - 1] - 1 - m;
		d_min[k] = tmp >= 0 ? tmp / 2 + 1 : 0;
		d_max[k] = d_max[k - 1] / 2;
	}

	for (int i = 1; i <= m; i += 2) q[K - 1][i] *= -1;

	const mint inv2 = mint(2).inv();
	mint inv2_pow[31];
	inv2_pow[0] = 1;
	rep(i, 30) inv2_pow[i + 1] = inv2_pow[i] * inv2;

	vm p{ 1 };

	repir(k, K - 1, 0) {
		// 持っている係数の範囲は
		//	p     : z^[d_min[k+1]..d_max[k+1]]
		//	p_dbl : z^[2 d_min[k+1] .. 2 d_max[k+1]]
		//	q[k]  : z^[0..m]
		// なので，p_dbl と q[k] を普通に畳み込むと 
		//          z^[2 d_min[k+1] .. 2 d_max[k+1] + m]
		// の範囲の係数が手に入る．でも欲しいのは
		//	        z^[d_min[k]..d_max[k]]
		// の範囲だけなので，端がエイリアシングっぽく壊れても問題ない．

		// W = 2^B : 必要な部分が壊れずに残るギリギリの 2 冪の長さ
		int B = msb(m + max(2 * d_max[k + 1] - d_min[k], 0LL)) + 1;
		int W = 1 << B;

		vm p_dbl(W);
		rep(i, sz(p)) p_dbl[i * 2] = p[i];

		q[k].resize(W);

		internal::butterfly(p_dbl);
		internal::butterfly(q[k]);
		rep(i, W) p_dbl[i] *= q[k][i];
		internal::butterfly_inv(p_dbl);

		p.resize(d_max[k] - d_min[k] + 1);
		int i_min = (int)max(-(d_min[k] - 2 * d_min[k + 1]), 0LL);
		int i_max = (int)min(d_max[k] - d_min[k], sz(p_dbl) - 1 - (d_min[k] - 2 * d_min[k + 1]));
		repi(i, i_min, i_max) {
			p[i] = p_dbl[i + d_min[k] - 2 * d_min[k + 1]] * inv2_pow[B];
		}
	}

	// 持っている係数の範囲は
	//	f : z^[0..n-1]
	//	p : z^[d_min[0]..d_max[0]]
	// なので，f と p を普通に畳み込むと 
	//      z^[d_min[0] .. n-1+d_max[0]]
	// の範囲の係数が手に入る．でも欲しいのは
	//	    z^[L..R-1]
	// の範囲だけなので，端がエイリアシングっぽく壊れても問題ない．

	// W = 2^B : 必要な部分が壊れずに残るギリギリの 2 冪の長さ
	int B = msb(n - 1 + d_max[0] - L) + 1;
	int W = 1 << B;

	f.resize(W);
	p.resize(W);

	internal::butterfly(f);
	internal::butterfly(p);
	rep(i, W) f[i] *= p[i];
	internal::butterfly_inv(f);

	g0_inv *= inv2_pow[B];
	vm res(R - L);
	rep(i, R - L) res[i] = f[i + L - d_min[0]] * g0_inv;

	return res;
}


//【平行移動】O(n log n)
/*
* 与えられた多項式 f(z) に対し，f(z + c) を返す．
*
* 制約 : fm は deg(f)! まで計算可能
*/
MFPS taylor_shift(const MFPS& f, mint c, const Factorial_mint& fm) {
	// 参考 : https://nyaannyaan.github.io/library/fps/taylor-shift.hpp.html
	// verify : https://judge.yosupo.jp/problem/polynomial_taylor_shift

	//【方法】
	//	f(x) = Σn=[0..N] f[n] x^n
	// と表されるとすると，
	//	f(x + c)
	//	= Σn=[0..N] f[n] (x + c)^n
	//	= Σn=[0..N] f[n] Σr=[0..n] nCr c^(n-r) x^r　（二項定理）
	//	= Σn=[0..N] Σr=[0..n] f[n] n! / ((n-r)! r!) c^(n-r) x^r
	//	= Σr=[0..N] Σn=[r..N] f[n] n! / ((n-r)! r!) c^(n-r) x^r　（和の順序交換）
	//	= Σr=[0..N] x^r / r! Σn=[r..N] (c^(n-r) / (n-r)!) n! f[n]
	//	= Σr=[0..N] x^r / r! Σm=[0..N-r] (c^(N-m-r) / (N-m-r)!) (N-m)! f[N-m]　（m=N-n）
	//	= Σj=[0..N] x^(N-j) / (N-j)! Σm=[0..j] (c^(j-m) / (j-m)!) (N-m)! f[N-m]　（j=N-r）
	// と書き直せる．
	//
	// よって
	//	g(x) = Σn=[0..N] (c^n / n!) x^n
	//	h(x) = Σn=[0..N] (N-n)! f[N-n] x^n
	// とおくと，
	//	f(x + c)
	//  = Σj=[0..N] x^(N-j) / (N-j)! (g*h)[j]
	//	= Σj=[0..N] x^j / j! (g*h)[N-j]
	// と表される．

	int n = f.deg() + 1;

	MFPS g(1);
	g.resize(n);
	repi(i, 1, n - 1) g[i] = g[i - 1] * c * fm.inv(i);

	MFPS h(f);
	rep(i, n) h[i] *= fm.fact(i);
	h = h.rev();

	MFPS fs = (g * h).resize(n);
	fs = fs.rev();
	rep(i, n) fs[i] *= fm.fact_inv(i);

	return fs;
}


//【拡縮】O(n log n)
/*
* 与えられた f(z) に対し，f(a z) を返す．
*/
MFPS scaling(MFPS f, mint a) {
	//【方法】
	//		f(z) = Σi=[0..n) f[i] z^i
	// と表されるとすると，
	//		f(az) = Σi=[0..n) f[i] (az)^i
	//		      = Σi=[0..n) a^i f[i] z^i
	// である．よって a の累積積を計算しながらそれを係数に乗じていけば良い．

	int n = sz(f);

	mint a_pow = 1;

	rep(i, n) {
		f[i] *= a_pow;
		a_pow *= a;
	}

	return f;
}


//【一次分数変換】O(n log n + m log m)
/*
* 与えられた多項式 f(z) に対し，f((a+bz)/(c+dz)) mod z^m を返す．
*
* 制約：c != 0, fm は deg(f)! まで計算可能
*
* 利用：【平行移動】
*/
MFPS linear_fractional_transformation(const MFPS& f, mint a, mint b, mint c, mint d, int m, const Factorial_mint& fm) {
	// 参考 : https://maspypy.com/%e5%a4%9a%e9%a0%85%e5%bc%8f%e3%83%bb%e5%bd%a2%e5%bc%8f%e7%9a%84%e3%81%b9%e3%81%8d%e7%b4%9a%e6%95%b0-%e9%ab%98%e9%80%9f%e3%81%ab%e8%a8%88%e7%ae%97%e3%81%a7%e3%81%8d%e3%82%8b%e3%82%82%e3%81%ae#toc46
	// verify : https://yukicoder.me/problems/no/2583

	if (d == 0) {
		// f((a+bz)/c) = f(a/c + (b/c)z)

		// f(z) → f(a/c + z)
		Assert(c != 0);
		mint c_inv = c.inv();
		MFPS g = taylor_shift(f, a * c_inv, fm);

		// f(a/c + z) → f(a/c + (b/c)z)
		mint r = b * c_inv, r_pow = 1;
		rep(i, sz(g)) {
			g[i] *= r_pow;
			r_pow *= r;
		}

		g.resize(m);
		return g;
	}

	// f((a+bz)/(c+dz)) = f(b/d + (ad-bc)/d^2 1/(c/d + z))

	// z → b/d + z
	// f(z) → f(b/d + z)
	mint d_inv = d.inv();
	MFPS g = taylor_shift(f, b * d_inv, fm);

	// z → (ad-bc)/d^2 z
	// f(b/d + z) → f(b/d + (ad-bc)/d^2 z)
	mint r = (a * d - b * c) * d_inv * d_inv, r_pow = 1;
	rep(i, sz(g)) {
		g[i] *= r_pow;
		r_pow *= r;
	}

	// 係数反転
	// f(b/d + (ad-bc)/d^2 z) → z^(n-1) f(b/d + (ad-bc)/d^2 1/z)
	int n = sz(f);
	g = g.rev();

	// z → c/d + z
	// z^(n-1) f(b/d + (ad-bc)/d^2 1/z) → (c/d + z)^(n-1) f(b/d + (ad-bc)/d^2 1/(c/d + z))
	r = c * d_inv;
	g = taylor_shift(g, r, fm);

	// (c/d + z)^(n-1) で割る
	// (c/d + z)^(n-1) f(b/d + (ad-bc)/d^2 1/(c/d + z)) → f(b/d + (ad-bc)/d^2 1/(c/d + z))
	MFPS dnm(0, m); r_pow = r.pow(n - 1); mint r_inv = r.inv();
	rep(i, m) {
		dnm[i] = fm.bin(n - 1, i) * r_pow;
		r_pow *= r_inv;
	}
	g.resize(m);
	g /= dnm;
	g.resize(m);

	return g;
}


//【一次式の積の展開（基本対称式）】O(n (log n)^2)
/*
* Πi∈[0..n) (z - x[i]) を返す．
* 
* 戻り値の i 次の項の係数は，x[0..n) の符号付き n-i 次基本対称式になる．
*/
MFPS expand(const vm& x) {
	// verify : https://judge.yosupo.jp/problem/factorial

	int n = sz(x);

	vector<MFPS> f(n);
	rep(i, n) f[i] = MFPS(vm({ -x[i], 1 }));

	// 2 冪個ずつ掛けていく（分割統治積）
	for (int k = 1; k < n; k *= 2) {
		for (int i = 0; i + k < n; i += 2 * k) {
			f[i] *= f[i + k];
		}
	}

	return f[0];
}


//【一次式の積の展開（等差数列）】O(n log n)
/*
* Πi∈[0..n) (z + a i + b) を返す．
*
* 制約 : fm は n! まで計算可能
*
* 利用：【平行移動】
*/
MFPS expand_arithmetic(int n, mint a, mint b, const Factorial_mint& fm) {
	// verify : https://judge.yosupo.jp/problem/stirling_number_of_the_first_kind

	//【方法】
	// 累乗をダブリングで計算するのと同様．
	// ただし同じものを掛けるのではなく平行移動したものを掛ける．

	//【備考】
	// a = -1, b = 0 なら係数は符号付き第 1 種スターリング数

	MFPS f(vm({ b, 1 })), res(1);

	while (n > 0) {
		if (n & 1) res = taylor_shift(res, f.deg() * a, fm) * f;
		f *= taylor_shift(f, f.deg() * a, fm);
		n /= 2;
	}

	return res;
}


//【一次式の積の展開（等比数列）】O(n log n)
/*
* Πi∈[0..n) (z + a r^i) を返す．
*
* 利用：【拡縮】
*/
MFPS expand_geometric(int n, mint a, mint r) {
	//【方法】
	// 代わりに
	//		Πi∈[0..n) (1 + a r^i z)
	// を計算して係数を反転すれば良い．
	//  
	//		f(z) = (1 + a z)(1 + a r z)
	// が計算できているとすると，
	//		f(r^2 z) = (1 + a r^2 z)(1 + a r^3 z)
	// なので，
	//		f(z) f(r^2 z) = (1 + a z)(1 + a r z)(1 + a r^2 z)(1 + a r^3 z)
	// が得られる．このようにダブリングを用いて計算していく．

	MFPS f(vm({ 1, a })), res(1);

	mint R = r;
	while (n > 0) {
		if (n & 1) res = scaling(res, R) * f;
		f = scaling(f, R) * f;
		R *= R;
		n /= 2;
	}

	return res.rev();
}


//【多項式の積の展開】O(n (log n)^2)
/*
* 多項式 fs[i] の積（次数は n）を返す．
*/
MFPS expand(vector<MFPS> fs) {
	// verify : https://judge.yosupo.jp/problem/product_of_polynomial_sequence

	if (fs.empty()) return MFPS(1);

	int m = sz(fs);

	// (次数, 多項式の番号) の組を要素数昇順に記録する．
	priority_queue_rev<pii> q;
	rep(i, m) q.push({ fs[i].deg(), i });

	while (sz(q) >= 2) {
		auto [di, i] = q.top(); q.pop();
		auto [dj, j] = q.top(); q.pop();

		fs[i] *= fs[j];
		q.push({ di + dj, i });
	}

	return fs[q.top().second];
}


//【二項式の積の展開】O(n (log n)^2 + m log m)
/*
* z^[0..m] Πi∈[0..n) (1 + c[i] z^d[i]) を返す．
*
* 制約：fm は (m+1)! まで計算可能
*
* 利用：【対数関数】
*/
MFPS expand_binomial(int m, const vm& c, const vi& d, const Factorial_mint& fm) {
	int n = sz(c);

	vector<vector<MFPS>> d_to_cs(m + 1); mint f0 = 1;
	rep(i, n) {
		if (d[i] == 0) {
			f0 *= 1 + c[i];
		}
		else if (d[i] <= m) {
			d_to_cs[d[i]].emplace_back(vm{ 1, c[i] });
		}
	}

	MFPS f(0, m + 1);

	repi(d, 1, m) {
		int K = sz(d_to_cs[d]);
		if (K == 0) continue;

		// 2 冪個ずつ掛けていく（分割統治積）
		for (int k = 1; k < K; k *= 2) {
			for (int i = 0; i + k < K; i += 2 * k) {
				d_to_cs[d][i] *= d_to_cs[d][i + k];
				if (sz(d_to_cs[d][i]) > m + 1) {
					d_to_cs[d][i].resize(m + 1);
				}
			}
		}

		auto g = log_fps(d_to_cs[d][0], m / d + 1, fm);

		rep(i, sz(g)) f[d * i] += g[i];
	}

	f = exp_fps(f, m + 1, fm);
	f *= f0;

	return f;
}


//【多項式の累積積の和】O(n (log n)^2)
/*
* 多項式の列 fs[0..k) について，Σi=[0..k] Πfs[0..i)（次数は n）を返す．
*/
MFPS cumulative_product_sum(const vector<MFPS>& fs_) {
	// verify : https://atcoder.jp/contests/abc269/tasks/abc269_h

	//【方法】
	// 1-indexed で考える．例えば k=7 のとき，答えは
	//	1 + f1 + f1 f2 + f1 f2 f3 + f1 f2 f3 f4 + f1 f2 f3 f4 f5 + f1 f2 f3 f4 f5 f6 + f1 f2 f3 f4 f5 f6 f7
	//	= (1 + f1) + f1 f2 (1 + f3) + f1 f2 f3 f4 (1 + f5 + f5 f6 (1 + f7))
	// である．
	//
	// フェニック木の初期化と同様にして
	//		g1 = f1, g2 = f1 f2, g3 = f3, g4 = f1 f2 f3 f4, ...
	// と定めれば，答えは
	//		(1 + g1) + g2 (1 + g3) + g4 (1 + g5 + g6 (1 + g7))
	// と表される．
	//
	// まず
	//		g[2i+1] += 1 
	// とすれば答えは
	//		g1 + g2 g3 + g4 (g5 + g6 g7)
	// と表される．さらに
	//		g[4i+1] += g[4i+2] g[4i+3]
	// とすれば答えは
	//		g1 + g4 g5
	// と表される．最後に
	//		g[8i+1] += g[8i+4] g[8i+5]
	// とすれば答えは
	//		g1
	// と表される．

	int n = sz(fs_);

	if (n == 0) return MFPS(1);

	// 1-indexed になおして格納する．
	vector<MFPS> fs(n + 1);
	rep(i, n) fs[i + 1] = fs_[i];

	// フェニック木の初期化段階
	for (int pow2 = 1; 2 * pow2 <= n; pow2 *= 2) {
		for (int i = 2 * pow2; i <= n; i += 2 * pow2) {
			fs[i] *= fs[i - pow2];
		}
	}

	// 奇数番目の要素への 1 の加算
	for (int i = 1; i <= n; i += 2) fs[i] += 1;

	// 積の加算の繰り返し
	for (int pow2 = 2; pow2 <= n; pow2 *= 2) {
		for (int i = 1; i + pow2 - 1 <= n; i += 2 * pow2) {
			fs[i] += fs[i + pow2 - 1] * (i + pow2 <= n ? fs[i + pow2] : 1);
		}
	}

	return fs[1];
}


//【多項式の累積積の加重和】O(n (log n)^2)
/*
* 与えられた多項式の列 as[0..k], fs[0..k) に対して Σi=[0..k] as[i] Πfs[0..i)（次数は n）を返す．
*/
MFPS weighted_cumulative_product_sum(const vector<MFPS>& as, const vector<MFPS>& fs) {
	// verify : https://atcoder.jp/contests/abc269/tasks/abc269_h

	//【方法】
	// f だけ 1-indexed で考える．例えば k=7 のとき，答えは
	//	a0 + a1 f1 + a2 f1 f2 + a3 f1 f2 f3 + a4 f1 f2 f3 f4 + a5 f1 f2 f3 f4 f5 + a6 f1 f2 f3 f4 f5 f6 + a7 f1 f2 f3 f4 f5 f6 f7
	//	= (a0 + a1 f1) + f1 f2 (a2 + a3 f3) + f1 f2 f3 f4 (a4 + a5 f5 + f5 f6 (a6 + a7 f7))
	// である．
	//
	// フェニック木の初期化と同様にして
	//		g1 = f1, g2 = f1 f2, g3 = f3, g4 = f1 f2 f3 f4, ...
	// と定めれば，答えは
	//		(a0 + a1 g1) + g2 (a2 + a3 g3) + g4 (a4 + a5 g5 + g6 (a6 + a7 g7))
	// と表される．
	//
	// まず
	//		g[2i+1] ← a[2i] + a[2i+1] g[2i+1] 
	// とすれば答えは
	//		g1 + g2 g3 + g4 (g5 + g6 g7)
	// と表される．さらに
	//		g[4i+1] += g[4i+2] g[4i+3]
	// とすれば答えは
	//		g1 + g4 g5
	// と表される．最後に
	//		g[8i+1] += g[8i+4] g[8i+5]
	// とすれば答えは
	//		g1
	// と表される．

	//【備考】
	// as が 0 次で，fs が全て同じであれば，これは多項式の合成 A(f(z)) の計算に他ならない．

	int n = sz(fs);

	if (n == 0) return MFPS(1);

	// 1-indexed になおして格納する．
	vector<MFPS> gs(n + 1);
	rep(i, n) gs[i + 1] = fs[i];

	// フェニック木の初期化段階
	for (int pow2 = 1; 2 * pow2 <= n; pow2 *= 2) {
		for (int i = 2 * pow2; i <= n; i += 2 * pow2) {
			gs[i] *= gs[i - pow2];
		}
	}

	// 奇数番目の要素の変更
	for (int i = 1; i <= n; i += 2) gs[i] = as[i - 1] + as[i] * gs[i];

	// 積の加算の繰り返し
	for (int pow2 = 2; pow2 <= n; pow2 *= 2) {
		for (int i = 1; i + pow2 - 1 <= n; i += 2 * pow2) {
			gs[i] += gs[i + pow2 - 1] * (i + pow2 <= n ? gs[i + pow2] : as.back());
		}
	}

	return gs[1];
}


//【多項式の累積積の和（両側）】O(n (log n)^2)
/*
* 多項式の列 fs[0..K), gs[0..K) について，Σi=[0..K] Πfs[0..i) Πgs[i..K)（次数は n）を返す．
*/
MFPS cumulative_product_sum(vector<MFPS> fs, vector<MFPS> gs) {
	// verify : https://atcoder.jp/contests/abc385/tasks/abc385_g
	// 参考 : https://x.com/maspy_stars/status/1871256501479952719

	//【方法】
	// 1-indexed で考える．以下 f14 := f1 f2 f3 f4 などと略記する．
	// 
	// 例えば K=7 のとき，答えは
	//		g17 + f1g27 + f12g37 + f13g47 + f14g57 + f15g67 + f16g7 + f17
	//		= (g1+f1)g27 + f12(g3+f3)g47 + f14(g5+f5)g67 + f16(g7+f7)
	//		= ((g1+f1)g23 + f12(g3+f3))g47 + f14((g5+f5)g67 + f56(g7+f7))
	// である．
	// 
	// まずフェニック木の初期化と同様にして
	//		f1 ← f1, f2 ← f12, f3 ← f3, f4 ← f14, f5 ← f5, f6 ← f56, f7 ← f7
	//		g1 ← g1, g2 ← g23, g3 ← g3, g4 ← g47, g5 ← g5, g6 ← g67, g7 ← g7
	// と更新すれば，答えは
	//		((g1+f1)g2 + f2(g3+f3))g4 + f4((g5+f5)g6 + f6(g7+f7))
	// と表される．
	//
	// 次に
	//		f[2i+1] ← g[2i+1] + f[2i+1]
	// と更新すれば，答えは
	//		(f1 g2 + f2 f3)g4 + f4(f5 g6 + f6 f7)
	// と表される．さらに
	//		f[4i+1] ← f[4i+1] g[4i+2] + f[4i+2] f[4i+3]
	// と更新すれば，答えは
	//		f1 g4 + f4 f5
	// と表される．最後に
	//		f[8i+1] ← f[8i+1] g[8i+4] + f[8i+4] f[8i+5]
	// とすれば，答えは
	//		f1
	// と表される．

	int K = sz(fs);

	if (K == 0) return MFPS(1);

	// フェニック木の初期化段階
	for (int pow2 = 1; 2 * pow2 <= K; pow2 *= 2) {
		for (int i = 2 * pow2; i <= K; i += 2 * pow2) {
			fs[i - 1] *= fs[(i - pow2) - 1];
		}
		for (int i = 2 * pow2; i + pow2 <= K; i += 2 * pow2) {
			gs[i - 1] *= gs[(i + pow2) - 1];
		}
	}

	// 奇数番目の要素への g の加算
	for (int i = 1; i <= K; i += 2) fs[i - 1] += gs[i - 1];

	// 積の加算の繰り返し
	for (int pow2 = 2; pow2 <= K; pow2 *= 2) {
		for (int i = 1; i + pow2 - 1 <= K; i += 2 * pow2) {
			fs[i - 1] *= gs[(i + pow2 - 1) - 1];
			fs[i - 1] += fs[(i + pow2 - 1) - 1] * (i + pow2 <= K ? fs[(i + pow2) - 1] : 1);
		}
	}

	return fs[1 - 1];
}


//【有理式の通分】O(n (log n)^2)
/*
* 有理式 num[i] / dnm[i] の和（分子[分母] の次数は n 以下）の (分子, 分母) の組を返す．
*/
pair<MFPS, MFPS> reduction(vector<MFPS> num, vector<MFPS> dnm) {
	// verify : https://judge.yosupo.jp/problem/polynomial_interpolation

	int n = sz(num);

	// 2 冪個ずつ足していく（分割統治法）
	for (int k = 1; k < n; k *= 2) {
		for (int i = 0; i + k < n; i += 2 * k) {
			num[i] = num[i] * dnm[i + k] + num[i + k] * dnm[i];
			dnm[i] *= dnm[i + k];
		}
	}

	return make_pair(num[0], dnm[0]);
}


//【累乗和（次数ごと）】O(n (log n)^2 + m log m)
/*
* 各 j∈[0..m) について，Σ(a[0..n)^j) を格納したリストを返す．
*
* 制約 : fm は m! まで計算可能
*
* 利用：【対数関数】
*/
vm powered_sum(const vi& a, int m, const Factorial_mint& fm) {
	// verify : https://yukicoder.me/problems/no/1145

	//【方法】
	// log のマクローリン展開の公式より
	//		-log Πi∈[0..n)(1 - a[i]z)
	//		= Σi∈[0..n) -log(1 - a[i]z)
	//		= Σi∈[0..n) Σk∈[1..∞) 1/k a[i]^k z^k
	//		= Σk∈[1..∞) 1/k (Σi∈[0..n) a[i]^k) z^k
	// となるので，これの係数から累乗和が得られる．

	if (m == 0) return vm();

	int n = sz(a);

	vvm fs(n);
	rep(i, n) fs[i] = vm{ 1, -a[i] };

	// 2 冪個ずつ足していく（分割統治法）
	for (int k = 1; k < n; k *= 2) {
		if (sz(fs[0]) <= 60) {
			for (int i = 0; i + k < n; i += 2 * k) {
				// 素朴に畳み込む．				
				fs[i] = internal::convolution_naive(fs[i], fs[i + k]);
			}
		}
		else {
			for (int i = 0; i + k < n; i += 2 * k) {
				int w1 = sz(fs[i]);
				int w2 = sz(fs[i + k]);
				int w = w1 + w2 - 1;

				// w1 = w2 = 2^hoge + 1 のときエイリアシングが生じるが，それより定数倍高速化を優先させる．
				int W = 1 << (msb(w - 2) + 1);

				fs[i].resize(W);
				fs[i + k].resize(W);

				internal::butterfly(fs[i]);
				internal::butterfly(fs[i + k]);

				rep(j, W) fs[i][j] *= fs[i + k][j];

				internal::butterfly_inv(fs[i]);

				mint inv = mint(W).inv();
				fs[i].resize(w);
				rep(j, w) fs[i][j] *= inv;

				// エイリアシングが生じている場合は修正する．
				if ((1 << (msb(w - 1) + 1)) != W) {
					fs[i][w - 1] = fs[i][0] - 1;
					fs[i][0] = 1;
				}
			}
		}
	}

	MFPS f(fs[0]);
	f.resize(m);
	f = log_fps(f, m, fm);
	f[0] = n;
	repi(i, 1, m - 1) f[i] = -f[i] * i;

	return f.c;
}


//【累乗和（連続整数，次数ごと）】O(m log m)
/*
* 各 j∈[0..m) について，Σi∈[0..n) i^j を格納したリストを返す（0^0 = 1 とする）
*
* 制約：fm は m! まで計算可能
*/
vm powered_sum(ll n, int m, const Factorial_mint& fm) {
	// 参考 : https://maspypy.com/%e5%a4%9a%e9%a0%85%e5%bc%8f%e3%83%bb%e5%bd%a2%e5%bc%8f%e7%9a%84%e3%81%b9%e3%81%8d%e7%b4%9a%e6%95%b0-%e9%ab%98%e9%80%9f%e3%81%ab%e8%a8%88%e7%ae%97%e3%81%a7%e3%81%8d%e3%82%8b%e3%82%82%e3%81%ae#toc34
	// verify : https://yukicoder.me/problems/no/2459

	//【方法】
	// 指数関数のテイラー展開の式より
	//		Σi∈[0..n) exp(i z)
	//		= Σi∈[0..n) Σ_j (i^j / j!) z^j
	//		= Σ_j ((Σi∈[0..n) i^j) / j!) z^j
	// となるので，この係数から求める累乗和が得られる．等比数列の和の公式より
	//		Σi∈[0..n) exp(i z)
	//		= Σi∈[0..n) exp(z)^i
	//		= (1 - exp(n z)) / (1 - exp(z))
	// として計算すれば高速に係数が得られる．

	vm num(m), dnm(m); mint pow_n = n;
	rep(j, m) {
		num[j] = -fm.fact_inv(j + 1) * pow_n;
		dnm[j] = -fm.fact_inv(j + 1);
		pow_n *= n;
	}

	auto f = MFPS(num) / MFPS(dnm);

	vm res(m);
	rep(j, m) res[j] = f[j] * fm.fact(j);

	return res;
}


//【累積和多項式】O(n log n)
/*
* 与えられた n-1 次多項式 f(z) に対し，以下で定まる n 次多項式 g(z) を返す：
*	g(z) = Σx∈[0..z] f(x)
*
* 制約：fm は n! まで計算可能
*/
MFPS prefix_sum_polynomial(const MFPS& f, const Factorial_mint& fm) {
	// 参考 : https://ja.wikipedia.org/wiki/%E3%83%95%E3%82%A1%E3%82%A6%E3%83%AB%E3%83%8F%E3%83%BC%E3%83%90%E3%83%BC%E3%81%AE%E5%85%AC%E5%BC%8F	
	// verify : https://yukicoder.me/problems/no/2580

	//【方法】
	// f(z) = Σi∈[0..n) a[i] z^i とおくと，
	//	g(z)
	//	= Σx∈[0..z] f(x)
	//	= Σi∈[0..n) a[i] Σx∈[0..z] x^i
	// となる．ファウルハーバーの公式
	//	Σi∈[1..n] i^k = 1/(k+1) Σj∈[0..k] bin(k+1,j) (-1)^j B_j n^(k+1-j)（B_j はベルヌーイ数）
	// を用いて整理すると，
	//	g(z)
	//	= a[0] + Σi∈[0..n) a[i] 1/(i+1) Σj∈[0..i] bin(i+1,j) (-1)^j B_j z^(i+1-j)
	//	= a[0] + z Σi∈[0..n) a[i] 1/(i+1) Σk∈[0..i] bin(i+1,i-k) (-1)^(i-k) B_(i-k) z^k
	//	= a[0] + z Σk∈[0..n) Σi∈[k..n) a[i] 1/(i+1) (i+1)! / ((i-k)! (k+1)!) (-1)^(i-k) B_(i-k) z^k
	//	= a[0] + z Σk∈[0..n) 1/(k+1)! Σi∈[k..n) (a[i] i!) ((-1)^(i-k) B_(i-k) / (i-k)!) z^k
	// となる．これは上側畳込みなので高速に計算できる．

	//【注意】
	// wikipedia の式では (-1)^j B_j をベルヌーイ数としている．

	int n = sz(f);

	if (n == 0) return MFPS();

	// b[i] = (-1)^i B(i) / i!（B(i) : ベルヌーイ数）
	MFPS b(0, n);
	rep(i, n) b[i] = (i & 1 ? -1 : 1) * fm.fact_inv(i + 1);
	b = b.inv(n);
	b = b.rev();

	MFPS g(0, n);
	rep(i, n) g[i] = f[i] * fm.fact(i);

	// 上側畳込み
	g *= b;
	if (n > 2) g <<= (n - 2);
	else if (n == 1) g >>= 1;

	g[0] = f[0];
	repi(i, 1, n) g[i] *= fm.fact_inv(i);

	return g;
}


//【多点評価】O(m (log m)^2 + n log n)
/*
* n 次多項式 f(z) について，f(x[0..m)) の値を並べたリストを返す．
*/
vm multipoint_evaluation(const MFPS& f, const vm& x) {
	// 参考 : https://37zigen.com/multipoint-evaluation/
	// verify : https://judge.yosupo.jp/problem/multipoint_evaluation

	int m = sz(x);
	int m2 = 1 << (msb(m - 1) + 1);

	// sp : (z - x[i]) の連続する 2 冪個の積からなる完全二分木
	vector<MFPS> sp(m2 * 2);
	repi(i, m2, m2 + m - 1) sp[i] = MFPS(vm({ -x[i - m2], 1 }));
	repi(i, m2 + m, 2 * m2 - 1) sp[i] = MFPS(1);
	repir(i, m2 - 1, 1) sp[i] = sp[2 * i] * sp[2 * i + 1];

	// sr : f を sp[i] で割った余りからなる完全二分木
	vector<MFPS> sr(m2 * 2);
	sr[1] = f.reminder(sp[1]);
	repi(i, 2, m2 + m - 1) sr[i] = sr[i / 2].reminder(sp[i]);

	// sr の葉は (z - x[i]) で割った余りなので，因数定理よりこれが f(x[i]) に等しい．
	vm y(m);
	rep(i, m) y[i] = sz(sr[m2 + i]) ? sr[m2 + i][0] : 0;

	return y;
}


//【多点評価（等比数列）】O((m + n) log(m + n))
/*
* n 次多項式 f について，f(a r^[0..m)) の値を並べたリストを返す．
*/
vm chirp_Z_transform(const MFPS& f, int m, mint a, mint r) {
	// 参考 : https://37zigen.com/multipoint-evaluation/#Chirp_Z-transform
	// verify : https://judge.yosupo.jp/problem/multipoint_evaluation_on_geometric_sequence

	//【方法】
	// f(z) = Σi∈[0..n) f[i] z^i とおく．恒等式
	//		2ij = (i+j)(i+j-1) - i(i-1) - j(j-1)
	// を用いると，
	//		f(a r^j)
	//		= Σi∈[0..n) f[i] (a r^j)^i
	//		= Σi∈[0..n) f[i] a^i r^(ij)
	//		= Σi∈[0..n) f[i] a^i r^((i+j)(i+j-1)/2 - i(i-1)/2 - j(j-1)/2)
	//		= r^(-j(j-1)/2) Σi∈[0..n) f[i] a^i r^(-i(i-1)/2) r^((i+j)(i+j-1)/2)
	// となる．これはほぼ畳込みの形なので高速に計算できる．

	// 公比が 0 の場合の例外処理
	if (r == 0) {
		vm res(m, f[0]);
		res[0] = f.assign(a);
		return res;
	}

	int n = sz(f);

	vm r_inv_ppow(max(n, m)); mint r_inv = r.inv(), r_inv_pow = 1;
	r_inv_ppow[0] = 1;
	rep(i, max(n, m) - 1) {
		r_inv_ppow[i + 1] = r_inv_ppow[i] * r_inv_pow;
		r_inv_pow *= r_inv;
	}

	MFPS r_ppow(0, n + m); mint r_pow = 1;
	r_ppow[0] = 1;
	rep(i, n + m - 1) {
		r_ppow[i + 1] = r_ppow[i] * r_pow;
		r_pow *= r;
	}

	MFPS F(0, n); mint a_pow = 1;
	rep(i, n) {
		F[i] = f[i] * a_pow * r_inv_ppow[i];
		a_pow *= a;
	}
	F = F.rev();

	auto G = F * r_ppow;

	vm res(m);
	rep(j, m) res[j] = G[n - 1 + j] * r_inv_ppow[j];

	return res;
}


//【差積】O(n (log n)^2)
/*
* 与えられた a[0..n) について差積 Πi<j (a[j]-a[i]) を返す．
*/
mint difference_product(const vm& a) {
	// verify : https://yukicoder.me/problems/no/2513

	int n = sz(a);
	int N = 1 << (msb(n - 1) + 1);

	// muls : (z - a[i]) の連続する 2 冪個の積からなる完全二分木
	vector<MFPS> muls(N * 2);
	repi(i, N, N + n - 1) muls[i] = MFPS(vm({ -a[i - N], 1 }));
	repi(i, N + n, 2 * N - 1) muls[i] = MFPS(1);
	repir(i, N - 1, 1) muls[i] = muls[2 * i] * muls[2 * i + 1];

	// mods : Π(z - a[m..r)) を Π(z - a[l..m)) で割った余りからなる完全二分木
	vector<MFPS> mods(N * 2);
	mods[1] = MFPS(1);
	repi(i, 2, N + n - 1) {
		if (i & 1) mods[i] = (mods[i / 2] * muls[i - 1]).reminder(muls[i]);
		else mods[i] = mods[i / 2].reminder(muls[i]);
	}

	// mods の葉は (z - x[i]) で割った余りなので，因数定理よりこれが Π(a[i] - a[0..i)) に等しい．
	mint res = 1;
	rep(i, n) {
		if (sz(mods[N + i]) == 0) return 0;
		res *= mods[N + i][0];
	}

	return res;
}


//【ラグランジュ補間（一点評価）】O(n)
/*
* 各 i∈[0..n) について f(a i + b) = y[i] を満たす n-1 次多項式 f についての f(c) を返す．
*
* 制約：fm は n! まで計算可能
*/
mint lagrange_interpolation(int a, int b, const vm& y, mint c, const Factorial_mint& fm) {
	// 参考 : https://37zigen.com/lagrange-interpolation/
	// verify : https://atcoder.jp/contests/arc033/tasks/arc033_4

	//【方法】
	// ラグランジュ基底関数を
	//		f_i(x) = Πj≠i (x - x[j])/(x[i] - x[j])　（x[i] = a i + b）
	// と定めると，
	//		f(c) = Σi=[0..n) y[i] f_i(c)
	// と表される．
	//
	// 基底関数 f_i(x) の評価値 f_i(c) の分子については，左右からの累積積
	//		acc_l[i] = (c - x[0])(c - x[1]) ... (c - x[i - 1])
	//		acc_r[i] = (c - x[i + 1]) ... (c - x[n - 2])(c - x[n - 1])
	// を前計算しておけば計算できる．
	//
	// 分母については x[i] = a i + b であったことを思い出すと
	//		x[i] - x[j] = (a i + b) - (a j + b) = a (i - j)
	// となるので，
	//		Πj≠i a (i - j) = a^(n-1) (-1)^(n-1-i) i! (n-1-i)!
	// と計算できる．

	int n = sz(y);

	// acc_l[i] = (c - x[0])(c - x[1]) ... (c - x[i - 1])
	vm acc_l(n);
	acc_l[0] = 1;
	repi(i, 1, n - 1) acc_l[i] = acc_l[i - 1] * (c - (mint(a) * (i - 1) - b));

	// acc_r[i] = (c - x[i + 1]) ... (c - x[n - 2])(c - x[n - 1])
	vm acc_r(n);
	acc_r[n - 1] = 1;
	repir(i, n - 2, 0) acc_r[i] = (c - (mint(a) * (i + 1) - b)) * acc_r[i + 1];

	// ラグランジュ基底の線形結合を計算する．
	mint res = 0;
	rep(i, n) {
		res += y[i] * acc_l[i] * acc_r[i] * ((n - 1 - i) & 1 ? -1 : 1)
			* fm.fact_inv(i) * fm.fact_inv(n - 1 - i);
	}
	return res * mint(a).pow(n - 1);
}


//【ラグランジュ補間（多項式復元）】O(n (log n)^2)
/*
* n 点での値 f(x[i]) = y[i] から定まる n-1 次多項式 f(x) を返す．
*
* 利用：【一次式の積の展開】,【多点評価】,【有理式の通分】
*/
MFPS lagrange_interpolation(const vm& x, const vm& y) {
	// 参考 : https://37zigen.com/lagrange-interpolation/
	// verify : https://judge.yosupo.jp/problem/polynomial_interpolation

	//【方法】
	// ラグランジュ補間の通常の式は，基底関数の線形和の形をした
	//		f(z) = Σi=[0..n) y[i] Πj≠i (z - x[j])/(x[i] - x[j])
	// である．
	// 
	// ここで
	//		g(z) = Πi=[0..n) (z - x[i])
	// とおくと，f(z) は
	//		f(z) = g(z) Σi=[0..n) y[i] / ( g'(x[i]) (z - x[i]) )
	// とも表される．
	//
	// g(z) は一次式の積の展開なので分割統治で O(n (log n)^2) で計算でき，
	// g'(x[i]) らは多点評価を用いて O(n (log n)^2) で計算できる．
	// よって
	//		a[i] = y[i] / g'(x[i])
	// とおけば，後は
	//		f(z) / g(z) = Σi=[0..n) a[i] / (z - x[i])
	// を計算できればよく，これも分割統治で通分すれば O(n (log n)^2) で計算できる．

	int n = sz(x);
	
	if (n == 0) return MFPS();

	MFPS g = expand(x);

	// g(z) ← g'(z)
	repi(i, 1, n) g[i - 1] = g[i] * i;
	g.resize(n);

	vm b = multipoint_evaluation(g, x);

	vector<MFPS> num(n), dnm(n);
	rep(i, n) {
		num[i] = MFPS(y[i] / b[i]);
		dnm[i] = MFPS(vm({ -x[i], 1 }));
	}

	return reduction(num, dnm).first;
}


//【ラグランジュ補間（多項式復元，等比数列）】O(n log n)
/*
* n 点での値 f(a r^i) = y[i] から定まる n-1 次多項式 f(x) を返す．
*
* 利用：【一次式の積の展開（等比数列）】,【多点評価（等比数列）】
*/
MFPS lagrange_interpolation(mint a, mint r, const vm& y) {
	// 参考 : https://37zigen.com/lagrange-interpolation/
	// verify : https://judge.yosupo.jp/problem/polynomial_interpolation_on_geometric_sequence

	//【方法】
	// 通常のラグランジュ補間による多項式復元と同じく，
	//		一次式の積の展開 → 微分 → 多点評価 → 通分
	// の順に計算する．
	// 
	// 微分の計算量は元々 O(n) なので問題ない．
	// 一次式の積の展開と多点評価については O(n log n) の等比数列 ver があるのでそれを用いる．
	// 通分については，これが多点評価と転置の関係にあることに注意すると，
	// 対角行列をヴァンデルモンド行列の左右どちらから掛けるかの違いしかないので
	// 多点評価の等比数列 ver を使い回すことができる．

	int n = sz(y);
	if (n == 0) return MFPS();

	MFPS g = expand_geometric(n, -a, r);

	MFPS Dg(0, n);
	repi(i, 1, n) Dg[i - 1] = g[i] * i;

	vm b = chirp_Z_transform(Dg, n, a, r);

	MFPS h(y);
	rep(i, n) h[i] /= b[i];

	vm fpg = chirp_Z_transform(h, n, 1, r);

	mint a_pow = 1;
	rep(i, n) {
		fpg[i] *= a_pow;
		a_pow *= a;
	}
	reverse(all(fpg));

	auto f = MFPS(fpg) * g;
	f <<= sz(f) - n;

	return f;
}


//【標本点シフト】O((n + m) log(n + m))
/*
* f([0..n)) = y[0..n) なる n 次未満の多項式 f(z) について，f([z0..z0+m)) を返す．
*
* 制約 : fm は n! まで計算可能
*
* 利用：【順列の数（一括，r が固定で小さい）】,【逆数（一括）】
*/
vm sampling_points_shift(const vm& y, int m, ll z0, const Factorial_mint& fm) {
	// verify : https://judge.yosupo.jp/problem/shift_of_sampling_points_of_polynomial

	//【方法】
	// 大体【ラグランジュ補間（一点評価）】と同じ．
	// 分子が jPn/(j-i) と書けるので，畳込みで一括計算できる．

	int n = sz(y);

	vm a = inverse(z0 - n + 1, z0 + m);

	vm b(n);
	rep(i, n) b[i] = ((n - 1 - i) & 1 ? -1 : 1) * fm.fact_inv(i) * fm.fact_inv(n - 1 - i) * y[i];

	// 持っている係数の範囲は
	//	a : z^[0..n+m-2]
	//	b : z^[0..n-1]
	// なので，a と b を普通に畳み込むと 
	//      z^[0..2n+m-3]
	// の範囲の係数が手に入る．でも欲しいのは
	//	    z^[n-1..n+m-2]
	// の範囲だけなので，端がエイリアシングっぽく壊れても問題ない．

	// W : 必要な部分が壊れずに残るギリギリの 2 冪の長さ
	int W = 1 << (msb((n + m - 2) + (n - 1) - (n - 1)) + 1);

	a.resize(W);
	b.resize(W);

	internal::butterfly(a);
	internal::butterfly(b);
	rep(i, W) a[i] *= b[i];
	internal::butterfly_inv(a);

	a.erase(a.begin(), a.begin() + (n - 1) - 0);
	a.resize(m);

	mint inv = mint(W).inv();

	auto perm = perm_fixed_r(z0, z0 + m, n);

	// 0 除算で壊れたところは答えが　y[0..n) の中にあるのでコピーする．
	constexpr ll MOD = 998244353LL;
	rep(j, m) {
		if (perm[j] != 0) a[j] *= perm[j] * inv;
		else a[j] = y[(j + z0) % MOD];
	}

	return a;
}


//【ニュートン展開】O(m (log m)^2 + n log n)
/*
* n 次多項式 f(z) と p[0..m) について，m 次多項式空間のニュートン基底を
*	e_i(z) = Π_k∈[0..i) (z - p[k])  (i∈[0..m])
* としたときの f(z) の m 次までの展開係数，すなわち
*	f(z) ≡ Σ_i∈[0..m] d[i] e_i(z)  (mod z^(m+1))
* を満たす d[0..m] を返す．
*/
vm newton_expand(const MFPS& f, const vm& p) {
	// 参考 : https://noshi91.hatenablog.com/entry/2023/05/01/022946
	// verify : https://judge.yosupo.jp/problem/conversion_from_monomial_basis_to_newton_basis

	int m = sz(p);
	int m2 = 1 << (msb(m - 1) + 1);

	// sp : (x - x[i]) の連続する 2 冪個の積からなる完全二分木
	// 本当はフェニック木みたいに間引けば定数倍高速化できる．
	vector<MFPS> sp(m2 * 2);
	repi(i, m2, m2 + m - 1) sp[i] = MFPS(vm({ -p[i - m2], 1 }));
	repi(i, m2 + m, 2 * m2 - 1) sp[i] = MFPS(1);
	repir(i, m2 - 1, 1) sp[i] = sp[2 * i] * sp[2 * i + 1];

	// sr : f を sp[i] で割った余りや商からなる完全二分木
	vector<MFPS> sr(m2 * 2);
	sr[1] = f.reminder(sp[1]);
	for (int i = 2; i <= m2 + m - 1; i += 2) {
		auto [q, r] = sr[i / 2].quotient_remainder(sp[i]);
		sr[i] = move(r);
		sr[i + 1] = move(q);
	}

	// sr の葉が展開係数に等しい．
	vm d(m);
	rep(i, m) d[i] = sr[m2 + i][0];

	return d;
}


//【ニュートン法】O(n log n)
/*
* 与えられた関数 G に対し，
*	G(f(z)) = 0, [z^0]f(z) = f0
* を満たす f(z) を求めるには，
*	f_1(z) = f0  (mod z^(2^0))
*	f_(t+1)(z) = f_t(z) - G(f_t(z)) / G'(f_t(z))  (mod z^(2^(t+1)))
* なる反復式を用いて計算すれば良い．
* 
* verify : https://atcoder.jp/contests/abc345/tasks/abc345_g
*/


//【多項式 GCD】O(N (log N)^2)（N = max(deg f, deg g)）
/*
* f(z) u(z) + g(z) v(z) = gcd(f(z), g(z)) の解 (u(z), v(z)) を u, v に格納する．
* またモニックな gcd(f(z), g(z)) を返す．
*/
MFPS half_GCD(MFPS f, MFPS g, MFPS& u, MFPS& v) {
	// 参考 : https://scrapbox.io/37zigen-43465887/half-GCD
	// verify : https://judge.yosupo.jp/problem/inv_of_polynomials

	f.resize();
	g.resize();

	bool swap_flag = false;
	if (sz(f) < sz(g)) {
		swap(f, g);
		swap_flag = true;
	}

	if (sz(f) == 0) {
		u = MFPS();
		v = MFPS();
		return MFPS();
	}

	using MAT = array<array<MFPS, 2>, 2>;

	MAT id;
	id[0][0] = id[1][1] = MFPS(1);
	id[0][1] = id[1][0] = MFPS();

	function<MAT(MFPS&, MFPS&)> hgcd = [&](MFPS& f, MFPS& g) {
		if (f.deg() >= 2 * g.deg()) return id;

		int hn = sz(f) / 2;

		MFPS f0(f), g0(g);
		f0 <<= hn;
		g0 <<= hn;

		auto mat = hgcd(f0, g0);

		MFPS nf = mat[0][0] * f + mat[0][1] * g;
		MFPS ng = mat[1][0] * f + mat[1][1] * g;
		nf.resize();
		ng.resize();

		if (nf.deg() >= 2 * ng.deg()) {
			f = move(nf);
			g = move(ng);
			return mat;
		}

		auto [q, r] = nf.quotient_remainder(ng);
		q.resize();
		r.resize();
		f = move(ng);
		g = move(r);

		mat[0][0] -= q * mat[1][0];
		mat[0][1] -= q * mat[1][1];
		mat[0][0].resize();
		mat[0][1].resize();
		swap(mat[0][0], mat[1][0]);
		swap(mat[0][1], mat[1][1]);

		if (f.deg() >= 2 * g.deg()) return mat;

		f0 = f, g0 = g;
		f0 <<= hn / 2;
		g0 <<= hn / 2;

		auto nmat = hgcd(f0, g0);

		nf = nmat[0][0] * f + nmat[0][1] * g;
		ng = nmat[1][0] * f + nmat[1][1] * g;
		nf.resize();
		ng.resize();
		f = move(nf);
		g = move(ng);

		MAT ret;
		ret[0][0] = nmat[0][0] * mat[0][0] + nmat[0][1] * mat[1][0];
		ret[0][1] = nmat[0][0] * mat[0][1] + nmat[0][1] * mat[1][1];
		ret[1][0] = nmat[1][0] * mat[0][0] + nmat[1][1] * mat[1][0];
		ret[1][1] = nmat[1][0] * mat[0][1] + nmat[1][1] * mat[1][1];
		ret[0][0].resize();
		ret[0][1].resize();
		ret[1][0].resize();
		ret[1][1].resize();

		return ret;
	};

	MAT mat(id);

	while (1) {
		auto mat2 = hgcd(f, g);

		MAT nmat;
		nmat[0][0] = mat2[0][0] * mat[0][0] + mat2[0][1] * mat[1][0];
		nmat[0][1] = mat2[0][0] * mat[0][1] + mat2[0][1] * mat[1][1];
		nmat[1][0] = mat2[1][0] * mat[0][0] + mat2[1][1] * mat[1][0];
		nmat[1][1] = mat2[1][0] * mat[0][1] + mat2[1][1] * mat[1][1];
		nmat[0][0].resize();
		nmat[0][1].resize();
		nmat[1][0].resize();
		nmat[1][1].resize();

		if (sz(g) == 0) {
			mat = move(nmat);
			break;
		}

		auto [q, r] = f.quotient_remainder(g);
		q.resize();
		r.resize();
		f = move(g);
		g = move(r);

		mat[0][0] = nmat[1][0];
		mat[0][1] = nmat[1][1];
		mat[1][0] = nmat[0][0] - q * nmat[1][0];
		mat[1][1] = nmat[0][1] - q * nmat[1][1];
		mat[1][0].resize();
		mat[1][1].resize();
	}

	u = move(mat[0][0]);
	v = move(mat[0][1]);

	mint c = f.back();
	f /= c;
	u /= c;
	v /= c;

	if (swap_flag) swap(u, v);

	return f;
}


//【多項式逆元】O(N (log N)^2)（N = max(deg a, deg b)）
/*
* a(x) u(x) = 1 (mod b(x)) を満たす u(x) を格納する．（なければ false を返す）
*
* 利用：【多項式 GCD】
*/
bool polynomial_inverse(const MFPS& a, const MFPS& b, MFPS& u) {
	MFPS v;
	MFPS g = half_GCD(a, b, u, v);
	return g == MFPS(1);
}


//【拡張ユークリッドの互除法】O(N^2)（N = min(deg f, deg g)）
/*
* a(x) u(x) + b(x) v(x) = g(x) の解 (u(x), v(x)) を u, v に格納する．
* またモニックな g(x) = gcd(a(x), b(x)) を返す．
*/
MFPS extended_gcd(MFPS a, MFPS b, MFPS& u, MFPS& v) {
	// verify : https://yukicoder.me/problems/no/2579

	a.resize(); b.resize();
	int n = sz(a), m = sz(b);

	if (n == 0 && m == 0) {
		u = MFPS();
		v = MFPS();
		return MFPS();
	}

	stack<MFPS> qs;

	if (n < m) {
		qs.push(MFPS());
		swap(a, b);
		swap(n, m);
	}

	while (m != 0) {
		// どうせ O(deg(a) deg(b)) かかるので素朴に割り算する．
		MFPS q(0, n - m + 1), r(a); mint b_inv = b[m - 1].inv();
		repi(i, 0, n - m) {
			mint c = r[n - 1 - i] * b_inv;
			q[n - m - i] = c;
			rep(j, m) r[n - 1 - i - j] -= b[m - 1 - j] * c;
		}

		qs.push(q);
		r.resize();

		a = move(b);
		b = move(r);
		n = sz(a);
		m = sz(b);
	}

	mint a_inv = a[n - 1].inv();
	u = MFPS(a_inv);
	v = MFPS();
	MFPS g = a * a_inv;

	while (!qs.empty()) {
		swap(u, v);
		v -= qs.top() * u;
		qs.pop();
	}

	return g;
}


//【多項式の求根（一括，mod 998244353）】O(?)（n=4000 くらいまで動く）
/*
* 多項式 f(z) の根のリストを返す．重複度は無視する．
* 
* 利用：【単項式の剰余】,【拡張ユークリッドの互除法】
*/
vm find_all_root(MFPS f) {
	f.resize();

	// f = 0 のときは根なしとする．
	if (sz(f) == 0) return vm();

	vm res;

	// z=0 は例外処理
	if (f[0] == 0) {
		res.push_back(0);

		int d = 0;
		while (d < sz(f) && f[d] == 0) d++;
		f <<= d;
	}

	constexpr int p = 998244353; // p-1 = 7 * 17 * 2^23
	mint r = 3; // mod p での原始根

	// (z-1)(z-2)...(z-(p-1)) = z^(p-1)-1 との GCD をとって余計な因数を除去する．
	MFPS tmp = monomial_power_mod(p - 1, f) - 1;
	MFPS u, v;
	f = extended_gcd(f, tmp, u, v);
	
	const mint R(470355006); // r^(2^23) : 1 の原始 7*17 乗根

	// f(z) と z^((p-1)/2^b) - r^s との共通根を探す．
	function<void(MFPS, int, int)> rf = [&](MFPS f, int b, int s) {
		// 共通根が 0 個の場合は何もせず帰る．
		if (f.deg() == 0) return;

		// 共通根が 1 個の場合は検出して帰る．
		if (f.deg() == 1) {
			res.push_back(-f[0]);
			return;
		}

		// これ以上平方根を取れなくなってしまったら諦めて 7*17 個全探索する．
		if (b == 23) {
			mint x = r.pow(s / (7 * 17));
			rep(hoge, 7 * 17) {
				if (f.assign(x) == 0) {
					res.push_back(x);
					f /= MFPS::SMFPS({ {0, -x}, {1, 1} });
				}
				x *= R;
			}
			return;
		}

		// z^((p-1)/2^b) - r^s を 2 つに分けて再帰する．
		MFPS tmp = monomial_power_mod((p - 1) >> (b + 1), f);

		int ns0 = s / 2;
		MFPS tmp0 = tmp - r.pow(ns0);
		MFPS f0 = extended_gcd(f, tmp0, u, v);
		rf(f0, b + 1, ns0);

		int ns1 = (s + p - 1) / 2;
		MFPS tmp1 = tmp - r.pow(ns1);
		MFPS f1 = extended_gcd(f, tmp1, u, v);
		rf(f1, b + 1, ns1);
	};
	rf(f, 0, p - 1);

	return res;
}


