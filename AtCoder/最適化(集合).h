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


//【1 ずつ引く操作（回数指定）】
/*
* 与えられた非負整数列 a[0..n)，K∈[0..n]，C∈[0..∞) について，
*		|set|=K なる set⊂[0..n) を選び a[set] から 1 ずつ引く
* という操作を a[0..n) ≧ 0 を保ったまま C 回繰り返せることは，
*		Σi min(a[i], C) ≧ K C
* が成り立つことと同値である．
*
* (証明)
* (=>) の対偶を示す．
* min(a[i], C) は set に i を選べる最大回数を表しているので，(左辺) / K は
* 操作を行える最大回数を表す．これが C 未満であれば，C 回の操作を行うことはできない．
*
* (<=) を示す．
* a[0..n) が降順ソートされているとして一般性を失わない．set=[0..K) として操作を 1 回行う．
* a[K] ≧ C のときは，
*		Σi∈[0..K) min(a[i]-1, C-1) + Σi∈[K..n) min(a[i], C-1)
*		≧ Σi∈[0..K) min(a[i]-1, C-1)
*		≧ Σi∈[0..K) min(C-1, C-1)
*		= K(C - 1)
* となるので不等式は成り立つ．a[K] < C のときは，
*		Σi∈[0..K) min(a[i]-1, C-1) + Σi∈[K..n) min(a[i], C-1)
*		= Σi∈[0..K) min(a[i], C) - K + Σi∈[K..n) a[i]
*		= Σi∈[0..K) min(a[i], C) + Σi∈[K..n) min(a[i], C) - K
*		≧ K C - K
*		= K(C - 1)
* となるので不等式は成り立つ．よって数学的帰納法が適用できる．
* 
* verify : https://atcoder.jp/contests/abc227/tasks/abc227_d
*/


//【1 ずつ引く操作（0 まで）】
/*
* 与えられた非負整数列 a[0..n) と n 以下の正整数 K について，
*		|set|=K なる set⊂[0..n) を選び a[set] から 1 ずつ引く
* という操作を繰り返して a[0..n) = 0 とできることは，
*		(1) Σa[0..n) ≡ 0 (mod K)
*		(2) max a[0..n) ≦ Σa[0..n) / K
* がともに成り立つことと同値である．
* 
* (証明)
* (1) については明らか．その下で C = Σa[0..n) / K として
* 【1 ずつ引く操作（回数指定）】を適用すれば良い．
* 
* verify : https://yukicoder.me/problems/11849
*/
