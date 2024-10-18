#pragma once
#include "header.h"
#include "数論.h"
#include "整除.h"
#include "ディリクレ畳込み.h"
#include "列挙(数論).h"
// ■■■■■ 一括で求めるための数論アルゴリズム ■■■■■


//【素因数分解（複数）】
/*
* Osa_k(int n) : O(n log(log n))
*	n 以下の自然数を高速に素因数分解する準備を行う．
*
* bool primeQ(int i) : O(1)
*	i が素数かを返す．
*
* map<int, int> factor_integer(int i) : O(log n)
*	i の素因数分解結果を返す．
*
* vi divisors(int i) : O(σ(n))
*	i の約数の昇順リストを返す．
*
* int euler_phi(int i) : O(log n)
*	オイラーのトーシェント関数 φ(i) の値を返す．
*
* vi unique_prime_factors(int i) : O(log n)
*	i の重複を除去した素因数のリストを返す．
*
* int radical(int i) : O(log n)
*	i の根基（重複を除去した素因数の積）を返す．
*
* vi prime_power_decomposition(int i) : O(log n)
*	i を素数冪の積に分解したリストを返す．
*/
struct Osa_k {
	int n;

	// gpf[i] : i を割り切る最大の素数
	vi gpf;

	// n 以下の自然数を高速に素因数分解する準備を行う．
	Osa_k(int n_) : n(n_), gpf(n + 1) {
		// verify : https://yukicoder.me/problems/no/2207

		iota(all(gpf), 0);

		for (int p = 2; p * p <= n; p++) {
			if (gpf[p] != p) continue;

			// ここは d の最大性のため p^2 からにはできない．
			for (int i = p; i <= n; i += p) gpf[i] = p;
		}
	}
	Osa_k() : n(0) {}

	// i が素数かを返す．
	bool primeQ(int i) {
		// verify : https://yukicoder.me/problems/no/1396

		Assert(i <= n);

		return i >= 2 && gpf[i] == i;
	}

	// i の素因数分解結果を返す．
	map<int, int> factor_integer(int i) const {
		// verify : https://yukicoder.me/problems/no/2207

		Assert(i <= n);

		map<int, int> pps;
		while (i > 1) {
			pps[gpf[i]]++;
			i /= gpf[i];
		}
		return pps;
	}

	// i の約数の昇順リストを返す．
	vi divisors(int i) const {
		// verify : https://atcoder.jp/contests/abc368/tasks/abc368_f

		Assert(i <= n);

		vi divs{ 1 };

		auto pps = factor_integer(i);
		for (auto [p, d] : pps) {
			vi powp(d);
			powp[0] = p;
			rep(i, d - 1) powp[i + 1] = powp[i] * p;

			int m = sz(divs);
			repir(j, m - 1, 0) rep(i, d) divs.push_back(divs[j] * powp[i]);
		}
		sort(all(divs)); // 不要なら削除可能

		return divs;
	}

	// オイラーのトーシェント関数 φ(i) の値を返す．
	int euler_phi(int i) {
		// verify : https://yukicoder.me/problems/no/2849

		Assert(i <= n);

		int phi = 1; int pp = INF;
		while (i > 1) {
			int p = gpf[i];
			phi *= (p == pp ? p : p - 1);

			pp = p;
			i /= p;
		}
		return phi;
	}

	// i の重複を除去した素因数のリストを返す．
	vi unique_prime_factors(int i) const {
		Assert(i <= n);

		vi res; int pp = INF;
		while (i > 1) {
			int p = gpf[i];
			if (p != pp) res.push_back(p);

			pp = p;
			i /= p;
		}
		return res;
	}

	// i の根基（重複を除去した素因数の積）を返す．
	int radical(int i) const {
		// verify : https://projecteuler.net/problem=518

		Assert(i <= n);

		int rad = 1; int pp = INF;
		while (i > 1) {
			int p = gpf[i];
			if (p != pp) rad *= p;

			pp = p;
			i /= p;
		}
		return rad;
	}

