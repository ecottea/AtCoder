#pragma once
#include "header.h"
#include "二項係数.h"
#include "合同式.h"
// ■■■■■ 形式的冪級数（スパース） ■■■■■


//【形式的冪級数（スパース）】
/*
* SMFPS() : O(1)
*	零多項式 f(z) = 0 で初期化する．
*
* SMFPS(mint c0) : O(1)
*	定数多項式 f(z) = c0 で初期化する．
*
* SMFPS(vector<pim> dcs) : O(n)
*	次数について昇順に並んだ n 個の (次数, 係数) の組で初期化する．
*
* c + f, f + c : O(|f|),	f + g : O(|f| + |g|)
* f - c, c - f : O(|f|),	f - g : O(|f| + |g|)
* c * f, f * c, -f : O(|f|)
*	和，差，定数倍の結果を返す．
*
* f * g : O(|f| |g| log(|f| |g|))
*	積を返す．
*
* f >> d, f << d : O(|f|)
*	係数列を d だけ右[左]シフトした多項式を返す．
*  （右シフトは z^d の乗算，左シフトは z^d で割った商と等価）
*
* shrink() : O(|f|)
*	次数が同じ項をまとめ，係数が 0 の項を削除する．
*/
struct SMFPS {
	// 非 0 係数の個数
	int n;

	// (次数, 係数) の組（次数について昇順）
	vector<pim> c;

	// 初期化
	SMFPS() : n(0) {}
	SMFPS(mint c0) : n(1), c({ { 0, c0 } }) {}
	SMFPS(const vector<pim>& c) : n(sz(c)), c(c) {}

	// 代入
	SMFPS(const SMFPS& f) = default;
	SMFPS& operator=(const SMFPS& f) = default;

	void puch_back(const pim& dc) { c.emplace_back(dc); ++n; }
	void puch_back(int deg, mint coef) { c.emplace_back(deg, coef); ++n; }
	void pop_back() { c.pop_back(); --n; }
	[[nodiscard]] pim back() { return c.back(); }

	// 比較
	[[nodiscard]] bool operator==(const SMFPS& g) const { return c == g.c; }
	[[nodiscard]] bool operator!=(const SMFPS& g) const { return !(*this == g); }

	// アクセス
	inline pim const& operator[](int i) const { return c[i]; }
	inline pim& operator[](int i) { return c[i]; }

	// 項数
	[[nodiscard]] int size() const { return n; }

	// 加算
	[[nodiscard]] SMFPS operator+(const SMFPS& g) const {
		// verify : https://yukicoder.me/problems/no/2660

		SMFPS res;
		int i = 0, j = 0;
		while (i < n || j < g.n) {
			if (j == g.n || (i < n && c[i].first < g[j].first)) {
				res.puch_back(c[i]);
				++i;
			}
			else if (i == n || c[i].first > g[j].first) {
				res.puch_back(g[j]);
				++j;
			}
			else {
				res.puch_back(c[i].first, c[i].second + g[j].second);
				++i; ++j;
			}
		}

		return res;
	}
	SMFPS& operator+=(const SMFPS& g) { *this = *this + g; return *this; }

	// 減算
	[[nodiscard]] SMFPS operator-(const SMFPS& g) const {
		SMFPS res;
		int i = 0, j = 0;
		while (i < n || j < g.n) {
			if (j == g.n || (i < n && c[i].first < g[j].first)) {
				res.puch_back(c[i]);
				++i;
			}
			else if (i == n || c[i].first > g[j].first) {
				res.puch_back(g[j].first, -g[j].second);
				++j;
			}
			else {
				res.puch_back(c[i].first, c[i].second - g[j].second);
				++i; ++j;
			}
		}

		return res;
	}
	SMFPS& operator-=(const SMFPS& g) { *this = *this - g; return *this; }

	// 定数倍
	SMFPS& operator*=(const mint& c0) {
		// verify : https://judge.yosupo.jp/problem/sqrt_of_formal_power_series_sparse

		repea(p, c) p.second *= c0; return *this;
	}
	[[nodiscard]] SMFPS operator*(const mint& c0) const { return SMFPS(*this) *= c0; }
	friend SMFPS operator*(const mint& c0, const SMFPS& f) { return f * c0; }
	[[nodiscard]] SMFPS operator-() const { return SMFPS(*this) *= -1; }

