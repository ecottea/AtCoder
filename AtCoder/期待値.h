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


//【不正確な射撃】
/*
* 位置 x[i] に n 個の的がある．左右それぞれに確率 1/3 でぶれる射撃を行うとき，
* 全ての的に命中するまでの射撃回数の期待値を返す．
*
*（bit DP）
*/
double blurred_shooting(const vi& x) {
	int n = *max_element(all(x)) + 1;

	// 初期状態の的の集合
	int set = 0;
	repe(i, x) {
		set += (1 << i);
	}

	// dp[set] : set に的がある場合の射撃回数の期待値
	vd dp(1 << n, INFL);
	vb seen(1 << n);
	dp[0] = 0;
	seen[0] = true;

	// set に的がある場合の射撃回数の期待値を返す．
	function<double(int)> rf = [&](int set) {
		// 計算済なら結果を返す．
		if (seen[set]) return dp[set];
		seen[set] = true;

		// i : 狙う位置
		rep(i, n) {
			double sum = 0; // 的に当たった場合の残り回数の期待値の和
			int tgts = 0; // 当たる可能性のある的の数

			// 狙いが左にぶれた場合
			if (i > 0 && (set & (1 << (i - 1)))) {
				sum += rf(set - (1 << (i - 1)));
				tgts++;
			}

			// 狙い通りに弾が飛んだ場合
			if (set & (1 << i)) {
				sum += rf(set - (1 << i));
				tgts++;
			}

			// 狙いが右にぶれた場合
			if (i < n - 1 && (set & (1 << (i + 1)))) {
				sum += rf(set - (1 << (i + 1)));
				tgts++;
			}

			// 範囲内に的が無いような所を狙う意味は無いので無視する．
			if (tgts == 0) continue;

			// いずれかの的に命中するまでの回数の期待値が 3 / tgts，
			// いずれかの的に命中してからの回数の期待値が sum / tgts，
			// それらの和をとったものが i を狙った場合の回数の期待値となる．
			chmin(dp[set], (3 + sum) / tgts);
		}

		return dp[set];
	};

	return rf(set);
}