	// i を素数冪の積に分解したリストを返す．
	vi prime_power_decomposition(int i) const {
		// verify : https://projecteuler.net/problem=407

		Assert(i <= n);

		vi res; int pp = INF;
		while (i > 1) {
			int p = gpf[i];
			if (p != pp) res.push_back(p);
			else res.back() *= p;

			pp = p;
			i /= p;
		}
		return res;
	}
};


//【一括素因数分解】O(n log(log n))
/*
* [1..n] の素因数分解結果のリストを返す．
*/
vector<map<int, int>> factor_integer_all(int n) {
	// verify : https://atcoder.jp/contests/abc052/tasks/arc067_a

	vector<map<int, int>> pps(n + 1);

	// 順に素数で割っていった残りの値を記録しておくためのテーブル
	vi a(n + 1);
	iota(all(a), 0);

	int p = 2;

	// √n 以下の p の処理（continue されない場合は p は素数）
	for (; p * p <= n; p++) {
		if (a[p] == 1) continue;

		for (int i = p; i <= n; i += p) {
			while (a[i] % p == 0) {
				pps[i][p]++;
				a[i] /= p;
			}
		}
	}

	// √n より大きい p の処理（if 内に入っても p は素数とは限らないので注意）
	for (; p <= n; p++) if (a[p] != 1) pps[p][a[p]]++;

	return pps;
}


//【一括素因数分解（区間）】O((√r + (r-l)) log(log r))
/*
* [l..r) に含まれる自然数 i の素因数分解を pps[i-l] に格納し，pps を返す．
*
* 利用：【素数の列挙】
*/
vector<map<ll, int>> factor_integer_interval(ll l, ll r) {
	vector<map<ll, int>> pps(r - l);

	// ps : √r 以下の素数のリスト
	vi ps = eratosthenes((int)(sqrt(r) + 0.01));

	// 順に素数で割っていった残りの値を記録しておくためのテーブル
	vl a(r - l);
	iota(all(a), l);

	repe(p, ps) {
		for (ll j = (l + p - 1) / p * p; j < r; j += p) {
			while (a[j - l] % p == 0) {
				pps[j - l][p]++;
				a[j - l] /= p;
			}
		}
	}

	for (ll j = l; j < r; j++) if (a[j - l] != 1) pps[j - l][a[j - l]]++;

	return pps;
}


//【非平方因子（一括）】O(n)
/*
* [1..n] の非平方因子のリストを返す．
*/
vi square_free_factor(int n) {
	// verify : https://atcoder.jp/contests/abc254/tasks/abc254_d

	vi res(n + 1);
	iota(all(res), 0);

	for (int i = 2; i * i <= n; i++) {
		int i2 = i * i;
		for (int j = i2; j <= n; j += i2) {
			while (res[j] % i2 == 0) res[j] /= i2;
		}
	}

	return res;
}


//【位数（法が素数，複数）】
/*
* Order_prime(T p) : O(√p)
*	法を p として初期化する．
*
* T pow(T a, T x) : O(log x)
*	a^x (mod p) を返す．
*
* T ord(T a) : O((log p)^2)
*	a^x ≡ 1 (mod p) となる最小の正整数 x を返す（なければ -1）
*/
template <class T>
class Order_prime {
	T P;
	vector<pair<T, T>> ppds; // {p, p^d} のリスト

public:
	Order_prime(T p) : P(p) {
		T n = p - 1;
		for (T i = 2; i * i <= n; i++) {
			T pd = 1;
			while (n % i == 0) {
				pd *= i;
				n /= i;
			}
			if (pd > 1) ppds.emplace_back(i, pd);
		}
		if (n > 1) ppds.emplace_back(n, n);
	}
	Order_prime() : P(0) {}

	// a^x (mod p) を返す．
	T pow(T a, T x) {
		__int128 res = 1, pow2 = a;
		while (x > 0) {
			if (x & 1) res = (res * pow2) % P;
			pow2 = (pow2 * pow2) % P;
			x /= 2;
		}
		return (T)res;
	}