	// 積
	SMFPS& operator*=(const SMFPS& g) {
		// verify : https://yukicoder.me/problems/no/2660

		vector<pim> tmp(n * g.n);
		rep(i, n) rep(j, g.n) {
			tmp[i * g.n + j] = { c[i].first + g[j].first, c[i].second * g[j].second };
		}
		sort(all(tmp), [](const pim& l, const pim& r) {
			return l.first < r.first;
		});
		tmp.emplace_back(INF, 0);

		n = 0; c.clear(); mint acc = 0;
		rep(i, sz(tmp) - 1) {
			if (tmp[i].first == tmp[i + 1].first) acc += tmp[i].second;
			else {
				if (acc + tmp[i].second != 0) {
					puch_back(tmp[i].first, acc + tmp[i].second);
				}
				acc = 0;
			}
		}

		return *this;
	}
	[[nodiscard]] SMFPS operator*(const SMFPS& g) const { return SMFPS(*this) *= g; }

	// 係数の右シフト（z^d 倍）
	SMFPS& operator>>=(int d) {
		repea(tmp, c) tmp.first += d;
		return *this;
	}
	[[nodiscard]] SMFPS operator>>(int d) const { return SMFPS(*this) >>= d; }

	// 係数の左シフト（z^d で割った商）
	[[nodiscard]] SMFPS operator<<(int d) const {
		// verify : https://judge.yosupo.jp/problem/sqrt_of_formal_power_series_sparse

		SMFPS res;
		for (auto [deg, coef] : c) {
			if (deg >= d) res.puch_back(deg - d, coef);
		}
		return res;
	}
	SMFPS& operator<<=(int d) { *this = *this << d; return *this; }

	// 次数が同じ項をまとめ，係数が 0 の項を削除する．
	SMFPS& shrink() {
		// verify : https://www.codechef.com/problems/RANDCOLORING

		int l = 0; int deg = -1; mint acc = 0;
		rep(i, n) {
			auto& [deg, coef] = c[i];
			acc += coef;

			if (i == n - 1 || c[i + 1].first != deg) {
				if (acc != 0) {
					c[l] = { deg, acc };
					acc = 0;
					l++;
				}
			}
		}
		n = l;
		c.resize(l);

		return *this;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const SMFPS& f) {
		rep(i, sz(f)) {
			os << f[i].second << "z^" << f[i].first << (i < sz(f) - 1 ? " + " : "");
		}
		return os;
	}
#endif
};


//【積（スパース）】O(n |f|)
/*
* a[0..n) の母関数を A(z) とし，[z^[0..n)] A(z)f(z) を返す．
*/
vm product_sfps(const vm& a, const SMFPS& f) {
	// verify : https://www.codechef.com/problems/RANDCOLORING

	int n = sz(a);

	vm res(n);

	// 配る DP
	rep(i, n) {
		for (auto& [deg, coef] : f.c) {
			if (i + deg >= n) break;
			res[i + deg] += a[i] * coef;
		}
	}

	return res;
}


//【商（スパース）】O(n |f|)
/*
* a[0..n) の母関数を A(z) とし，[z^[0..n)] A(z)/f(z) を返す．
*/
vm divide_sfps(const vm& a, const SMFPS& f) {
	// verify : https://www.codechef.com/problems/RANDCOLORING

	int n = sz(a), K = sz(f);

	Assert(K > 0);

	auto [d_min, f0] = f[0];
	Assert(f0 != 0);
	mint f0_inv = f0.inv();

	vm res(n);
	rep(i, n - d_min) res[i] = a[i + d_min];

	// インライン配る DP
	rep(i, n) {
		res[i] *= f0_inv;

		repi(k, 1, K - 1) {
			auto [deg, coef] = f[k];
			deg -= d_min;
			if (i + deg >= n) break;
			res[i + deg] -= res[i] * coef;
		}
	}

	return res;
}


//【微分（スパース）】O(|f|)
/*
* f'(z) を返す．
*/
SMFPS derivative(const SMFPS& f) {
	// verify : https://judge.yosupo.jp/problem/exp_of_formal_power_series_sparse

	SMFPS res;
	for (auto [deg, coef] : f.c) {
		if (deg == 0) continue;
		res.puch_back(deg - 1, coef * deg);
	}
	return res;
}


