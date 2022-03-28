#pragma once
#include "header.h"
#include "FPS(mint).h"
// ■■■■■ 文字列の数え上げ ■■■■■


//【1 が k 個連続しないビット列の数え上げ】O(n)
/*
* 長さ [0..n] のビット列のうち，1 が k 個連続しないものの個数を cnt に格納する．
*/
void count_noncontinuous_bitsequences(int n, int k, vm& cnt) {
	cnt.resize(n + 1);

	// acc[i] : 長さ i 以下のビット列で，最後が '0' のものの個数
	vm acc(n + 1);
	acc[0] = 1;

	repi(i, 1, n) {
		// "[j個の1]0" を付けられるのは長さが i - 1 - j のものである．
		// acc[i] - acc[i-1] は j=[0..k) についてこれらを足し合わせたものとなる．
		acc[i] = acc[i - 1] + (acc[i - 1] - (i - 1 - k >= 0 ? acc[i - 1 - k] : 0));
	}

	repi(i, 0, n) {
		// 最後に "[j個の1]" を付けられるのは長さが i - j のものである．
		// j=[0..k) についてこれらを足し合わせたものが求める場合の数となる．
		cnt[i] = acc[i] - (i - k >= 0 ? acc[i - k] : 0);
	}
}


//【1 が k 個連続しないビット列の数え上げ】O(k log k log n)
/*
* 長さ n のビット列のうち，1 が k 個連続しないものの個数を返す．
*
* 利用：【展開係数／ボスタン－森法】
*/
mint count_noncontinuous_bitsequences(ll n, int k) {
	if (k == 0) return 0;

	// 母関数は (1+z+...+z^(k-1)) / (1-z-...-z^k) である．
	vm fc(k, 1), gc(k + 1, -1);
	gc[0] = 1;

	return coef(MFPS(fc), MFPS(gc), n);
}


