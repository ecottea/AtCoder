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
* c * f, f * c : O(n)	f * g : O(n log n)		f * g_sp : O(n k)（k : g の項数）
* f / c : O(n)			f / g : O(n log n)		f / g_sp : O(n k)（k : g の項数）
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
* MFPS::monomial(int d) : O(d)
*	単項式 z^d を返す．
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
* MFPS power_mod(MFPS f, ll d, MFPS g) : O(m log m log d)　（m = deg g）
*	f(z)^d mod g(z) を返す．
*/
struct MFPS {
	using SMFPS = vector<pair<int, mint>>;

	int n; // 係数の個数（次数 + 1）
	vm c; // 係数列
	inline static vm(*CONV)(const vm&, const vm&) = convolution; // 畳込み用の関数

	// コンストラクタ（0，定数，係数列で初期化）
	MFPS() : n(0) {}
	MFPS(const mint& c0) : n(1), c({ c0 }) {}
	MFPS(const int& c0) : n(1), c({ mint(c0) }) {}
	MFPS(const mint& c0, int d) : n(d), c(n) { c[0] = c0; }
	MFPS(const int& c0, int d) : n(d), c(n) { c[0] = c0; }
	MFPS(const vm& c_) : n(sz(c_)), c(c_) {}
	MFPS(const vi& c_) : n(sz(c_)), c(n) { rep(i, n) c[i] = c_[i]; }

	// 代入
	MFPS(const MFPS& f) = default;
	MFPS& operator=(const MFPS& f) = default;
	MFPS& operator=(const mint& c0) { n = 1; c = { c0 }; return *this; }

	// 比較
	bool operator==(const MFPS& g) const { return c == g.c; }
	bool operator!=(const MFPS& g) const { return c != g.c; }

	// アクセス
	mint const& operator[](int i) const { return c[i]; }
	mint& operator[](int i) { return c[i]; }

	// 次数
	int deg() const { return n - 1; }
	int size() const { return n; }

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
	MFPS operator+(const MFPS& g) const { return MFPS(*this) += g; }

	// 定数加算
	MFPS& operator+=(const mint& sc) {
		if (n == 0) { n = 1; c = { sc }; }
		else { c[0] += sc; }
		return *this;
	}
	MFPS operator+(const mint& sc) const { return MFPS(*this) += sc; }
	friend MFPS operator+(const mint& sc, const MFPS& f) { return f + sc; }
	MFPS& operator+=(const int& sc) { *this += mint(sc); return *this; }
	MFPS operator+(const int& sc) const { return MFPS(*this) += sc; }
	friend MFPS operator+(const int& sc, const MFPS& f) { return f + sc; }

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
	MFPS operator-(const MFPS& g) const { return MFPS(*this) -= g; }

	// 定数減算
	MFPS& operator-=(const mint& sc) { *this += -sc; return *this; }
	MFPS operator-(const mint& sc) const { return MFPS(*this) -= sc; }
	friend MFPS operator-(const mint& sc, const MFPS& f) { return -(f - sc); }
	MFPS& operator-=(const int& sc) { *this += -sc; return *this; }
	MFPS operator-(const int& sc) const { return MFPS(*this) -= sc; }
	friend MFPS operator-(const int& sc, const MFPS& f) { return -(f - sc); }

	// 加法逆元
	MFPS operator-() const { return MFPS(*this) *= -1; }

	// 定数倍
	MFPS& operator*=(const mint& sc) { rep(i, n) c[i] *= sc; return *this; }
	MFPS operator*(const mint& sc) const { return MFPS(*this) *= sc; }
	friend MFPS operator*(const mint& sc, const MFPS& f) { return f * sc; }
	MFPS& operator*=(const int& sc) { *this *= mint(sc); return *this; }
	MFPS operator*(const int& sc) const { return MFPS(*this) *= sc; }
	friend MFPS operator*(const int& sc, const MFPS& f) { return f * sc; }

	// 右からの定数除算
	MFPS& operator/=(const mint& sc) { *this *= sc.inv(); return *this; }
	MFPS operator/(const mint& sc) const { return MFPS(*this) /= sc; }
	MFPS& operator/=(const int& sc) { *this /= mint(sc); return *this; }
	MFPS operator/(const int& sc) const { return MFPS(*this) /= sc; }

