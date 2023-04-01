#pragma once
#include "header.h"
#include "ビット全探索.h"
// ■■■■■ スコア和（集合の分割） ■■■■■


//【分割の積スコアの和】O(3^n)
/*
* [0..n) の分割 P = 凵_k S_k に対するスコアが Π_k a[S_k] で与えられるとする．
* set⊂[0..n) の全ての分割のスコアの和を格納したリストを返す．
*
* 利用：【下位集合の全探索】
*/
vm set_partition_mul_score_sum(const vm& a) {
	// verify : https://atcoder.jp/contests/abc236/tasks/abc236_h

	//【備考】
	// 非交和畳込みを使えば O(n^2 2^n) まで高速化できるらしい．

	int N = sz(a);

	vm dp(N);
	dp[0] = 1;

	// SoS bit DP
	rep(set, N) {
		if (set == 0) continue;

		// set1 : set から特定の要素 x を取り除いた集合
		int set1 = (set - 1) & set;

		// set の分割において x を含む部分集合 set\sub を全探索する．
		repbs(sub, set1) dp[set] += dp[sub] * a[set - sub];
	}

	return dp;
}