//【微分方程式（スパース）】O(n (|a| + |b|))
/*
* 与えられた a(z), b(z), f0 に対し，変数係数 1 階線形斉次常微分方程式
*	a(z) f'(z) + b(z) f(z) = 0,  [z^0]f(z) = f0
* の解 f(z) を求め，[z^[0..n]] f(z) を返す．
*
* 制約 : [z^0]a(z) != 0，fm は n! まで計算可能
*/
vm differential_equation(int n, const SMFPS& a, const SMFPS& b, mint f0, const Factorial_mint& fm) {
	// 参考 : https://maspypy.com/%E5%A4%9A%E9%A0%85%E5%BC%8F%E3%83%BB%E5%BD%A2%E5%BC%8F%E7%9A%84%E3%81%B9%E3%81%8D%E7%B4%9A%E6%95%B0-%E9%AB%98%E9%80%9F%E3%81%AB%E8%A8%88%E7%AE%97%E3%81%A7%E3%81%8D%E3%82%8B%E3%82%82%E3%81%AE#toc44
	// verify : https://judge.yosupo.jp/problem/exp_of_formal_power_series_sparse

	//【方法】
	//		f'(z) = Σi∈[1..∞) i f[i] z^(i-1) = Σi∈[0..∞) (i+1) f[i+1] z^i
	// と表せることに注意すると，微分方程式の両辺の z^n を係数を比較して
	//		Σi∈[0..n] a[i] (n-i+1) f[n-i+1] + Σi∈[0..n] b[i] f[n-i] = 0
	// なる等式を得る．f[n+1] のみが左辺に残るように変形すると，
	//		a[0] (n+1) f[n+1] + Σi∈[1..n] a[i] (n-i+1) f[n-i+1] + Σi∈[0..n] b[i] f[n-i] = 0
	//		⇔ f[n+1] = -1/a[0](n+1) (Σi∈[1..n] a[i] (n-i+1) f[n-i+1] + Σi∈[0..n] b[i] f[n-i])
	// なる漸化式を得る．

	Assert(a[0].first == 0 && a[0].second != 0);

	vm f(n + 1);
	f[0] = f0;

	mint a0_inv = a[0].second.inv();

	rep(i, n) {
		mint num = 0;

		repi(k, 1, sz(a) - 1) {
			auto [deg, coef] = a[k];
			if (i - deg + 1 < 0) break;
			num += coef * (i - deg + 1) * f[i - deg + 1];
		}

		for (auto [deg, coef] : b.c) {
			if (i - deg < 0) break;
			num += coef * f[i - deg];
		}

		f[i + 1] = -a0_inv * fm.inv(i + 1) * num;
	}

	return f;
}


//【微分方程式（非斉次，スパース）】O(n (|a| + |b|))
/*
* 与えられた a(z), b(z), c[0..n), f0 に対し，変数係数 1 階線形非斉次常微分方程式
*	a(z) f'(z) + b(z) f(z) = c(z),  [z^0]f(z) = f0
* の解 f(z) を求め，[z^[0..n]] f(z) を返す．
*
* 制約 : [z^0]a(z) != 0，fm は n! まで計算可能
*/
vm differential_equation(const SMFPS& a, const SMFPS& b, const vm& c, mint f0, const Factorial_mint& fm) {
	// verify : https://judge.yosupo.jp/problem/log_of_formal_power_series_sparse

	//【方法】
	//		f'(z) = Σi∈[1..∞) i f[i] z^(i-1) = Σi∈[0..∞) (i+1) f[i+1] z^i
	// と表せることに注意すると，微分方程式の両辺の z^n を係数を比較して
	//		Σi∈[0..n] a[i] (n-i+1) f[n-i+1] + Σi∈[0..n] b[i] f[n-i] = c[n]
	// なる等式を得る．f[n+1] のみが左辺に残るように変形すると，
	//		a[0] (n+1) f[n+1] + Σi∈[1..n] a[i] (n-i+1) f[n-i+1] + Σi∈[0..n] b[i] f[n-i] = c[n]
	//		⇔ f[n+1] = 1/a[0](n+1) (c[n] - Σi∈[1..n] a[i] (n-i+1) f[n-i+1] - Σi∈[0..n] b[i] f[n-i])
	// なる漸化式を得る．

	Assert(a[0].first == 0 && a[0].second != 0);

	int n = sz(c);

	vm f(n + 1);
	f[0] = f0;

	mint a0_inv = a[0].second.inv();

	rep(i, n) {
		mint num = c[i];

		repi(k, 1, sz(a) - 1) {
			auto [deg, coef] = a[k];
			if (i - deg + 1 < 0) break;
			num -= coef * (i - deg + 1) * f[i - deg + 1];
		}

		for (auto [deg, coef] : b.c) {
			if (i - deg < 0) break;
			num -= coef * f[i - deg];
		}

		f[i + 1] = a0_inv * fm.inv(i + 1) * num;
	}

	return f;
}


