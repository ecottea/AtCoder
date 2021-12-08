#pragma once
#include "header.h"
// ■■■■■ 桁 DP ■■■■■


//【広義昇順，個数】O(n b^2)
/*
* b 進数で n 桁の数 num 以下の非負の整数で，数字が広義昇順に並んでいるものの個数を返す．
* 
*（桁 DP）
*/
mint count_increasing_number(string& num) {
	int n = sz(num);
	const int b = 10;

	// dp[i][j][k] : 以下の条件を満たす num 以下の数が何通りあるか．
	//	i : 上からの桁 d[0..i) まで決まっている．
	//	j : d[0..i) だけで num より小さいことが確定していれば 1，さもなくば 0
	//	k : d[i-1]
	vvvm dp(n + 1, vvm(2, vm(10)));
	dp[0][0][0] = 1;

	// 上の桁から順に配る DP
	rep(i, n) {
		// num の上から i 桁目の数
		int x = num[i] - '0';

		rep(k, b) {
			// もう num より小さいことが確定している場合，
			// t = d[i] は何であっても num より小さいと確定する．
			repi(t, k, b - 1) {
				dp[i + 1][1][t] += dp[i][1][k];
			}

			// まだ num より小さいと確定していない場合，
			// t = d[i] によって以下の場合分けが生じる：
			//		d[i] < x であれば num より小さいと確定する．
			//		d[i] = x であればまだ確定しない．
			//		d[i] > x とは選べない．
			repi(t, k, x - 1) {
				dp[i + 1][1][t] += dp[i][0][k];
			}
			if (x >= k) dp[i + 1][0][x] += dp[i][0][k];
		}
	}
	dumpel(dp);

	mint res = 0;
	rep(j, 2) rep(k, b) res += dp[n][j][k];

	return res;
}


//【数字和倍数指定，個数】O(n m b)
/*
* b 進数で n 桁の数 num 以下の非負の整数で，数字和が m の倍数であるものの個数を返す．
* 
*（桁 DP）
*/
mint count_digit_sum(string& num, int m) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_s

	int n = sz(num);
	const int b = 10;

	// dp[i][j][k] : 以下の条件を満たす num 以下の数が何通りあるか．
	//	i : 上から i 桁目まで決まっている（0-indexed）
	//	j : ここまでだけで num より小さいことが確定していれば 1，さもなくば 0
	//	k : 数字和が k (mod m)
	vvvm dp(n + 1, vvm(2, vm(m)));
	dp[0][0][0] = 1;

	// 上の桁から順に配る DP
	rep(i, n) {
		// num の上から i 桁目の数
		int x = num[i] - '0';

		rep(k, m) {
			// もう num より小さいことが確定している場合，
			// i + 1 桁目の数は何であっても num より小さいと確定する．
			rep(t, b) {
				dp[i + 1][1][(k + t) % m] += dp[i][1][k];
			}

			// まだ num より小さいと確定していない場合，
			// i + 1 桁目の数によって以下の場合分けが生じる：
			//		x 未満であれば num より小さいと確定する．
			//		x ちょうどであればまだ確定しない．
			//		x を超える数は選ぶことができない．
			rep(t, x) {
				dp[i + 1][1][(k + t) % m] += dp[i][0][k];
			}
			dp[i + 1][0][(k + x) % m] += dp[i][0][k];
		}
	}

	return dp[n][0][0] + dp[n][1][0];
}


