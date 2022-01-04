#pragma once
#include "header.h"
// ■■■■■ 桁ダブリング ■■■■■


//【倍数の数え上げ（桁の数の制限あり）】O(b^2 log n)
/*
* 桁の数に 1 <= c[i] <= 9 しか含まない n 桁の正の b の倍数の個数を返す．
*
*（ダブリングで高速化した DP）
*/
mint count_restricted_multiplier(ll n, const vi& c, int b) {
	// verify : https://atcoder.jp/contests/typical90/tasks/typical90_e

	int k = sz(c);
	const int m = msbll(n) + 1;

	// pow10[i] : 10^(2^i) mod b
	vi pow10(m + 1);
	pow10[0] = 10 % b;

	rep(i, m) {
		pow10[i + 1] = (pow10[i] * pow10[i]) % b;
	}

	// dp_pow2[i][j] : 2^i 桁で j (mod b) となる数の個数
	vvm dp_pow2(m + 1, vm(b));
	rep(i, k) dp_pow2[0][c[i] % b]++;

	// 配る DP
	rep(i, m) {
		// j1 : 上 2^i 桁，j2 : 下 2^i 桁の mod b の値
		rep(j1, b) {
			rep(j2, b) {
				int nj = (j1 * pow10[i] + j2) % b;
				dp_pow2[i + 1][nj] += dp_pow2[i][j1] * dp_pow2[i][j2];
			}
		}
	}

	// n の 2^i の位より下の桁だけを読んだ数を n(i) と書くことにする．
	// dp[i][j] : n(i) 桁の数で j (mod b) となる数の個数
	vvm dp(m + 1, vm(b));
	dp[0][0] = 1;

	// 配る DP
	int p = 1; // 10^n(i) mod b
	rep(i, m) {
		if (n & 1LL) {
			// j1 : 上 2^i 桁，j2 : 下 n(i) 桁の mod b の値
			rep(j1, b) {
				rep(j2, b) {
					dp[i + 1][(j1 * p + j2) % b] += dp_pow2[i][j1] * dp[i][j2];
				}
			}
			p = (p * pow10[i]) % b;
		}
		else {
			rep(j2, b) {
				dp[i + 1][j2] = dp[i][j2];
			}
		}
		n /= 2;
	}

	return dp[m][0];
}


