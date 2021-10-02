#pragma once
#include "header.h"
// ■■■■■ 期待値 ■■■■■



//【クーポンコレクター問題】O(n^max(a))
/*
* 等確率で当たる n 枚のクーポンを各 a[i] (∈[0..3]) 枚揃えるまでの回数の期待値を返す．
*
*（期待値 DP）
*/
double coupon_collectors_problem(const vi& a) {
	int n = sz(a);

	vector<int> cnt(4);
	rep(i, n) {
		cnt[a[i]]++;
	}

	auto f1 = [&]() {
		double res = 0;
		repir(i, cnt[1], 1) res += 1. / i;
		res *= n;
		return res;
	};

	auto f2 = [&]() {
		// dp[i][j] : 残り 1, 2 個のクーポンが各 i, j 種類残っている場合の
		// 揃えるまでの残り試行回数の期待値
		vvd dp(n + 1, vd(n + 1));

		for (int j = 0; j <= cnt[2]; j++) {
			for (int i = 0; i + j <= cnt[1] + cnt[2]; i++) {
				if (i == 0 && j == 0) {
					dp[i][j] = 0;
					continue;
				}

				dp[i][j] = (double)n / (i + j);
				if (i > 0) {
					dp[i][j] += dp[i - 1][j] * i / (i + j);
				}
				if (j > 0) {
					dp[i][j] += dp[i + 1][j - 1] * j / (i + j);
				}
			}
		}

		return dp[cnt[1]][cnt[2]];
	};

	auto f3 = [&]() {
		// dp[i][j][k] : 残り 1, 2, 3 個のクーポンが各 i, j, k 種類残っている場合の
		// 揃えるまでの残り試行回数の期待値
		vvvd dp(n + 1, vvd(n + 1, vd(n + 1)));

		for (int k = 0; k <= cnt[3]; k++) {
			for (int j = 0; j + k <= cnt[2] + cnt[3]; j++) {
				for (int i = 0; i + j + k <= cnt[1] + cnt[2] + cnt[3]; i++) {
					if (i == 0 && j == 0 && k == 0) {
						dp[i][j][k] = 0;
						continue;
					}

					dp[i][j][k] = (double)n / (i + j + k);
					if (i > 0) {
						dp[i][j][k] += dp[i - 1][j][k] * i / (i + j + k);
					}
					if (j > 0) {
						dp[i][j][k] += dp[i + 1][j - 1][k] * j / (i + j + k);
					}
					if (k > 0) {
						dp[i][j][k] += dp[i][j + 1][k - 1] * k / (i + j + k);
					}
				}
			}
		}

		return dp[cnt[1]][cnt[2]][cnt[3]];
	};

	if (cnt[3] > 0) return f3();
	else if (cnt[2] > 0) return f2();
	else if (cnt[1] > 0) return f1();
	else return 0;
}


