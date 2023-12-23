#pragma once
#include "header.h"
#include "FPS(mint).h"
// ■■■■■ スコア和（集合） ■■■■■


//【部分集合の大きさの和】
/*
* Σset⊂[0..n) |set| = n 2^(n-1)
* 
* 証明：各 i∈[0..n) が数えられる回数は，[0..n)-{i} の部分集合の個数と同じ 2^(n-1) 回である．
*/


//【部分集合の和の和（要素数ごと）】O(n)
/*
* 与えられた a[0..n) について，各 k=[0..n] についての
*		Σ|set|=k Σi∈set a[i]
* の値を格納したリストを返す．
*
* 利用：【階乗など（法が大きな素数）】
*/
template <class T>
vm subset_sum_sum(const vector<T>& a) {
	//【方法】
	// a[i] が res[k] に何回寄与するかを考えると，
	// i を含む大きさ k の部分集合の個数 bin(n-1, k-1) 回と分かる．
	//
	// よって
	//		res[k]
	//		= Σi=[0..n) a[i] * bin(n-1, k-1)
	//		= bin(n-1, k-1) * Σi=[0..n) a[i]
	// となる．
	
	//【補足】
	// これら全てを加えあわえれば
	//		Σset⊂[0..n) Σi∈set a[i] = 2^(n-1) Σa[0..n)
	// を得る．

	int n = sz(a);
	vm res(n + 1);

	if (n == 0) return;

	Factorial_mint fm(n);
	mint a_sum = accumulate(all(a), mint(0));

	repi(k, 0, n) res[k] = a_sum * fm.bin(n - 1, k - 1);

	return res;
}


//【部分集合の積の和（要素数ごと，mod 998244353）】O(n (log n)^2)
/*
* 与えられた a[0..n) について，各 k=[0..n] について
*		Σ|set|=k Πi∈set a[i]
* の値を格納したリストを返す．
*
* 利用：【形式的冪級数】,【一次式の積の展開（基本対称式）】
*/
template <class T>
vm subset_product_sum(const vector<T>& a) {
	//【方法】
	// 一次式の積
	//		g(x) = Πi=[0..n) (1 + a[i] x)
	// を計算したときの x^k の係数として求めることができる．

	int n = sz(a);
	vm resn + 1);

	if (n == 0) {
		res[0] = 1;
		return;
	}

	// 代わりに Πi=[0..n) (x + a[i]^(-1)) を求め，Πa[0..n) 倍する．
	vm x(n); mint c = 1;
	rep(i, n) {
		if (a[i] == 0) continue;

		x[i] = -mint(a[i]).inv();
		c *= a[i];
	}

	MFPS g = c * expand(x);

	repi(k, 0, n) res[k] = g[k];

	return res;
}


