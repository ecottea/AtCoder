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


//【XOR の和】O(n log n)
/*
* Σi=[0..n) Σj=[0..m) a[i] XOR b[j] の値を返す．
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