//【逆元（スパース）】O(n |f|)
/*
* [z^[0..n]] 1/f(z) を返す．
*
* 制約 : [z^0]f(z) != 0，fm は n! まで計算可能
*
* 利用：【微分方程式（スパース）】,【微分（スパース）】
*/
vm inv_sfps(int n, const SMFPS& f, const Factorial_mint& fm) {
	// verify : https://judge.yosupo.jp/problem/inv_of_formal_power_series_sparse

	//【方法】
	// g(z) = 1/f(z) とおく．両辺の対数をとり
	//		log g(z) = - log f(z)
	// とし，さらに両辺を z で微分すると
	//		g'(z) / g(z) = - f'(z) / f(z)
	//		⇔ f(z) g'(z) + f'(z) g(z) = 0
	// なる 1 階線形斉次常微分方程式を得る．

	Assert(f[0].first == 0 && f[0].second != 0);

	return differential_equation(n, f, derivative(f), f[0].second.inv(), fm);
}


//【指数関数（スパース）】O(n |f|)
/*
* [z^[0..n]] exp f(z) を返す．
*
* 制約 : [z^0]f(z) = 0，fm は n! まで計算可能
*
* 利用：【微分方程式（スパース）】,【微分（スパース）】
*/
vm exp_sfps(int n, const SMFPS& f, const Factorial_mint& fm) {
	// verify : https://judge.yosupo.jp/problem/exp_of_formal_power_series_sparse

	//【方法】
	// g(z) = exp f(z) とおく．両辺の対数をとり
	//		log g(z) = f(z)
	// とし，さらに両辺を z で微分すると
	//		g'(z) / g(z) = f'(z)
	//		⇔ 1 g'(z) - f'(z) g(z) = 0
	// なる 1 階線形斉次常微分方程式を得る．

	Assert(f[0].first != 0);

	return differential_equation(n, mint(1), -derivative(f), 1, fm);
}


//【対数関数（スパース）】O(n |f|)
/*
* [z^[0..n]] log f(z) を返す．
*
* 制約 : [z^0]f(z) = 1，fm は n! まで計算可能
*
* 利用：【微分方程式（非斉次，スパース）】
*/
vm log_sfps(int n, const SMFPS& f, const Factorial_mint& fm) {
	// verify : https://judge.yosupo.jp/problem/log_of_formal_power_series_sparse

	//【方法】
	// g(z) = log f(z) とおく．両辺を z で微分すると
	//		g'(z) = f'(z) / f(z)
	//		⇔ f(z) g'(z) + 0 g(z) = f'(z)
	// なる 1 階線形非斉次常微分方程式を得る．

	Assert(f[0].first == 0 && f[0].second == 1);

	vm c(n);
	for (auto [deg, coef] : f.c) {
		if (0 <= deg - 1 && deg - 1 < n) c[deg - 1] = coef * deg;
	}

	return differential_equation(f, mint(0), c, 0, fm);
}


//【累乗（スパース）】O(n |f|)
/*
* [z^[0..n]] f(z)^m を返す．
*
* 制約 : fm は n! まで計算可能
*
* 利用：【微分方程式（スパース）】,【微分（スパース）】
*/
vm pow_sfps(int n, const SMFPS& f, ll m, const Factorial_mint& fm) {
	// verify : https://judge.yosupo.jp/problem/pow_of_formal_power_series_sparse

	//【方法】
	// f(z) を z の冪で割って [z^0]f(z) != 0 としておく．
	// 
	// g(z) = f(z)^m とおく．両辺の対数をとり
	//		log g(z) = m log f(z)
	// とし，さらに両辺を z で微分すると
	//		g'(z) / g(z) = m f'(z) / f(z)
	//		⇔ f(z) g'(z) - m f'(z) g(z) = 0
	// なる 1 階線形斉次常微分方程式を得る．

	vm g(n + 1);

	// f(z) = 0 のとき
	if (sz(f) == 0) {
		// 0^0 = 1 とする．
		if (m == 0) g[0] = 1;

		return g;
	}

	auto [d_min, f0] = f[0];

	// d_min * m ≧ n + 1 なら結果は 0
	if (d_min > 0 && m > n / d_min) return g;

	SMFPS f2 = f << d_min;
	vm g2 = differential_equation(n - d_min * (int)m, f2, -m * derivative(f2), f0.pow(m), fm);

	rep(i, sz(g2)) g[i + d_min * m] = g2[i];

	return g;
}


