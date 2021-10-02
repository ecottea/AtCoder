#pragma once
#include "header.h"
// ■■■■■ 二重和 ■■■■■



//【差の総和】O(n log n)
/*
* ΣΣi<j |a[j] - a[i]| の値を返す．
*/
template <class T> mint difference_sum(const vector<T>& a_) {
	auto a = a_;
	int n = sz(a);

	// a を昇順ソートしておく．
	sort(all(a));

	mint res = 0;

	// 隣り合う要素の差に重みを付けて足し込んでいく．
	repi(i, 1, n - 1) {
		res += mint(a[i] - a[i - 1]) * i * (n - i);
	}

	return res;
}


//【xor の和】O(n log(max(a, b)))
/*
* Σi=[0..n) Σj=[0..m) a[i] xor b[j] の値を返す．
*/
template<class T> T xor_sum(const vl& a, const vl& b) {
	int n = sz(a), m = sz(b);
	const int d = 63;

	// 各ビット位置の 1 の個数を数えておく．
	vi ones_a(d), ones_b(d);
	rep(i, n) {
		rep(j, d) {
			if (a[i] & (1LL << j)) ones_a[j]++;
		}
	}
	rep(i, m) {
		rep(j, d) {
			if (b[i] & (1LL << j)) ones_b[j]++;
		}
	}

	// ビット毎に独立に寄与を計算し和をとればよい．
	T res = 0;
	rep(j, d) {
		// (0,1), (1,0) の組だけがビット位置に応じた寄与をもつ．
		res += ((T)ones_a[j] * (m - ones_b[j])
			+ (T)ones_b[j] * (n - ones_a[j])) * (1LL << j);
	}

	return res;
}


//【和の xor】O(n log m log(max(a, b)))
/*
* XORi=[0..n) XORj=[0..m) (a[i] + b[j]) の値を返す．
*/
int sum_xor(const vi& a, const vi& b) {
	int n = sz(a), m = sz(b);
	const int d = 30;

	// 各ビット位置に現れる 1 の個数の偶奇
	int a_xor = accumulate(all(a), 0, [](int x, int y) { return x ^ y; });
	int b_xor = accumulate(all(b), 0, [](int x, int y) { return x ^ y; });

	int res = 0;
	rep(k, d) {
		ll cnt = 0;

		// (1,0), (0,1) の組合せがあるごとに xor 結果のビットが反転する．
		if ((a_xor & (1 << k)) && (((b_xor & (1 << k)) ^ (n % 2 == 1)))) cnt++;
		if ((b_xor & (1 << k)) && (((a_xor & (1 << k)) ^ (n % 2 == 1)))) cnt++;

		// 注目ビットより上を全て 0 にする．
		vi a_masked(n), b_masked(n);
		rep(i, n) a_masked[i] = a[i] & ((1 << k) - 1);
		rep(i, m) b_masked[i] = b[i] & ((1 << k) - 1);
		sort(all(b_masked));

		// 各 a[i] ごとに下位桁からの繰り上がりが起こる回数を数える．
		rep(i, n) {
			auto it = lower_bound(all(b_masked), (1 << k) - a_masked[i]);
			cnt += distance(it, b_masked.end());
		}

		// これらの和の偶奇に応じて xor 結果のビットが求まる．
		res += (cnt % 2) << k;
	}

	return res;
}


