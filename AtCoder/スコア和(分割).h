#pragma once
#include "header.h"
#include "bit全探索.h"
// ■■■■■ スコア和（集合の分割） ■■■■■


//【分割の積スコアの和】O(3^n)
/*
* [0..n) の分割 π = 凵k S_k に対するスコアが Πk a[S_k] で与えられるとする．
* 各 set⊂[0..n) について，全ての分割のスコアの和を格納したリストを返す．
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

		// sub ⊂ set1 を全探索する（set - sub が x を含む）
		for (int sub = set1, tmp = 1; tmp > 0; tmp = sub, sub = (sub - 1) & set1) {
			dp[set] += dp[sub] * a[set - sub];
		}
	}

	return dp;
}


//【分割のブロック和の積の和】O(n (log n)^2)
/*
* [0..n) の分割 π = 凵k S_k に対するスコアが Πk Σa[S_k] で与えられるとし，全ての分割を渡るスコアの和を返す．
*/
mint set_partition_block_sum_mul_score_sum(const vm& a) {
	// verify : https://yukicoder.me/problems/no/2817

	//【方法】
	// K 個のブロックに分かれる分割からの寄与を考える．
	// 
	// 積の和典型の考え方を用いる．
	// まず各 i∈[0..n) に対して K 色の色 c[i] を割り当て，色 c[i] の球が a[i] 個あるとし，
	// さらに各色の球の代表を 1 つずつ選ぶ方法の数が求める寄与であると表現できる．
	// 
	// 事象の順序交換の考え方を用いる．
	// まず [0..n) の中から K 個選び，それぞれから代表を選び，残りの [0..n) には自由に色を割り当てるとしてよい．
	// 代表の選び方が何通りあるかは，a[0..n) の K 次の基本対称式に等しく，
	// 残りの色の割り当て方は K^(n-K) 通りである．

	int n = sz(a);

	vvm f(n);
	rep(i, n) f[i] = vm{ 1, a[i] };

	// 2 冪個ずつ掛けていく（分割統治積）
	for (int w = 1; w < n; w *= 2) {
		for (int i = 0; i + w < n; i += 2 * w) {
			f[i] = convolution(f[i], f[i + w]);
		}
	}

	mint res = 0;

	repi(k, 1, n) {
		res += mint(k).pow(n - k) * f[0][k];
	}

	return res;
}


