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


//【部分集合の和の総和（要素数ごと）】O(n)
/*
* 与えられた a[0..n) について，各 k=[0..n] についての
*		Σ|set|=k Σi∈set a[i]
* の値を格納したリストを返す．
*
* 制約：fm は (n-1)! まで計算可能
*/
template <class T>
vm subset_sum_sum(const vector<T>& a, const Factorial_mint& fm) {
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

	mint a_sum = accumulate(all(a), mint(0));

	repi(k, 0, n) res[k] = a_sum * fm.bin(n - 1, k - 1);

	return res;
}


//【部分集合の積の総和（要素数ごと，mod 998244353）】O(n (log n)^2)
/*
* 与えられた a[0..n) について，各 k=[0..n] について
*		Σ|set|=k Πi∈set a[i]
* の値を格納したリストを返す．
*/
template <class T>
vm subset_product_sum(const vector<T>& a) {
	// verify : https://mojacoder.app/users/bayashiko/problems/multi-sum-hard

	//【方法】
	// 一次式の積
	//		g(x) = Πi=[0..n) (1 + a[i] x)
	// を計算したときの x^k の係数として求めることができる．

	int n = sz(a);
	if (n == 0) return vm{ 1 };

	vvm f(n);
	rep(i, n) f[i] = vm{ 1, a[i] };

	// 2 冪個ずつ掛けていく（分割統治積）
	for (int k = 1; k < n; k *= 2) {
		for (int i = 0; i + k < n; i += 2 * k) {
			f[i] = convolution(f[i], f[i + k]);
		}
	}

	return f[0];
}


//【部分集合の XOR の総和（要素数ごと，mod 998244353）】O(n log n log A)
/*
* 与えられた a[0..n) について，各 k=[0..n] について
*		Σ|set|=k XOR_i∈set a[i]
* の値を格納したリストを返す．
* 
* 制約：fm は n! まで計算可能
*/
template <class T>
vm subset_XOR_sum(const vector<T>& a, const Factorial_mint& fm) {
	//【方法】
	// ビットごとに独立に考える．
	// 寄与があるのは 1 を奇数個，0 を偶数個選んでいる場合なので，そのような選び方の数を畳込みで求める．

	int n = sz(a);

	vm res(n + 1);

	int B = msb(*max_element(all(a))) + 1;
	rep(b, B) {
		array<int, 2> cnt{ 0, 0 };
		rep(i, n) cnt[getb(a[i], b)]++;

		vm c1(cnt[1] + 1);
		repi(i, 0, cnt[1]) if (i & 1) c1[i] = fm.bin(cnt[1], i);

		vm c0(cnt[0] + 1);
		repi(i, 0, cnt[0]) c0[i] = fm.bin(cnt[0], i);

		vm c = convolution(c1, c0);

		repi(i, 0, n) res[i] += c[i] * (1 << b);
	}

	return res;
}


//【x との XOR の和】
/*
* XOR_sum<T>(vT a) : O(n log max(a))
*	a[0..n) で初期化する．
*
* ll sum(T x) : O(log max(a))
*	Σi∈[0..n) a[i] XOR x の値を返す．
*/
template <class T>
struct XOR_sum {
	int d;
	vvi cnt;

	// a[0..n) で初期化する．
	XOR_sum(const vector<T>& a) {
		// verify : https://atcoder.jp/contests/arc135/tasks/arc135_c

		T a_max = *max_element(all(a));
		d = msb((ll)a_max) + 1;

		cnt = vvi(d, vi(2));

		repe(v, a) rep(j, d) cnt[j][(v >> j) & 1]++;
	}
	XOR_sum() : d(0) {}

	// Σi=[0..n) a[i] XOR x の値を返す．
	ll sum(T x) {
		// verify : https://atcoder.jp/contests/arc135/tasks/arc135_c

		// ビット毎に独立に寄与を計算し和をとればよい．
		ll res = 0;
		rep(j, d) {
			// (0,1), (1,0) の組だけがビット位置に応じた寄与をもつ．
			res += (ll)cnt[j][1 - ((x >> j) & 1)] << j;
		}
		return res;
	}
};