	// 積
	MFPS& operator*=(const MFPS& g) { c = CONV(c, g.c); n = sz(c); return *this; }
	MFPS operator*(const MFPS& g) const { return MFPS(*this) *= g; }

	// 除算
	MFPS inv(int d) const {
		// 参考：https://nyaannyaan.github.io/library/fps/formal-power-series.hpp
		// verify : https://judge.yosupo.jp/problem/inv_of_formal_power_series

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

		Assert(!c.empty());
		Assert(c[0] != 0);

		MFPS g(c[0].inv());
		for (int k = 1; k < d; k *= 2) {
			g = (2 - *this * g) * g;
			g.resize(2 * k);
		}

		return g.resize(d);
	}
	MFPS& operator/=(const MFPS& g) { return *this *= g.inv(max(n, g.n)); }
	MFPS operator/(const MFPS& g) const { return MFPS(*this) /= g; }

	// 余り付き除算
	MFPS quotient(const MFPS& g) const {
		// 参考 : https://nyaannyaan.github.io/library/fps/formal-power-series.hpp
		// verify : https://judge.yosupo.jp/problem/division_of_polynomials

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

		if (n < g.n) return MFPS();
		return ((this->rev() / g.rev()).resize(n - g.n + 1)).rev();
	}

	MFPS reminder(const MFPS& g) const {
		// verify : https://judge.yosupo.jp/problem/division_of_polynomials

		return (*this - this->quotient(g) * g).resize(g.n - 1);
	}

	pair<MFPS, MFPS> quotient_remainder(const MFPS& g) const {
		// verify : https://judge.yosupo.jp/problem/division_of_polynomials

		pair<MFPS, MFPS> res;
		res.first = this->quotient(g);
		res.second = (*this - res.first * g).resize(g.n - 1);
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
				int j; mint gj;
				tie(j, gj) = *it;

				if (i + j >= n) break;

				c[i + j] += c[i] * gj;
			}

