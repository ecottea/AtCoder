#pragma once
#include "header.h"
#include "合同式.h"
#include "二項係数.h"
// ■■■■■ 形式的冪級数 ■■■■■



//【形式的冪級数】
/*
* mod 998244353 以外だと積が遅くなる（O(n^2)）ので注意．
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
* c * f, f * c : O(n)	f * g : O(n log n)		f * g_sp : O(n k)（k : g の項数）
* f / c : O(n)			f / g : O(n log n)		f / g_sp : O(n k)（k : g の項数）
*	形式的冪級数としての和，差，積，商の結果を返す．
*	g_sp はスパース多項式であり，{次数, 係数} の次数昇順の組の vector で表す．
*	制約 : 商では g(0) ≠ 0
*
* f.inv(d) : O(n log n)
*	1 / f mod x^d を返す．
*	制約 : f(0) ≠ 0
*
* f.quotient(g) : O(n log n)
* f.reminder(g) : O(n log n)
* f.quotient_remainder(g) : O(n log n)
*	多項式としての f を g で割った商，余り，商と余りの組を返す．
*
* f.pow(k, d) : O(n log n)
*	f(x)^k mod x^d を返す．
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
* power_mod(f, d, g) : O(m log m log d)　（m = deg g）
*	f(x)^d % g(x) を返す．
*
* derivative(f) : O(n)
*	f'(x) を返す．
*
* integral(f) : O(n)
*	∫ f(x) dx を返す．（定数項は 0 とする）
*
* log(f, d) : O(n log n)
*	log f(x) mod x^d を返す．
*	制約 : f(0) = 1
*
* exp(f, d) : O(n log n)
*	exp f(x) mod x^d を返す．
*	制約 : f(0) = 0;
*/
struct FPS {
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
		c = convolution(c, g.c); n = sz(c); return *this; // mod 998244353 用
//		return mul_other(g);
	}
	FPS& mul_other(const FPS& g) {
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

	// 単項式
	friend FPS monomial(int d) {
		FPS mono(0, d + 1);
		mono[d] = 1;
		return mono;
	}

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
		g.resize(d);

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
				os << f[i].val() << "x^" << i;
				if (i < f.n - 1) os << " + ";
			}
		}
		return os;
	}
};


//【平方根】O(n log n)
/*
* √f(x) mod x^d の 1 つを返す．（なければ find = false を格納する）
*
* 利用：【平方剰余／トネリ－シャンクスのアルゴリズム】
*/
FPS sqrt(const FPS& f, int d, bool& find) {
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
		FPS g;
		g.resize(d);
		return g;
	}

	// 最低次の項が奇数次の項なら平方根は存在しない．
	if (i0 % 2 == 1) {
		find = false;
		return FPS();
	}

	// 最低次の項の係数が平方剰余でなければ平方根は存在しない．
	int c0 = (f[i0] == 1 ? 1 : sqrt(f[i0])); // 1 のことが多いので高速化
	if (c0 == -1) {
		find = false;
		return FPS();
	}

	// 定数項が 0 でないようにシフトした多項式を得る．
	FPS fs = f << i0;

	// ニュートン法で g = √f を見つける．
	FPS gs(c0);
	for (int k = 1; k < d; k *= 2) {
		gs = (gs + fs * gs.inv(2 * k)) / 2;
		gs.resize(2 * k);
	}

	// シフトした分を元に戻す．
	FPS g = gs >> (i0 / 2);
	g.resize(d);

	find = true;
	return g;
}


//【展開係数／ボスタン－森法】O(n log n log d)
/*
* 有理式 f(x) / g(x) を形式的冪級数に展開したときの x^d の係数を返す．
*
* 制約 : deg f < deg g,
*/
mint coef(const FPS& f, const FPS& g, ll d) {
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
		return f[0] / g[0];
	}

	// f2(x) = f(x) g(-x), g2(x) = g(x) g(-x) を求める．
	FPS f2, g2 = g;
	rep(i, g2.n) {
		if (i % 2) {
			g2[i] *= -1;
		}
	}
	f2 = f * g2;
	g2 *= g;

	// f3(x) = E(x) or O(x), g3(x) = e(x) を求める．
	FPS f3, g3;
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


