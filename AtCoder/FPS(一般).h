#pragma once
#include "header.h"
#include "合同式.h"
#include "組合せ論.h"
// ■■■■■ 形式的冪級数（mod 1000000007 等） ■■■■■



//【形式的冪級数（mod 1000000007 等）】
/*
* 積が遅いこと以外は mod 998244353 と同じ
* 
* FPS() : O(1)
*	零多項式 f = 0 で初期化する．
*
* FPS(c0) : O(1)
*	定数多項式 f = c0 で初期化する．
*
* FPS(c0, d) : O(d)
*	d 次未満の項をもつ定数多項式 f = c0 で初期化する．
*
* FPS(c) : O(|c|)
*	f(x) = c[0] + c[1] x + ... + c[n - 1] x^(n-1) で初期化する．
*
* c + f, f + c : O(1)	f + g : O(n)
* f - c : O(1)			c - f, f - g, -f : O(n)
* c * f, f * c : O(n)	f * g : O(n^2)		f * g_sp : O(n k)（k : g の項数）
* f / c : O(n)			f / g : O(n^2)		f / g_sp : O(n k)（k : g の項数）
*	形式的冪級数としての和，差，積，商の結果を返す．
*	g_sp はスパース多項式であり，{次数, 係数} の次数昇順の組の vector で表す．
*	制約 : 商では g(0) ≠ 0
*
* f.inv(d) : O(n^2)
*	1 / f mod x^d を返す．
*	制約 : f(0) ≠ 0
*
* f.quotient(g) : O(n^2)
* f.reminder(g) : O(n^2)
* f.quotient_remainder(g) : O(n^2)
*	多項式としての f を g で割った商，余り，商と余りの組を返す．
*
* f.pow(k, d) : O(n^2)
*	f(x)^k mod x^d を返す．
*
* f.deg(), f.size() : O(1)
*	多項式 f の次数[+1]を返す．
*
* monomial(d) : O(d)
*	単項式 x^d を返す．
*
* f.assign(v) : O(n)
*	多項式 f の不定元 x に v を代入した値を返す．
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
*
* derivative(f) : O(n)
*	f'(x) を返す．
*
* integral(f) : O(n)
*	∫ f(x) dx を返す．（定数項は 0 とする）
*
* log(f, d) : O(n^2)
*	log f(x) mod x^d を返す．
*	制約 : f(0) = 1
*
* exp(f, d) : O(n^2)
*	exp f(x) mod x^d を返す．
*	制約 : f(0) = 0;
*/
struct FPS {
	using vm = vector<mint>;
	using SFPS = vector<pair<int, mint>>;

	int n; // 係数の個数（次数 + 1）
	vm c; // 係数列

	// コンストラクタ（0，定数，係数列で初期化）
	FPS() : n(0) {}
	FPS(const mint& c0) : n(1), c({ c0 }) {}
	FPS(const int& c0) : n(1), c({ mint(c0) }) {}
	FPS(const mint& c0, int d) : n(d), c(n) { c[0] = c0; }
	FPS(const int& c0, int d) : n(d), c(n) { c[0] = c0; }
	FPS(const vm& c_) : n(sz(c_)), c(c_) {}
	FPS(const vi& c_) : n(sz(c_)), c(n) { rep(i, n) c[i] = c_[i]; }

	// 代入
	FPS(const FPS& f) = default;
	FPS& operator=(const FPS& f) = default;
	FPS& operator=(const mint& c0) { n = 1; c = { c0 }; return *this; }

	// アクセス
	mint const& operator[](int i) const { return c[i]; }
	mint& operator[](int i) { return c[i]; }

	// 次数
	int deg() const { return n - 1; }
	int size() const { return n; }

	// 加算
	FPS& operator+=(const FPS& g) {
		if (n >= g.n) rep(i, g.n) c[i] += g.c[i];
		else {
			rep(i, n) c[i] += g.c[i];
			repi(i, n, g.n - 1)	c.push_back(g.c[i]);
			n = g.n;
		}
		return *this;
	}
	FPS operator+(const FPS& g) const { return FPS(*this) += g; }

	// 定数加算
	FPS& operator+=(const mint& sc) {
		if (n == 0) { n = 1; c = { sc }; }
		else { c[0] += sc; }
		return *this;
	}
	FPS operator+(const mint& sc) const { return FPS(*this) += sc; }
	friend FPS operator+(const mint& sc, const FPS& f) { return f + sc; }
	FPS& operator+=(const int& sc) { *this += mint(sc); return *this; }
	FPS operator+(const int& sc) const { return FPS(*this) += sc; }
	friend FPS operator+(const int& sc, const FPS& f) { return f + sc; }

	// 減算
	FPS& operator-=(const FPS& g) {
		if (n >= g.n) rep(i, g.n) c[i] -= g.c[i];
		else {
			rep(i, n) c[i] -= g.c[i];
			repi(i, n, g.n - 1) c.push_back(-g.c[i]);
			n = g.n;
		}
		return *this;
	}
	FPS operator-(const FPS& g) const { return FPS(*this) -= g; }

	// 定数減算
	FPS& operator-=(const mint& sc) { *this += -sc; return *this; }
	FPS operator-(const mint& sc) const { return FPS(*this) -= sc; }
	friend FPS operator-(const mint& sc, const FPS& f) { return -(f - sc); }
	FPS& operator-=(const int& sc) { *this += -sc; return *this; }
	FPS operator-(const int& sc) const { return FPS(*this) -= sc; }
	friend FPS operator-(const int& sc, const FPS& f) { return -(f - sc); }

