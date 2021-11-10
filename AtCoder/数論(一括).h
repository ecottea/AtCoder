#pragma once
#include "header.h"
#include "数論.h"
#include "畳み込み.h"
// ■■■■■ 一括で求めるための数論アルゴリズム ■■■■■


//【約数関数 σ_k(n)】O(n log(log n))
/*
* i = [1..n] について約数関数 σ_k(i) = (i の約数の k 乗和) を s[i] に格納する．
* 特に k = 0 なら約数の個数，k = 1 なら約数の総和と等価である．
*
* 利用：【添字 lcm での畳込み】
*/
template <class T> void divisor_sigma(int k, int n, vector<T>& s) {
	// 参考 : https://maspypy.com/%E6%95%B0%E5%AD%A6-%E7%95%B3%E3%81%BF%E8%BE%BC%E3%81%BF%E5%85%A5%E9%96%80%EF%BC%9Adirichlet%E7%A9%8D%E3%81%A8%E3%82%BC%E3%83%BC%E3%82%BF%E5%A4%89%E6%8F%9B%E3%83%BB%E3%83%A1%E3%83%93%E3%82%A6

	s.resize(n + 1LL);
	s[0] = 0;
	repi(i, 1, n) s[i] = T(pow(i, k));

	LCM_convolution<T> l(n);
	l.zeta(s);
}


//【オイラー関数 φ(n)】O(n log(log n))
/*
* i = [1..n] についてオイラー関数 φ(i) の値を e[i] に格納する．
*
* 利用：【添字 lcm での畳込み】
*/
void euler_phi(int n, vi& phi) {
	// 参考 : https://maspypy.com/%E6%95%B0%E5%AD%A6-%E7%95%B3%E3%81%BF%E8%BE%BC%E3%81%BF%E5%85%A5%E9%96%80%EF%BC%9Adirichlet%E7%A9%8D%E3%81%A8%E3%82%BC%E3%83%BC%E3%82%BF%E5%A4%89%E6%8F%9B%E3%83%BB%E3%83%A1%E3%83%93%E3%82%A6

	phi.resize(n + 1LL);
	phi[0] = 0;
	repi(i, 1, n) phi[i] = i;

	LCM_convolution<int> l(n);
	l.mobius(phi);
}


//【メービウス関数 μ(n)】O(n log(log n))
/*
* i = [1..n] についてメービウス関数 μ(i) の値を mu[i] に格納する．
*
* 利用：【添字 lcm での畳込み】
*/
void mobius_mu(int n, vi& mu) {
	// 参考 : https://maspypy.com/%E6%95%B0%E5%AD%A6-%E7%95%B3%E3%81%BF%E8%BE%BC%E3%81%BF%E5%85%A5%E9%96%80%EF%BC%9Adirichlet%E7%A9%8D%E3%81%A8%E3%82%BC%E3%83%BC%E3%82%BF%E5%A4%89%E6%8F%9B%E3%83%BB%E3%83%A1%E3%83%93%E3%82%A6

	mu = vi(n + 1LL, 0);
	mu[1] = 1;

	LCM_convolution<int> l(n);
	l.mobius(mu);
}


//【gcd の分布】O(√n + d(n)^2 log d(n))　（d(n) : n の約数の個数）
/*
* gcd(i, n) = g になるような i∈[1..K] の個数を cnt[g] に昇順に格納する．
*
* K=n のとき，cnt[g] は Z/nZ に位数 n/g の元が何個あるかとも解釈できる．
* 
* 利用：【約数列挙】
*/
void gcd_distribution(ll K, ll n, map<ll, ll>& cnt) {
	// ds : n の約数の集合（昇順）
	vl ds;
	divisors(n, ds);
	int m = sz(ds);

	cnt.clear();
	repir(j, m - 1, 0) {
		// gcd(i, n) が d の倍数になるような i∈[1..K] の個数
		cnt[ds[j]] = K / ds[j];

		// gcd(i, n) = 2d, 3d, ... になるような i∈[1..K] の個数を引く
		repi(j2, j + 1, m - 1) {
			if (ds[j2] % ds[j] == 0) cnt[ds[j]] -= cnt[ds[j2]];
		}
	}
}


//【添字 gcd の和】O(√n + d(n)^2 log d(n))　（d(n) : n の約数の個数）
/*
* gcd(i, n) = g になるような i∈[1..K] について，Σ i を val[g] に昇順に格納する．
*
* 利用：【約数列挙】
*/
void gcd_indexed_sum(ll K, ll n, map<ll, mint>& val) {
	// ds : n の約数の集合（昇順）
	vl ds;
	divisors(n, ds);
	int m = sz(ds);

	mint inv2 = mint(2).inv();
	val.clear();
	repir(j, m - 1, 0) {
		// gcd(i, n) が d の倍数になるような i∈[1..K] について Σ i を求める．
		// これは等差数列の和で，初項は d，末項は K / d * d，項数は K / d である．
		val[ds[j]] = mint(ds[j] + K / ds[j] * ds[j]) * (K / ds[j]) * inv2;

		// gcd(i, n) = 2d, 3d, ... になるような i∈[1..K] の Σ i を引く
		repi(j2, j + 1, m - 1) {
			if (ds[j2] % ds[j] == 0) val[ds[j]] -= val[ds[j2]];
		}
	}
}