//【線形漸化式】O(d log d log n)
/*
* 初項 a[0..d) と漸化式 a[i] = Σj=[0..d) c[j]a[i-1-j] で定義される
* 数列 a について，a[n] の値を返す．
*
* 利用：【展開係数／ボスタン－森法】
*/
mint linearly_recurrent_sequence(const vm& a, const vm& c, ll n) {
	int d = sz(a);

	FPS A(a), C(c);
	FPS Dnm = 1 - (C >> 1);
	FPS Num = (Dnm * A).resize(d);
	return coef(Num, Dnm, n);
}


//【平行移動】O(n log n)
/*
* f(x + c) を返す．
*
* 制約 : fm は def(f) までの階乗計算が可能であること．
*
* 利用：【階乗と二項係数（mint利用）】
*/
FPS taylor_shift(const FPS& f, mint c, const factorial_mint& fm) {
	// 参考 : https://nyaannyaan.github.io/library/fps/taylor-shift.hpp.html

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

	FPS g(1);
	g.resize(n);
	repi(i, 1, n - 1) g[i] = g[i - 1] * c * fm.inv(i);

	FPS h(f);
	rep(i, n) h[i] *= fm.fac(i);
	h = h.rev();

	FPS fs = (g * h).resize(n);
	fs = fs.rev();
	rep(i, n) fs[i] *= fm.fac_inv(i);

	return fs;
}


//【一次式の積の展開】O(n (log n)^2)
/*
* (x - x[0]) ... (x - x[n-1]) を返す．
*/
FPS expand(const vm& x) {
	int n = sz(x);

	vector<FPS> f(n);
	rep(i, n) f[i] = FPS(vm({ -x[i], 1 }));

	// 2 冪個ずつ掛けていく（分割統治法）
	for (int k = 1; k < n; k *= 2) {
		for (int i = 0; i + k < n; i += 2 * k) {
			f[i] *= f[(ll)i + k];
		}
	}

	return f[0];
}


//【多点評価】O(m (log m)^2 + n log n)
/*
* n 次多項式 f について，f(x[0..m)) の値を y[0..m) に格納する．
*/
void multipoint_evaluation(const FPS& f, const vm& x, vm& y) {
	// 参考 : https://37zigen.com/multipoint-evaluation/

	int m = sz(x);
	y = vm(m);
	int m2 = 1 << (msb(m - 1) + 1);

	// sp : (x - x[i]) の連続する 2 冪個の積からなる完全二分木
	vector<FPS> sp(m2 * 2LL);
	repi(i, m2, m2 + m - 1) sp[i] = FPS(vm({ -x[(ll)i - m2], 1 }));
	repi(i, m2 + m, 2 * m2 - 1) sp[i] = FPS(1);
	repir(i, m2 - 1, 1) sp[i] = sp[2LL * i] * sp[2LL * i + 1];

	// sr : f を sp[i] で割った余りからなる完全二分木
	vector<FPS> sr(m2 * 2LL);
	sr[1] = f.reminder(sp[1]);
	repi(i, 2, m2 + m - 1) sr[i] = sr[i / 2LL].reminder(sp[i]);

	// sr の葉は (x - x[i]) で割った余りなので，因数定理よりこれが f(x[i]) に等しい．
	rep(i, m) y[i] = sr[(ll)m2 + i][0];
}