	// 加法逆元
	FPS operator-() const { return FPS(*this) *= -1; }

	// 定数倍
	FPS& operator*=(const mint& sc) { rep(i, n) c[i] *= sc; return *this; }
	FPS operator*(const mint& sc) const { return FPS(*this) *= sc; }
	friend FPS operator*(const mint& sc, const FPS& f) { return f * sc; }
	FPS& operator*=(const int& sc) { *this *= mint(sc); return *this; }
	FPS operator*(const int& sc) const { return FPS(*this) *= sc; }
	friend FPS operator*(const int& sc, const FPS& f) { return f * sc; }

	// 右からの定数除算
	FPS& operator/=(const mint& sc) { *this *= sc.inv(); return *this; }
	FPS operator/(const mint& sc) const { return FPS(*this) /= sc; }
	FPS& operator/=(const int& sc) { *this /= mint(sc); return *this; }
	FPS operator/(const int& sc) const { return FPS(*this) /= sc; }

	// 積
	FPS& operator*=(const FPS& g) {
		int m = g.deg();
		resize(n + m);

		// 後ろからインライン配る DP
		repir(i, n - 1, 0) {
			// 上位項に係数倍して配っていく．
			repi(j, 1, m) {

				if (i + j >= n) break;

				c[(ll)i + j] += c[i] * g[j];
			}

			// 定数項は最後に配るか消去しないといけない．
			c[i] *= g[0];
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

		FPS g(c[0].inv());
		for (int k = 1; k < d; k *= 2) {
			g = (2 - *this * g) * g;
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

				c[(ll)i + j] += c[i] * gj;
			}

			// 定数項は最後に配るか消去しないといけない．
			c[i] *= g0;
		}

		return *this;
	}
	FPS operator*(const SFPS& g) const { return FPS(*this) *= g; }

	// スパース商
	FPS& operator/=(const SFPS& g) {
		// g の定数項だけ例外処理
		auto it0 = g.begin();
		assert(it0->first == 0 && it0->second != 0);
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

				c[(ll)i + j] -= c[i] * gj;
			}
		}

		return *this;
	}
	FPS operator/(const SFPS& g) const { return FPS(*this) /= g; }

	// 係数反転
	FPS rev() const { FPS h = *this; reverse(all(h.c)); return h; }

	// 不要な高次項の除去
	FPS& resize() {
		// 最高次の係数が非 0 になるまで削る．
		while (n > 0 && c[n - 1LL] == 0) {
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
	mint assign(const mint& x) const {
		mint val = 0;
		repir(i, n - 1, 0) val = val * x + c[i];
		return val;
	}

	// 係数のシフト
	FPS& operator>>=(int d) {
		n += d;
		c.insert(c.begin(), d, 0);
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
		FPS res(1), pow2(f);
		while (d > 0) {
			if (d & 1) res = (res * pow2).reminder(g);
			pow2 = (pow2 * pow2).reminder(g);
			d /= 2;
		}
		return res;
	}

	// 微分
	friend FPS derivative(const FPS& f) {
		FPS res;
		repi(i, 1, f.n - 1) res.c.push_back(f[i] * i);
		res.n = sz(res.c);
		return res;
	}

	// 不定積分
	friend FPS integral(const FPS& f) {
		FPS res(0);
		repi(i, 0, f.n - 1) res.c.push_back(f[i] / (i + 1));
		res.n = sz(res.c);
		return res;
	}

	// 対数関数
	friend FPS log(const FPS& f, int d) {
		// 参考 : https://qiita.com/hotman78/items/f0e6d2265badd84d429a

		return integral((derivative(f) * f.inv(d - 1)).resize(d - 1));
	}

	// 指数関数
	friend FPS exp(const FPS& f, int d) {
		// 参考 : https://qiita.com/hotman78/items/f0e6d2265badd84d429a

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
		FPS g(1);
		for (int k = 1; k < d; k *= 2) {
			g = g * (f + 1 - log(g, 2 * k));
			g.resize(2 * k);
		}

		return g;
	}

	// 累乗
	FPS pow(ll k, int d) const {
		// 参考 : https://qiita.com/hotman78/items/f0e6d2265badd84d429a

		// 最低次の項を見つける．
		int i0 = 0;
		while (i0 < n && c[i0] == 0) i0++;

		// f = 0 なら f^k = 0 である．
		if (i0 == n) return FPS(0, d);

		// 最低次の項の係数を記録する．
		mint c0 = c[i0];

		// 定数項が 1 になるようシフトかつ定数除算した多項式を得る．
		FPS fs = (*this << i0) / c0;
		ll ds = d - k * i0;

		// 最終的に k * i0 次以上の項しか残らないことに注意し，0 になるケースを処理する．
		if (ds <= 0) return FPS(0, d);

		// f^k = exp(k log f(x)) を用いて f^k を計算する．
		FPS gs = exp(mint(k) * log(fs, (int)ds), (int)ds);

		// シフトと定数除算した分を元に戻す．
		FPS g = (gs * c0.pow(k)) >> ((int)k * i0);

		return g;
	}

	// デバッグ出力
	friend ostream& operator<<(ostream& os, const FPS& f) {
		if (f.n == 0) os << 0;
		else {
			rep(i, f.n) {
				os << f.c[i].val() << "x^" << i;
				if (i < f.n - 1) os << " + ";
			}
		}
		return os;
	}
};