			// 定数項は最後に配るか消去しないといけない．
			c[i] *= g0;
		}

		return *this;
	}
	MFPS operator*(const SMFPS& g) const { return MFPS(*this) *= g; }

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
				int j; mint gj;
				tie(j, gj) = *it;

				if (i + j >= n) break;

				c[i + j] -= c[i] * gj;
			}
		}

		return *this;
	}
	MFPS operator/(const SMFPS& g) const { return MFPS(*this) /= g; }

	// 係数反転
	MFPS rev() const { MFPS h = *this; reverse(all(h.c)); return h; }

	// 単項式
	static MFPS monomial(int d) {
		MFPS mono(0, d + 1);
		mono[d] = 1;
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
	mint assign(const mint& x) const {
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
	MFPS operator>>(int d) const { return MFPS(*this) >>= d; }
	MFPS operator<<(int d) const { return MFPS(*this) <<= d; }

	// 累乗の剰余
	friend MFPS power_mod(const MFPS& f, ll d, const MFPS& g) {
		MFPS res(1), pow2(f);
		while (d > 0) {
			if (d & 1LL) res = (res * pow2).reminder(g);
			pow2 = (pow2 * pow2).reminder(g);
			d /= 2;
		}
		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const MFPS& f) {
		if (f.n == 0) os << 0;
		else {
			rep(i, f.n) {
				os << f[i].val() << "z^" << i;
				if (i < f.n - 1) os << " + ";
			}
		}
		return os;
	}
#endif
};


//【微分】O(n)
/*
* f'(x) を返す．
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
* ∫ f(x) dx を返す．（定数項は 0 とする）
*
* 制約：fm は (deg(f) + 1)! まで計算可能であること
*
* 利用：【階乗など（法が大きな素数）】
*/
MFPS integral(const MFPS& f, const Factorial_mint& fm) {
	// verify : https://judge.yosupo.jp/problem/log_of_formal_power_series

	MFPS res(0);
	repi(i, 0, f.n - 1) res.c.push_back(f[i] * fm.inv(i + 1));
	res.n = sz(res.c);
	return res;
}


//【対数関数】O(n log n)
/*
* log f(x) mod x^d を返す．
*
* 制約 : f(0) = 1，fm は d! まで計算可能であること
*
* 利用：【微分】,【不定積分】,【階乗など（法が大きな素数）】
*/
MFPS log(const MFPS& f, int d, const Factorial_mint& fm) {
	// 参考 : https://qiita.com/hotman78/items/f0e6d2265badd84d429a
	// verify : https://judge.yosupo.jp/problem/log_of_formal_power_series

	return integral((derivative(f) * f.inv(d - 1)).resize(d - 1), fm);
}


//【指数関数】O(n log n)
/*
* exp f(x) mod x^d を返す．
*
* 制約 : f(0) = 0，fm は (2d)! まで計算可能であること
*
* 利用：【対数関数】,【階乗など（法が大きな素数）】
*/
MFPS exp(const MFPS& f, int d, const Factorial_mint& fm) {
	// 参考 : https://qiita.com/hotman78/items/f0e6d2265badd84d429a
	// verify : https://judge.yosupo.jp/problem/exp_of_formal_power_series

	//【方法】
	// g(x) = exp(f(x)) とおき，方程式
	//		log g(x) = f(x)
	// に対してニュートン法を用いる．
	// 
	// f(0) = 0 なので，mod x^1 では
	//		log(1) ≡ f(x) mod x^1
	// が成り立つ．
	//
	// mod x^k で
	//		log h(x) ≡ f(x) mod x^k
	// が成り立っていると仮定すると，ニュートン法より
	//		g = h - (log h - f) / (log h)'
	//   ⇔ g = h (f + 1 - log h)
	// と置くと
	//		log g(x) ≡ f(x) mod x^(2 k)
	// が成り立つ．
	//
	// これを繰り返せば所望の g が求まる．

	// ニュートン法で log g = f なる g を見つける．
	MFPS g(1);
	for (int k = 1; k < d; k *= 2) {
		g = g * (f + 1 - log(g, 2 * k, fm));
		g.resize(2 * k);
	}
	g.resize(d);

	return g;
}


//【累乗】O(n log n)
/*
* f(x)^k mod x^d を返す．（0^0 = 1 とする）
*
* 制約 : fm は (2d)! まで計算可能であること
*
* 利用：【指数関数】,【対数関数】,【階乗など（法が大きな素数）】
*/
MFPS pow(const MFPS& f, ll k, int d, const Factorial_mint& fm) {
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
	MFPS gs = exp(mint(k) * log(fs, ds, fm), ds, fm);

	// シフトと定数除算した分を元に戻す．
	MFPS g = (gs * c0.pow(k)) >> (int)(k * i0);

	return g;
}


//【平方根】O(n log n)
/*
* √f(x) mod x^d の 1 つを返す．（なければ find = false を格納する）
*
* 利用：【平方剰余】
*/
MFPS sqrt(const MFPS& f, int d, bool& find) {
	// 参考 : https://nyaannyaan.github.io/library/fps/fps-sqrt.hpp
	// verify : https://judge.yosupo.jp/problem/sqrt_of_formal_power_series

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
		MFPS g;
		g.resize(d);
		return g;
	}

	// 最低次の項が奇数次の項なら平方根は存在しない．
	if (i0 % 2 == 1) {
		find = false;
		return MFPS();
	}

	// 最低次の項の係数が平方剰余でなければ平方根は存在しない．
	int c0 = (f[i0] == 1 ? 1 : tonelli_shanks(f[i0])); // 1 のことが多いので高速化
	if (c0 == -1) {
		find = false;
		return MFPS();
	}

	// 定数項が 0 でないようにシフトした多項式を得る．
	MFPS fs = f << i0;

	// ニュートン法で g = √f を見つける．
	MFPS gs(c0);
	for (int k = 1; k < d; k *= 2) {
		gs = (gs + fs * gs.inv(2 * k)) / 2;
		gs.resize(2 * k);
	}

	// シフトした分を元に戻す．
	MFPS g = gs >> (i0 / 2);
	g.resize(d);

	find = true;
	return g;
}


//【展開係数】O(n log n log d)
/*
* 有理式 f(x)/g(x) を形式的冪級数に展開したときの x^d の係数を返す．
*
* 制約 : deg f < deg g, g[0] != 0
*/
mint bostan_mori(const MFPS& f, const MFPS& g, ll d) {
	// 参考 : http://q.c.titech.ac.jp/docs/progs/polynomial_division.html
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_fibonacci

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

	Assert(g.n >= 1 && g[0] != 0);

	// d = 0 のときは定数項を返す．
	if (d == 0) return f[0] / g[0];

	// f2(x) = f(x) g(-x), g2(x) = g(x) g(-x) を求める．
	MFPS f2, g2 = g;
	rep(i, g2.n) if (i % 2 == 1) g2[i] *= -1;
	f2 = f * g2;
	g2 *= g;

	// f3(x) = E(x) or O(x), g3(x) = e(x) を求める．
	MFPS f3, g3;
	if (d % 2 == 0) rep(i, (f2.n + 1) / 2) f3.c.push_back(f2[2 * i]);
	else rep(i, f2.n / 2) f3.c.push_back(f2[2 * i + 1]);
	f3.n = sz(f3.c);
	rep(i, g.n) g3.c.push_back(g2[2 * i]);
	g3.n = sz(g3.c);

	// d を半分にして再帰を回す．
	return bostan_mori(f3, g3, d / 2);
}


