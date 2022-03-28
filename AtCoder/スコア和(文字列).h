#pragma once
#include "header.h"
#include "FPS(mint).h"
// ■■■■■ 文字列のスコア和 ■■■■■


//【1 が k 個連続しないビット列のスコア和】O(k log k log n)
/*
* 長さ n のビット列のうち，1 が k 個連続しないもの全てについて，
* スコアを p^(1 の個数) と定めたときのスコア和を返す．
*
* 利用：【展開係数／ボスタン－森法】
*/
mint scoresum_noncontinuous_bitsequences(ll n, int k, mint p) {
	if (k == 0) return 0;

	vm powp(k);
	powp[0] = 1;
	repi(i, 1, k - 1) powp[i] = powp[i - 1] * p;

	// 母関数は (1+pz+...+p^(k-1)z^(k-1)) / (1-z-pz^2...-p^(k-1)z^k) である．
	vm fc(k), gc(k + 1);
	rep(i, k) fc[i] = powp[i];
	gc[0] = 1;
	repi(i, 1, k) gc[i] = -powp[i - 1];

	return coef(MFPS(fc), MFPS(gc), n);
}


