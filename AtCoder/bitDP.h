#pragma once
#include "header.h"
#include "ビット全探索.h"
// ■■■■■ bit DP ■■■■■


//【直径最小化クラスタリング】O(3^n k)
/*
* 距離が dist で与えられる n 点をクラスタ直径の最大値が最小になるように
* k 個のクラスタに分割したときの直径の大きさを返す．
*
*（bit DP）
*
* 利用：【下位集合の全探索】
*/
ll minimize_diameter_clustering(const vvl& dist, int k) {
	int n = sz(dist);

	// dp[set][c] : 点集合 set を c 個に分割するときの最小直径
	vvl dp(1 << n, vl(k + 1, INFL));

	// 点集合 set の直径 dp[set][1] を前計算しておく．O(2^n n^2)
	repb(set, n) {
		dp[set][1] = 0;

		// set に含まれる 2 点の組全てを調べ，その距離の最大値をとる．
		rep(i, n) {
			rep(j, i) {
				if ((set & (1 << i)) && (set & (1 << j))) {
					chmax(dp[set][1], dist[i][j]);
				}
			}
		}
	}

	// bit DP
	repb(set, n) {
		if (set == 0) continue;

		// set1 : set から 1 つ目の要素を取り除いた集合
		int set1 = (set - 1) & set;

		// set の 1 つ目の要素を含まない部分集合 sub を全探索する．
		repbs(sub, set1) {
			repi(c, 2, k) {
				chmin(dp[set][c], max(dp[sub][1], dp[set - sub][c - 1]));
			}
		}
	}

	return dp[(1 << n) - 1][k];
}


//【スコア和最大化クラスタリング】O(3^n)
/*
* i と j が同クラスタに居るとスコア sc[i][j] が得られる条件で，
* n 点をいくつかのクラスタに分けて得られるスコアの最大値を返す．
*
*（bit DP）
*
* 利用：【下位集合の全探索】
*/
ll maximize_score_clustering(const vvl& sc) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_u

	int n = sz(sc);

	// dp[set] : 部分集合 set での得点の最大値
	vector<ll> dp(1 << n);

	rep(set, 1 << n) {
		rep(i, n) {
			if (!(set & (1 << i))) continue;

			rep(j, i) {
				if (set & (1 << j)) {
					dp[set] += sc[i][j];
				}
			}
		}

		// set の部分集合 sub を全探索する．
		repbs(sub, set) {
			chmax(dp[set], dp[sub] + dp[set - sub]);
		}
	}

	return dp[(1 << n) - 1];
}