//【展開係数（分子がスパース）】O(n m log m log d)（n : f の項数，m : deg g）
/*
* 有理式 f(x)/g(x) を形式的冪級数に展開したときの x^d の係数を返す．
*
* 制約 : g[0] != 0
*
* 利用：【展開係数】
*/
mint bostan_mori(const SPoly<mint>& f, const MFPS& g, ll d) {
	// verify : https://atcoder.jp/contests/abc241/tasks/abc241_h

	//【方法】
	// 分子の c x^k という項からの [x^d](f / g) への寄与は
	//		c [x^(d - k)] (1 / g)
	// である．これを分子の全ての項について足し合わせる．
	mint res = 0;

	repe(p, f.c) {
		ll fd; mint fc;
		tie(fd, fc) = p;

		if (d - fd < 0) continue;

		res += fc * bostan_mori(MFPS(1), g, d - fd);
	}

	return res;
}


//【展開係数（分母が二項式の積）】O(deg(g) m log d)
/*
* f(z) = Σi∈[0..n) f[i] z^i，g(z) = Πj∈[0..m) (1 + c[j] z^d[j]) とし，
* 有理式 f(z)/g(z) を形式的冪級数に展開したときの z^d の係数を返す．
*
* 制約 : deg f < deg g
*/
mint bostan_mori(vm f, vector<pair<int, mint>> dcs, ll d) {
	// verify : https://yukicoder.me/problems/no/137

	// d = 0 のときは定数項を返す．
	if (d == 0) return f[0];

	int n = sz(f), m = sz(dcs);

	// g(z) の奇多項式因子で z ← -z としたものを分母分子に掛け，分母は z ← z^2 としておく．
	for (auto& [d, c] : dcs) {
		f.resize(sz(f) + d);
		repir(i, sz(f) - 1, d) f[i] -= f[i - d] * c;
		c *= -c;
	}

	// d の偶奇に応じて f の偶[奇]多項式部分を取り出す．
	vm f2;
	if (d % 2 == 0) rep(i, (sz(f) + 1) / 2) f2.push_back(f[2 * i]);
	else rep(i, sz(f) / 2) f2.push_back(f[2 * i + 1]);

	// d を半分にして再帰を回す．
	return bostan_mori(f2, dcs, d / 2);
}


//【展開係数（間引き和）】O(n log n log(k d))
/*
* 有理式 f(x)/g(x) を形式的冪級数に展開したときの 1, x^d, x^2d, ..., x^(k-1)d の係数の和を返す．
*
* 制約 : deg f < deg g, g[0] != 0
* 
*（高速きたまさ法）
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
	rep(i, n - 1) res += a[i] * p2[i];

	return res;
}


//【線形漸化式】O(d log d log n)
/*
* 初項 a[0..d) と漸化式 a[i] = Σj=[0..d) c[j]a[i-1-j] で定義される
* 数列 a について，a[n] の値を返す．
*
* 利用：【展開係数】
*/
mint linearly_recurrent_sequence(const vm& a, const vm& c, ll n) {
	// verify : https://judge.yosupo.jp/problem/kth_term_of_linearly_recurrent_sequence

	int d = sz(a);
	if (d == 0) return 0;

	MFPS A(a), C(c);
	MFPS Dnm = 1 - (C >> 1);
	MFPS Num = (Dnm * A).resize(d);
	return bostan_mori(Num, Dnm, n);
}