//【ラグランジュ補間（一点評価）】O(n)
/*
* i=[0..n) について f(a i + b) = y[i] を満たす n - 1 次多項式 f について f(c) を返す．
*
* 利用：【階乗と二項係数（mint利用）】
*/
mint lagrange_interpolation(int a, int b, const vm& y, mint c) {
	// 参考 : https://37zigen.com/lagrange-interpolation/

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
	repi(i, 1, n - 1) acc_l[i] = acc_l[i - 1LL] * (c - (mint(a) * (i - 1) - b));

	// acc_r[i] = (c - x[i + 1]) ... (c - x[n - 2])(c - x[n - 1])
	vm acc_r(n);
	acc_r[n - 1LL] = 1;
	repir(i, n - 2, 0) acc_r[i] = (c - (mint(a) * (i + 1) - b)) * acc_r[i + 1LL];

	// ラグランジュ基底の線形結合を計算する．
	factorial_mint fm(n);
	mint res = 0;
	rep(i, n) {
		res += y[i] * acc_l[i] * acc_r[i] * ((n - 1 - i) & 1 ? -1 : 1)
			* fm.fac_inv(i) * fm.fac_inv(n - 1 - i);
	}
	return res * mint(a).pow(n - 1LL);
}


//【ラグランジュ補間（多項式復元）】O(n (log n)^2)
/*
* n 点での値 f(x[i]) = y[i] から定まる n - 1 次多項式 f(x) を返す．
*
* 利用：【一次式の積の展開】，【多点評価】
*/
FPS lagrange_interpolation(const vm& x, const vm& y) {
	// 参考 : https://37zigen.com/lagrange-interpolation/

	//【方法】
	// ラグランジュ補間の通常の式は，基底関数の線形和の形をした
	//		f(x) = Σi=[0..n) y[i] Πj≠i (x - x[j])/(x[i] - x[j])
	// である．
	// 
	// ここで
	//		g(x) = Πi=[0..n) (x - x[i])
	// とおくと，f(x) は
	//		f(x) = g(x) Σi=[0..n) y[i] / (g'(x[i]) (x - x[i]))
	// とも表される．
	//
	// g(x) は一次式の積の展開なので分割統治で O(n (log n)^2) で計算でき，
	// g'(x[i]) らは多点評価を用いて O(n (log n)^2) で計算できる．
	// よって
	//		a[i] = y[i] / g'(x[i])
	// とおけば，後は
	//		Σi=[0..n) a[i] / (x - x[i])
	// を O(n (log n)^2) で計算できればよいが，
	// これは有理式として分母分子を持ちながら分割統治で通分すればよい．

	int n = sz(x);

	FPS g = expand(x);
	g = derivative(g);
	vm b;
	multipoint_evaluation(g, x, b);

	vector<FPS> num(n), dnm(n);
	rep(i, n) {
		num[i] = FPS(y[i] / b[i]);
		dnm[i] = FPS(vm({ -x[i], 1 }));
	}

	// 2 冪個ずつ足していく（分割統治法）
	for (int k = 1; k < n; k *= 2) {
		for (int i = 0; i + k < n; i += 2 * k) {
			num[i] = num[i] * dnm[(ll)i + k] + num[(ll)i + k] * dnm[i];
			dnm[i] *= dnm[(ll)i + k];
		}
	}

	return num[0];
}


//【下降階乗冪（第 1 種スターリング数）】O(n log n)
/*
* x(x-1)(x-2)...(x-(n-1)) を返す（係数は第 1 種スターリング数）
*
* 制約 : fm は n 以上の最小の 2 冪までの階乗計算が可能であること（2 n で良い）
*
* 利用：【階乗と二項係数（mint利用）】，【平行移動】
*/
FPS falling_factorial(int n, const factorial_mint& fm) {
	//【方法】
	// 累乗をダブリングで計算するのと同様．
	// ただし同じものを掛けるのではなく平行移動したものを掛ける．

	FPS f(vm({ 0, 1 })), res(1);

	while (n > 0) {
		if (n & 1) res = taylor_shift(res, -f.deg(), fm) * f;
		f *= taylor_shift(f, -f.deg(), fm);
		n /= 2;
	}

	return res;
}