	// a^x ≡ 1 (mod p) となる最小の自然数 x を返す（なければ -1）
	T ord(T a) {
		if (a % P == 0) return -1;

		T res = 1;

		for (auto [p, pd] : ppds) {
			T a2 = pow(a, (P - 1) / pd);

			T e = 1;
			while (true) {
				if (pow(a2, e) == 1) {
					res *= e;
					break;
				}
				e *= p;
			}
		}

		return res;
	}
};


//【約数和関数（一括）】O(n log(log n))
/*
* 各 i∈[1..n] について約数和関数 σ_k(i) = (i の約数の k 乗和) を格納したリストを返す．
* 特に k = 0 なら約数の個数，k = 1 なら約数の総和と等価である．
*
* 利用：【約数倍数変換】
*/
template <class T>
vector<T> divisor_sigma(int k, int n) {
	// 参考 : https://maspypy.com/%E6%95%B0%E5%AD%A6-%E7%95%B3%E3%81%BF%E8%BE%BC%E3%81%BF%E5%85%A5%E9%96%80%EF%BC%9Adirichlet%E7%A9%8D%E3%81%A8%E3%82%BC%E3%83%BC%E3%82%BF%E5%A4%89%E6%8F%9B%E3%83%BB%E3%83%A1%E3%83%93%E3%82%A6
	// verify : https://atcoder.jp/contests/abc172/tasks/abc172_d

	//【方法】
	// 約数和関数の定義より，等式
	//		σ_k(i) = Σ_(d|i) d^k
	// を得る．これは σ_k が a[i] = i^k を約数ゼータ変換したものであることを意味する．

	vector<T> a(n + 1);
	a[0] = 0;
	repi(i, 1, n) a[i] = T(powi(i, k));

	Div_mul_transform<T> dt(n);
	dt.divisor_zeta(a);

	return a;
}


//【オイラー関数（一括）】O(n log(log n))
/*
* φ[1..n] を返す．
*
* 利用：【約数倍数変換】
*/
vl euler_phi(int n) {
	// 参考 : https://maspypy.com/%E6%95%B0%E5%AD%A6-%E7%95%B3%E3%81%BF%E8%BE%BC%E3%81%BF%E5%85%A5%E9%96%80%EF%BC%9Adirichlet%E7%A9%8D%E3%81%A8%E3%82%BC%E3%83%BC%E3%82%BF%E5%A4%89%E6%8F%9B%E3%83%BB%E3%83%A1%E3%83%93%E3%82%A6
	// verify : https://yukicoder.me/problems/no/2249

	//【方法】
	// i の各約数 d について，GCD(i, x) = d となる x∈[1..i] の個数は，
	// x が GCD(i/d, y) = 1 なる y∈[1..i/d] を用いて x = y d と表されるので
	// オイラー関数の定義より φ(i/d) に等しい．
	// これらを全ての d にわたって足し合わせることで，[1..i] を i との GCD で分類した等式
	//		i = Σ_(d|i) φ(i/d)
	//		⇔ i = Σ_(d|i) φ(d)
	// を得る．これは φ を約数ゼータ変換したものが a[i] = i であることを意味する．

	vl a(n + 1);
	repi(i, 1, n) a[i] = i;

	// int にすると途中計算でオーバーフローするので注意
	Div_mul_transform<ll> dt(n);
	dt.divisor_mobius(a);

	return a;
}


//【メビウス関数（一括）】O(n log(log n))
/*
* μ[1..n] を返す．
*
* 利用：【約数倍数変換】
*/
vi mobius_mu(int n) {
	// 参考 : https://maspypy.com/%E6%95%B0%E5%AD%A6-%E7%95%B3%E3%81%BF%E8%BE%BC%E3%81%BF%E5%85%A5%E9%96%80%EF%BC%9Adirichlet%E7%A9%8D%E3%81%A8%E3%82%BC%E3%83%BC%E3%82%BF%E5%A4%89%E6%8F%9B%E3%83%BB%E3%83%A1%E3%83%93%E3%82%A6
	// verify : https://yukicoder.me/problems/no/1514

	vi a(n + 1, 0);
	a[1] = 1;

	Div_mul_transform<int> dt(n);
	dt.divisor_mobius(a);

	return a;
}