//【線形漸化式の発見】O(n^2)
/*
* 与えられた数列 a[0..n) に対し，以下の等式を満たす c[0..d) で d を最小とするものを返す：
*		a[i] = Σj=[0..d) c[j] a[i-1-j]  (∀i∈[d..n))
*/
vm berlekamp_massey(const vm& a) {
	// 参考 : https://en.wikipedia.org/wiki/Berlekamp%E2%80%93Massey_algorithm
	// verify : https://judge.yosupo.jp/problem/find_linear_recurrence

	MFPS S(a), C(1), B(1);
	int N = sz(a), m = 1; mint b = 1;

	rep(n, N) {
		mint d = 0;
		rep(i, sz(C)) d += C[i] * S[n - i];

		if (d == 0) {
			m++;
		}
		else if (2 * C.deg() <= n) {
			MFPS T(C);
			C -= d * b.inv() * (B >> m);
			B = T;
			b = d;
			m = 1;
		}
		else {
			C -= d * b.inv() * (B >> m);
			m++;
		}
	}

	return (-C << 1).c;
}


//【平行移動】O(n log n)
/*
* f(x + c) を返す．
*
* 制約 : fm は deg(f) までの階乗計算が可能であること．
*
* 利用：【階乗など（法が大きな素数）】
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
	//	= Σr=[0..N] x^r / r! Σm=[0..N-r] (c^(N-m-r) / (N-m-r)!) (N-m)! f[N-m]　（m = N - n）
	//	= Σj=[0..N] x^(N-j) / (N-j)! Σm=[0..j] (c^(j-m) / (j-m)!) (N-m)! f[N-m]　（j = N - r）
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


//【一次式の積の展開（基本対称式）】O(n (log n)^2)
/*
* Πi∈[0..n) (z - x[i]) を返す．
* 
* 戻り値の i 次の項の係数は，x[0..n) の符号付き n - i 次基本対称式になる．
*/
MFPS expand(const vm& x) {
	// verify : https://atcoder.jp/contests/abc231/tasks/abc231_g

	int n = sz(x);

	vector<MFPS> f(n);
	rep(i, n) f[i] = MFPS(vm({ -x[i], 1 }));

	// 2 冪個ずつ掛けていく（分割統治法）
	for (int k = 1; k < n; k *= 2) {
		for (int i = 0; i + k < n; i += 2 * k) {
			f[i] *= f[i + k];
		}
	}

	return f[0];
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
		int di, i, dj, j;
		tie(di, i) = q.top(); q.pop();
		tie(dj, j) = q.top(); q.pop();

		fs[i] *= fs[j];
		q.push({ di + dj, i });
	}

	return fs[q.top().second];
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
* 与えられた多項式の列 as[0..k] fs[0..k) に対して Σi=[0..k] as[i] Πfs[0..i)（次数は n）を返す．
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

	int n = sz(fs);

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


