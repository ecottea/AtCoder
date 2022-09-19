#pragma once
#include "header.h"
#include "数論.h"
#include "約数変換.h"
// ■■■■■ 一括で求めるための数論アルゴリズム ■■■■■


//【素数の列挙】O(n log(log n))
/*
* n 以下の素数を列挙し，ps に昇順に格納する．
*/
void eratosthenes(int n, vi& ps) {
	// verify : https://algo-method.com/tasks/330

	ps.clear();

	// 素数かどうかを記録しておくためのテーブル
	vb is_prime(n + 1, true);
	is_prime[0] = is_prime[1] = false;

	int i = 2;

	// √n 以下の i の処理
	for (; i <= n / i; i++) {
		if (is_prime[i]) {
			ps.push_back(i);

			for (int j = i * i; j <= n; j += i) is_prime[j] = false;
		}
	}

	// √n より大きい i の処理
	for (; i <= n; i++) if (is_prime[i]) ps.push_back(i);
}


//【素数の列挙（区間）】O((√r + (r - l))log(log r))
/*
* [l..r) に含まれる素数を ps に昇順に格納する．
*
* 利用：【素数の列挙】
*/
void eratosthenes_interval(ll l, ll r, vl& ps) {
	// verify : https://algo-method.com/tasks/332

	ps.clear();

	vi ps_sub;
	eratosthenes(int(sqrt(r) + EPS), ps_sub);

	// 素数かどうかを記録しておくためのテーブル
	vb is_prime(r - l, true);
	if (l == 1) is_prime[0] = false; // 1 は素数ではない
	repe(p, ps_sub) {
		for (ll j = (l + p - 1) / p * p; j < r; j += p) {
			if (j != p) is_prime[j - l] = false;
		}
	}

	rep(i, r - l) {
		if (is_prime[i]) ps.push_back(l + i);
	}
}


//【素因数分解（複数）】
/*
* Factor_integer(int n) : O(n log(log n))
*	n 以下の自然数を高速に素因数分解する準備を行う．
*
* factor_integer(int i, map<int, int>& pps) : O(log n)
*	i の素因数分解結果を pps に格納する．
*/
struct Factor_integer {
	// verify : https://atcoder.jp/contests/abc215/tasks/abc215_d

	int n;

	// d[i] : i を割り切る最小の素数
	vi d;

	// n 以下の自然数を高速に素因数分解する準備を行う．
	Factor_integer(int n_) : n(n_), d(n + 1) {
		iota(all(d), 0);

		for (int p = 2; p * p <= n; p++) {
			if (d[p] != p) continue;

			for (int i = p; i <= n; i += p) {
				d[i] = p;
			}
		}
	}

	// i の素因数分解結果を pps に格納する．
	void factor_integer(int i, map<int, int>& pps) {
		Assert(i <= n);

		pps.clear();

		while (i > 1) {
			pps[d[i]]++;
			i /= d[i];
		}
	}
};


//【一括素因数分解】O(n log(log n))
/*
* n 以下の自然数 i の素因数分解を pps[i] に格納する．（pps[0] は使わない）
*
*（エラトステネスの篩）
*/
void factor_integer_all(int n, vector<map<int, int>>& pps) {
	// verify : https://atcoder.jp/contests/abc052/tasks/arc067_a

	pps = vector<map<int, int>>(n + 1);

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

	// √n より大きい p の処理（この p は素数とは限らないので注意）
	for (; p <= n; p++) {
		if (a[p] != 1) pps[p][a[p]]++;
	}
}


//【一括素因数分解（区間）】O((√r + (r - l))log(log r))
/*
* [l..r) に含まれる自然数 i の素因数分解を pps[i - l] に格納する．
*
*（エラトステネスの区間篩）
*
* 利用：【素数の列挙】
*/
void factor_integer_interval(ll l, ll r, vector<map<ll, int>>& pps) {
	pps = vector<map<ll, int>>(r - l);

	// ps : √r 以下の素数のリスト
	vi ps;
	eratosthenes(int(sqrt(r) + EPS), ps);

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

	for (ll j = l; j < r; j++) {
		if (a[j - l] != 1) pps[j - l][a[j - l]]++;
	}
}


//【約数関数 σ_k(n)】O(n log(log n))
/*
* i = [1..n] について約数関数 σ_k(i) = (i の約数の k 乗和) を s[i] に格納する．
* 特に k = 0 なら約数の個数，k = 1 なら約数の総和と等価である．
*
* 利用：【約数変換，LCM 畳込み】
*/
template <class T> void divisor_sigma(int k, int n, vector<T>& s) {
	// 参考 : https://maspypy.com/%E6%95%B0%E5%AD%A6-%E7%95%B3%E3%81%BF%E8%BE%BC%E3%81%BF%E5%85%A5%E9%96%80%EF%BC%9Adirichlet%E7%A9%8D%E3%81%A8%E3%82%BC%E3%83%BC%E3%82%BF%E5%A4%89%E6%8F%9B%E3%83%BB%E3%83%A1%E3%83%93%E3%82%A6
	// verify : https://atcoder.jp/contests/arc068/tasks/arc068_c

	s.resize(n + 1);
	s[0] = 0;
	repi(i, 1, n) s[i] = T(pow(i, k));

	Divisor_transform<T> dt(n);
	dt.divisor_zeta(s);
}


//【オイラー関数 φ(n)】O(n log(log n))
/*
* i = [1..n] についてオイラー関数 φ(i) の値を e[i] に格納する．
*
* 利用：【約数変換，LCM 畳込み】
*/
void euler_phi(int n, vi& phi) {
	// 参考 : https://maspypy.com/%E6%95%B0%E5%AD%A6-%E7%95%B3%E3%81%BF%E8%BE%BC%E3%81%BF%E5%85%A5%E9%96%80%EF%BC%9Adirichlet%E7%A9%8D%E3%81%A8%E3%82%BC%E3%83%BC%E3%82%BF%E5%A4%89%E6%8F%9B%E3%83%BB%E3%83%A1%E3%83%93%E3%82%A6
	// verify : https://onlinejudge.u-aizu.ac.jp/challenges/sources/VPC/RUPC/2286?year=2011

	phi.resize(n + 1);
	phi[0] = 0;
	repi(i, 1, n) phi[i] = i;

	Divisor_transform<int> dt(n);
	dt.divisor_mobius(phi);
}


//【メビウス関数 μ(n)】O(n log(log n))
/*
* i = [1..n] についてメビウス関数 μ(i) の値を mu[i] に格納する．
*
* 利用：【約数変換，LCM 畳込み】
*/
void mobius_mu(int n, vi& mu) {
	// 参考 : https://maspypy.com/%E6%95%B0%E5%AD%A6-%E7%95%B3%E3%81%BF%E8%BE%BC%E3%81%BF%E5%85%A5%E9%96%80%EF%BC%9Adirichlet%E7%A9%8D%E3%81%A8%E3%82%BC%E3%83%BC%E3%82%BF%E5%A4%89%E6%8F%9B%E3%83%BB%E3%83%A1%E3%83%93%E3%82%A6
	// verify : https://yukicoder.me/problems/no/1514

	mu = vi(n + 1, 0);
	mu[1] = 1;

	Divisor_transform<int> dt(n);
	dt.divisor_mobius(mu);
}


