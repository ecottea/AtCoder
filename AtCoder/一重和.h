#pragma once
#include "header.h"
#include "二項係数.h"
// ■■■■■ 一重の和など ■■■■■


//【等差数列の和】O(1)
/*
* Σi=[i0..i1) (a + b i) を返す．
*/
template<class T> T arithmetic_series(T a, T b, ll i0, ll i1) {
	// verify : https://atcoder.jp/contests/arc035/tasks/arc035_b

	if (i0 >= i1) return 0;

	// 2^(-1) が存在しない場合でも問題ないように偶数を先に 2 で割っておく．
	if ((i1 - i0) % 2 == 0) {
		return a * (i1 - i0) + b * (i1 + i0 - 1) * ((i1 - i0) / 2);
	}
	else {
		return a * (i1 - i0) + b * ((i1 + i0 - 1) / 2) * (i1 - i0);
	}
}


//【等比数列の和】
/*
* geometric_series(r, n) : O(log n)
*	Σi=[0..n) r^i を返す．
*
* geometric_series(r, i0, i1) : O(log max(i0, i1))
*	Σi=[i0..i1) r^i を返す．
* 
*（ダブリング）
*/
mint geometric_series(mint r, ll n) {
	// verify : https://atcoder.jp/contests/arc050/tasks/arc050_c

	//【方法】
	// おなじみの公式では r-1 による割り算を必要としてまずいのでダブリングする．

	// pow2 = r^(2^i), sumpow2 = 1 + r + ... + r^((2^i) - 1)
	mint res(0), pow2 = r, sumpow2 = 1;
	while (n > 0) {
		if (n & 1LL) res = res * pow2 + sumpow2;
		sumpow2 += sumpow2 * pow2;
		pow2 *= pow2;
		n /= 2;
	}
	return res;
}
mint geometric_series(mint r, ll i0, ll i1) {
	return geometric_series(r, i1) - geometric_series(r, i0);
}


//【等差×等比型数列の和】O(log n)
/*
* arithmetic_geometric_series(r, n) : O(log n)
*	Σi=[0..n) i r^i を返す．
*
* arithmetic_geometric_series(a, b, r, i0, i1) : O(log max(i0, i1))
*	Σi=[i0..i1) (a i + b) r^i を返す．
*
*（ダブリング）
* 
* 利用：【等比数列の和】
*/
mint arithmetic_geometric_series(mint r, ll n) {
	//【方法】
	// おなじみの公式では r-1 による割り算を必要としてまずいのでダブリングする．

	// pow2 = 2^i, powr = r^(2^i)
	// sumpowr1 = 0 * 1 + 1 * r + ... + ((2^i) - 1) * r^((2^i) - 1)
	// sumpowr0 = 1 + r + ... + r^((2^i) - 1)
	mint res1(0), res0(0), pow2 = 1, powr = r, sumpowr1 = 0, sumpowr0 = 1;
	while (n > 0) {
		if (n & 1LL) {
			res1 += pow2 * res0;
			res1 *= powr;
			res1 += sumpowr1;
			res0 *= powr;
			res0 += sumpowr0;
		}
		sumpowr1 *= 1 + powr;
		sumpowr1 += pow2 * powr * sumpowr0;
		sumpowr0 *= 1 + powr;
		powr *= powr;
		pow2 *= 2;

		n /= 2;
	}
	return res1;
}
mint arithmetic_geometric_series(mint a, mint b, mint r, ll i0, ll i1) {
	mint res = a * geometric_series(r, i0, i1);
	res += b * (arithmetic_geometric_series(r, i1) - arithmetic_geometric_series(r, i0));
	return res;
}


//【累乗×等比型数列の無限和】O(d log d)
/*
* Σi=[0..∞) i^d r^i を返す．
*
* 制約 : r != 1
* 
* 利用：【階乗など（法が大きな素数）】
*/
mint powered_geometric_series(mint r, int d) {
	// 参考 : https://kyopro-friends.hatenablog.com/entry/2020/03/11/073122
	// verify : https://judge.yosupo.jp/problem/sum_of_exponential_times_polynomial_limit

	vm acc(d + 2), pow_r(d + 2);
	pow_r[0] = 1;
	rep(i, d + 1) {
		acc[i + 1] = acc[i] + pow_r[i] * mint(i).pow(d);
		pow_r[i + 1] = pow_r[i] * r;
	}

	Factorial_mint fm(d + 1);

	mint res = 0;
	repi(i, 0, d) {
		res += ((d - i) % 2 == 0 ? 1 : -1) * pow_r[d - i]
			* fm.binomial(d + 1, i + 1) * acc[i + 1];
	}

	res /= mint(1 - r).pow(d + 1);

	return res;
}


//【x との XOR の和】
/*
* Xor_sum(vT a) : O(n log max(a))
*	a[0..n) で初期化する．
*
* ll sum(T x) : O(log max(a))
*	Σi=[0..n) a[i] XOR x の値を返す．
*/
template <class T> struct Xor_sum {
	// verify : https://atcoder.jp/contests/arc135/tasks/arc135_c

	int d;
	vvi cnt;

	// a[0..n) で初期化する．
	Xor_sum(const vector<T>& a) {
		T a_max = *max_element(all(a));
		if (a_max > 0) d = msbll((ll)a_max) + 1;
		else d = 0;

		cnt = vvi(d, vi(2));

		repe(v, a) {
			rep(j, d) {
				cnt[j][(v >> j) & 1]++;
			}
		}
	}

	// Σi=[0..n) a[i] XOR x の値を返す．
	ll sum(T x) {
		// ビット毎に独立に寄与を計算し和をとればよい．
		ll res = 0;
		rep(j, d) {
			// (0,1), (1,0) の組だけがビット位置に応じた寄与をもつ．
			res += (ll)cnt[j][1 - ((x >> j) & 1)] << j;
		}
		return res;
	}
};


//【m で割った余りの和】
/*
* Mod_sum_query(a) : O(n)
*	配列 a で初期化する．
*
* sum_mod(m) : O(max(a) log(n) / m)
*	a[0..n) mod m の和を返す．
*
* sum_lack(m) : O(max(a) log(n) / m)
*	a[0..n) を m で割った不足の和を返す．
*/
struct Mod_sum_query {
	vi a;    // ★ a でなくバケツで累積和を持てば O(log n) を落とせる．
	int n;
	ll asum; // a[0..n) の和

	// コンストラクタ（何もしない）
	Mod_sum_query() : n(0), asum(0) {}

	// 配列 a で初期化
	Mod_sum_query(const vi& a_) : a(a_), n(sz(a)), asum(0) {
		sort(all(a));
		rep(i, n) asum += a[i];
	}

	// a[0..n) mod m の和を返す．
	ll sum_mod(int m) {
		ll res = asum;

		for (ll v = m; v <= a[n - 1]; v += m) {
			// 通常の和とくらべて何個 m を引かれるかを二分探索で求めれば良い．
			res -= (ll)m * distance(lower_bound(all(a), v), a.end());
		}

		return res;
	}

	// a[0..n) を m で割った不足の和を返す．
	ll sum_lack(int m) {
		// sum : 1-indexed での a[0..n) mod m の和
		ll sum = asum;

		for (ll v = m; v < a[n - 1]; v += m) {
			// 通常の和とくらべて何個 m を引かれるかを二分探索で求めれば良い．
			sum -= (ll)m * distance(lower_bound(all(a), v + 1), a.end());
		}

		// 不足分を返す．
		return (ll)m * n - sum;
	}
};