//【多点評価】O(m (log m)^2 + n log n)
/*
* n 次多項式 f について，f(x[0..m)) の値を並べたリストを返す．
*/
vm multipoint_evaluation(const MFPS& f, const vm& x) {
	// 参考 : https://37zigen.com/multipoint-evaluation/
	// verify : https://judge.yosupo.jp/problem/multipoint_evaluation

	int m = sz(x);
	vm y(m);
	int m2 = 1 << (msb(m - 1) + 1);

	// sp : (x - x[i]) の連続する 2 冪個の積からなる完全二分木
	vector<MFPS> sp(m2 * 2);
	repi(i, m2, m2 + m - 1) sp[i] = MFPS(vm({ -x[i - m2], 1 }));
	repi(i, m2 + m, 2 * m2 - 1) sp[i] = MFPS(1);
	repir(i, m2 - 1, 1) sp[i] = sp[2 * i] * sp[2 * i + 1];

	// sr : f を sp[i] で割った余りからなる完全二分木
	vector<MFPS> sr(m2 * 2);
	sr[1] = f.reminder(sp[1]);
	repi(i, 2, m2 + m - 1) sr[i] = sr[i / 2].reminder(sp[i]);

	// sr の葉は (x - x[i]) で割った余りなので，因数定理よりこれが f(x[i]) に等しい．
	rep(i, m) y[i] = sr[m2 + i][0];

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
	//		f(r^j)
	//		= Σi∈[0..n) f[i] (a r^j)^i
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


//【ラグランジュ補間（一点評価）】O(n)
/*
* 各 i∈[0..n) について f(a i + b) = y[i] を満たす n - 1 次多項式 f についての f(c) を返す．
*
* 利用：【階乗など（法が大きな素数）】
*/
mint lagrange_interpolation(int a, int b, const vm& y, mint c) {
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
	Factorial_mint fm(n);
	mint res = 0;
	rep(i, n) {
		res += y[i] * acc_l[i] * acc_r[i] * ((n - 1 - i) & 1 ? -1 : 1)
			* fm.fact_inv(i) * fm.fact_inv(n - 1 - i);
	}
	return res * mint(a).pow(n - 1);
}


//【ラグランジュ補間（多項式復元）】O(n (log n)^2)
/*
* n 点での値 f(x[i]) = y[i] から定まる n - 1 次多項式 f(x) を返す．
*
* 利用：【微分】,【一次式の積の展開】,【多点評価】,【有理式の通分】
*/
MFPS lagrange_interpolation(const vm& x, const vm& y) {
	// 参考 : https://37zigen.com/lagrange-interpolation/
	// verify : https://judge.yosupo.jp/problem/polynomial_interpolation

	//【方法】
	// ラグランジュ補間の通常の式は，基底関数の線形和の形をした
	//		f(x) = Σi=[0..n) y[i] Πj≠i (x - x[j])/(x[i] - x[j])
	// である．
	// 
	// ここで
	//		g(x) = Πi=[0..n) (x - x[i])
	// とおくと，f(x) は
	//		f(x) = g(x) Σi=[0..n) y[i] / ( g'(x[i]) (x - x[i]) )
	// とも表される．
	//
	// g(x) は一次式の積の展開なので分割統治で O(n (log n)^2) で計算でき，
	// g'(x[i]) らは多点評価を用いて O(n (log n)^2) で計算できる．
	// よって
	//		a[i] = y[i] / g'(x[i])
	// とおけば，後は
	//		f(x) / g(x) = Σi=[0..n) a[i] / (x - x[i])
	// を計算できればよく，これも分割統治で通分すれば O(n (log n)^2) で計算できる．

	int n = sz(x);

	MFPS g = expand(x);
	g = derivative(g);
	vm b = multipoint_evaluation(g, x);

	vector<MFPS> num(n), dnm(n);
	rep(i, n) {
		num[i] = MFPS(y[i] / b[i]);
		dnm[i] = MFPS(vm({ -x[i], 1 }));
	}

	return reduction(num, dnm).first;
}


//【下降階乗冪（符号付き第 1 種スターリング数）】O(n log n)
/*
* x(x-1)(x-2)...(x-(n-1)) を返す（係数は符号付き第 1 種スターリング数）
*
* 制約 : fm は n 以上の最小の 2 冪までの階乗計算が可能であること（2n で良い）
*
* 利用：【階乗など（法が大きな素数）】,【平行移動】
*/
MFPS falling_factorial(int n, const Factorial_mint& fm) {
	// verify : https://judge.yosupo.jp/problem/stirling_number_of_the_first_kind

	//【方法】
	// 累乗をダブリングで計算するのと同様．
	// ただし同じものを掛けるのではなく平行移動したものを掛ける．

	MFPS f(vm({ 0, 1 })), res(1);

	while (n > 0) {
		if (n & 1) res = taylor_shift(res, -f.deg(), fm) * f;
		f *= taylor_shift(f, -f.deg(), fm);
		n /= 2;
	}

	return res;
}


//【拡張ユークリッドの互除法】O(deg(a) deg(b)) (?) // TODO：遅いので作り直す
/*
* a(x) u(x) + b(x) v(x) = g(x) の解 (u(x), v(x)) を u, v に格納する．
* またモニックな g(x) = gcd(a(x), b(x)) を返す．
*/
MFPS extended_gcd(MFPS a, MFPS b, MFPS& u, MFPS& v) {
	b.resize();
	if (sz(b) == 0) {
		u = MFPS(a[a.deg()].inv());
		v = MFPS();
		a /= a[a.deg()];
		return a;
	}

	MFPS d = extended_gcd(b, a.reminder(b), v, u);
	v -= a.quotient(b) * u;
	return d;
}


//【多項式逆元】O(deg(a) deg(b)) (?) // TODO：遅いので作り直す
/*
* a(x) u(x) = 1 (mod b(x)) を満たす u(x) を格納する．（なければ false を返す）
*
* 利用：【拡張ユークリッドの互除法】
*/
bool polynomial_inverse(const MFPS& a, const MFPS& b, MFPS& u) {
	MFPS v;
	MFPS g = extended_gcd(a, b, u, v);
	return g == MFPS(1);
}


