#pragma once
#include "header.h"
// ■■■■■ 最適化（集合） ■■■■■


//【部分集合の XOR の最大化】O(n log A) （A = max(a)）
/*
* 与えられた非負整数列 a[0..n) について，
*		max_(set⊂[0..n)) XOR_(i∈set) a[i]
* を返す．
*/
template <class T>
T maximize_subset_XOR(vector<T> a) {
	// verify : https://atcoder.jp/contests/abc141/tasks/abc141_f

	//【方法】
	// 各 a[i] をビットベクトルだとみなせば，max Span a[0..n) を求める問題となる．
	// Span a[0..n) の元は a[0..n) に基本変形を施すことによって全て得られる．
	// 第 b ビットに対するスコアが 2^b と指数的なので，上の桁から貪欲に 1 にしていけば良い．

	int n = sz(a);

	int B = -1;
	rep(i, n) chmax(B, msb(a[i]));

	T res = 0;
	repir(b, B, 0) {
		// a[i0] : 第 b ビットが 1 である要素（なければ i0 = -1）
		int i0 = -1;
		rep(i, n) {
			if (getb(a[i], b)) {
				i0 = i;
				break;
			}
		}

		// 第 b ビットが 1 である要素が無いなら何も出来ない．
		if (i0 == -1) continue;

		// 暫定解の第 b ビットが 0 なら v = a[i0] と XOR をとって 1 にする．
		T v = a[i0];
		if (!getb(res, b)) res ^= v;

		// 残る要素の第 b ビットを全て 0 にするよう XOR をとる．
		repi(i, i0, n - 1) if (getb(a[i], b)) a[i] ^= v;
	}

	return res;
}