//【平方根（スパース）】O(n |f|)
/*
* [z^[0..n]] √f(z) を返す（なければ空リストを返す）
*
* 制約 : fm は n! まで計算可能
*
* 利用：【微分方程式（スパース）】,【微分（スパース）】,【平方剰余】
*/
vm sqrt_sfps(int n, const SMFPS& f, const Factorial_mint& fm) {
	// verify : https://judge.yosupo.jp/problem/sqrt_of_formal_power_series_sparse

	//【方法】
	// f(z) の最低次の項の次数が奇数であれば √f(z) は存在しない．
	// さもなくば z の偶数冪で割って [z^0]f(z) != 0 としてよい．
	// 
	// g(z) = √f(z) とおく．両辺の対数をとり
	//		log g(z) = 1/2 log f(z)
	// とし，さらに両辺を z で微分すると
	//		g'(z) / g(z) = 1/2 f'(z) / f(z)
	//		⇔ 2 f(z) g'(z) - f'(z) g(z) = 0
	// なる 1 階線形斉次常微分方程式を得る．

	vm g(n + 1);

	// f(z) = 0 なら √f(z) = 0 でよい．
	if (sz(f) == 0) return g;

	// f(z) の最低次数が奇数なら √f(z) は存在しない．
	auto [d_min, f0] = f[0];
	if (d_min & 1) return vm();
	int hd_min = d_min / 2;

	// f0 が平方非剰余なら √f(z) は存在しない．
	int g0 = cipolla(f0);
	if (g0 == -1) return vm();

	SMFPS f2 = f << d_min;
	vm g2 = differential_equation(n - d_min, 2 * f2, -derivative(f2), g0, fm);

	rep(i, sz(g2)) g[i + hd_min] = g2[i];

	return g;
}


//【累乗の積（スパース）】O(n |f1| |f2|)
/*
* [z^[0..n]] f1(z)^m1 f2(z)^m2 を返す．
*
* 制約 : [z^0]f1(z) = [z^0]f2(z) = 1，fm は n! まで計算可能
*
* 利用：【微分方程式（スパース）】,【微分（スパース）】
*/
vm power_product(int n, const SMFPS& f1, ll m1, const SMFPS& f2, ll m2, const Factorial_mint& fm) {
	// 参考 : https://maspypy.com/%E5%A4%9A%E9%A0%85%E5%BC%8F%E3%83%BB%E5%BD%A2%E5%BC%8F%E7%9A%84%E3%81%B9%E3%81%8D%E7%B4%9A%E6%95%B0-%E9%AB%98%E9%80%9F%E3%81%AB%E8%A8%88%E7%AE%97%E3%81%A7%E3%81%8D%E3%82%8B%E3%82%82%E3%81%AE#toc45
	// verify : https://yukicoder.me/problems/no/2660

	//【方法】
	// g(z) = f1(z)^m1 f2(z)^m2 とおく．両辺の対数をとり
	//		log g(z) = m1 log f1(z) + m2 log f2(z)
	// とし，さらに両辺を z で微分すると
	//		g'(z) / g(z) = m1 f1'(z) / f1(z) +  m2 f2'(z) / f2(z)
	//		⇔ f1(z) f2(z) g'(z) - (m1 f1'(z) f2(z) + m2 f1(z) f2'(z)) g(z) = 0
	// なる 1 階線形斉次常微分方程式を得る．

	Assert(f1[0].first == 0 && f1[0].second == 1);
	Assert(f2[0].first == 0 && f2[0].second == 1);

	SMFPS a = f1 * f2;
	SMFPS b = -(m1 * derivative(f1) * f2 + m2 * f1 * derivative(f2));

	return differential_equation(n, a, b, 1, fm);
}


